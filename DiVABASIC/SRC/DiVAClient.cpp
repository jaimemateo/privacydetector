#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <conio.h>
#include <winsock2.h>
#include <vector>
#include <string>

#include "DiVAClient.h"
#include "DiVAImage.h"
#include "DiVACrono.h"
#include "Description.h"
//#include "highgui.h"
//#include <opencv2/highgui/highgui.hpp>
#include "include\opencv2\highgui\highgui.hpp"
//#include <opencv/cv.h>
//#include <cv.h>

//cabeceras para realizar las funciones de compresion
#include "RunLength.h"
#include "ZLIB32B.h"

using namespace std;

#define COMMAND_LENGTH 64
/**
*Constructor
*/
DiVAClient::DiVAClient(char* servername,int port)
{
	_servername = new char[25];
	strcpy(_servername,servername);
	_port = port;
	_isEnd = FALSE;
	_pconn=(void*)malloc(sizeof(SOCKET));
	_sourceId = NULL;
}

/**
*Destructor
*/	
DiVAClient::~DiVAClient()
{
	if (_servername)
		delete _servername;
	//if (_hp)
	//	delete _hp;
	if (_pconn)
	{
		closesocket(*((SOCKET*)_pconn));
		free(_pconn);
		_pconn=NULL;
	}

	int intentos = 0;
	/*int ret = WSACleanup();
	while (ret && (intentos < 100))
	{
#ifdef _DEBUG
	cout << "DiVAClient::~DiVAClient(): Intentando terminar el uso de sockets con WSACleanup()";
#endif
		intentos++;
		ret = WSACleanup();
		cvWaitKey(10);
	}*/
#ifdef _DEBUG
	cout << "DiVAClient::~DiVAClient(). Finalizacion correcta del uso de sockets.\r\n";
#endif
	if(_sourceId)
		free(_sourceId);
}
/**
*This method connect to the frame server
*\return TRUE if everything goes right
*/

int DiVAClient::Connect()
{
	int ret = -1;
	char buff[32];
	int z=0;
	struct sockaddr_in server;
	WSADATA wsaData;
	unsigned int addr;

	int wsaret=0;
	wsaret=WSAStartup(MAKEWORD(2,0),&wsaData);

	if(wsaret)
	{
		printf("DiVAClient::Connect(): No fue posible habilitar el uso de sockets\r\n");
#ifdef _DEBUG
		printf("DiVAClient::Connect(): No fue posible habilitar el uso de sockets\r\n.");
#endif
		return ret;
	}
	SOCKET conn=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        /*
        int PASCAL FAR setsockopt (SOCKET s, int level, int optname,
                           const char FAR * optval, int optlen);
                                                   */
        //BOOL bOptVal = TRUE;
        //int bOptLen = sizeof(BOOL);
        //setsockopt(conn,IPPROTO_TCP,TCP_NODELAY,(char*)&bOptVal,bOptLen);//TCP_NODELAY

	(*(SOCKET*)_pconn) = conn;
	if((*((SOCKET*)_pconn))==INVALID_SOCKET)
	{
#ifdef _DEBUG
		printf("DiVAClient::Connect(): No fue posible crear el socket\r\n.");
#endif
		return ret;
	}

	if(inet_addr(_servername)==INADDR_NONE)
	{
		_hp=gethostbyname(_servername);
	}
	else
	{
		addr=inet_addr(_servername);
		_hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
	}
	if(_hp==NULL)
	{
		closesocket(*((SOCKET*)_pconn));
	#ifdef _DEBUG
		printf("DiVAClient::Connect(): No fue posible obtener las caracteristicas del servidor\r\n.");
	#endif
		return ret;
	}
	server.sin_addr.s_addr=*((unsigned long*)_hp->h_addr);
	server.sin_family=AF_INET;
	//server.sin_port=htons(20248);
	server.sin_port=htons(_port);
	if(connect((*((SOCKET*)_pconn)),(struct sockaddr*)&server,sizeof(server)))
	{
		closesocket(*((SOCKET*)_pconn));	
	#ifdef _DEBUG
		printf("DiVAClient::Connect(): No fue posible la conexion con el servidor\r\n.");
	#endif
		return ret;
	}
	//Espero al mensaje: Server Ready
		
	int bOptVal = 3000;
	int bOptLen = sizeof(int);
	setsockopt((*((SOCKET*)_pconn)), SOL_SOCKET,SO_RCVTIMEO,(char*)&bOptVal,bOptLen);
	z=	this->WaitForACK();
	
	if (z<=0)
		{
			closesocket((*((SOCKET*)_pconn)));	
#ifdef _DEBUG
			printf("DiVAClient::Connect(): No se ha rebido el ACK del servidor.\r\n");
#endif
			return ret;
		}

	sprintf(buff,"ID_REQUEST\n");//Solicito un identificacion
	z =this->SendCommand(buff);
	if (z<(int)strlen(buff))
	{
#ifdef _DEBUG
		printf("DiVAClient::Connect. Error al enviar la peticion de ID.\r\n");
#endif
		return ret;
	}
	this->setId(this->WaitForId());
	if (this->getId()>=0)
		ret = TRUE;
	else
	{
#ifdef _DEBUG
		printf("DiVAClient::Connect(): El identificador de cliente recibido no es valido.\r\n");
#endif
	}
	return ret;
}
/**
*This method waits for the reception of a frame from the frame server.
*\param pbuffer This is where i will store the memory address with the frame.The memory allocation will be
*		performed inside the method. It must be freed by the calling process.
*\return The possible return values are:
*       - the size of the received frame.
*		- -1 if any problem occurs
*/
int DiVAClient::receiveFrame(void** pbuffer,long idFrame)
{
	int z = 0;
	char buff[64];
	//Pido el frame idFrame+1 identificandome con idClient. Si el server no tiene ese frame me dará el siguiente más cercano
	//Si no tiene ninguno y pasa el timeout sabre que el server ya no me va a dar más frames
	sprintf(buff,"FRAME\n%d %d\n",idFrame,this->getId());//Solicito el envio de un frame
	z =this->SendCommand(buff);
	if (z<(int)strlen(buff))
	{

#ifdef _DEBUG
		printf("DiVAClient::receiveFrame Enviando peticion. Conexion con el servidor perdida.\r\n");
#endif
		return -2;
	}
	z=this->WaitForImage((void**)pbuffer);
	if (z<=0)
	{
		printf("DiVAClient::receiveFrame Enviando peticion. Conexion con el servidor perdida.\r\n");
	#ifdef _DEBUG
		printf("DiVAClient::receiveFrame Esperando datos. Conexion con el servidor perdida.\r\n");
	#endif
		return z;
	}
	//#ifdef _DEBUG
	//	printf("DiVAClient::receiveFrame Frame pedido/recibido: %d/%d.\r\n",idFrame,((DiVAImage*)*pbuffer)->getId());
	//#else
		if (idFrame!=((DiVAImage*)*pbuffer)->getId())
			printf("DiVAClient::receiveFrame Frame pedido/recibido: %d/%d.\r\n",idFrame,((DiVAImage*)*pbuffer)->getId());
	//#endif
	return z;
}


int DiVAClient::receiveDescription(void** pbuffer,long idFrame, char* descriptorName)
{
	int z = 0;
	char buff[64];
	char dNameAux[64];
	dNameAux[0]=0;
	char* dName=NULL;
	if (descriptorName==NULL)
	{
		strcpy(dNameAux,"DESCRIPTOR");
		dName = &dNameAux[0];
	}
	else
	{
		strcpy(dNameAux,descriptorName);
		dName = &dNameAux[0];
	}
	//Pido el frame idFrame+1 identificandome con idClient. Si el server no tiene ese frame me dará el siguiente más cercano
	//Si no tiene ninguno y pasa el timeout sabre que el server ya no me va adar más frames
	sprintf(buff,"%s\n%d %d\n",dName,idFrame,this->getId());//Solicito el de la descripción
	z =this->SendCommand(buff);
	int l=strlen(buff);
	if (z<l)
	{
#ifdef _DEBUG
		printf("DiVAClient::receiveDescription Enviando peticion. Conexion con el servidor perdida.\r\n");
#endif
		return -1;
	}
	z=this->WaitForDescription((void**)pbuffer);
	if (z<=0)
	{
	#ifdef _DEBUG
		printf("DiVAClient::receiveDescription Esperando datos. Conexion con el servidor perdida.\r\n");
	#endif
		return -1;
	}
	//#ifdef _DEBUG
	//	printf("DiVAClient::receiveDescription Frame pedido/recibido: %d/%d.\r\n",idFrame,((DiVAImage*)*pbuffer)->getId());
	//#else
		if (idFrame!=((Description*)*pbuffer)->getId())
			printf("DiVAClient::receiveDescription Frame pedido/recibido: %d/%d.\r\n",idFrame,((Description*)*pbuffer)->getId());
	//#endif
	return z;
}
int DiVAClient::SendCommand(char* command)
{
	if (_pconn){
		char aux[COMMAND_LENGTH+1];
		if (strlen(command)>COMMAND_LENGTH)
		{
			printf("Longitud del comando demasiado larga, %d Bytes.\r\n",strlen(command));
			return -1;
		}
		memset(aux,0,COMMAND_LENGTH+1);
		strcpy(aux,command);
		int z=send((*((SOCKET*)_pconn)),command,COMMAND_LENGTH,0);
		#ifdef _DEBUG
		if (z<(int)strlen(command))
			printf("DiVAClient::SendCommand Error enviando el comando: %s\r\n",command);
		#endif
		return z;
	}

	return -1;
		
}

int DiVAClient::ReceiveKeepAlive()
{
	int z = 0;
	char buff[64];
	int x1;
	char aux[512];

	strcpy(buff,"KEEP_ALIVE");//Solicito el envio del comando KEEP_ALIVE
	z =this->SendCommand(buff);
	if (z<(int)strlen(buff))
	{

#ifdef _DEBUG
		printf("DiVAClient::receiveFrame Enviando peticion. Conexion con el servidor perdida.\r\n");
#endif
		return -1;
	}

	x1 = DiVArecv(aux , COMMAND_LENGTH);

	int index	=0;
	char command[512];
	//Busco el final de la primera línea
	for (index=1;index<(int)strlen(aux);index++)
	{
		if (aux[index]=='\n')
			break;
	}
	strcpy(command,aux);
	command[index]='\0';

	if (x1 < 0)
		return -1;

	if (strcmp(aux, "YES"))
		return -1;
	
	return 0;
	
}

int DiVAClient::WaitForImage(void** buffer)
{
	long longitud =0;
	char aux[512];
	int error=0;

	DiVACrono * cronometro = new DiVACrono();
	FILE * archivo;
	double duracion,duracion1;

	//Espero la recepción de los parametros
	//Primero su longitud
	longitud = 0;
	long	x1 = 0;
	long	x = 0;
	long porRecibir =sizeof(long);
	long longTotal=0;

	x1 = DiVArecv(aux ,porRecibir);
	if(x1<0){
		return x1;
	}
	memcpy(&longitud,aux,sizeof(long));
	longTotal=longTotal+x1;

	//Despues los parametros en si
	porRecibir =longitud;
	long x2= DiVArecv(aux ,porRecibir);
	if(x2<0){
		return x2;
	}
	x=0;
	longTotal=longTotal+x2;
	long id,w,h,nc,d;
	int origin,comp;
	sscanf(aux,"%d %d %d %d %d %d %d\n",&id,&w,&h,&nc,&d,&origin,&comp);

	if ((id == -1) && (w == -1) && (h == -1) && (nc == 0)&& (d == 0)&& (origin == 0)) // ver si hay que añadir algo aquí
	{
		return END_COMMUNICATION;
	}
	else
	{
		if ((id == 0) && (w == 0) && (h == 0) && (nc == -1) && (d == -1) && (origin == -1))
		{
			return RESET_COMMUNICATION;
		}
	}

	//Creo la instancia que devolveremos
	DiVAImage* pimage = new DiVAImage(w,h,nc,d);
	pimage->setId(id);
	pimage->setVAlign(origin);
	//No sin antes almacenar el buffer de la imagen en ella
	//Primero recibimos la longitud del buffer
	porRecibir =sizeof(long);
	long	x3 = DiVArecv(aux ,porRecibir);
	if(x3<=0)
	{
		if(pimage)
			delete pimage;
		return x3;
	}
	x=0;
	longTotal=longTotal+x3;

	memcpy(&longitud,aux,sizeof(long));
	//Despues el buffer en si

	long x4;
	// Decodificacion
	if (comp == 1)
	{
		char * dataBuffer= (char*)malloc(longitud);
		porRecibir =longitud;
		cronometro->start();
		x4 = DiVArecv(dataBuffer ,porRecibir);		
		duracion = cronometro->stop();
		archivo = fopen("decodificacion.txt","a");
		if(archivo == NULL)
			return -1;
		cronometro->start();
		vector<uchar> aux_buf(dataBuffer, dataBuffer + porRecibir);;		
		error = pimage->decode(aux_buf, 1);
		duracion1 = cronometro->stop();
		fprintf(archivo,"%d;%lf;%lf\n",x4,duracion,duracion1);
		fclose(archivo);
		free(dataBuffer);// recién añadida

	}
	else // La imagen no está codificada
	{
		char * dataBuffer = (char*)pimage->getPixels();
		porRecibir =longitud;
		cronometro->start();
		x4 = DiVArecv(dataBuffer ,porRecibir);
		duracion = cronometro->stop();
		
		archivo = fopen("no_decodificacion.txt","a");
		if(archivo == NULL)
			return -1;
		fprintf(archivo,"%d;%lf\n",x4,duracion);
		fclose(archivo);
	}	


	if(x4<=0)
	{
		printf("DiVAClient::WaitForImage: Conexion perdida\n");
		if(pimage)
			delete pimage;
		return x4;
	}
	x=0;
	*buffer = (void*)pimage;
	longTotal=longTotal+x4;
	delete cronometro;
	return longTotal;
}

int DiVAClient::WaitForDescription(void** buffer)
{
	long longitud =0;
	char aux[512];
	memset(aux,0,512);
	//Espero la recepción de los parametros
	//Primero su longitud
	longitud = 0;
	long	x1 = 0;
	long	x = 0;
	long porRecibir =sizeof(long);
	long longTotal=0;

	x1 = DiVArecv(aux ,porRecibir);
	if(x1<0)
		return -1;
	memcpy(&longitud,aux,sizeof(long));
	longTotal=longTotal+x1;

	//Despues los parametros en si
	porRecibir =longitud;
	long x2= DiVArecv(aux ,porRecibir);
	if(x2<0)
		return -1;
	x=0;
	longTotal=longTotal+x2;
	long id;
	long bufferSize;
		sscanf(aux,"%d %d\n",&id,&bufferSize);

	
	//No sin antes almacenar el buffer de la imagen en ella
	//Primero recibimos la longitud del buffer
	porRecibir =sizeof(long);
	long	x3 = DiVArecv(aux ,porRecibir);
	if(x3<=0)
	{
		return -1;
	}
	x=0;
	longTotal=longTotal+x3;

	memcpy(&longitud,aux,sizeof(long));
	//Creo la instancia que devolveremos
	Description* pdesc = new Description(bufferSize);///////////////////////////////////////////////////////
	pdesc->setId(id);
	//Despues el buffer en si
	char * dataBuffer = (char*)pdesc->getBuffer();
	porRecibir =longitud;
	long	x4 = DiVArecv(dataBuffer ,porRecibir);
	if(x4<=0)
	{
		if(pdesc)
			delete pdesc;
		return -1;
	}
	x=0;
	*buffer = (void*)pdesc;
	longTotal=longTotal+x4;
	return longTotal;
}
int DiVAClient::WaitForId()
{
	int longitud = sizeof(int);
	int porrecibir = longitud;
	//Recibo id
	char aux[512];
	int z= DiVArecv(aux ,512);
	if (z<0)
		return -1;	
	int id=0;
	char aux2[512];
	sscanf(aux,"%d %s",&id,aux2);
	setId(id);
	setSourceId(aux2);
	return id;
}
int DiVAClient::WaitForACK()
{
	//Recibo confirmación
	char aux[512];
	int longitud = strlen("OK\n");
	int porrecibir = longitud;
		int z= DiVArecv(aux ,porrecibir);
	if (z<0)
		return -1;
	return z;
}

int DiVAClient::DiVArecv(char*buffer ,int longitud)
{
	int porRecibir=longitud;
	int x = 0;
	int x1 = 0;
	int error = 0;
	while((porRecibir>0)&(!this->isEnd()))
	{
		
		x1=recv((*((SOCKET*)_pconn)),&buffer[x],porRecibir,0);
		//printf("DiVAClient::DiVArecv: X1=%d----SOCKET_ERROR= %d\n",x1,SOCKET_ERROR);
		if ((x1 == SOCKET_ERROR)||(x1 == 0))
		{	
			error = WSAGetLastError();

			if (error==10054)//WSAECONNRESET, El servidor ha  sido cerrado o ha caido
			{
	#ifdef _DEBUG
				printf("DiVAClient::DiVArecv(), Conexion con frame server perdida, %d .\r\n",error);
	#endif 
				return -1;
			}
			else if (error == WSAETIMEDOUT){
				return CLIENT_TIME_OUT;
			}
			else if(error!=0)
			{

	#ifdef _DEBUG
				printf("DiVAClient::DiVArecv(), Conexion con frame server perdida, %d .\r\n",error);
	#endif 
				return -1;
			}
			

			return -1;
		}
		if (x1>0)
		{
				x = x+x1;
				porRecibir = porRecibir-x1;
		}
		
	}
	return x;
}
BOOL DiVAClient::setEnd()
{ 
	_isEnd=TRUE; //Esto solo lo utiliza, por ahora el DiVAtestClient 
	//Envio un comando al servidor, indicandole quien soy y que mis peticiones han tgerminado.
	char buff[16];
	sprintf(buff,"END\n%d\n",this->getId());//Indico el fin de la COMMUNICATION y mi identificador en 4 caracteres ¡SIEMPRE!
	int z =this->SendCommand(buff);
	if (z<(int)strlen(buff))
	{
		return -1;
	}
	//Despues de que haya terminado el envio, cierro el socket, evitando el bloqueo esperando recepción en el mismo.
	//Es decir, un recv sobre el SOCKET pasará a dar error. El método llamante lo interpretará
	DWORD ExitCode =WSAEINPROGRESS;
	int ret = 0;
	if (_pconn)
	{
		while(ExitCode == WSAEINPROGRESS)
		{
			ret = closesocket(*((SOCKET*)_pconn));
			if (!ret)
				break;
			ExitCode = WSAGetLastError();
		}		
		
		free(_pconn);
		_pconn=NULL;
	}
	return _isEnd;
}

char* DiVAClient::setSourceId(char* sourceId)
{
	if (_sourceId!=NULL)
	{
		free(_sourceId);
		_sourceId=NULL;
	}
	_sourceId = (char*)malloc(strlen(sourceId)+1);
	strcpy(_sourceId,sourceId);
	int i = 0;
	for (i=0;i<(int)strlen(sourceId);i++)//We change certain chars in order to make possible its use for creating a folder
	{
		if((sourceId[i]=='/')||(sourceId[i]=='\\')||(sourceId[i]==':')||(sourceId[i]=='.'))
			_sourceId[i]='_';
	}
	_sourceId[i]=0;
	return _sourceId;
}

/**
*This is a static method shows an example of use of this class
*/
#if 0
int DiVAClient::testFrameClient(char* servername)
{
	if(servername==NULL)
	{		
		return-1;
	}	
	
	DiVAClient cliente(servername);
	BOOL retbool = cliente.Connect();
	if (retbool==FALSE)
	{
		return -1;
	}

	DiVAImage* buffer=NULL;


	long idFrame= 0;
	DiVADisplay *pdisplay =NULL;
	
	int retvalue= 0;
	int ret = 0;
	while (!cliente.isEnd())
	{
		ret = cliente.receiveFrame((void**)(&buffer),idFrame);
		if(ret==SOCKET_ERROR)
		{
			retvalue=-2;
			break;
		}
		if (ret)
		{
		}
		if (!pdisplay)
			pdisplay = new DiVADisplay("testDiVAFrameClient", buffer->getHeight(), buffer->getWidth(),1, 1, 1, 1, true, 1.0, true);
		//Actualizo el identificador de frame para la siguiente petición
		idFrame=buffer->getId()+1;
	pdisplay->insertImage(buffer, 1, 1, "last frame in");
	if (buffer)
			delete buffer;
	buffer = NULL;

	pdisplay->windowVisible();
	}
	
	if (pdisplay)
		delete pdisplay;

	return retvalue;
}



#endif
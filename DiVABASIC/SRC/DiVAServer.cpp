#ifdef _WIN32
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
#include "DiVAServer.h"
#include "DiVACrono.h"
//#include "DiVACaptureAVI.h"
//#include <cv.h>
#include "include\opencv2\highgui\highgui.hpp"
using namespace std;
#endif

//cabeceras para compresion de datos
#include "RunLength.h"
#include "ZLIB32B.h"

#define DESCONEXION 0
#define CONEXION 1
#define COMMAND_LENGTH 64
#ifdef _DEBUG
#define MAX_NUMBER_CLIENTS BUFFERC_MAX_CONSUMIDORES
#else
#define MAX_NUMBER_CLIENTS 25
#endif

//Thread de atención a cada cliente que se conecte
class DiVAThreadClientAttention:public DiVAThread
{
public:
	DiVAThreadClientAttention(LPVOID pparam);
	~DiVAThreadClientAttention();
	int initResources();
	int releaseResources();
	int process();
	PARAMETERS* _pparam;
	int flag_conexion;
};

DiVAThreadClientAttention::DiVAThreadClientAttention(LPVOID pparam)	
{
	_pparam=(PARAMETERS*)pparam;
	char nombre[64];
	char aux[64];
	sprintf(aux,"%d",_pparam->id);
	strcpy(nombre,"DiVAThreadClientAttention ");
	strcat(nombre, aux);
	setName(nombre);
	flag_conexion = CONEXION;
}
DiVAThreadClientAttention::~DiVAThreadClientAttention()
{
	SOCKET client = *((SOCKET*)_pparam->pclientsocket);
	if (_pparam->pclientsocket)
		free((SOCKET*)_pparam->pclientsocket);
	if (_pparam)
		free(_pparam);	
}
int DiVAThreadClientAttention::initResources()
{
		DiVAServer* padre = _pparam->padre;
		SOCKET client	 = *((SOCKET*)_pparam->pclientsocket);	
		char buff[512];
		CString cmd;
		CString params;
		
		strcpy(buff,"OK\n");
		int x = 0;
#ifdef _DEBUG
	//	cout<<"Respondiendo al cliente:\r\n"; No es recomendale imprimir por pantalla desde un thread hijo
#endif
		while(x<=0)
		{
			x = send (client,buff,strlen(buff),0);
			//WSAGetLastError
			if (padre->isEnd()) //Si he cerrado el server
				return -1;
			cvWaitKey(10);
		}	
		return 0;
}
int DiVAThreadClientAttention::releaseResources()
{	
	return 0;
}

//Recordemos que si queremos que el thread finalize, entonces el valor devuelto habrá de ser <0
int DiVAThreadClientAttention::process()
{
		DiVAServer* padre = _pparam->padre;
		SOCKET client	 = *((SOCKET*)_pparam->pclientsocket);	
		char buff[512];
		CString cmd;
		CString params;
		
		int n = 0;

		while(1)
		{
			n=padre->recvCommand(&client,buff);
			if(n==SOCKET_ERROR){	//Si el socket a caido		
				//AfxMessageBox("DivaThreadAttention::process\n");				
				printf("DivaThreadAttention::process --> Socket ERROR\n");
				this->flag_conexion = DESCONEXION;
				padre->setFallos(padre->getFallos()+1);
				return -1;
			}
			else if (n==SERVER_TIME_OUT){ //Si he consumido el timeout, vuelvo ha intentarlo, pero de esta forma compruebo padre->isEnd()
				this->flag_conexion = DESCONEXION;
				printf("DivaThreadAttention::process --> TIMEOUT\n");
				padre->setFallos(padre->getFallos()+1 );
				return SERVER_TIME_OUT;
			}
			else if (n>0)
				break;
		}
		buff[n]=0;
		int ret = padre->processCommands(buff,(PARAMETERS*)_pparam);
		if(ret==-1)
		{	
#if _DEBUG
		//cout<<"DiVAThreadClientAttention::process() Error al procesar el comando\r\n";
#endif
		cout<<"DiVAThreadClientAttention::process() Error al procesar el comando\r\n";
		this->flag_conexion = DESCONEXION;
		return -2;
		}
		else if(ret==1)
		{
			this->flag_conexion = DESCONEXION;
			//processCommands devuelve 1: cliente ha enviado END.
#if _DEBUG
		//cout<<"DiVAThreadClientAttention::process() END\r\n ";
#endif
			return -3;
		}	
	return 0;
}

BOOL SendFrame(SOCKET s, DiVAImage* pimagen);
BOOL SendFrame(SOCKET s, DiVAImage* pimagen,const char* ext, const vector<int> param);
BOOL SendDescription(SOCKET s,Description* pDesc);

BOOL SendFrame(SOCKET s, DiVAImage* pimagen)
{	
	/*
	"<length>" (length of the following line, in long type variable)
	"%<id>\n"
	"<dataBuffer length" (length of the following line, in long type variable)
	"<dataBuffer>"
	*/

	BOOL ret= TRUE;
	//Preparo los parametros que enviaré para que el cliente cree la instancia de DiVAImage
	char aux[512];
	char aux2[512];
	sprintf(aux,"%d %d %d %d %d %d\n",pimagen->getId(),pimagen->getWidth(),pimagen->getHeight(),pimagen->getNChannels(),pimagen->getDepth(),pimagen->getVAlign());	
	//Envio primero la longitud de la cadena de parámetros
	long paramlen = strlen(aux);
	memcpy(aux2,(void*)&paramlen,sizeof(long));
	long longitud = sizeof(long);
	int x=send(s,aux2,sizeof(long),0);

	//Depues los parametros en si
	longitud=paramlen;
	x=send(s,aux,longitud,0);

	//Sin codificar (Antes)
	//Ahora enviaremos el buffer de la imagen
	//Primero la longitud
	long bufferlen = (long)pimagen->getDataSize();
	memcpy(aux2,(void*)&bufferlen,sizeof(long));
	x=send(s,aux2,sizeof(long),0);
	//Depues el buffer en si
	longitud = bufferlen;
	x=send(s,(char*)pimagen->getPixels(),longitud,0);

	// para  guardar timestamp de los frames que se envían
	//FILE *tiempos;
	//tiempos = fopen("time_192.168.11.23.txt","a");
	//fprintf(tiempos,"%lf\n",((double)cvGetTickCount()/(cvGetTickFrequency()*1000.)));
	//fclose(tiempos);
	if (x!=longitud)
	{
		ret= FALSE;
	}
	//El cliente tambien conoce el número de bits que debe esperar ahora. Obviamos la espera del ACK	
	return ret;

}
BOOL SendFrame(SOCKET s, DiVAImage* pimagen,const char* ext, vector<int> param)
{	
	/*
	"<length>" (length of the following line, in long type variable)
	"%<id>\n"
	"<dataBuffer length" (length of the following line, in long type variable)
	"<dataBuffer>"
	*/
	//double duracion;
	//DiVACrono *cronometro = new DiVACrono();
	BOOL ret= TRUE,ret2 = 0;
	//Preparo los parametros que enviaré para que el cliente cree la instancia de DiVAImage
	char aux[512];
	char aux2[512];
	char * ayuda;
	int comp = 1; // 1 para codificadas 0 imágenes sin codificar.
	FILE * archivo;

	if (!strcmp(ext,"NULL"))
		comp = 0;

	sprintf(aux,"%d %d %d %d %d %d %d\n",pimagen->getId(),pimagen->getWidth(),pimagen->getHeight(),pimagen->getNChannels(),pimagen->getDepth(),pimagen->getVAlign(),comp);	
	//Envio primero la longitud de la cadena de parámetros
	long paramlen = strlen(aux);
	memcpy(aux2,(void*)&paramlen,sizeof(long));
	long longitud = sizeof(long);
	int x=send(s,aux2,sizeof(long),0);
	//Depues los parametros en si
	longitud=paramlen;
	x=send(s,aux,longitud,0);
	vector<uchar> buffer;

	//Codificacion
	//cronometro->start();
	ret2 = pimagen->encode(ext,param,buffer);
	ayuda = reinterpret_cast<char*>(&buffer[0]);

	//duracion = cronometro->stop();

	if (ret2 == -1)
	{
		printf("Error en la codificacion.\n");
		return ret2;
	}

	//Ahora enviaremos el buffer de la imagen
	//Primero la longitud
	long bufferlen = buffer.size();//(long)sizeof(buffer);
	memcpy(aux2,(void*)&bufferlen,sizeof(long));
	x=send(s,aux2,sizeof(long),0);
	//Depues el buffer en si
	longitud = bufferlen;

	//archivo = fopen("tiempos codificacion.txt","a");
	//if(archivo == NULL)
	//	return -1;

	//Codificacion

	x=send(s,ayuda,longitud,0);
	
	//fprintf(archivo,"%lf\n",duracion);
	//fclose(archivo);

	if (x!=longitud)
	{
		ret= FALSE;
	}
	//El cliente tambien conoce el número de bits que debe esperar ahora. Obviamos la espera del ACK	
//	delete cronometro;
	return ret;

}
BOOL SendDescription(SOCKET s,Description* pDesc)
{	
	/*
	"<length>" (length of the following line, in long type variable)
	"%<id> <bufferSize>\n"
	"<dataBuffer length" (length of the following line, in long type variable)
	"<dataBuffer>"
	*/
	BOOL ret= TRUE;
	//Preparo los parametros que enviaré para que el cliente cree la instancia de DiVAImage
	char aux[512];
	char aux2[512];
	sprintf(aux,"%d %d\n",pDesc->getId(),pDesc->getSize());	
	//Envio primero la longitud de la cadena de parámetros
	long paramlen = strlen(aux);
	memcpy(aux2,(void*)&paramlen,sizeof(long));
	long longitud = sizeof(long);
	int x=send(s,aux2,sizeof(long),0);

	//Depues los parametros en si
	longitud=paramlen;
	x=send(s,aux,longitud,0);

	//Ahora enviaremos el buffer de la descripción en si.
	//Primero la longitud
	long bufferlen = (long)pDesc->getSize();
	memcpy(aux2,(void*)&bufferlen,sizeof(long));
	x=send(s,aux2,sizeof(long),0);
	//Depues el buffer en si
	longitud = bufferlen;		
	x=send(s,(char*)pDesc->getBuffer(),longitud,0);
	
	if (x!=longitud)
	{
		ret= FALSE;
	}
	//El cliente tambien conoce el número de bits que debe esperar ahora. Obviamos la espera del ACK	
	return ret;

}

BOOL SendKeepAlive(SOCKET s)
{	
	/*
	"<length>" (length of the following line, in long type variable)
	"%<id>\n"
	"<dataBuffer length" (length of the following line, in long type variable)
	"<dataBuffer>"
	*/
	char aux[COMMAND_LENGTH + 1];
	memset(aux,0,COMMAND_LENGTH+1);
	strcpy(aux,"YES");
	
	int z=send(s,aux,COMMAND_LENGTH,0);
	return z;
	
}

/**
*Constructor
*\param source Buffer from which I will read the desired data. i.e. frames.
*\param portNumber The port number in which I will be waiting for connections.
*/
DiVAServer::DiVAServer(DiVAImageBuffer* source,DescriptionBuffer* descSource,int portNumber)
{
	if ((source!=NULL)&&(descSource!=NULL))
	{
		printf("DiVAServer::DiVAServer==> Only one kind of buffer its allowed at ine time.");
		return;
	}
	
	char nombre[64];
	char aux[64];
	sprintf(aux,"%d",portNumber);
	strcpy(nombre,"DiVAServer ");
	strcat(nombre, aux);
	setName(nombre);

	_nclients = 0;
	this->_fallos = 0;
	_framesServed = 0;
	_source=source;
	_descSource = descSource;
	_portNumber=portNumber;
	_end=FALSE;
	_clientTable = (void*)malloc(sizeof(DiVAThreadClientAttention*)*MAX_NUMBER_CLIENTS);
	memset(_clientTable,0,sizeof(DiVAThreadClientAttention*)*MAX_NUMBER_CLIENTS);
	_pserver = NULL;	
}
/**
*Destructor
*/
DiVAServer::~DiVAServer()
{
	this->stop();
	//releaseAllClients();
#if _DEBUG
	printClientsInfo();
#endif

	if (_clientTable)
		free(_clientTable);
	if (_pserver)
		free((SOCKET*)_pserver);
}

int DiVAServer::printClientsInfo()
{
	int contador=0;
	if (_clientTable==NULL)
		return -1;
	DiVAThreadClientAttention* pClient=NULL;
	for(int i=0;i<MAX_NUMBER_CLIENTS;i++)
	{
		pClient=((DiVAThreadClientAttention**)_clientTable)[i];
		if (pClient!=NULL)
		{
			contador++;
			printf ("DiVAServer::printClientsInfo()=> Id: %02d Active: %d Memory Pos: %p Thread Pointer: %p\n",i, pClient->isActive(),pClient,pClient->getThreadPointer());
		}
	}
	if (contador == 0)
		printf("DiVAServer::printClientsInfo()=>  NO HAY CLIENTES ACTIVOS\n");
	return 0;
}
/**
*This method stops the execution of the attending threads and, callind DiVAThread::stop(),
*	the listening thread.
*/
int DiVAServer::stop()
{
	int ret;
	//Cierro el servidor de escucha
	_end=TRUE;
	if (this->_pserver)
	{
		closesocket(*((SOCKET*)this->getServerSocket()));
	}

	//Activo el flag de finalización para que los threads que atienden a los distintos clientes terminen
#ifdef _DEBUG
	cout << "Los threads clientes estan en proceso de finalizacion.\r\n";
#endif
	int intentos=0;
	ret = WSACleanup();
	while (ret && (intentos < 100))
	{
#ifdef _DEBUG
	cout << "Intentando terminar el uso de sockets con WSACleanup()";
#endif
		ret = WSACleanup();
		cvWaitKey(10);
		intentos++;
	}
#ifdef _DEBUG
	cout << "Finalizacion correcta del uso de sockets.\r\n";
#endif

	ret = releaseAllClients();//I stop the attention thread and I free the attention threads.	
	ret = DiVAThread::stop(); //I stop the listening thread

	return ret;

}

int DiVAServer::process()
{

	//YO
	int result;
	struct fd_set readset;
	struct timeval tv;

	sockaddr_in from;
	int fromlen=sizeof(from);
	PARAMETERS*  pParametros=NULL;
	SOCKET server = *((SOCKET*)this->getServerSocket());
	SOCKET client;

	//MODIFICACION: YO
	FD_ZERO(&readset);
	FD_SET(server, &readset);
	maxfd = server;

	tv.tv_sec = 3;
	tv.tv_usec = 0;

	result = select(maxfd, &readset, NULL, NULL, &tv);
	//printClientsInfo();
	//Sleep(3000);
	if (_end != TRUE){
		//printf("Puerto: %d\n",this->_portNumber);
		if (result == 0) {
			//printf("select() timed out!\n");
			DiVAThreadClientAttention* pClientAux=NULL;
			for (int i=0; i<MAX_NUMBER_CLIENTS; i++)
			{
				pClientAux=((DiVAThreadClientAttention**)_clientTable)[i];
				if((pClientAux)&&(pClientAux->flag_conexion == DESCONEXION))
				{				
					releaseId(pClientAux->_pparam->id);
					releaseClient(pClientAux->_pparam->id);
				}
			}
			return 0;
		}
		else if (result < 0) {
			//printf("Error in select(): %s\n", strerror(errno));
			return -1;
		}
		else if (result > 0) {

			if (FD_ISSET(server, &readset)) {

				client=accept(server,(struct sockaddr*)&from,&fromlen);
				if (client < 0) {
					//printf("Error in accept(): %s\n", strerror(errno));
					return 0;
				}
				else printf("Conexion con cliente\n");
			}
			else {
				//printf("Error in FD_ISSET:\n");
				return 0;
			}

		}
	}
	else return -1;
	//MODIFICACION: FIN_YO

	//MODIFICACION: ANTERIOR
	//client=accept(server,(struct sockaddr*)&from,&fromlen);

	if (client==INVALID_SOCKET){
		return 0;
	}
	//Reservo aquí la memoria, pero TIENE que ser liberada en ClientThread 
	pParametros = (PARAMETERS*)malloc(sizeof(PARAMETERS));
	pParametros->padre			= this;		//Me dará acceso al buffer del que obtendré los datos a enviar
	pParametros->pclientsocket	= (void*) malloc(sizeof(SOCKET));
	*((SOCKET*)pParametros->pclientsocket)	= client;	//El socket del que el hilo tomará las request y dará las respuestas
	BOOL bOptVal = 10000;
	int bOptLen = sizeof(int);
	setsockopt(*((SOCKET*)pParametros->pclientsocket), SOL_SOCKET,SO_RCVTIMEO,(char*)&bOptVal,bOptLen);

	pParametros->id 			=-1;		//Se le asígnará más adelante, cuanto abramos el acceso a nuestro buffer.
	//Lanzo el thread que atenderá al cliente
	DiVAThreadClientAttention* patencion = new DiVAThreadClientAttention((LPVOID)pParametros);
	int ret = addClient((void*)patencion);
	if (ret>=0)
		patencion->start();	
	return 0;
}
int DiVAServer::initResources()
{
	//unsigned long mod=1;
	WSADATA wsaData;
	sockaddr_in local;
	int wsaret=WSAStartup(MAKEWORD(2,0),&wsaData);
	if(wsaret!=0)
	{
		return 0;
	}
	local.sin_family=AF_INET;
	local.sin_addr.s_addr=INADDR_ANY;
	u_short portNumber = this->getPortNumber();
	local.sin_port=htons(portNumber);
	SOCKET server=socket(AF_INET,SOCK_STREAM,0);
	//WSAAsyncSelect (server, NULL, 0 ,0);
	//WSAEventSelect (server, NULL, 0); 
	//ioctlsocket (server, FIONBIO, &mod);

	if(server==INVALID_SOCKET)
	{
		return 1;
	}
	if(bind(server,(sockaddr*)&local,sizeof(local))!=0)
	{
		return 1;
	}
	if(listen(server,10)!=0)
	{
		return 1;
	}

   //int PASCAL FAR setsockopt (SOCKET s, int level, int optname,
   //                      const char FAR * optval, int optlen);
   //                      */
   BOOL bOptVal = TRUE;
   int bOptLen = sizeof(BOOL);
   setsockopt(server,IPPROTO_TCP,TCP_NODELAY,(char*)&bOptVal,bOptLen);//TCP_NODELAY

	this->setServerSocket(&server);
	return 0;
}
int DiVAServer::releaseResources()
{
	return 0;
}
	

DiVAImage* DiVAServer::getFrame(long idFrame, long idConsumidor, void* source)
{
	DiVAImageBuffer* sourceAux = (DiVAImageBuffer*)source;
	if (source==NULL)
	{
		if (_source!=NULL)
			sourceAux = ((DiVAImageBuffer*)_source);
		else
			return NULL;
	}
	//printf("\nDiVAServer::getFrame => Nombre del imagebuffer: %s\n",sourceAux->getSourceId());
	//getSampleElement reserva menoria para la nueva imagen. Es responsabilidad del que llama 
	// a DiVAServer::getFrame liberar esa memoria
	DiVAImage*  pImage = (DiVAImage*)sourceAux->getSampleElement();

	//Extraigo el frame del buffer
	long ret = BUFFERC_EMPTY;
	long intentos = 0;
	while ((ret == BUFFERC_EMPTY)&&(!isEnd()) && (intentos < 4)){
		//printf("Depuracion %ld\n", depuracion++);
		intentos++;
		ret = sourceAux->get(pImage,idFrame,idConsumidor);
	}
	if (ret!=BUFFERC_OK)
	{
		//Si no recupero un frame, imprimo el error y retorno un frame NULL
		sourceAux->printError(ret);
#ifdef _DEBUG
		sourceAux->printError(ret);
#endif
		sourceAux->printError(ret);
		return NULL;
	}
#ifdef _DEBUG
	if ((idFrame!=pImage->getId())&(idFrame!=0))
		printf("DiVAServer::getFrame: Cliente: %d. Frame solicitado/devuelto: %d / %d\r\n", idConsumidor,idFrame, pImage->getId());
#endif
	return pImage;
	
}

int		DiVAServer::getId()
{
	int idimages = -1;
	int iddescriptions = -1;

	if (_source!=NULL)
	{
		DiVAImageBuffer* sourceAux = ((DiVAImageBuffer*)_source);
		idimages=sourceAux->registrarConsumidor();
		return idimages;
	}
	if (_descSource!=NULL)
	{
		DescriptionBuffer* sourceAux = ((DescriptionBuffer*)_descSource);
		iddescriptions=sourceAux->registrarConsumidor();
		return iddescriptions;
	}
	return -1;
}
int	DiVAServer::releaseId(long idClient,void* source)
{
	if (_source!=NULL)
	{
		DiVAImageBuffer* sourceAux = ((DiVAImageBuffer*)_source);
		sourceAux->eliminarConsumidor(idClient);
	}
	if (_descSource!=NULL)
	{
		DescriptionBuffer* sourceAux = ((DescriptionBuffer*)_descSource);
		sourceAux->eliminarConsumidor(idClient);
	}
	return 0;
}

void*	DiVAServer::setServerSocket(void* pserver)
{
	if(_pserver==NULL)
		_pserver = (void*)malloc(sizeof(SOCKET*));
	*((SOCKET*)_pserver)=*((SOCKET*)pserver);
	return _pserver;
}

int	DiVAServer::addClient(void* pclient)
{
	int i = 0;
	DiVAThreadClientAttention* pClientAux=NULL;
	for(i=0;i<MAX_NUMBER_CLIENTS;i++)
	{
		pClientAux=((DiVAThreadClientAttention**)_clientTable)[i];
		//pClientAux=(((DiVAThreadClientAttention*)_clientTable)[i]);
		if (pClientAux)
		{
			if (pClientAux->isActive())
				continue;
			else
				delete pClientAux;
		}
				
		DiVAThreadClientAttention** paux = (DiVAThreadClientAttention**)_clientTable;
		paux[i]= (DiVAThreadClientAttention*)pclient;
		_nclients++;
		break;
	}
	if (i>=MAX_NUMBER_CLIENTS)
		return -1;
	else
		return i;
}

int DiVAServer::activaFlagDesconexion(int _idclient)
{
	int i = 0;
	DiVAThreadClientAttention* pClientAux=NULL;
	for(i=0;i<MAX_NUMBER_CLIENTS;i++)
	{
		pClientAux=((DiVAThreadClientAttention**)_clientTable)[i];
		if(pClientAux)
		{
			if (_idclient==pClientAux->_pparam->id)				
			{
				pClientAux->flag_conexion = DESCONEXION;
				return 0;
			}
		}		
	}
	return -1;	
	
}

int	DiVAServer::releaseClient(int _idclient)
{
	int i = 0;
	DiVAThreadClientAttention* pClientAux=NULL;
	for(i=0;i<MAX_NUMBER_CLIENTS;i++)
	{
		pClientAux=((DiVAThreadClientAttention**)_clientTable)[i];
		if(pClientAux)
		{
			if (_idclient==pClientAux->_pparam->id)				
			{
				closesocket(*((SOCKET*)pClientAux->_pparam->pclientsocket));//Cierro el socket ¡¡ESTO INDICARÁ QUE EL THREAD HA DE FINALIZAR!! NO HACERLO ANTES
				//NO PUEDO EJECUTAR LLAMAR AL STOP DEL CLIENTE PORQUE ESTOY EN SU MISMO THREAD.
				//AL CERRAR EL SOCKET TERMINARÁ FINALIZANDO. LO COMPROBARE EN EL DESTRUCTOR DEL
				//SERVIDOR(CON releaseAllClients()), QUE SERÁ LLAMADO DESDE OTRO THREAD DISTINTO AL DE LOS CLIENTES.
					//delete pClientAux;
					//DiVAThreadClientAttention** paux = (DiVAThreadClientAttention**)_clientTable;
					//paux[i]= NULL;
					//return 0;
					//I wait till it is finished
				pClientAux->stop();
				_nclients--;
				delete pClientAux;
				((DiVAThreadClientAttention**)_clientTable)[i]=NULL;
				return 0;
			}
		}		
	}
	return -1;
}
int	DiVAServer::releaseAllClients()
{
	printClientsInfo();
	int i = 0;
	DiVAThreadClientAttention* pClient=NULL;
	for(i=0;i<MAX_NUMBER_CLIENTS;i++)
	{
		pClient=((DiVAThreadClientAttention**)_clientTable)[i];
		if (pClient)
		{
			//printf("Detengo la atencion %d\n",i);

			#if _DEBUG
				printf("\n");
				printClientsInfo();
			#endif

			releaseClient(pClient->_pparam->id);
			/*pClient->stop();
			_nclients--;
			delete pClient;
			DiVAThreadClientAttention** paux = (DiVAThreadClientAttention**)_clientTable;
			paux[i]= NULL;*/
		}		
		//pClient=NULL;
	}
	return 0;
}

/**
*Method on charge of processing each received command
*\param str Text string received from client
*\param parametros A pointer to a structure containig:
*		-	The DiVAServer instance from which I call this thread. 
*		-	The socket handler for having communication with the client
*\return1 It returns:
*		0 if OK
*		1 if END
*		-1 if error
*/

int	DiVAServer::processCommands(char *str,void* parameters)
{
	PARAMETERS* parametros = (PARAMETERS*)parameters;
	int ret = 0;
	TCHAR eol='\n';
	int index	=0;
	char command[512];
	char auxStr[512];
	//Busco el final de la primera línea
	for (index=1;index<(int)strlen(str);index++)
	{
		if (str[index]=='\n')
			break;
	}
	strcpy(command,str);
	command[index]='\0';
	
//Ahora parsearemos los parametros dependiendo de cada parámetro.

	//Obtengo el socket del cliente de este hilo
	SOCKET		cliente = *((SOCKET*)(parametros->pclientsocket));
	DiVAServer*	padre	 = parametros->padre;

	/*COMANDO FRAME****************************************************************/
	if (!strcmp(command,"FRAME"))
	{


		//Recupero el frame "id" de la camara/fichero "source",
		long idFrame;
		long idClient;
		sscanf(&str[index],"%d %d\r\n",&idFrame,&idClient);
		//Recupero la imagen	
		DiVAImage* pimage= padre->getFrame(idFrame,idClient);
		//printf("Peticion de FRAME\n");
		//Envio la imagen por el socket
		if (pimage!=NULL)
		{
			vector<int> param;
			BOOL retaux;
			switch(this->_TypeCod)
			{
			case 1:
				param.push_back(CV_IMWRITE_JPEG_QUALITY);
				param.push_back(100);
				retaux = SendFrame(cliente,pimage,".jpg", param);
				break;
			case 2:
				param.push_back(CV_IMWRITE_JPEG_QUALITY);
				param.push_back(90);
				retaux = SendFrame(cliente,pimage,".jpg", param);
				break;
			case 3:
				param.push_back(CV_IMWRITE_JPEG_QUALITY);
				param.push_back(50);
				retaux = SendFrame(cliente,pimage,".jpg", param);
				break;
			case 4:
				param.push_back(CV_IMWRITE_PNG_COMPRESSION);
				param.push_back(2);
				retaux = SendFrame(cliente,pimage,".png", param);
				break;
			case 5:
				param.push_back(CV_IMWRITE_PNG_COMPRESSION);
				param.push_back(8);
				retaux = SendFrame(cliente,pimage,".png", param);
				break;
			case 0:
			default:
				retaux = SendFrame(cliente,pimage);
				break;
			}
			if (retaux==FALSE){
				printf("DiVAServer::processCommands: Error al enviar frame\n");
				ret=-1;
			}
			else {
				delete pimage;//Despues de enviar la imagen, la libero
				_framesServed++;
			}
		}
		else
		{
		//Libero al cliente
		#ifdef	_DEBUG
		cout<<"DiVAServer::processCommands: Timeout of client: " <<idClient<<" releasing it.\r\n";
		#endif
		printf("DiVAServer::processCommands: Error al capturar frame\n");
		//padre->releaseId(idClient);
		//Pongo en el buffer un nuevo frame
		//padre->releaseClient(idClient);
		activaFlagDesconexion(idClient);
		ret=-1;//No lo he enviado
		}
		
	}
	/*COMANDO ID_REQUEST****************************************************************/
	else if (!strcmp(command,"ID_REQUEST"))
	{
		//printf("Peticion de ID\n");
		int id= padre->getId();
		//sprintf(auxStr,"%d %s",id,((DiVAImageBuffer*)_source)->getSourceId());
		if (_source!=NULL)
			sprintf(auxStr,"%d %s",id,((DiVAImageBuffer*)_source)->getSourceId());
		else if (_descSource!=NULL)
			sprintf(auxStr,"%d %s",id,((DescriptionBuffer*)_descSource)->getSourceId());
		parametros->id=id;
#ifdef	_DEBUG
		cout<<"Identificador asignado a cliente: " <<id<<"\r\n";
#endif
		int x=send(cliente,auxStr,512,0);
		if (x<0)
			ret=-1;
	}	
		else if (!strcmp(command,"END"))
	{	

		long idClient=-1;
		sscanf(&str[index],"%d\n",&idClient);
		//cout<<"DiVAServer::processCommands el cliente " <<idClient<<" pide END.\r\n";
#ifdef	_DEBUG

#endif
		printf("Cliente solicita desconexion\n");
		//Activo el flag de conexion de la atencion al cliente a DESCONEXION
		if (activaFlagDesconexion(idClient) == -1){
			printf("Cliente desconocido solicita desconexion\n");
			return -1;
		}
		ret =1;
#ifdef	_DEBUG
			cout<<"DiVAServer::processCommands END procesado.\r\n";
#endif
	}
	if (!strcmp(command,"DESCRIPTOR"))//Added for permitting the handling of DESCRIPTIONS> Queries
	{
				//Recupero el descriptor "name", el frame "id" de la camara/fichero "source",
		char descName[64];
		long idFrame;
		long idClient;
		sscanf(&str[index],"%s\n",descName);
		sscanf(&str[index],"%d %d\r\n",&idFrame,&idClient);
		//Recupero la descripción
		Description* description = this->getDescription(idFrame,idClient);
		if (description==NULL)
			return -1;
		//Envio la descripción por el socket
		
		if ((description->getBuffer()!=NULL)&&(description->getSize()>0))
		{
			BOOL retaux = SendDescription(cliente,description);
			if (retaux==FALSE)
				ret=-1;
			else
				delete description;//Despues de enviar el buffer lo libero
		}
		else
		{
		}
	}
	/*COMANDO KEEP_ALIVE ****************************************************************/
	else if (!strcmp(command,"KEEP_ALIVE"))
	{
		//Sleep(4000);
		ret = SendKeepAlive(cliente);
	}
	if(((strcmp(command,"FRAME"))&&(strcmp(command,"ID_REQUEST"))&&(strcmp(command,"END")&&strcmp(command,"DESCRIPTOR"))&&(strcmp(command,"KEEP_ALIVE"))))
	{
#ifdef	_DEBUG
			cout<<"DiVAServer::processCommands Comando desconocido: "<<command<<" .\r\n";
#endif
		ret = -1;
	}
	return ret;

}


int DiVAServer::recvCommand(void* pclient,char*buffer)
{
	SOCKET client= *((SOCKET*)pclient);
	int porRecibir=COMMAND_LENGTH;
	int x = 0;
	int x1 = 0;
	int error = 0;
	while((porRecibir>0)&(!this->isEnd()))
	{
		x1=recv(client,&buffer[x],porRecibir,0);
		error = WSAGetLastError();
		if (error==10038)//WSAENOTSOCK, El socket ha sido cerrado. El cliente ha solicitado la finalizacion.
		{
#ifdef _DEBUG
			printf("DiVAServer::recvCommand, Conexion con cliente cerrada.\r\n");
#endif 
			printf("DiVAServer::recvCommand, Conexion con cliente cerrada.\r\n");
			return -1;
		}
		if (error==10054)//WSAECONNRESET, El servidor ha  sido cerrado o ha caido
		{
#ifdef _DEBUG
			printf("DiVAServer::recvCommand, Conexion con cliente perdida, %d .\r\n",error);
#endif 
			return -1;
		}
		else if (error == WSAETIMEDOUT)
			return SERVER_TIME_OUT;
		else if (error!=0)
		{
#ifdef _DEBUG
			printf("DiVAServer::recvCommand, Conexion con cliente perdida, %d .\r\n",error);
#endif 
			printf("DiVAServer::recvCommand, Conexion con cliente perdida, %d .\r\n",error);
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

Description* DiVAServer::getDescription(long idFrame, long idConsumidor, void* source)
{
	DescriptionBuffer* sourceAux = (DescriptionBuffer*)source;
	if (sourceAux ==NULL)
		sourceAux = _descSource;
	//getSampleElement reserva menoria para la nueva imagen. Es responsabilidad del que llama 
	// a DiVAServer::getFrame liberar esa memoria
	Description*  pDesc = (Description*)sourceAux->getSampleElement();//New memoy alloced for this purpose

	//Extraigo el frame del buffer
	long ret = BUFFERC_EMPTY;
	while ((ret == BUFFERC_EMPTY)&&(!isEnd()))	
		ret = sourceAux->get(pDesc,idFrame,idConsumidor);
	if (ret!=BUFFERC_OK)
	{
		//Si no recupero un frame, imprimo el error y retorno un frame NULL
#ifdef _DEBUG
		sourceAux->printError(ret);
#endif
		delete pDesc;
		return NULL;
	}
#ifdef _DEBUG
	if ((idFrame!=pDesc->getId())&(idFrame!=0))
		printf("DescriptionsServer::getDescription: Cliente: %d. Desc. solicitada/devuelta: %d / %d\r\n", idConsumidor,idFrame, pDesc->getId());
#endif
	return pDesc;
	
}


int DiVAServer::sendEndClient(void *s){
	
	/* Para no modificar en exceso el protocolo, END se enviará en los parámetros */
	//Preparo los parametros que enviaré para que el cliente cree la instancia de DiVAImage
	char aux[512];
	char aux2[512];
	sprintf(aux,"%d %d %d %d %d %d\n",-1,-1,-1,0,0,0);
	//Envio primero la longitud de la cadena de parámetros
	long paramlen = strlen(aux);
	memcpy(aux2,(void*)&paramlen,sizeof(long));
	long longitud = sizeof(long);
	int x=send(*((SOCKET*)s),aux2,sizeof(long),0);

	//Depues los parametros en si
	longitud=paramlen;
	x=send(*((SOCKET*)s),aux,longitud,0);

	return x;

}

int DiVAServer::sendResetClient(void  *s){
	
	/* Para no modificar en exceso el protocolo, RESET se enviará en los parámetros */
	//Preparo los parametros que enviaré para que el cliente cree la instancia de DiVAImage
	char aux[512];
	char aux2[512];
	sprintf(aux,"%d %d %d %d %d %d\n",0,0,0,-1,-1,-1);
	//Envio primero la longitud de la cadena de parámetros
	long paramlen = strlen(aux);
	memcpy(aux2,(void*)&paramlen,sizeof(long));
	long longitud = sizeof(long);
	int x=send(*((SOCKET*)s),aux2,sizeof(long),0);

	//Depues los parametros en si
	longitud=paramlen;
	x=send(*((SOCKET*)s),aux,longitud,0);

	return x;

}

void DiVAServer::sendEndAllClients(){
	
	int i = 0;
	DiVAThreadClientAttention* pClientAux=NULL;
	for(i=0;i<MAX_NUMBER_CLIENTS;i++)
	{
		pClientAux=((DiVAThreadClientAttention**)_clientTable)[i];
		if(pClientAux){
			sendEndClient(pClientAux->_pparam->pclientsocket);
			Sleep(10);
		}
		
	}
	//releaseAllClients();
	return;
}

void DiVAServer::sendResetAllClients(){
	
	int i = 0;
	DiVAThreadClientAttention* pClientAux=NULL;
	for(i=0;i<MAX_NUMBER_CLIENTS;i++)
	{
		pClientAux=((DiVAThreadClientAttention**)_clientTable)[i];
		if(pClientAux)
			sendResetClient(pClientAux->_pparam->pclientsocket);
			Sleep(10);
		
	}
	//releaseAllClients();
	return;
}


void DiVAServer::setTypeCodification(int cod){
	this->_TypeCod = cod;
}
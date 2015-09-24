#include "DiVAAlgorithm.h"
#include "DiVAImage.h"


using namespace std;
/**
*Constructor
* \param frameServername Frame server ip address
* \param portnumber Port number
* \param fileDumping Flag for dumping results into files
* \param display Flag for showing resulting images in a display
* \param dataServerName Data server ip address
* \param contentServerName Content server ip address
* \param dataServerDumping Flag for dumping results into the Data Server
* \param contentServerDumping Flag for dumping results into Content Server
*/
DiVAAlgorithm::DiVAAlgorithm(char* frameServername, int portnumber,
			BOOL fileDumping,
			BOOL display,
			char* dataServerName,
			char* contentServerName,
			BOOL dataServerDumping,
			BOOL contentServerDumping,
			int captureMode,
			BOOL displayQt)
{
	if (frameServername)
	{
		_frameServername = new char[strlen(frameServername)+1];
		strcpy(_frameServername,frameServername);
	}
	else
		return;
	_portnumber = portnumber;
	_fileDumping=fileDumping;
	_display = display;
	_displayQt = displayQt;

	if (dataServerName)
	{
		_dataServerName = new char[strlen(dataServerName)+1];
		strcpy(_dataServerName,dataServerName);
	}
	else
		_dataServerName=NULL;

	if (contentServerName)
	{
		_contentServerName = new char[strlen(contentServerName)+1];
		strcpy(_contentServerName,contentServerName);
	}
	else
		_contentServerName = NULL;


	_dataServerDumping		= dataServerDumping;
	_contentServerDumping	= contentServerDumping;
	_algorithmName		= new char[128];
	_algorithmName[0]	=0;	
	setAlgorithmName("Visualization Algorithm");
	_pFrameClient	= NULL;
	_pFrameBuffer	= NULL;
	_pDisplay		= NULL;	
	_pDataClient	= NULL;
	_pCrono=new DiVACrono();
	_dumpingpath 	 = NULL;	
		_dumpingpath = new char[512];
		_dumpingpath[0]=0;

	_dumpingpathRoot = new char[512];
	_dumpingpathRoot[0]=0;
	
	
	setLastId((long)0);
	_captureMode = captureMode;
	hMutex = CreateMutex( NULL, FALSE, NULL );  
}
/**
*Destructor
*/
DiVAAlgorithm::~DiVAAlgorithm()
{
	if (_frameServername)
		delete [] _frameServername;

	if (_dataServerName)
		delete [] _dataServerName;

	if (_contentServerName)
		delete [] _contentServerName;

	if (_pFrameClient)
		delete _pFrameClient;


	if (_pDisplay)
		delete _pDisplay;
	if (_algorithmName)
		delete [] _algorithmName;
	if(_dumpingpath)
		delete[] _dumpingpath;
	if (_dumpingpathRoot)
		delete[] _dumpingpathRoot;
	if(_pCrono)
		delete _pCrono;	
	if (_pFrameBuffer)
		delete _pFrameBuffer;


	CloseHandle(hMutex);
}
/**
*This method sets the name of the algorithm.
*It is recomended to be used in the constructor method in the children classes.
* \param name The name. Not longer than 128 characters.
* \return The name asigned.
*/
char* DiVAAlgorithm::setAlgorithmName(char* name)
{
	if(name)
	{
		if(strlen(name)<=128)
			strcpy(_algorithmName,name);
	}
	return _algorithmName;
}

/**
*This method initialize and connect the client
* 
* \return "0" if OK
*/
char* DiVAAlgorithm::getSourceId()
{
	switch(_captureMode){
		case CAPTURE_SEQ:	
			return _pFrameClient->getSourceId();
			break;

		case CAPTURE_BUF:		
			return _pFrameBuffer->getSourceId();
			break;

		default:
			return _pFrameClient->getSourceId();
		}
	return NULL;

}

int DiVAAlgorithm::init()
{
	// Creo el frame client

	switch(_captureMode){
		case CAPTURE_SEQ:	
			_pFrameClient = new DiVAClient(_frameServername,_portnumber);
			break;

		case CAPTURE_BUF:		
			_pFrameBuffer = new DiVAClientBuffer(_frameServername,_portnumber);
			break;

		default:
			_pFrameClient = new DiVAClient(_frameServername,_portnumber);
	}
	/*
	En un futuro crearé el resto de clientes
	*/
	// Me conecto
	int ret = connect();
	if (ret<0)
		return ret;
	//Creo el path en el volcaré los ficheros resultado si procede.
	char command[512];
 	command[0]=0;
	strcpy(_dumpingpath,"../output/");
	
	if (getSourceId())
	{
		strcat(_dumpingpath,getSourceId());
	}
		
	if (_fileDumping)
	{
		strcpy(command,"mkdir \"");
		strcat(command,_dumpingpath);
		strcat(command,"\"");				
		int ret = system(command); //Creo el directorio
	}

	strcpy(_dumpingpathRoot,_dumpingpath);

	strcat(_dumpingpath,"/");
	
	strcat(_dumpingpath,getAlgorithmName());
	if (_fileDumping)
	{
		strcpy(command,"mkdir \"");
		strcat(command,_dumpingpath);
		strcat(command,"\"");
		ret = system(command);
		strcat(_dumpingpath,"/");		
	}
	return ret;

}


int DiVAAlgorithm::stop()
{		

	WaitForSingleObject(hMutex,ESPERA_MUTEX);
	if (_captureMode == CAPTURE_BUF)		
		_pFrameBuffer->stop();			

	int ret = DiVAThread::stop();
	ReleaseMutex(hMutex);
	return ret;
}


/**
*This method connect the clients to their server if it is possible and it is configured so.
* 
*\return "0" if OK
*/
int DiVAAlgorithm::connect()
{
	int ret = -1;
	ret = connect2FrameServer();
	if (ret<0)
		return ret;
	ret = connect2DataServer();
	if (ret<0)
		return ret;
	ret = connect2ContentServer();
	if (ret<0)
		return ret;
	return ret;
}
int DiVAAlgorithm::connect2FrameServer()
{
	int ret = -1;
	switch(_captureMode){

		case CAPTURE_SEQ:
			if (_pFrameClient)
				ret = _pFrameClient->Connect();
			break;

		case CAPTURE_BUF:
			if (_pFrameBuffer)
				ret = _pFrameBuffer->start();
			break;
			
		default:
			if (_pFrameClient)
				ret = _pFrameClient->Connect();
			break;
	}

	return ret;
}

/**
*This method receives a frame using the frame client, attribute of this class.
* 
*\return The size of the received frame if OK. Error code if something goes wrong
*/
int DiVAAlgorithm::receiveFrame(DiVAImage** pImage,long idFrame)
{
	DiVAImage* buffer=NULL;
	int ret = _pFrameClient->receiveFrame((void**)(&buffer),idFrame);
	if(ret<0)
	{
		return ret;
	}
	*pImage = buffer;
	return ret;
}

int DiVAAlgorithm::process()
{
		int ret = 0;
		DiVAImage*	pImage			= NULL;
		void*		pData			= NULL;
		void*		pContentData	= NULL;
		DiVACrono* pCrono = this->getCrono();
#ifdef _DEBUG
		long	trcv,tdataserver,tcontentserver,tprocess,tdumpDataServer,tdumpContentServer,tdumpInFiles,tDisplay=0;
#endif 
		flush(cout);
#ifdef _DEBUG
			pCrono->start();
#endif 
		/*Recibo frame****************************************************************/
			WaitForSingleObject(hMutex,ESPERA_MUTEX);
			if (!isEnd())
			{

				long id = getLastId();
				//DEPURACION RECONEXIONES (detenemos a propósito la petición de frames)
				//if ((this->getLastId() != 0) &&(this->getLastId()%100 == 0)){
				//	Sleep(2100);
				//	this->setLastId(this->getLastId() + 1);
				//}
				//printf("DIVAALgorithm::process frame pedido\n");
				//FIN DEPURACIÓN DESCONEXIONES
				ret = this->getFrame(&pImage,id);	
				if (ret<0)
				{
					if (ret == END_COMMUNICATION){
						_pFrameClient->setEnd();
						ReleaseMutex(hMutex);
						printf("PETICION DE END COMMUNICATION\n");
						return -1;
					}
					if (ret == RESET_COMMUNICATION)						
						printf("PETICION DE RESET COMMUNICATION\n");
					else if (ret == CLIENT_TIME_OUT)
						printf("EXPIRÓ EL TIMEOUT SIN RECIBIR DATOS DEL SERVIDOR\n");
					else printf("ERROR EN EL SERVIDOR\n");

					_pFrameClient->setEnd();
					this->setLastId(0);
					//printf("DiVAAlgorithm::process: DESPUES DE PROCESS FRAME\n");
					if (_pFrameClient){
						delete _pFrameClient;
						_pFrameClient = new DiVAClient(_frameServername,_portnumber);
					}
					else if (_pFrameBuffer){
						delete _pFrameBuffer;
						_pFrameBuffer = new DiVAClientBuffer(_frameServername,_portnumber);
					}

					Sleep(3000);

					int n_reintentos=4;
					printf("DiVAAlgorithm::process: Conexión con el servidor perdida. Intentando reconexion...\n");
					while ((n_reintentos >= 0)&&(ret < 0))
					{
						if (!isEnd())
						{
							printf("Intento: %d\n",4-n_reintentos+1);
							ret = connect();
							if (ret < 0){
								Sleep(2000);
								n_reintentos--;
							}
							else {
								printf("Conexion restablecida\n");
								strcpy(_dumpingpath,"../output/");
								
								if (getSourceId())
								{
									strcat(_dumpingpath,getSourceId());
								}
								ReleaseMutex(hMutex);
								return 0;
							}
						}
						else{ 
							ReleaseMutex(hMutex);
							return 0;
						}
					}

					#if _DEBUG
						//	cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::receiveFrame, "<< ret <<", no contemplado.\r\n";
					#endif
					if (n_reintentos < 0){
						ReleaseMutex(hMutex);
						printf("No ha sido posible la reconexion\n");
						return -1;
					}
					
					ReleaseMutex(hMutex);
					return -1;
				}
				
				setLastId(pImage->getId());
	#ifdef _DEBUG
				trcv = (long)pCrono->stop();
	#endif 
		
			/*Obtengo información del DataServer******************************************/
					ret = this->receiveData(&pData);
					if (ret<0)
					{
					#if _DEBUG
							cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::receiveData, "<< ret <<", no contemplado.\r\n";
					#endif
						ReleaseMutex(hMutex);
						return ret;
					}
	#ifdef _DEBUG
				tdataserver = (long)pCrono->stop();
	#endif 
			/*Obtengo información del ContentServer***************************************/
				if (this->getcontentServerName())
				{
					ret = this->receiveContentData(&pContentData);
					if (ret<0)
					{
					#if _DEBUG
							//cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::receiveContentData, "<< ret <<", no contemplado.\r\n";
					#endif
						ReleaseMutex(hMutex);
						return ret;
					}
				}	
	#ifdef _DEBUG
				tcontentserver = (long)pCrono->stop();
	#endif
			/*Proceso el frame************************************************************/
				ret = this->processFrame(pImage,pData,pContentData);
				if (ret<0)
				{
				#if _DEBUG
						//cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::processFrame, "<< ret <<", no contemplado.\r\n";
				#endif
					ReleaseMutex(hMutex);
					return ret;
				}
	#ifdef _DEBUG			
				tprocess = (long)pCrono->stop();
	#endif
			/*Vuelco a DataServer************************************************************/
			if(this->getdataServerDumping())
			{
				ret = this->dumpData();
				if (ret<0)
				{
				#if _DEBUG
					//	cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::dumpData, "<< ret <<", no contemplado.\r\n";
				#endif
					ReleaseMutex(hMutex);
					return ret;
				}
			}
	#ifdef _DEBUG	
				tdumpDataServer = (long)pCrono->stop();
	#endif
			/*Vuelco a ContentServer*********************************************************/
			if (this->getcontentServerDumping())
			{
				ret = this->dumpContentData();
				if (ret<0)
				{
				#if _DEBUG
						//cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::dumpContentData, "<< ret <<", no contemplado.\r\n";
				#endif
					ReleaseMutex(hMutex);
					return ret;
				}
			}
	#ifdef _DEBUG	
				tdumpContentServer = (long)pCrono->stop();
	#endif
			/*Vuelco a Ficheros**************************************************************/
			if (this->getfileDumping())
			{
				ret = this->dumpResultsInFiles();
				if (ret<0)
				{
				#if _DEBUG
						//cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::dumpResultsInFiles, "<< ret <<", no contemplado.\r\n";
				#endif
					ReleaseMutex(hMutex);
					return ret;
				}
			}
	#ifdef _DEBUG
				tdumpInFiles = (long)pCrono->stop();
	#endif
			/*Muestro por display*********************************************************/
				if (this->getdisplay())
				{
					ret = this->refreshDisplay();
					if (ret<0)
					{
					#if _DEBUG
						//	cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::refreshDisplay, "<< ret <<", no contemplado.\r\n";
					#endif
						ReleaseMutex(hMutex);
						return ret;
					}
				}
	#ifdef _DEBUG
				tDisplay = (long)pCrono->stop();
	#endif

				
			/*Display aplicaciones con Qt*********************************************************/
				if (this->getdisplayQt())
				{
					ret = this->sendFrameQt();
					if (ret<0)
					{
					#if _DEBUG
						//	cout << "DiVAAlgorithm::process(). Valor devuelto por DiVAAlgorithm::refreshDisplay, "<< ret <<", no contemplado.\r\n";
					#endif
						ReleaseMutex(hMutex);
						return ret;
					}
				}

	#ifdef _DEBUG
	//	cout <<pImage->getId()<<" tr:"<<trcv<<" trDS:"<<tdataserver<<" trCS:"<<tcontentserver<<" tp:"<<tprocess<<"  tdDS:"<<tdumpDataServer<<" tdCS:"<<tdumpContentServer<<" tdF:"<<tdumpInFiles<<" tD:"<<tDisplay<<"\r\n";
	#endif
			setLastId(pImage->getId()+1);//Actualizo el próximo identificador de frame
			/*Libero memoria**************************************************************/
				if (pImage)
				{
					delete pImage;
					pImage=NULL;
				}
				if (pData)
				{
					this->releaseData(pData);
					pData=NULL;
				}
				if(pContentData)
				{
					this->releaseContentData(pContentData);
					pContentData=NULL;
				}
			} else return 0;
			
	ReleaseMutex(hMutex);	
	return ret;
}
int DiVAAlgorithm::setEnd()
{
	int ret = 0;
	if (_pFrameClient)
		ret = _pFrameClient->setEnd();  //It basically sends a END command to the server.
	return DiVAThread::setEnd();
};

BOOL DiVAAlgorithm::createDisplay(char* title,int rows,int columns,int h, int w)
{
	if (!_display)
		return FALSE;
	unsigned int gapR= 10;
	unsigned int gapC= 10;
	bool titles = TRUE;
	_pDisplay = new DiVADisplay(title,h,w,rows,columns,gapR,gapC,titles);
	_pDisplay->onSizeWindow(600,800);
	if (_pDisplay)
		return TRUE;
	else
		return FALSE;
}

/**
*This method is called from the execution loop.
*\param pImage It is the image I want to process
* 
*\return "0" if OK.
*/
int DiVAAlgorithm::processFrame(DiVAImage* pImage,void* pdata,void* pContentData)
{
	int ret = 0;
	return ret;
}


int DiVAAlgorithm::getFrame(DiVAImage** pImage,long idFrame){

	int ret = 0;

	switch(_captureMode){

		case CAPTURE_SEQ:
			ret = this->receiveFrame(pImage,idFrame);			
			break;

		case CAPTURE_BUF:
			ret = _pFrameBuffer->getFromBuffer(pImage, idFrame);
			break;
			
		default:
			ret = this->receiveFrame(pImage,idFrame);
			break;
	}

	return ret;
}

void DiVAAlgorithm::setDisplay(bool display) {

	_display = display;
	
	if(display == true)
		_pDisplay->onVisible();			
	else
		_pDisplay->offVisible();	
	
}

int fileExists(char* filename)
{
	FILE *f = fopen(filename, "r");	
	if (!f)
		return 0;	
	 fclose(f);
	 return 1;
}


#include "DiVAAlgorithm_multiCam.h"
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
DiVAAlgorithm_multiCam::DiVAAlgorithm_multiCam(char* frameServername1, int portnumber1,
			char* frameServername2, int portnumber2,
			char* frameServername3, int portnumber3,
			BOOL fileDumping,
			BOOL display,
			char* dataServerName,
			char* contentServerName,
			BOOL dataServerDumping,
			BOOL contentServerDumping, int captureMode)
{
	if (frameServername1)
	{
		_frameServername1 = new char[strlen(frameServername1)+1];
		strcpy(_frameServername1,frameServername1);
	}
	if (frameServername2)
	{
		_frameServername2 = new char[strlen(frameServername2)+1];
		strcpy(_frameServername2,frameServername2);
	}
	if (frameServername3)
	{
		_frameServername3 = new char[strlen(frameServername3)+1];
		strcpy(_frameServername3,frameServername3);
	}
	else
		return;
	_portnumber1 = portnumber1;
	_portnumber2 = portnumber2;
	_portnumber3 = portnumber3;
	_fileDumping=fileDumping;
	_display = display;

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
	_pFrameClient1	= NULL;
	_pFrameClient2	= NULL;
	_pFrameClient3	= NULL;
	_pFrameBuffer1	= NULL;
	_pFrameBuffer2	= NULL;
	_pFrameBuffer3	= NULL;
	_pDisplay		= NULL;	
	_pDataClient	= NULL;
	_pCrono=new DiVACrono();
	_dumpingpath 	 = NULL;	
		_dumpingpath = new char[512];
		_dumpingpath[0]=0;

	_dumpingpathRoot = new char[512];
	_dumpingpathRoot[0]=0;
	
	
	setLastId1((long)0);
	setLastId2((long)0);
	setLastId3((long)0);
	_captureMode = captureMode;
	hMutex = CreateMutex( NULL, FALSE, NULL );  
}




/**
*Destructor
*/
DiVAAlgorithm_multiCam::~DiVAAlgorithm_multiCam()
{
	if (_frameServername1)
		delete [] _frameServername1;
	if (_frameServername2)
		delete [] _frameServername2;
	if (_frameServername3)
		delete [] _frameServername3;

	if (_dataServerName)
		delete [] _dataServerName;

	if (_contentServerName)
		delete [] _contentServerName;
	
	if (_pFrameClient1)
		delete _pFrameClient1;
	if (_pFrameClient2)
		delete _pFrameClient2;
	if (_pFrameClient3)
		delete _pFrameClient3;


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
	if (_pFrameBuffer1)
		delete _pFrameBuffer1;
	if (_pFrameBuffer2)
		delete _pFrameBuffer2;
	if (_pFrameBuffer3)
		delete _pFrameBuffer3;


	CloseHandle(hMutex);
}
/**
*This method sets the name of the algorithm.
*It is recomended to be used in the constructor method in the children classes.
* \param name The name. Not longer than 128 characters.
* \return The name asigned.
*/
char* DiVAAlgorithm_multiCam::setAlgorithmName(char* name)
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
char* DiVAAlgorithm_multiCam::getSourceId() 
{
	switch(_captureMode){
		case CAPTURE_SEQ:	
			return _pFrameClient1->getSourceId();
			break;

		case CAPTURE_BUF:		
			return _pFrameBuffer1->getSourceId();
			break;

		default:
			return _pFrameClient1->getSourceId();
		}
	return NULL;

}

int DiVAAlgorithm_multiCam::init()
{
	// Creo el frame client

	switch(_captureMode){
		case CAPTURE_SEQ:	
			_pFrameClient1 = new DiVAClient(_frameServername1,_portnumber1);
			_pFrameClient2 = new DiVAClient(_frameServername2,_portnumber2);
			_pFrameClient3 = new DiVAClient(_frameServername3,_portnumber3);
			break;

		case CAPTURE_BUF:		
			_pFrameBuffer1 = new DiVAClientBuffer(_frameServername1,_portnumber1);
			_pFrameBuffer2 = new DiVAClientBuffer(_frameServername2,_portnumber2);
			_pFrameBuffer3 = new DiVAClientBuffer(_frameServername3,_portnumber3);
			break;

		default:
			_pFrameClient1 = new DiVAClient(_frameServername1,_portnumber1);
			_pFrameClient2 = new DiVAClient(_frameServername2,_portnumber2);
			_pFrameClient3 = new DiVAClient(_frameServername3,_portnumber3);
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
	
	if (getSourceId())// cambiar para multicam
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


int DiVAAlgorithm_multiCam::stop()
{		

	WaitForSingleObject(hMutex,ESPERA_MUTEX);
	if (_captureMode == CAPTURE_BUF){		
		_pFrameBuffer1->stop();				
		_pFrameBuffer2->stop();				
		_pFrameBuffer3->stop();		
	}

	int ret = DiVAThread::stop();
	ReleaseMutex(hMutex);
	return ret;
}


/**
*This method connect the clients to their server if it is possible and it is configured so.
* 
*\return "0" if OK
*/
int DiVAAlgorithm_multiCam::connect()
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
int DiVAAlgorithm_multiCam::connect2FrameServer()
{
	int ret = -1;
	switch(_captureMode){

		case CAPTURE_SEQ:
			if (_pFrameClient1)
				ret = _pFrameClient1->Connect();
			if (_pFrameClient2 && ret != -1)
				ret = _pFrameClient2->Connect();
			if (_pFrameClient3 && ret !=-1)
				ret = _pFrameClient3->Connect();
			break;

		case CAPTURE_BUF:
			if (_pFrameBuffer1)
				ret = _pFrameBuffer1->start();
			if (_pFrameBuffer2 && ret !=-1)
				ret = _pFrameBuffer2->start();
			if (_pFrameBuffer3 && ret !=-1)
				ret = _pFrameBuffer3->start();
			break;
			
		default:
			if (_pFrameClient1)
				ret = _pFrameClient1->Connect();
			if (_pFrameClient2 && ret != -1)
				ret = _pFrameClient2->Connect();
			if (_pFrameClient3 && ret !=-1)
				ret = _pFrameClient3->Connect();
			break;
	}

	return ret;
}

/**
*This method receives a frame using the frame client, attribute of this class.
* 
*\return The size of the received frame if OK. Error code if something goes wrong
*/
int DiVAAlgorithm_multiCam::receiveFrame(DiVAImage** pImage1,DiVAImage** pImage2,DiVAImage** pImage3,long idFrame1,long idFrame2,long idFrame3)
{
	DiVAImage* buffer=NULL;
	int ret = _pFrameClient1->receiveFrame((void**)(&buffer),idFrame1);
	if(ret<0)
	{
		return ret;
	}
	*pImage1 = buffer;
	ret = _pFrameClient2->receiveFrame((void**)(&buffer),idFrame2);
	if(ret<0)
	{
		return ret;
	}
	*pImage2 = buffer;
	ret = _pFrameClient3->receiveFrame((void**)(&buffer),idFrame3);
	if(ret<0)
	{
		return ret;
	}
	*pImage3 = buffer;
	return ret;
}

int DiVAAlgorithm_multiCam::process()
{
		int ret = 0;
		DiVAImage*	pImage1			= NULL;
		DiVAImage*	pImage2			= NULL;
		DiVAImage*	pImage3			= NULL;
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
				
				long id1 = getLastId1();
				long id2 = getLastId2();
				long id3 = getLastId3();
				//DEPURACION RECONEXIONES (detenemos a propósito la petición de frames)
				//if ((this->getLastId() != 0) &&(this->getLastId()%100 == 0)){
				//	Sleep(2100);
				//	this->setLastId(this->getLastId() + 1);
				//}
				//printf("DIVAALgorithm::process frame pedido\n");
				//FIN DEPURACIÓN DESCONEXIONES
				ret = this->getFrame(&pImage1,&pImage2,&pImage3,id1,id2,id3);	
				if (ret<0)
				{
					if (ret == END_COMMUNICATION){
						_pFrameClient1->setEnd();
						_pFrameClient2->setEnd();
						_pFrameClient3->setEnd();
						ReleaseMutex(hMutex);
						printf("PETICION DE END COMMUNICATION\n");
						return -1;
					}
					if (ret == RESET_COMMUNICATION)						
						printf("PETICION DE RESET COMMUNICATION\n");
					else if (ret == CLIENT_TIME_OUT)
						printf("EXPIRÓ EL TIMEOUT SIN RECIBIR DATOS DEL SERVIDOR\n");
					else printf("ERROR EN EL SERVIDOR\n");
					
					_pFrameClient1->setEnd();
					_pFrameClient2->setEnd();
					_pFrameClient3->setEnd();
					this->setLastId1(0);
					this->setLastId2(0);
					this->setLastId3(0);
					//printf("DiVAAlgorithm::process: DESPUES DE PROCESS FRAME\n");
					if (_pFrameClient1){
						delete _pFrameClient1;
						_pFrameClient1 = new DiVAClient(_frameServername1,_portnumber1);
					}
					if (_pFrameClient2){
						delete _pFrameClient2;
						_pFrameClient2 = new DiVAClient(_frameServername2,_portnumber2);
					}
					if (_pFrameClient3){
						delete _pFrameClient3;
						_pFrameClient3 = new DiVAClient(_frameServername3,_portnumber3);
					}
					if (_pFrameBuffer1){
						delete _pFrameBuffer1;
						_pFrameBuffer1 = new DiVAClientBuffer(_frameServername1,_portnumber1);
					}
					 if (_pFrameBuffer2){
						delete _pFrameBuffer2;
						_pFrameBuffer2 = new DiVAClientBuffer(_frameServername2,_portnumber2);
					 }
					 if (_pFrameBuffer3){
						delete _pFrameBuffer3;
						_pFrameBuffer3 = new DiVAClientBuffer(_frameServername3,_portnumber3);
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
				
				setLastId1(pImage1->getId());
				setLastId2(pImage2->getId());
				setLastId3(pImage3->getId());
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
				ret = this->processFrame(pImage1,pImage2,pImage3,pData,pContentData);
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
	#ifdef _DEBUG
	//	cout <<pImage->getId()<<" tr:"<<trcv<<" trDS:"<<tdataserver<<" trCS:"<<tcontentserver<<" tp:"<<tprocess<<"  tdDS:"<<tdumpDataServer<<" tdCS:"<<tdumpContentServer<<" tdF:"<<tdumpInFiles<<" tD:"<<tDisplay<<"\r\n";
	#endif
			setLastId1(pImage1->getId()+1);//Actualizo el próximo identificador de frame
			setLastId2(pImage2->getId()+1);//Actualizo el próximo identificador de frame
			setLastId3(pImage3->getId()+1);//Actualizo el próximo identificador de frame
			/*Libero memoria**************************************************************/
				if (pImage1)
				{
					delete pImage1;
					pImage1=NULL;
				}
				if (pImage2)
				{
					delete pImage2;
					pImage2=NULL;
				}
				if (pImage3)
				{
					delete pImage3;
					pImage3=NULL;
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
int DiVAAlgorithm_multiCam::setEnd()
{
	int ret = 0;
	if (_pFrameClient1)
		ret = _pFrameClient1->setEnd();  //It basically sends a END command to the server.
	if (_pFrameClient2)
		ret = _pFrameClient2->setEnd();  //It basically sends a END command to the server.
	if (_pFrameClient3)
		ret = _pFrameClient3->setEnd();  //It basically sends a END command to the server.
	return DiVAThread::setEnd();
};

BOOL DiVAAlgorithm_multiCam::createDisplay(char* title,int rows,int columns,int h, int w)
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
int DiVAAlgorithm_multiCam::processFrame(DiVAImage* pImage1,DiVAImage* pImage2,DiVAImage* pImage3,void* pdata,void* pContentData)
{
	int ret = 0;
	return ret;
}


int DiVAAlgorithm_multiCam::getFrame(DiVAImage** pImage1,DiVAImage** pImage2,DiVAImage **pImage3,long idFrame1,long idFrame2,long idFrame3){

	int ret = 0;

	switch(_captureMode){

		case CAPTURE_SEQ:
			ret = this->receiveFrame(pImage1,pImage2,pImage3,idFrame1,idFrame2,idFrame3);
			break;

		case CAPTURE_BUF:
			ret = _pFrameBuffer1->getFromBuffer(pImage1, idFrame2);
			if (ret != -1){
				ret = _pFrameBuffer2->getFromBuffer(pImage2, idFrame2);			
				if (ret != -1)
					ret = _pFrameBuffer3->getFromBuffer(pImage3, idFrame3);			
			}
			break;
			
		default:
			ret = this->receiveFrame(pImage1,pImage2,pImage3,idFrame1,idFrame2,idFrame3);
			break;
	}

	return ret;
}

void DiVAAlgorithm_multiCam::setDisplay(bool display) {

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


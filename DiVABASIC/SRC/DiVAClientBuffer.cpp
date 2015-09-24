//file description
/**
* \file DiVAClientBuffer.cpp 
* \author Juan Carlos San Miguel Avedillo
* \date 16-03-07
* \version 1.0						
* \brief Implementation of DiVAClientBuffer class
* 
* This file is the implementation of the DiVAClientBuffer class.
* DiVAClientBuffer is an interface to store data received from FrameServer
*
*/

#include "DiVAClientBuffer.h"
#include "stdio.h"

//Default constructor
DiVAClientBuffer::DiVAClientBuffer(char *frameServername, int portnumber, int bufferLength):DiVAThread()
{
	_bufferLength = bufferLength;
	_pCrono=new DiVACrono();
	_frame=NULL;
	_pimageBuffer=NULL;
	
	if(strlen(frameServername)<100)		
		strcpy(_frameServername,frameServername);	
	_portnumber = portnumber;
	_pFrameClient =NULL;

	_idFrame = 0;
	_idClient = 0;
}

//destructor of the class
DiVAClientBuffer::~DiVAClientBuffer()
{
	if(_pimageBuffer)
		delete _pimageBuffer;
	if(_pCrono)
		delete _pCrono;
	if(_pFrameClient)
		delete _pFrameClient;
	if(_frame)
		delete _frame;

}

int DiVAClientBuffer::process()
{
	int ret=0;
	DiVAImage* pImage =NULL;

	//_pCrono->start();	
	ret = this->receiveFrame(&pImage,_idFrame);
	//double trcv = _pCrono->stop();
	if (ret<0)
	{	//NO BLOQUEO, retorno sin meter nada en el buffer
		return ret;
	}

	//Actualizo el próximo identificador de frame
	_idFrame = pImage->getId()+1;
	//Lo escribo en el buffer
	writeInBuffer(pImage);	

	
	//printf("ClientBuffer::receiveFrame %d  tr:%.3g (estado del buffer %d/%d)\n", _idFrame-1, trcv, _pimageBuffer->getOcupados(),_pimageBuffer->getTamBuffer());
	
	delete pImage;
	return 0;
}

int DiVAClientBuffer::start()
{
	int ret=0;	
	DiVAImage* pImage =NULL;

	//Create connection to frameServer	
	_pFrameClient = new DiVAClient(_frameServername,_portnumber);
	ret = _pFrameClient->Connect();
	if (ret<0)
		return ret;	
	
	//Receive one frame to init pimageBuffer
	//_pCrono->start();
	ret = this->receiveFrame(&pImage,_idFrame);			
	//printf("ClientBuffer::receiveFrame %d  tr:%d\n", _idFrame-1, _pCrono->stop());

	if (ret<0)
	{	//NO BLOQUEO, retorno sin meter nada en el buffer
		return ret;
	}
	this->_pimageBuffer = new DiVAImageBuffer(this->_bufferLength,pImage,"Frames Received from FS");	
	writeInBuffer(pImage);
	_idFrame = pImage->getId()+1;
	_idClient = _pimageBuffer->registrarConsumidor();		

	delete pImage;	
	return ((DiVAThread*)this)->start();//Y el metodo padre lanza el thread
}

int DiVAClientBuffer::writeInBuffer(DiVAImage* pImage)
{	
	return _pimageBuffer->put(pImage);
}


int DiVAClientBuffer::getFromBuffer(DiVAImage** pImage,long idFrame){

	*pImage = (DiVAImage*)_pimageBuffer->getSampleElement();
	//printf("\nObteniendo frame del buffer del cliente\n");
	return _pimageBuffer->get(*pImage,idFrame,_idClient);
}


char *DiVAClientBuffer::getSourceId(){

	return _pFrameClient->getSourceId();
}


int DiVAClientBuffer::setEnd(){

	//It basically sends a END command to the server.
	return _pFrameClient->setEnd();


}

/**
*This method receives a frame using the frame client, attribute of this class.
* 
*\return The size of the received frame if OK. Error code if something goes wrong
*/
int DiVAClientBuffer::receiveFrame(DiVAImage** pImage,long idFrame)
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
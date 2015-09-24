/**
* \file DiVAImageBuffer.h
* \author Alvaro Garcia Martin
* \date 15-11-06
* \version 1.0
* \brief Class to describe an buffer in this framework
* 
* This file is the header of the DiVAImageBuffer class.
* Interface for the DiVAImageBuffer class.
*
*/	


//class description
/**
 * \class DiVAImageBuffer
 * \brief To instanciate a image buffer
 *
 * Write detailed description here.
 */

#if !defined(DiVA_IMAGEBUFFER_H_INCLUDED)
#define DiVA_IMAGEBUFFER_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DiVAFreeBuffer.h"
#include "DiVAImage.h"

#ifdef _DEBUG
#define DEFAULT_TIME_OUT 5000 
#else
#define DEFAULT_TIME_OUT 2000
#endif

class DiVAImageBuffer: public DiVAFreeBuffer
{
private:

	//To store images in a buffer
	DiVAImage **datos;
	char*		_sourceId;

public:
	
	///Constructor with basic parameters
	DiVAImageBuffer(long tamBuffer, long sizeX, long sizeY, int nChannels, int type,int valign =0,char* sourceId=NULL ,long waitTimeout = DEFAULT_TIME_OUT, int consumecontrol = 1);

	///Constructor with a reference image
	DiVAImageBuffer(long tamBuffer, DiVAImage *refImage,char* originIdentifier=NULL, long waitTimeout = DEFAULT_TIME_OUT, int consumecontrol = 0);

	///Default destructor
	virtual ~DiVAImageBuffer();

	///Sets image source id
	char* setSourceId(char* sourceId);
	///Gets image source id
	char* getSourceId(){return _sourceId;};

	///Internal data read implementation: *DiVAImage
	void copiarLectura(void *elemento, long posicion);

	///Internal data copy implementation
	void copiarEscritura(void *elemento, long posicion);	

	///Method to get data from buffer from a certain id client
	long get(DiVAImage *elemento, long idFrame, long idConsumidor);

	///Method to put data into buffer
	long put(DiVAImage *elemento);

	
	long buscarElemento(long idFrame,int* position);

	///Method to get an empty image with its parameters
	void* getSampleElement();
//	void print();

	///Elements in Buffer
	int getTamBuffer();

private:

	///To init the buffer
	void initImageBuffer(long tamBuffer, long sizeX, long sizeY, int nChannels, int type,int valign);

};

#endif // !defined(DiVA_IMAGEBUFFER_H_INCLUDED)

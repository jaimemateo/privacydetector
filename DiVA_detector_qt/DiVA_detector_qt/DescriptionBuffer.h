//file DescriptionBuffer
/**
* \file DescriptionBuffer.h
* \author Alvaro Garcia Martin
* \date 01-03-07
* \version 1.2
* \brief Class to define create a buffer of descriptors
* 
* This file is the header of the DescriptionBuffer class.
*
*/	


//class DescriptionBuffer
/**
 * \class DescriptionBuffer
 * \brief Define a buffer for storing and retrieving descriptors
 * 
 */

#if !defined(DESCRIPTIONBUFFER_H_INCLUDED)
#define DESCRIPTIONBUFFER_H_INCLUDED

#include "DiVAFreeBuffer.h"
#include "Description.h"

class DescriptionBuffer: public DiVAFreeBuffer
{
	public:
	DescriptionBuffer(long tamDescripcion =8192, long tamBuffer = 4, long waitTimeout = 2000, int consumecontrol = 1,int minimumNumberOfBufferConsumers=0);
	~DescriptionBuffer();

	///Data copy implementation
	void copiarLectura(void *elemento, long posicion);
	///Data read implementation
	void copiarEscritura(void *elemento, long posicion);

	///Data sample copy creation implementation
	void* getSampleElement();

	///Element by id search implementation
	long buscarElemento(long idFrame,int* position);
	char* getSourceId(){return _sourceId;};
private:
	Description** _datos;
	long _tamBuffer;
	long _tamDescripcion;
	char _sourceId[128];
};
#endif
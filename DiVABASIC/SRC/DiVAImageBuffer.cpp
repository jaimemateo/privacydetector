//file description
/**
* \file  DiVAImageBuffer.cpp 
* \author Alvaro Garcia Martin
* \date 15-11-06
* \version 1.0						
* \brief Implementation of DiVAImageBuffer class.
* 
* This file is the implementation of the DiVAImageBuffer class.
*
*/

#include "DiVAImageBuffer.h"
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
#include <stdlib.h>
#include <stdio.h>


/**
 *	Constructor of DiVAImageBuffer class
 *
 * \param tamBuffer Number of elements in buffer
 * \param refImage Reference image to elements stored in buffer
 *
 * \return Control code
 */
DiVAImageBuffer::DiVAImageBuffer(long tamBuffer, DiVAImage *refImage,char* sourceId, long waitTimeout,int consumecontrol) : DiVAFreeBuffer(tamBuffer,waitTimeout,consumecontrol)
{
	initImageBuffer(tamBuffer, refImage->getWidth(), refImage->getHeight(), refImage->getNChannels(), refImage->getDepth(),refImage->getVAlign());
	if (sourceId)
	{
		_sourceId= new char[strlen(sourceId)+1];
		strcpy(_sourceId,sourceId);
	}
	else
		_sourceId=NULL;
}

/**
 *	Constructor of DiVAImageBuffer class
 *
 * \param tamBuffer Number of elements in buffer
 * \param sizeX Width of the image elements in buffer
 * \param sizeY Height of the image elements in buffer
 * \param nChannels Number of image channels
 * \param type Depth of pixel's image (see DiVAImage depth for more detail)
 *
 * \return Control code
 */
DiVAImageBuffer::DiVAImageBuffer(long tamBuffer, long sizeX, long sizeY, int nChannels, int type,int valign,char* sourceId, long waitTimeout,int consumecontrol) : DiVAFreeBuffer(tamBuffer,waitTimeout,consumecontrol)
{
	initImageBuffer(tamBuffer, sizeX, sizeY, nChannels, type,valign);
	if (sourceId)
	{
		setSourceId(sourceId);
	}
	else
		_sourceId=NULL;
}

/**
 *	Default Destructor of DiVAImageBuffer class
 *	It delete all buffer data
 *
 *
 * \return Control code
 */
DiVAImageBuffer::~DiVAImageBuffer()
{
	// Liberar recursos propios
	for (int i=0; i<tamBuffer; i++)
	{
		delete datos[i];
	}
	free(datos);
	if(_sourceId)
		free(_sourceId);
}
/**
 *	Sets the image source id
 *
 *
 * \return The source id
 */
char* DiVAImageBuffer::setSourceId(char* sourceId)
{
	if (sourceId)
	{
		if(_sourceId!=NULL)
			free(_sourceId);

		_sourceId= new char[strlen(sourceId)+1];
	
		strcpy(_sourceId,sourceId);
	}
	return _sourceId;
}
/**
 *	Internal Method to read one element from buffer
 *
 * \param elemento pointer to structure to copy data from buffer
 * \param posicion Position to read
 *
 */
void DiVAImageBuffer::copiarLectura(void *elemento, long posicion)
{
	DiVAImage *im = (DiVAImage *) elemento;
	im->copy(datos[posicion]);
#ifdef _DEBUG
	 //printf("DiVAImageBuffer::copiarLectura: imagen %d ; posicion %d\n",im->getId(),posicion);
#endif
}

/**
 *	Internal Method to copy one element in buffer
 *
 * \param elemento pointer to structure to copy in buffer
 * \param posicion Position to copy
 *
 */
void DiVAImageBuffer::copiarEscritura(void *elemento, long posicion)
{
	DiVAImage *im = (DiVAImage *) elemento;
	datos[posicion]->copy(im);
#ifdef _DEBUG
	 //printf("DiVAImageBuffer::copiarEscritura: imagen %d ; posicion %d\n",im->getId(),posicion);
#endif
}


/**
 *	Method to get one element from buffer
 *
 * \param elemento pointer to structure to copy data from buffer
 * \param id Client identification number
 *
 * \return Return Code
 *
 */
long DiVAImageBuffer::get(DiVAImage *elemento, long idFrame, long idConsumidor)
{
	return DiVAFreeBuffer::get((void *) elemento, idFrame, idConsumidor);
}


/**
 *	Method to put one element in buffer
 *
 * \param elemento pointer to structure to copy data in buffer 
 *
 * \return Return Code
 *
 */
long DiVAImageBuffer::put(DiVAImage *elemento)
{
	return DiVAFreeBuffer::put((void *) elemento);
}

/**
 *	Returns the position of image with idFrame. If it is not in the buffer, returns position of next available frame
 *
 * \param idFrame identifier of requested frame
 *
 * \return Position in the table if OK, BUFFERC_ERROR if error, BUBBERC_EMPTY if requested frame is not already available
 *
 */
long DiVAImageBuffer::buscarElemento(long idFrame,int* position)
{
	int pos = -1;	
	int retpos = -1;
	long id = -1;
	bool tieneFrame = FALSE;
	for (long cont = 0; cont < ocupados; cont++)
	{
		pos = cont;	
		if (datos[pos]->getId() >= idFrame)
		{
			if (id>0)
			{
				if(datos[pos]->getId()<id)
				{
					id = datos[pos]->getId();
					retpos = pos;
				}
			}
			else
			{
				id = datos[pos]->getId();
				retpos = pos;
			}
			tieneFrame = TRUE;			
		}
	}
	*position=retpos;
	if (retpos>=0) 
	{
#ifdef _DEBUG
		//if ((idFrame!=datos[pos]->getId())&(idFrame!=0))
		//	printf("DiVAImageBuffer::buscarElemento: Frame buscado/devuelto: %d/%d. Posicion: %d\r\n",idFrame,datos[pos]->getId(),pos);
#endif
		*position=retpos;
		return id;
	}
	else 
#ifdef _DEBUG
	//printf("DiVAImageBuffer::buscarElemento: No hay frames nuevos.\r\n");
#endif
		return BUFFERC_EMPTY;
}

///////////////////////////////////////////
// PRIVATE MEMBER FUNCTIONS
///////////////////////////////////////////

/**
 *	Private Method to init image buffer
 *
 * \param tamBuffer Number of elements in buffer
 * \param sizeX Width of the image elements in buffer
 * \param sizeY Height of the image elements in buffer
 * \param nChannels Number of image channels
 * \param type Depth of pixel's image (see DiVAImage depth for more detail)
 *
 * \return Return Code
 *
 */
void DiVAImageBuffer::initImageBuffer(long tamBuffer, long sizeX, long sizeY, int nChannels, int type,int valign)
{
	// Inicializar atributos propios

	// Reservar recursos propios
	this->datos = (DiVAImage**)malloc(tamBuffer * sizeof(DiVAImage*));

	// Inicializar todos los elementos del buffer
	for (int i=0; i<tamBuffer; i++)
	{
		datos[i] = new DiVAImage(sizeX, sizeY, nChannels, type);
		datos[i]->setVAlign(valign);
	}
}

void*  DiVAImageBuffer::getSampleElement()
{
	DiVAImage *ref = datos[0];
	if (ref != NULL)
		return (void*)ref->clone();
	else 
		return NULL;
}


int DiVAImageBuffer::getTamBuffer()
{
	return this->tamBuffer;
}
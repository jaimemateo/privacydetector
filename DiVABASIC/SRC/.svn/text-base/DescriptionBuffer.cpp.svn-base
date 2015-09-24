#include "DescriptionBuffer.h"

DescriptionBuffer::DescriptionBuffer(long tamDescripcion, long tamBuffer, long waitTimeout, int consumecontrol,int minimumNumberOfBufferConsumers):DiVAFreeBuffer(tamBuffer, waitTimeout, consumecontrol,minimumNumberOfBufferConsumers)
{
	// Reservar recursos propios
	strcpy(_sourceId,"Descriptions Server 1\n");
	_datos = (Description**)malloc(tamBuffer * sizeof(Description*));
	_tamBuffer = tamBuffer;
	_tamDescripcion = tamDescripcion;
	// Inicializar todos los elementos del buffer
	for (int i=0; i<tamBuffer; i++)
	{
		_datos[i] = new Description(tamDescripcion);
	}
}
 DescriptionBuffer::~DescriptionBuffer()
{
	for (int i=0; i<_tamBuffer; i++)
	{
		 delete _datos[i];
	}
	free((void*)_datos);
}
void DescriptionBuffer::copiarLectura(void *elemento, long posicion)
{
	Description *elem = (Description *) elemento;
	Description *dato = _datos[posicion];
	char* buffer = elem->getBuffer();
	dato->getBufferCopied(&buffer);
	elem->setBuffer(buffer,dato->getSize());//It is only for updating the _usedSize of the buffer
	elem->setId(dato->getId());
	return;
#ifdef _DEBUG
	 //printf("DescriptionBuffer::copiarLectura: imagen %d ; posicion %d\n",im->getId(),posicion);
#endif
}

void DescriptionBuffer::copiarEscritura(void *elemento, long posicion)
{
	Description *desc = (Description *) elemento;
	_datos[posicion]->copy(desc);
#ifdef _DEBUG
#endif
//	Description *elem = (Description *) elemento;
//	Description *elemBuffer = (Description *)_datos[posicion];
//	elemBuffer->setBuffer(elem->getBuffer(),elem->getSize());
	#ifdef _DEBUG
	 //printf("DescriptionBuffer::copiarEscritura: imagen %d ; posicion %d\n",im->getId(),posicion);
#endif
}

void* DescriptionBuffer::getSampleElement()
{
	Description * elemAux = new Description(_tamDescripcion);
	return elemAux;
}

long DescriptionBuffer::buscarElemento(long idFrame,int* position)
{
	int pos = -1;	
	int retpos = -1;
	long id = -1;
	bool tieneFrame = FALSE;
	for (long cont = 0; cont < ocupados; cont++)
	{
		pos = cont;	
		if (_datos[pos]->getId() >= idFrame)
		{
			if (id>0)
			{
				if(_datos[pos]->getId()<id)
				{
					id = _datos[pos]->getId();
					retpos = pos;
				}
			}
			else
			{
				id = _datos[pos]->getId();
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






#if 0	
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
#endif
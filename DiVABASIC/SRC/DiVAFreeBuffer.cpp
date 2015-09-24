/**
* \file  DiVAFreeBuffer.cpp 
* \author Alvaro Garcia Martin
* \date 23-01-2007
* \version 1.0						
* \brief Implementation of DiVAFreeBuffer class.
* 
* This file is the implementation of the DiVAFreeBuffer class.
*
*/

#include "DiVAFreeBuffer.h"

#include <stdio.h>


/**
 *	Method to init buffer
 *
 * \param tamBuffer Number of elements in buffer 
 * \param putPolicy Policy to put data in a shared buffer
 * \param waitTimeout Maximum time to wait in system
 *
 *
 */
DiVAFreeBuffer::DiVAFreeBuffer(long tamBuffer, long waitTimeout,int consumecontrol,int minNumberOfConsumers)
{
	_minNumberOfConsumers = minNumberOfConsumers;
	_numberOfConsumers = 0;
	// Crear e inicializar mutex (tomando el recurso y sin nombre)
	hMutexBuffer = CreateMutex( NULL, FALSE, NULL );       
	hMutexWriting = CreateEvent(NULL, FALSE, FALSE, NULL);  
	// Inicializar cola
	this->tamBuffer = tamBuffer;
	first = -1;
	last = -1;
	ocupados = 0;

	_consumecontrol=consumecontrol;
	// Inicializar tabla de consumidores
	for (int i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		consumidores[i].activo = FALSE;
		consumidores[i].lastReadenItemId = -1;
		consumidores[i].lastReadenPosition = -1;
		consumidores[i].esperandoLiberacion = FALSE;
		consumidores[i].hSemConsumidor = CreateEvent(NULL, FALSE, FALSE, NULL);
		consumidores[i].userData = NULL;
	}

	// Tiempo de timeout para esperas en semaforos
	if (waitTimeout == 0) 
		this->waitTimeout = INFINITE;
	else
		this->waitTimeout = waitTimeout;

	// Asignar politica para put
	if (_consumecontrol)
		politicaPut = BUFFERC_PUT_WAIT;
	else
		politicaPut = BUFFERC_PUT_REPLACE;
	_writerWaitingReleasing = 0;
}

/**
 *	Method to destroy buffer
 *
 */
DiVAFreeBuffer::~DiVAFreeBuffer()
{
	// Liberar mutex
	
	CloseHandle( hMutexBuffer );
	CloseHandle(hMutexWriting);
	// Eliminar semaforos de los consumidores
	for (int i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		CloseHandle(consumidores[i].hSemConsumidor);
	}
}

/**
 *	Method to register a consumer in buffer
 *
 * \param userData Pointer to user data structure. This structure can be defined in child class
 *
 * \return Return buffer status
 */
long DiVAFreeBuffer::registrarConsumidor(void *userData)
{
	int i = 0;
	// Control de acceso
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;

	// Buscar un hueco en la tabla de consumidores
	for (i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		if (consumidores[i].activo == FALSE)
		{
			_numberOfConsumers++;
			consumidores[i].activo = TRUE;
			//consumidores[i].datoLeido = FALSE;
			consumidores[i].lastReadenItemId=-1;
			consumidores[i].lastReadenPosition=-1;
			consumidores[i].esperandoLiberacion = FALSE;
			ResetEvent(consumidores[i].hSemConsumidor);
			consumidores[i].userData = userData;

			ReleaseMutex(hMutexBuffer);
			return i;
		}
	}
	if (i>=BUFFERC_MAX_CONSUMIDORES)
		{
#ifdef _DEBUG
	printf("Numero máximo de consumidores, excedido.r\n");
#endif
		}

	// Si no ha encontrado hueco, retorna BUFFERC_FULL
	// Liberar el control del buffer y retornar
	ReleaseMutex(hMutexBuffer);
	return BUFFERC_FULL;
}

/**
 *	Method to eliminate a consumer from buffer
 *
 * \param id Identification client number
 *
 * \return Return buffer status
 */
int DiVAFreeBuffer::eliminarConsumidor(long id)
{
	if ((id >= 0) && (id < BUFFERC_MAX_CONSUMIDORES))
	{
		int ret =WaitForSingleObject(hMutexBuffer, waitTimeout);
		if (ret == WAIT_TIMEOUT)
			return BUFFERC_TIMEOUT;
		else if (ret == WAIT_FAILED)
			return BUFFERC_ERROR;
		_numberOfConsumers--;
		consumidores[id].activo = FALSE;
		ReleaseMutex(hMutexBuffer);
	}
	return BUFFERC_OK;
}

void * DiVAFreeBuffer::getEmptySample()
{
	// Control de acceso
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return NULL;
	else if (ret == WAIT_FAILED)
		return NULL;

	void *elem = getSampleElement();

	// Liberar el control del buffer y retornar
	ReleaseMutex(hMutexBuffer);
	return elem;

}
/**
 *	Method to get an element from buffer and extract it
 *
 * \param elemento Pointer to copy data from buffer
 * \param idElem Identification of requested element
 * \param idConsumidor Identification client number
 *
 * \return Return buffer status
 */
// Lee el primer elemento de la cola y lo saca del buffer
long DiVAFreeBuffer::get(void *elemento, long idElem, long idConsumidor)
{
	// Control de acceso
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;
	else if(ret<0)
		return BUFFERC_ERROR;
	// Buscar el elemento solicitado, o el posterior más cercano en su defecto
	int pos = -1;
	long id = buscarElemento(idElem,&pos);
	
	/* Si buscar elemento no encuentra el elemento porque...
	... el buffer está vacio
	... el elemento que quiero todavía no está
	entonces el consumidor tendrá que esperar a ser liberado, cuando put introduzca un nuevo elemento.
	*/
	/*if ((pos == BUFFERC_EMPTY)||
		((_consumecontrol)&&
		(consumidores[idConsumidor].lastReadenPosition)))
		*/
	if (id<0)
	{
		consumidores[idConsumidor].esperandoLiberacion = TRUE;
		ReleaseMutex(hMutexBuffer);

		ret = WaitForSingleObject(consumidores[idConsumidor].hSemConsumidor, waitTimeout); 
		consumidores[idConsumidor].esperandoLiberacion = FALSE;
		if (ret == WAIT_TIMEOUT){
		//	printf("DiVAFreeBuffer BUFFER EMPTY, WAIT_TIMEOUT\n");
			return BUFFERC_EMPTY;
		}
		else if (ret == WAIT_FAILED){
		//	printf("DiVAFreeBuffer BUFFER ERROR, WAIT_FAILED\n");
			return BUFFERC_ERROR;
		}
		else if(ret<0){
		//	printf("DiVAFreeBuffer BUFFER ERROR, WAIT_FAILED\n");
			return BUFFERC_ERROR;
		}
		//printf("DiVAFreeBuffer BUFFER ERROR, WAIT_FAILED\n");
		return get(elemento, idElem, idConsumidor); // If we get here, someone had NECESSARY introduce a new element in the buffer
	}

	// Copiar el dato en el puntero recibido
	copiarLectura(elemento, pos);
	consumidores[idConsumidor].lastReadenPosition = pos;
	consumidores[idConsumidor].lastReadenItemId	  = id;
	//Si tendo el lastReadenItemId más pequeño y el put espera liberación, le libero
	
	int i;
	int libero = 1;
	if(_consumecontrol)
	{
		for (i=0;i<BUFFERC_MAX_CONSUMIDORES;i++)
		{
			if (consumidores[i].activo)
			{
				if (consumidores[i].lastReadenItemId<id)
				{
				//No libero
				libero =0;
				break;				
				}
			}
			
		}	
		if (libero)
		{	
			if (_writerWaitingReleasing)
			{
				_writerWaitingReleasing = 0;
				SetEvent(hMutexWriting);
			}			
		}
	}
	ReleaseMutex(hMutexBuffer);
	return BUFFERC_OK;
}



/**
 *	Method to insert element in buffer
 *
 * \param elemento Pointer to copy data to buffer
 *
 * \return Return buffer status
 */
// Inserta un elemento en el buffer, al final de la cola
// Si el buffer está lleno, descarta el dato y retorna BUFFERC_FULL
long DiVAFreeBuffer::put(void *elemento)
{
	int ret = 0;
	ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;
		if ((_numberOfConsumers<_minNumberOfConsumers)&&(ocupados==tamBuffer))
	{
		ReleaseMutex(hMutexBuffer);
		return BUFFERC_NO_MIN_CONSUMERS;
	}
	int pos = getPositionToWrite();
	if (pos==-1)
	{	
		_writerWaitingReleasing = 1;
		// If I am not able to write I make it know to the caller, who will be the resposible of managing it.
		//This is only expected to happen in _consumeControl mode.
		ReleaseMutex(hMutexBuffer);
		ret = WaitForSingleObject(hMutexWriting, waitTimeout);
		_writerWaitingReleasing = 0;
		if (ret == WAIT_TIMEOUT)
			//return BUFFERC_TIMEOUT;
			return  BUFFERC_FULL;
		else if (ret == WAIT_FAILED)
			return BUFFERC_ERROR;
		ReleaseMutex(hMutexWriting);
		return put(elemento);
		
	}

	// Copiar el elemento en el buffer
	copiarEscritura(elemento, pos);
	//Actualizo last y ocupados(Despues de la primera pasada, ocupados pasará a ser el tamaño del buffer. Lo usaremos en buscarElemento)
	last = pos;
	if(ocupados<tamBuffer)
		ocupados++;
	liberarConsumidores();
	// Liberar el control del buffer y retornar
	ReleaseMutex(hMutexBuffer);

	return BUFFERC_OK;
}

/**
 *	Method to get buffer wait timeout
 * 
 *
 * \return Return the desired value
 */
long DiVAFreeBuffer::getWaitTimeout()
{
	return waitTimeout;
}

/**
 *	Method to get buffer policy for put operation. Posible values are:
 *		- BUFFERC_PUT_DISCARD	
 *		- BUFFERC_PUT_WAIT
 *		- BUFFERC_PUT_REPLACE
 *
 * \return Return the buffer policy
 */
int DiVAFreeBuffer::getPoliticaPut()
{
	return politicaPut;
}

/**
 *	Method to set Buffer timeout
 * 
 * \param newValue System Timeout 
 *
 */
// Para cambiar parametros del buffer en ejecucion
void DiVAFreeBuffer::setWaitTimeout(long newValue)
{
	waitTimeout = newValue;
}

/**
 *	Internal Method to read one element from buffer. This method can be implemented in child classes
 *
 * \param elemento pointer to structure to copy data from buffer
 * \param posicion Position to read
 *
 */
void DiVAFreeBuffer::copiarLectura(void *elemento, long posicion) {}

/**
 *	Internal Method to copy one element in buffer.
 *	This method can be implemented in child classes
 *
 * \param elemento pointer to structure to copy in buffer
 * \param posicion Position to copy
 *
 */
void DiVAFreeBuffer::copiarEscritura(void *elemento, long posicion) {}



void DiVAFreeBuffer::printError(long res)
{
	if (res == BUFFERC_FULL)
		printf("DiVAFreeBuffer: Buffer lleno\n");
	else if (res == BUFFERC_EMPTY)
		printf("DiVAFreeBuffer: Buffer vacio\n");
	else if (res == BUFFERC_ERROR)
		printf("DiVAFreeBuffer: Error\n");
	else if (res == BUFFERC_TIMEOUT)
		printf("DiVAFreeBuffer: Timeout\n");
	else
		printf("DiVAFreeBuffer: Error desconocido, %d\n", res);
}

//////////////////////////////////
// METODOS PRIVADOS
//////////////////////////////////
#if 0
void DiVAFreeBuffer::liberarHueco()
{
	// Dejar un hueco libre
	first = (first+1) % tamBuffer;
	ocupados--;	
	liberarConsumidores();
	SetEvent(hMutexWriting);
}
#endif
int DiVAFreeBuffer::liberarConsumidores()
{
	// Liberar a los consumidores que puedan estar esperando
	for (int i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		if (consumidores[i].activo == TRUE)
		{
		//	consumidores[i].datoLeido = FALSE;

			if (consumidores[i].esperandoLiberacion)  
			{
				//Esto es: Este consumidor esta esperando al resto, pero no puedo asegurar que entre la liberación
				//del mutex general para dar paso a los que quedan y el bloqueo del suyo propio no terminen de leer
				//el resto. Por ello uso este flag.
				consumidores[i].esperandoLiberacion = FALSE;
				SetEvent(consumidores[i].hSemConsumidor);
//				PulseEvent(consumidores[i].hSemConsumidor);
			}
		}
	}
	return BUFFERC_OK;
}

int DiVAFreeBuffer::getPositionToWrite()//A hacer: Primero buscar id minimo y luego comprobar si está leido
{
	int pos = (last+1)%tamBuffer;
	
	//En el caso de que no haya control de buffer
	if (_consumecontrol==FALSE)
		return pos;
	
	//CON CONTROL DE CONSUMO: Buscaré el elmento de menor id y comprobaré que  <= que el el más pequeño de  lost lastReadenItemId
	
	
	int minId=-1;
	int i;
	int nConsumers = 0;
	for (i=0;i<BUFFERC_MAX_CONSUMIDORES;i++)
	{
		if (consumidores[i].activo)
		{	
			nConsumers++;
			int idAux = consumidores[i].lastReadenItemId;
			if (minId==-1)
			{
				minId=idAux;
				pos = consumidores[i].lastReadenPosition;
				if (ocupados==0)//Caso especial. Primer elemento insertado (ocupados==0) con un cliente ya esperando
				{
					minId =-1;
					pos=0;
				}
			}
			else if (idAux<minId)
			{
				minId=idAux;
				pos = consumidores[i].lastReadenPosition;
			}
		}
	}
	if ((nConsumers==0)&&(ocupados==tamBuffer))
		return -1;

	long id = -1;
	if (minId>=0)
	{
		id = buscarElemento(minId,&pos);
		if (id!=minId)
			return -1;
	}

	int retpos = pos;
	minId--;
	while(minId>=0)
	{

		id = buscarElemento(minId,&pos);
		if (id==minId)
		{
			retpos =pos;
			minId--;
		}
		else
		{
		//#if _DEBUG
		#if 0
			printf("\n");
		#endif
			break;
		}
		//#if _DEBUG
		#if 0
			printf(".");
		#endif

	}		
	return retpos;
}


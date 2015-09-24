/**
* \file  GTISharedBuffer.cpp 
* \author Alvaro Garcia Martin
* \date 15-11-06
* \version 1.0						
* \brief Implementation of GTISharedBuffer class.
* 
* This file is the implementation of the GTISharedBuffer class.
*
*/

#include "GTISharedBuffer.h"

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
GTISharedBuffer::GTISharedBuffer(long tamBuffer, int putPolicy, long waitTimeout)
{
	// Crear e inicializar mutex (tomando el recurso y sin nombre)
	hMutexBuffer = CreateMutex( NULL, FALSE, NULL );       

	// Inicializar cola
	this->tamBuffer = tamBuffer;
	first = 0;
	last = 0;
	ocupados = 0;

	// Inicializar tabla de consumidores
	for (int i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		consumidores[i].activo = FALSE;
		consumidores[i].datoLeido = FALSE;
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
	politicaPut = putPolicy;
}

/**
 *	Method to destroy buffer
 *
 */
GTISharedBuffer::~GTISharedBuffer()
{
	// Liberar mutex
	CloseHandle( hMutexBuffer );

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
long GTISharedBuffer::registrarConsumidor(void *userData)
{
	// Control de acceso
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;

	// Buscar un hueco en la tabla de consumidores
	for (int i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		if (consumidores[i].activo == FALSE)
		{
			consumidores[i].activo = TRUE;
			consumidores[i].datoLeido = FALSE;
			consumidores[i].esperandoLiberacion = FALSE;
			ResetEvent(consumidores[i].hSemConsumidor);
			consumidores[i].userData = userData;

			ReleaseMutex(hMutexBuffer);
			return i;
		}
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
int GTISharedBuffer::eliminarConsumidor(long id)
{
	if ((id >= 0) && (id < BUFFERC_MAX_CONSUMIDORES))
	{
		int ret =WaitForSingleObject(hMutexBuffer, waitTimeout);
		if (ret == WAIT_TIMEOUT)
			return BUFFERC_TIMEOUT;
		else if (ret == WAIT_FAILED)
			return BUFFERC_ERROR;

		consumidores[id].activo = FALSE;
		ReleaseMutex(hMutexBuffer);
	}
	return BUFFERC_OK;
}

void * GTISharedBuffer::getEmptySample()
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
 *	Method to get first element from buffer and extract it
 *
 * \param elemento Pointer to copy data from buffer
 * \param id Identification client number
 *
 * \return Return buffer status
 */
// Lee el primer elemento de la cola y lo saca del buffer
long GTISharedBuffer::get(void *elemento, long id)
{
	printf("El cliente %d quiere el elemento %d\n", id, first);
	// Control de acceso
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	printf("El cliente %d ya tiene el mutex general el elemento %d\n", id, first);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;

	// Si el buffer esta vacio,  esperar a que se inserte un elemento y volverlo a intentar
	if (ocupados == 0)
	{
		ReleaseMutex(hMutexBuffer);
		printf("GTISharedBuffer::get => Buffer vacio. Cliente %d esperando a que venga alguno...\n", id);
		ret = WaitForSingleObject(consumidores[id].hSemConsumidor, waitTimeout);
		if (ret == WAIT_TIMEOUT)
			return BUFFERC_EMPTY;
		else if (ret == WAIT_FAILED)
			return BUFFERC_ERROR;
		return get(elemento, id);
//		return BUFFERC_EMPTY;
	}

	// Si ya ha leido el dato mas antiguo, esperar a que se quite del buffer y volverlo a intentar
	if (consumidores[id].datoLeido == TRUE)
	{
		printf("GTISharedBuffer::get => El cliente %d ya tiene el elemento. Esperando a que se libere...\n ", id);
		for (int h=0; h < BUFFERC_MAX_CONSUMIDORES; h++)
		{
			if (consumidores[h].activo == 1)
				printf("El %d tiene un %d\n", h, consumidores[h].datoLeido);
		}
		consumidores[id].esperandoLiberacion = TRUE;
		ReleaseMutex(hMutexBuffer);



		ret = WaitForSingleObject(consumidores[id].hSemConsumidor, waitTimeout); //2000); 
		if (ret == WAIT_TIMEOUT)
			return BUFFERC_TIMEOUT;
		else if (ret == WAIT_FAILED)
			return BUFFERC_ERROR;

//printf("Vuelve a intentar el get\n");
		return get(elemento, id);
	}

	// Copiar el dato en el puntero recibido
	copiarLectura(elemento, first);
	consumidores[id].datoLeido = TRUE;
	
	// Si todos los consumidores han leido el dato, avanzar 1 la posicion del ultimo elemento y restar 1 a ocupados
	bool todosHanLeido = TRUE;
	for (int i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		if ((consumidores[i].activo == TRUE) && (consumidores[i].datoLeido == FALSE))
		{
			todosHanLeido = FALSE;
			break;
		}
	}
	if (todosHanLeido)
		liberarHueco();

	// Liberar el control del buffer y retornar
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
long GTISharedBuffer::put(void *elemento)
{
	int insertarElemento = 1;

	// Control de acceso
	long res = BUFFERC_ERROR;
	
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;

	// Si el buffer esta vacio, liberar a consumidores que estuvieran esperando
	if (ocupados == 0)
		liberarConsumidores();

	// Si el buffer esta lleno, actuar en funcion de la politica elegida
	if (tamBuffer - ocupados == 0)
	{
		// Discard : Retornar sin insertar elemento
		if (politicaPut == BUFFERC_PUT_DISCARD)
		{
			// Liberar el control del buffer
			ReleaseMutex(hMutexBuffer);

			return BUFFERC_OK;
		}

		// Wait : Esperar a que quede un hueco y repetir put
		else if (politicaPut == BUFFERC_PUT_WAIT)
		{
			// Liberar el control del buffer
			ReleaseMutex(hMutexBuffer);

			// Esperar a que quede un hueco -> Aqui mejor un semaforo, ahora repetira hasta conseguirlo

			// Repetir put
			return put(elemento);
			
		}

		// Replace : Liberar el hueco del elemento mas antiguo para insertar
		else if (politicaPut == BUFFERC_PUT_REPLACE)
		{
			liberarHueco();
		}

		// Politica desconocida: Error
		else 
		{
			// Liberar el control del buffer y retornar
			ReleaseMutex(hMutexBuffer);

			fprintf(stderr, "Error en GTISharedBuffer::put() -> politicaPut desconocida (%d)\n", politicaPut);

			return BUFFERC_ERROR;
		}

	}

	// Copiar el elemento en el buffer
	copiarEscritura(elemento, last);

	// Avanzar 1 la posicion del primer elemento y sumar 1 a ocupados
	last = (last+1) % tamBuffer;
	ocupados++;

	// Liberar el control del buffer y retornar
	ReleaseMutex(hMutexBuffer);

	return BUFFERC_OK;
}

/**
 *	Method to get number of non-free positions in buffer
 * 
 *
 * \return Return the desired value
 */
// Retorna el numero de huecos ocupados en el buffer
long GTISharedBuffer::getOcupados()
{
	// Control de acceso
	long res;
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;

	res = ocupados;

	// Liberar el control del buffer y retornar
	ReleaseMutex(hMutexBuffer);
	return res;
}

/**
 *	Method to get number of free positions in buffer
 * 
 *
 * \return Return the desired value
 */
long GTISharedBuffer::getLibres()
{
	// Control de acceso
	long res;
	int ret = WaitForSingleObject(hMutexBuffer, waitTimeout);
	if (ret == WAIT_TIMEOUT)
		return BUFFERC_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return BUFFERC_ERROR;

	// Calcular huecos libres
	res = tamBuffer - ocupados;

	// Liberar el control del buffer y retornar
	ReleaseMutex(hMutexBuffer);
	return res;
}

/**
 *	Method to get buffer wait timeout
 * 
 *
 * \return Return the desired value
 */
long GTISharedBuffer::getWaitTimeout()
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
int GTISharedBuffer::getPoliticaPut()
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
void GTISharedBuffer::setWaitTimeout(long newValue)
{
	waitTimeout = newValue;
}

/**
 *	Method to set Buffer put policy
 *	Posible values are:
 *		- BUFFERC_PUT_DISCARD	
 *		- BUFFERC_PUT_WAIT
 *		- BUFFERC_PUT_REPLACE
 * 
 * \param newPolicy Value for new policy in system
 *
 */
void GTISharedBuffer::setPoliticaPut(int newPolicy)
{
	politicaPut = newPolicy;
}


/**
 *	Internal Method to read one element from buffer. This method can be implemented in child classes
 *
 * \param elemento pointer to structure to copy data from buffer
 * \param posicion Position to read
 *
 */
void GTISharedBuffer::copiarLectura(void *elemento, long posicion) {}

/**
 *	Internal Method to copy one element in buffer.
 *	This method can be implemented in child classes
 *
 * \param elemento pointer to structure to copy in buffer
 * \param posicion Position to copy
 *
 */
void GTISharedBuffer::copiarEscritura(void *elemento, long posicion) {}



void GTISharedBuffer::printError(long res)
{
	if (res == BUFFERC_FULL)
		printf("Buffer lleno\n");
	else if (res == BUFFERC_EMPTY)
		printf("Buffer vacio\n");
	else if (res == BUFFERC_ERROR)
		printf("Error\n");
	else if (res == BUFFERC_TIMEOUT)
		printf("Timeout\n");
	else
		printf("Error desconocido en GTISharedBuffer: %d\n", res);
}

//////////////////////////////////
// METODOS PRIVADOS
//////////////////////////////////

void GTISharedBuffer::liberarHueco()
{
	// Dejar un hueco libre
	first = (first+1) % tamBuffer;
	ocupados--;

	liberarConsumidores();
}

int GTISharedBuffer::liberarConsumidores()
{
	// Liberar a los consumidores que puedan estar esperando
	for (int i = 0; i < BUFFERC_MAX_CONSUMIDORES; i++)
	{
		if (consumidores[i].activo == TRUE)
		{
			consumidores[i].datoLeido = FALSE;

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

/**
* \file DiVAFreeBuffer.h
* \author Alvaro Garcia Martin
* \date 23-01-2007
* \version 1.0
* \brief Class to describe an shared buffer in this framework
* 
* This file is the header of the DiVAFreeBuffer class.
* This class represents a buffer with one producer and multiple consumer
* It encapsulates the control and access buffer (for a shared buffer)
*
* All the class that inherit from this class must:
*	- Overload constructor and destructor
*	- Overload methods copiarLectura(...) y copiarEscritura(...)
*
* Recommended use:
*	- Producer(only one) create the buffer and define its behavior
*	- Once the buffer is created, each consumer must:
*		- Call  registrarConsumidor(...) and save the identification number
*		- Use this id in all the get(...) calls
*		- Call eliminarConsumidor(...) when no longer it is going to use the buffer
* 
*
*/	


//class description
/**
 * \class DiVAFreeBuffer
 * \brief To instanciate a image shared buffer
 *
 * Write detailed description here.
 */


#if !defined(AFX_BUFFERCOMPARTIDO_H_INCLUDED_)
#define AFX_BUFFERCOMPARTIDO_H_INCLUDED_

//#include <afxmt.h>
#define NOMINMAX
#include <windows.h>

/**
 *	\def BUFFERC_MAX_CONSUMIDORES
 *	Maximun number of consumer in the system
 */
#define BUFFERC_MAX_CONSUMIDORES	30

// Return code
/**
 *	\def BUFFERC_OK
 *	Return code when buffer status is OK
 */
#define BUFFERC_OK	   0

/**
 *	\def BUFFERC_ERROR
 *	Return code when a generic error happends
 */
#define BUFFERC_ERROR  -1	
/**
 *	\def BUFFERC_FULL
 *	Return code when the buffer is full of data
 */
#define BUFFERC_FULL   -2	
/**
 *	\def BUFFERC_EMPTY
 *	Return code when the buffer is empty
 */
#define BUFFERC_EMPTY  -3	

/**
 *	\def BUFFERC_TIMEOUT
 *	Return code when the timeout operation don't be made
 */
#define BUFFERC_TIMEOUT -4	
/**
 *	\def BUFFERC_TIMEOUT
 *	Return code when the timeout operation don't be made
 */
#define BUFFERC_NO_MIN_CONSUMERS -5	

/**
 *	\def BUFFERC_PUT_DISCARD
 *	Put policy - Discard new element when buffer is full
 */
#define BUFFERC_PUT_DISCARD	0	

/**
 *	\def BUFFERC_PUT_WAIT
 *	Put policy - Wait until free space for new data when buffer is full
 */
#define BUFFERC_PUT_WAIT	1	

/**
 *	\def BUFFERC_PUT_REPLACE
 *	Put policy - Replace oldest data with new data when buffer is full
 */
#define BUFFERC_PUT_REPLACE	2	

// Otros parametros de funcionamiento
//#define BUFFERC_WAIT_TIMEOUT INFINITE
//#define BUFFERC_WAIT_TIMEOUT 1000

 /** 
  * \struct ConsumidorBuffer
  * \brief ConsumidorBuffer instance structure
  * 
  * Structure to define a buffer consumer
  * 
  */
typedef struct tagConsumidorBuffer
{
	///Control if consumer is active
	bool activo;

	int 	lastReadenPosition;
	long	lastReadenItemId;
	// Control if this consumer is waiting for the rest to read current element
	bool esperandoLiberacion;

	///Semaphore for consumer
	HANDLE hSemConsumidor;

	///???
	void *userData;
}ConsumidorBuffer;


class DiVAFreeBuffer 
{

protected:

// Attributes to basic handle buffer	

	///Number of images in the buffer
	long tamBuffer;

	///First no overwritable position (use with consume control) 
	long first;			

	///Last writen
	long last;		
	
	///Number of non free positions
	long ocupados;		

	///Buffer mutex
	HANDLE  hMutexBuffer;
	///Mutex for waiting for space for writing
	HANDLE  hMutexWriting;
	int		_writerWaitingReleasing;

// Attibutes to allow multiple put policies

	///Put Policy when buffer is full(see BUFFER_PUT_XXXX for policies)
	int politicaPut;
	int _minNumberOfConsumers;
	int _numberOfConsumers;

	// Semaforo para espera del productor
//	HANDLE  hSemProductor;	

// Attributes to work with multiple consumers	

	///Consumer table to follow customer behavior
	ConsumidorBuffer consumidores[BUFFERC_MAX_CONSUMIDORES];

	///Timeout for operation
	long waitTimeout;
	///Flag that indicates wheter wait or not to all consumers to have read a certain frame
	int _consumecontrol;

// Metodos privados
private:
	///Allow to consumer (that are waiting a new data) read data	
	int liberarConsumidores();
	int getPositionToWrite();

public:
	
	///Init the buffer in memory, init 'semaforos'
	DiVAFreeBuffer(long tamBuffer = 4, long waitTimeout = 2000, int consumecontrol = 1,int minNumberOfConsumers=0);
	
	///Free buffer and 'semaforos'
	virtual ~DiVAFreeBuffer();

	// Registra un nuevo consumidor en el buffer
	// Retorna un identificador para que lo utilice en get
	// Debe ser utilizada por cualquier consumidor antes de ningun get(...)

	///Register a new consumer in buffer
	long registrarConsumidor(void *userData = NULL);

	// Da de baja un consumidor. Debe ser llamada por el cliente cuando ya no vaya a solicitar elementos
	///Eliminate a consumer from system. This method must be called when consumer don't solicit more elements
	int eliminarConsumidor(long id);
	int getNumberConsumers(){return _numberOfConsumers;};
	// Retorna un puntero a una copia de ejemplo de un elemento del buffer
	void * getEmptySample();

	///Read the element with specified identifier 
	long get(void *elemento, long idElem, long idConsumidor);

//	///Read the first position (removing it)
//	long get(void *elemento, long idConsumidor);
	
	///Insert one element in the last position. If the buffer is full, data is discarded and returns BUFFERC_FULL
	long put(void *elemento);
	///Get occupied positions
	long getOcupados(){return ocupados;};
#if 0
	///Get free positions
	long getLibres();
#endif
	///Get timeout parameters
	long getWaitTimeout();

	///Get PUT policy
	int getPoliticaPut();

	///Set wait timeout
	void setWaitTimeout(long newValue);

	///Data copy implementation
	virtual void copiarLectura(void *elemento, long posicion);

	///Data read implementation
	virtual void copiarEscritura(void *elemento, long posicion);

	///Data sample copy creation implementation
	virtual void* getSampleElement(){return NULL;};

	///Element by id search implementation
	virtual long buscarElemento(long idFrame,int* position){return -1;};
	virtual char* getSourceId(){return NULL;};
	///To print error ocurred
	static void printError(long res);


};

#endif // !defined(AFX_BUFFERCOMPARTIDO_H_INCLUDED_)

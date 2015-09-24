//file DiVAAlgorithm
/**
* \file DiVAAlgorithm.h
* \author Alvaro Garcia Martin
* \date 01-03-07
* \version 1.2
* \brief Class to define a template for integrating video processing algorithms in DiVA
* 
* This file is the header of the DiVAAlgorithm class.
*
*/	


//class DiVAAlgorithm
/**
 * \class DiVAAlgorithm
 * \brief Defines a template for integrating video processing algorithms in DiVA
 * 
 */

#include "DiVAThread.h"
#include "DiVAClientBuffer.h"
#include "DiVADisplay.h"
#include "DiVAImage.h"
#include "DiVACrono.h"
#include "DiVADataClient.h"

#include <iostream>
#include <conio.h>

#if !defined(AFX_DIVAALGORITHM_H_INCLUDED_)
#define AFX_DIVAALGORITHM_H_INCLUDED_

#define CAPTURE_BUF 1
#define CAPTURE_SEQ 2

//Dumping Formats:
#define IMAGE_FILE_FORMAT 	"image_%06d.bmp"
#define MASK_FILE_FORMAT 	"mask_%06d.bmp"
#define EDGES_FILE_FORMAT   "edges_%06d.bmp"
#define ESPERA_MUTEX 2000

class DiVAAlgorithm:public DiVAThread
{
	public:
	DiVAAlgorithm(char* frameServername, int portnumber,
			BOOL fileDumping=TRUE,
			BOOL display=TRUE,
			char* dataServerName=NULL,
			char* contentServerName=NULL,	
			BOOL dataServerDumping=FALSE,
			BOOL contentServerDumping=FALSE,
			int captureMode=CAPTURE_BUF,
			BOOL displayQt=TRUE);

	~DiVAAlgorithm(); 
	char* getAlgorithmName(){return _algorithmName;};
	//Initialization
	virtual int init();
	//Execution Control
	int setEnd();
	int process();

	//Stop algorithm
	int stop();

	//Connections
	int connect();
	int getFrame(DiVAImage** pImage,long idFrame);
	int receiveFrame(DiVAImage** pImage,long idFrame);
	/***************************************************************************************************/
	/*To be overwriten**********************************************************************************/
	//Data bases
	virtual int receiveData(void** data){return 0;};
	virtual int receiveContentData(void** data){return 0;};
	virtual int releaseData(void* data){return 0;};
	virtual int releaseContentData(void* data){return 0;};
	virtual int dumpData(){return 0;};
	virtual int dumpContentData(){return 0;};
	virtual int dumpResultsInFiles(){return 0;};
	//Image Processing
	virtual int processFrame(DiVAImage* pImage,void* pdata=NULL,void* pContentData=NULL);
	//Displaying
	virtual int refreshDisplay(){return 0;};
	virtual int sendFrameQt(){return 0;};
	/***************************************************************************************************/
	private:
	BOOL	_fileDumping;
	char*	_dataServerName;
	char*	_contentServerName;
	BOOL	_dataServerDumping;
	BOOL	_contentServerDumping;
	BOOL	_display;
	BOOL	_displayQt;
	long _lastId;
	int _captureMode;

	//semaforo para sincronizacion entre los metodos stop() y process().
	HANDLE hMutex;

	public:
	int		getCaptureMode(){return _captureMode;};
	BOOL	getfileDumping(){return _fileDumping;};
	char*	getdataServerName(){return _dataServerName;};
	char*	getcontentServerName(){return _contentServerName;};
	BOOL	getdataServerDumping(){return _dataServerDumping;};
	BOOL	getcontentServerDumping(){return _contentServerDumping;};
	BOOL	getdisplay(){return _display;};
	BOOL	getdisplayQt(){return _displayQt;};
	DiVACrono* getCrono(){return _pCrono;};
	long getLastId(){return _lastId;};
	long setLastId(long lastId){_lastId = lastId;return _lastId;};
	char* getDumpingpathRoot(){return _dumpingpathRoot;};
	void setDisplay(bool display);
	protected:
	char* setAlgorithmName(char* name);
	char* getSourceId();
	char* getDumpingpath(){return _dumpingpath;};
	
	char* _dumpingpath;
	char* _dumpingpathRoot;
	char*	_frameServername;
	int		_portnumber;
	char*	_algorithmName;
	DiVAClientBuffer*	_pFrameBuffer;
	DiVAClient*	_pFrameClient;
	
	DiVADisplay*	_pDisplay;
	DiVACrono*	_pCrono;
	
	DiVADataClient* _pDataClient;
	/*En un futuro
	DiVAdataClient* _pContentClient;
	*/
	BOOL createDisplay(char* title,int rows,int columns,int h, int w);
//Connections
	int connect2FrameServer();
	int connect2DataServer(){return 0;};
	int connect2ContentServer(){return 0;};
};

int fileExists(char* filename);

#endif

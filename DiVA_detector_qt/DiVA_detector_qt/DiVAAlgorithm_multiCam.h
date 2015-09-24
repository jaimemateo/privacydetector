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

class DiVAAlgorithm_multiCam:public DiVAThread
{
	public:
	DiVAAlgorithm_multiCam(char* frameServername1, int portnumber1,
			char* frameServername2, int portnumber2,
			char* frameServername3, int portnumber3,
			BOOL fileDumping=TRUE,
			BOOL display=TRUE,
			char* dataServerName=NULL,
			char* contentServerName=NULL,	
			BOOL dataServerDumping=FALSE,
			BOOL contentServerDumping=FALSE,
			int captureMode=CAPTURE_BUF);

	~DiVAAlgorithm_multiCam(); 
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
	int getFrame(DiVAImage** pImage1,DiVAImage** pImage2,DiVAImage **pImage3,long idFrame1,long idFrame2,long idFrame3);
	int receiveFrame(DiVAImage** pImage1,DiVAImage** pImage2,DiVAImage** pImage3,long idFrame1,long idFrame2,long idFrame3);
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
	virtual int processFrame(DiVAImage* pImage1,DiVAImage* pImage2,DiVAImage* pImage3,void* pdata=NULL,void* pContentData=NULL);
	//Displaying
	virtual int refreshDisplay(){return 0;};
	/***************************************************************************************************/
	private:
	BOOL	_fileDumping;
	char*	_dataServerName;
	char*	_contentServerName;
	BOOL	_dataServerDumping;
	BOOL	_contentServerDumping;
	BOOL	_display;
	long _lastId1;
	long _lastId2;
	long _lastId3;
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
	DiVACrono* getCrono(){return _pCrono;};
	long getLastId1(){return _lastId1;};
	long getLastId2(){return _lastId2;};
	long getLastId3(){return _lastId3;};
	long setLastId1(long lastId){_lastId1 = lastId;return _lastId1;};
	long setLastId2(long lastId){_lastId2 = lastId;return _lastId2;};
	long setLastId3(long lastId){_lastId3 = lastId;return _lastId3;};
	char* getDumpingpathRoot(){return _dumpingpathRoot;};
	void setDisplay(bool display);
	protected:
	char* setAlgorithmName(char* name);
	char* getSourceId();
	char* getDumpingpath(){return _dumpingpath;};
	
	char* _dumpingpath;
	char* _dumpingpathRoot;
	char*	_frameServername1;
	char*	_frameServername2;
	char*	_frameServername3;
	int		_portnumber1;
	int		_portnumber2;
	int		_portnumber3;
	char*	_algorithmName;
	DiVAClientBuffer*	_pFrameBuffer1;
	DiVAClientBuffer*	_pFrameBuffer2;
	DiVAClientBuffer*	_pFrameBuffer3;
	DiVAClient*	_pFrameClient1;
	DiVAClient*	_pFrameClient2;
	DiVAClient*	_pFrameClient3;

	
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

#include <QtWidgets/QMainWindow>
#include "DiVAAlgorithm.h"
#include "DETECTOR.h"
#include "opencvconverter.h"
#include "DiVAImage.h"
#include "DiVA_QtSignals.h"

class DiVA_detector:public DiVAAlgorithm 
{
public:
	DiVA_detector(char* frameServername = NULL, int portnumber= NULL,
			BOOL fileDumping= NULL,
			BOOL display= NULL,
			char* dataServerName= NULL,
			char* contentServerName= NULL,	
			BOOL dataServerDumping= NULL,
			BOOL contentServerDumping= NULL,
			int captureMode=NULL);
	~DiVA_detector();
	int processFrame(DiVAImage* pImage,void* pdata=NULL,void* pContentData=NULL);
	int setParam(int parametro, int valor_int=0);
	int getParam(int parametro, int *valor_int);
	int refreshDisplay();

	DiVA_QtSignals *senales;

public:
	detector *Algor;
	OpenCVConverter *pconversor;
};

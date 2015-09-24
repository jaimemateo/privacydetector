#include "DiVA_detector.h"
#include "DETECTOR.h"
using namespace std;

DiVA_detector::DiVA_detector(char* frameServername,
	int portnumber,
	BOOL fileDumping,
	BOOL display,
	char* dataServerName,
	char* contentServerName,
	BOOL dataServerDumping,
	BOOL contentServerDumping,
	int captureMode) :DiVAAlgorithm(frameServername, portnumber,
	fileDumping,
	display,
	dataServerName,
	contentServerName,
	dataServerDumping,
	contentServerDumping,
	captureMode)
{
	this->Algor = new detector;
	this->pconversor = new OpenCVConverter;
	this->senales = new DiVA_QtSignals();
	setAlgorithmName("Scrambling");
}

DiVA_detector::~DiVA_detector()
{

	delete this->Algor;
	delete this->pconversor;
}


int DiVA_detector::processFrame(DiVAImage* pImage, void* pdata, void* pContentData)
{
	IplImage * frame;
	frame = this->pconversor->getIplImage(pImage);
	this->Algor->process(frame);

	return 1;
}

int DiVA_detector::refreshDisplay()
{
	switch (this->Algor->scram)
	{
		case PRESCRAM:
		{
			IplImage imagen_ipl(this->Algor->framepre);
			QImage *img1 = new QImage((uchar*)imagen_ipl.imageData, imagen_ipl.width, imagen_ipl.height, imagen_ipl.widthStep, QImage::Format_RGB888);
			emit this->senales->refresh_display(QPixmap::fromImage(img1->rgbSwapped()));
			return 1;
		}
		case POSTSCRAM:
		{
			IplImage imagen_ipl(this->Algor->framepost);
			QImage *img2 = new QImage((uchar*)imagen_ipl.imageData, imagen_ipl.width, imagen_ipl.height, imagen_ipl.widthStep, QImage::Format_RGB888);
			emit this->senales->refresh_display(QPixmap::fromImage(img2->rgbSwapped()));
			return 1;
		}
	}
	//this->Algor->showresults();
	return 1;
	
}

int DiVA_detector::setParam(int parametro, int valor_int){

	this->Algor->setParam(parametro, valor_int);
	return 1;
}
int DiVA_detector::getParam(int parametro, int *valor_int){
	this->Algor->getParam(parametro, valor_int);
	return 1;
}

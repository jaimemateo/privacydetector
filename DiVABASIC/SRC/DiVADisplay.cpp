//file description
/**
 * \file DiVADisplay.cpp
 * \author Víctor Fernández-Carbajales Cañete
 * \date 16-11-2006
 * \version V1.0                                        
 * \brief File from DiVADisplay class
 *															
 * OpenCV Methods: CvFont, CvSize, CVScalar, cvInitFont, cvNamedWindow, 
 *                 cvDestroyWindow, cvShowImage, cvWaitKey, cvReleaseImage, 
 *                 cvSetImageROI, cvResize, cvResetImageROI, cvCvtColor, 
 *                 cvGetTextSize, cvSet2D, cvPutText.
 */

// Need Libraries
#include "DiVADisplay.h"

/// Public Method

/**
 *	Default constructor of class
 *
 *
 *
 * \return New DiVADisplay
 */
DiVADisplay::DiVADisplay() {
}

/**
 *	Constructor of DiVADisplay class
 *
 * \param name Name of window
 * \param imH Base image height (default 500)
 * \param imW Base image widht (default 500)
 * \param rows Number of rows in multiple image (default 1)
 * \param columns Number of columns in multiple image (dafult 1)
 * \param gapR Size of frame inter-rows of images (default 10)
 * \param gapC Size fo frame inter-column of images (default 10)
 * \param titles Active titles of image (default false)
 * \param size Size of font (default 1.0)
 * \param visible Active a windows display (default false)
 * \param allEquals Indicate if all image have equals size (default false) ! Deprecated
 * \param sizeWindow Indicate the objetive size of the display (default false)
 *
 * \return New DiVADisplay
 */
DiVADisplay::DiVADisplay(char* name, unsigned int imH, unsigned int imW,unsigned int rows,
		                   unsigned int columns,unsigned int gapR, unsigned int gapC, bool titles,
						   double size, bool visible, bool allEquals, bool sizeWindow) {

	int sizeArray = 0, titleGaps = 0, i = 0; 
	int baseSize = MINGAP;
	
	/// Initialitation of base arrays
	sizeArray = rows * columns;
	this->imageArray = (DiVAImage**) malloc(sizeArray * sizeof(DiVAImage*));
	this->text = (char**) malloc(sizeArray * sizeof(char*));
	for(i = 0; i < sizeArray; i++) {
		this->imageArray[i] = NULL;
		this->text[i] = NULL;
	}
	this->position = (int**) malloc(sizeof(int)*2);
	this->position[0] = (int*)malloc(sizeof(int)*sizeArray);
	this->position[1] = (int*)malloc(sizeof(int)*sizeArray);

	/// Complete the basic attributes
	this->setWindowsName(name);
	this->setImWidthOutput(imW);
	this->setImHeightOutput(imH);
	this->setGlobalWidht((imW * columns) + (gapC * (columns+1)));
	this->setGlobalHeight((imH * rows) + (gapR * (rows+1)));
	this->setRows(rows);
	this->setColumns(columns);
	this->setGapRows(gapR);
	this->setGapColumns(gapC);
	this->setSizeFont(size);
	if (titles) {
		titleGaps += (((int)(baseSize * this->getSizeFont()) * this->getRows())) + 
					 (2*((((int)(baseSize * this->getSizeFont())) * this->getRows())/8));
		this->setGlobalHeight(this->getGlobalHeight()+titleGaps);
		this->onHasTitle();
	}
	else 
		this->offHasTitle();
	if (visible) 
		this->onVisible();
	else 
		this->offVisible();
	if (sizeWindow)
		this->onSizeWindow();
	else
		this->offSizeWindow();

	/// Initial state of display
	this->offActive();
	this->image = NULL;
	this->createFont();
	this->update = true;
	this->initialization = false;
	cvNamedWindow(name);
}

/**
 * Destructor of class
 *
 *
 *
 *
 */
DiVADisplay::~DiVADisplay() {
	unsigned int i = 0;
	unsigned int initialX = 0, initialY = 0;

	/// Destroy internal arrays
	if (this->imageArray != NULL) {
		for(i = 0; i < (this->rows * this->columns); i++) {
			if (this->imageArray[i] != NULL) {
				delete this->imageArray[i];
				this->imageArray[i] = NULL;
			}
		}
		free(this->imageArray);
		this->imageArray = NULL;
	}
	if (this->text != NULL) {
		for(i = 0; i < (this->rows * this->columns); i++) {
			if (this->text[i] != NULL) {
				delete this->text[i];
				this->text[i] = NULL;
			}
		}
		free(this->text);
		this->text = NULL;
	}
	if (this->position != NULL) {
		for(i = 0; i < 2; i++) {
			if (this->position[i] != NULL) {
				delete this->position[i];
				this->position[i] = NULL;
			}
		}
		free(this->position);
		this->position = NULL;
	}

	/// Destroy font and image
	if (this->getDiVAImage() != NULL) {
		delete this->getDiVAImage();
		this->image = NULL;
	}
	if (this->getFont() != NULL) {
		this->destroyFont();
		this->font = NULL;
	}

	/// Final state of display
	if (this->getActive())
		this->offWindows();
	cvDestroyWindow(name);
}

/**
 *	This method set value of attribute name of class
 *
 * \param name New name of display's window
 *
 *
 */
void DiVADisplay::setWindowsName(char* name) {
	if (name != NULL)
		sprintf(this->name, "%s", name);
}

/**
 * This method give a name of windows
 *
 *
 *
 * \return Name of display's window
 */
char* DiVADisplay::getWindowName() {
	return this->name;
	this->update = false;
}

/**
 * This method give a image of windows
 *
 *
 *
 * \return Image of display
 */
DiVAImage* DiVADisplay::getDiVAImage() {
	return this->image;
}

/**
 * This method give state of visible attribute
 *
 *
 *
 * \return State of visible
 */
bool DiVADisplay::getVisible() {
	return this->visible;
}

/**
 * This method active attribute visible
 *
 *
 *
 *
 */
void DiVADisplay::onVisible() {
	this->visible = true;
	this->update = false;
}

/**
 * This method desactive attribute visible
 *
 *
 *
 *
 */
void DiVADisplay::offVisible() {
	this->visible = false;
	this->update = false;
}

/**
 * This private method active attribute sizewindow
 *
 *
 * \param wHeight Height of window
 * \param wWidth  Width of window
 * 
 */
void DiVADisplay::onSizeWindow(unsigned int wHeight, unsigned int wWidth) {
	this->sizeWindow = true;

	float w1 = (float)getGlobalWidth();
	float h1 = (float)getGlobalHeight();
	float w2,h2;
	w2=w1;
	h2=h1;
	if (w1>wWidth) {
		w2	=	(float)wWidth;
		h2	= 	h1*(wWidth/w1);
	}
	if (h2>h1) {		
		h2	= 	(float)h1;
		w2	=	w2*(h1/h2);
	}
	int he= (int)ceil(h2);
	int wi= (int)ceil(w2);
	this->wHeight = he;
	this->wWidth = wi;
	this->update = false;
}

/**
 * This private method desactive attribute sizeWindow
 *
 *
 *
 *
 */
void DiVADisplay::offSizeWindow() {
	this->sizeWindow = false;
	this->update = false;
}

/**
 * This method active a window of display
 *
 *
 *
 *
 */
void DiVADisplay::onWindows() {
	char window[8] = "Windows";

	if (this->getWindowName() != NULL)  {
		cvNamedWindow(this->getWindowName(), CV_WINDOW_AUTOSIZE);
		return;
	}
	else {
		sprintf(window,"%s",this->name);
		cvNamedWindow(this->getWindowName(), CV_WINDOW_AUTOSIZE);
		return;
	}
}

/**
 * This method desactive a window of display
 *
 *
 *
 *
 */
void DiVADisplay::offWindows() {
	cvDestroyWindow(this->getWindowName());
	return;
}

/**
 * This method insert image in some place of display
 *
 * \param image New insert image
 * \param row Row position of new image
 * \param column Column position of new image
 * \param title Title of this image (default NULL)
 *
 *
 */
void DiVADisplay::insertImage(DiVAImage* image, unsigned int row, unsigned int column, char* title) {
	if (this->getHasTitle()) 
		if (title != NULL)
			this->insertText(title,row,column);
	
	if (image != NULL)
		this->setDiVAImageN(image,row,column);

	if (this->initialization == false) {
		this->initializate(row,column);
		this->initialization = true;
	}

	this->update = false;
	return;
}

/**
 * This method insert text on top of image
 *
 * \param text New text to insert
 * \param row Row position of new text
 * \param column Column position of new text
 *
 *
 */
void DiVADisplay::insertText(char* text, unsigned int row, unsigned int column) {
	if (text != NULL)
		this->setTitleN(text,row,column);

	this->update = false;
	return;
}

/**
 * This method is a web display
 *
 *
 *
 *
 */
void DiVADisplay::showWeb() {
}

/**
 * This method show a image in normal display
 *
 *
 *
 *
 */
void DiVADisplay::windowVisible() {
	unsigned int baseSize = MINGAP;
	unsigned int i = 0, j = 0, k = 0, w = 0;
	unsigned int limitX = 0, sizeText = 0;
	unsigned int x = 0, y = 0;
	unsigned int gapText = 0, sizefont = 0;
	IplImage *tmp = NULL, *tmp2 = NULL, *global = NULL;
	DiVAImage* newImage = NULL, *temporal = NULL;
	DiVASCALAR black = DiVAScalarAll(1);
	CvSize size;

	if (!update) {
		this->CreateImage();
		global = OpenCVConverter::getIplImage(this->getDiVAImage());

		// Check space color
		for(i = 1; i <= this->getRows(); i++) {
			for(j = 1; j <= this->getColumns(); j++) {
				if (this->getDiVAImageN(i,j) != NULL) {
					if (this->getDiVAImageN(i,j)->getNChannels() != 3) {
						temporal = new DiVAImage(this->getImWidthInput(),this->getImHeightInput(),3 ,this->getDiVAImageN(i,j)->getDepth()); 
						tmp2 = OpenCVConverter::getIplImage(temporal);
						delete temporal;
						tmp = OpenCVConverter::getIplImage(this->getDiVAImageN(i,j));
						cvCvtColor(tmp,tmp2,CV_GRAY2RGB);
						cvReleaseImage(&tmp);
					}
					else 
						tmp2 = OpenCVConverter::getIplImage(this->getDiVAImageN(i,j));
					// Allocate images (imagen inicial = tmp2)
					x = this->getPositionxN(i,j);
					y = this->getPositionyN(i,j);
					cvSetImageROI(global, cvRect(x,y,this->getImWidthInput(),this->getImHeightInput()));
					cvCopy(tmp2,global);
					cvReleaseImage(&tmp2);
					cvResetImageROI(global);
					// Include Title
					if (this->getTitleN(i,j) != NULL) {
						limitX = this->getGlobalWidth();
						x = this->getPositionxN(i,j);
						y = this->getGlobalHeight() - (this->getImHeightInput() * (i-1));
						cvGetTextSize(this->getTitleN(i,j),this->getFont(),&size,NULL);
						sizeText = this->getImWidthInput() - size.width;
						if (sizeText > 0)
							x += (((int)(this->getImWidthInput()/2)) - ((int)(size.width/2)));
	
						sizefont = MINGAP;
						sizeText = (int)(sizefont * this->getSizeFont());
						gapText = sizeText + (int)(2*(sizeText/8));
						y -= (gapText * (i));
						y += (int)(sizeText/8);

						temporal = new DiVAImage(sizeText,sizeText,3,8);
						temporal->setPixels(black);
						tmp = OpenCVConverter::getIplImage(temporal);
						delete temporal;
						cvSetImageROI(global, cvRect(x,y,sizeText,sizeText));
						cvCopy(tmp,global);
						cvResetImageROI(global);
						cvReleaseImage(&tmp);
						cvPutText(global ,this->getTitleN(i,j),cvPoint(x,y),this->getFont(),cvScalar(256,256,256,0));
					}
				}
			}
		}
	
		// Global image resize (imagen inicial = global)
		if (this->sizeWindow)
			temporal = new DiVAImage(this->wWidth,this->wHeight,3,this->getDiVAImage()->getDepth());
		else 
			temporal = new DiVAImage(this->getGlobalWidth(),this->getGlobalHeight(),3 ,this->getDiVAImage()->getDepth());
		tmp2 = OpenCVConverter::getIplImage(temporal);
		delete temporal;
		cvResize(global,tmp2);
		newImage = OpenCVConverter::getDiVAImage(tmp2);
		//newImage->setVAlign(1);
		newImage->setVAlign(0);
		this->setDiVAImage(newImage);
		delete newImage;
		cvReleaseImage(&global);
		cvReleaseImage(&tmp2);
	}
	// Check and Visualize
	if (this->getActive() == false)
		this->onWindows();
	if (this->getDiVAImage() != NULL) {
		tmp = OpenCVConverter::getIplImage(this->image);
		if (tmp != NULL) {
			cvShowImage(this->getWindowName(),tmp);
			cvWaitKey(1);
			cvReleaseImage(&tmp);
		}
	}
	this->update = true;
	return;
}

/// Private Method

/**
 * This private method set value of attribute image of class
 *
 * \param image New image of display
 *
 *
 */
void DiVADisplay::setDiVAImage(DiVAImage* image) {
	if (this->getDiVAImage() != NULL) 
		delete this->getDiVAImage();
	this->image = image->clone();
	this->update = false;
}

/**
 * This private method set value of attribute imageArray of class in pos n * m
 *
 * \param image New image of display in position (n,m)
 * \param n Row of image
 * \param m Column of image
 *
 *
 */
void DiVADisplay::setDiVAImageN(DiVAImage* image, int n, int m) {
	int position = 0;

	if (this->getDiVAImageN(n, m) != NULL) 
		delete this->getDiVAImageN(n, m);
	position = this->calcule(n,m);
	this->imageArray[position] = image->clone();
	this->update = false;
}

/**
 * This private method set value of attribute text of class in pos n * m
 *
 * \param char* New text of display in position (n,m)
 * \param n Row of text
 * \param m Column of text
 *
 *
 */
void DiVADisplay::setTitleN(char* text, int n, int m) {
	int position = 0, i = 0;

	if (this->getTitleN(n, m) != NULL) 
		free(this->getTitleN(n, m));
	position = this->calcule(n,m);
	for(i = 0; text[i] != '\0'; i++);
	this->text[position] = (char*)malloc((i+1)*sizeof(char));
	strcpy(this->text[position],text);
	this->update = false;
}

/**
 * This private method set value of position (x,y) initial of the imagen in pos n * m
 *
 * \param char* New text of display in position (n,m)
 * \param n Row of text
 * \param m Column of text
 *
 *
 */
void DiVADisplay::setPositionN(int x,int y, int n, int m) {
	int position = 0;

	position = this->calcule(n,m);
	this->position[0][position] = x;
	this->position[1][position] = y;
	this->update = false;
}

/**
 * This private method set value of positions
 *
 * \param int** New positions of display
 *
 *
 */
void DiVADisplay::setPositions(int** position) {
	int i = 0;

	if (this->position != NULL) {
		for(i = 0; i < 1; i++) {
			if (this->position[i] != NULL) {
				delete this->position[i];
				this->position[i] = NULL;
			}
		}
		free(this->position);
		this->position = NULL;
	}		
	this->position = position;
	this->update = false;
}

/**
 * This private method set value of attribute imHeightOutput of class
 *
 * \param imHeight New height of image output
 *
 *
 */
void DiVADisplay::setImHeightOutput(unsigned int imHeightOutput) {
	this->imHeightOutput = imHeightOutput;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute imWdihtOutput of class
 *
 * \param iWidth New width of image output
 *
 *
 */
void DiVADisplay::setImWidthOutput(unsigned int imWidthOutput) {
	this->imWidthOutput = imWidthOutput;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute imHeightInput of class
 *
 * \param imHeight New height of image input
 *
 *
 */
void DiVADisplay::setImHeightInput(unsigned int imHeightInput) {
	this->imHeightInput = imHeightInput;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute imWdihtInput of class
 *
 * \param iWidth New width of image input
 *
 *
 */
void DiVADisplay::setImWidthInput(unsigned int imWidthInput) {
	this->imWidthInput = imWidthInput;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute globalHeight of class
 *
 * \param imHeight New height of global image
 *
 *
 */
void DiVADisplay::setGlobalHeight(unsigned int globalHeight) {
	this->globalHeight = globalHeight;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute globalWidth of class
 *
 * \param iWidth New width of global image
 *
 *
 */
void DiVADisplay::setGlobalWidht(unsigned int globalWidth) {
	this->globalWidth = globalWidth;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute windowHeight of class
 *
 * \param imHeight New height of window image
 *
 *
 */
void DiVADisplay::setWindowHeight(unsigned int windowHeight) {
	this->wHeight = windowHeight;
	this->update = false;
}

/**
 * This private method set value of attribute windowWidth of class
 *
 * \param iWidth New width of window image
 *
 *
 */
void DiVADisplay::setWindowWidht(unsigned int windowWidth) {
	this->wWidth = windowWidth;
	this->update = false;
}

/**
 * This private method set value of attribute rows of class
 *
 * \param rows New number rows of multiple image display
 *
 *
 */
void DiVADisplay::setRows(unsigned int rows) {
	this->rows = rows;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute columns of class
 *
 * \param columns New number columns of multiple image display
 *
 *
 */
void DiVADisplay::setColumns(unsigned int columns) {
	this->columns = columns;
	this->initialization = false;
}

/**
 * This private method set value of attribute gapRows of class
 *
 * \param gapR New frame inter rows image
 *
 *
 */
void DiVADisplay::setGapRows(unsigned int gapR) {
	this->gapRows = gapR;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute gapColumns of class
 *
 * \param gapC New frame inter columns image
 *
 *
 */
void DiVADisplay::setGapColumns(unsigned int gapC) {
	this->gapColumns = gapC;
	this->update = false;
	this->initialization = false;
}

/**
 * This private method set value of attribute font of class
 *
 * \param font New font of titles
 *
 *
 */
void DiVADisplay::setFont(CvFont* font) {
	if (font != NULL)
		this->destroyFont();
	this->font = font;
}

/**
 * This private method set value of attribute size of class
 *
 * \param size New size of font
 *
 *
 */
void DiVADisplay::setSizeFont(double size) {
	this->size = size;
	this->update = false;
}

/**
 * This method give image n of windows
 *
 * \param n Row of image
 * \param m Column of image
 *
 *
 * \return Image of display
 */
DiVAImage* DiVADisplay::getDiVAImageN(int n, int m) {
	int position = this->calcule(n,m);
	return this->imageArray[position];
}

/**
 * This method give title n of windows
 *
 * \param n Row of text
 * \param m Column of text
 *
 *
 * \return Image of display
 */
char* DiVADisplay::getTitleN(int n, int m) {
	int position = this->calcule(n,m);
	return this->text[position];
}

/**
 * This method give positions of windows
 *
 *
 * \return Positions of display
 */
int** DiVADisplay::getPositions() {
	return this->position;
}

/**
 * This method give position (x) n of windows
 *
 * \param n Row of text
 * \param m Column of text
 *
 *
 * \return Image of display
 */
int DiVADisplay::getPositionxN(int n, int m) {
	int position = this->calcule(n,m);
	return this->position[0][position];
}

/**
 * This method give position (y) n of windows
 *
 * \param n Row of text
 * \param m Column of text
 *
 *
 * \return Image of display
 */
int DiVADisplay::getPositionyN(int n, int m) {
	int position = this->calcule(n,m);
	return this->position[1][position];
}

/**
 * This private method give a height of base image
 *
 *
 *
 * \return Height of base image
 */
unsigned int DiVADisplay::getImHeightOutput() {
	return this->imHeightOutput;
}

/**
 * This private method give a width of base image
 *
 *
 *
 * \return Width of base image
 */
unsigned int DiVADisplay::getImWidthOutput() {
	return this->imWidthOutput;
}

/**
 * This private method give a height of base image
 *
 *
 *
 * \return Height of base image
 */
unsigned int DiVADisplay::getImHeightInput() {
	return this->imHeightInput;
}

/**
 * This private method give a width of base image
 *
 *
 *
 * \return Width of base image
 */
unsigned int DiVADisplay::getImWidthInput() {
	return this->imWidthInput;
}

/**
 * This private method give a height of window image
 *
 *
 *
 * \return Height of window image
 */
unsigned int DiVADisplay::getWindowHeight() {
	return this->wHeight;
}

/**
 * This private method give a width of window image
 *
 *
 *
 * \return Width of window image
 */
unsigned int DiVADisplay::getWindowWidth() {
	return this->wWidth;
}

/**
 * This private method give a height of global image
 *
 *
 *
 * \return Height of global image
 */
unsigned int DiVADisplay::getGlobalHeight() {
	return this->globalHeight;
}

/**
 * This private method give a width of global image
 *
 *
 *
 * \return Width of global image
 */
unsigned int DiVADisplay::getGlobalWidth() {
	return this->globalWidth;
}

/**
 * This private method give a rows of image
 *
 *
 *
 * \return Number rows of image
 */
unsigned int DiVADisplay::getRows() {
	return this->rows;
}

/**
 * This private method give a columns of image
 *
 *
 *
 * \return Number columns of image
 */
unsigned int DiVADisplay::getColumns() {
	return this->columns;
}

/**
 * This private method give gap inter-rows of image
 *
 *
 *
 * \return Gap inter-rows
 */
unsigned int DiVADisplay::getGapRows() {
	return this->gapRows;
}

/**
 * This private method give gap inter-columns of image
 *
 *
 *
 * \return Gap inter-columns
 */
unsigned int DiVADisplay::getGapColumns() {
	return this->gapColumns;
}

/**
 * This private method give state of active attribute
 *
 *
 *
 * \return State of active
 */
bool DiVADisplay::getActive() {
	return this->active;
}

/**
 * This private method give state of hasTitle attribute
 *
 *
 *
 * \return State of hasTitle
 */
bool DiVADisplay::getHasTitle() {
	return this->hasTitle;
}

/**
 * Private method to get a font of display
 *
 *
 *
 * \return Font of display
 */
CvFont* DiVADisplay::getFont() {
	return this->font;
}

/**
 * This private method give size of font
 *
 *
 *
 * \return Size of font
 */
double DiVADisplay::getSizeFont() {
	return this->size;
}

/**
 * This private method set active attribute active
 *
 *
 *
 *
 */
void DiVADisplay::onActive() {
	this->active = true;
}

/**
 * This private method desactive attribute hasTitle
 *
 *
 *
 *
 */
void DiVADisplay::offActive() {
	this->active = false;
}

/**
 * This private method active attribute hasTitle
 *
 *
 *
 *
 */
void DiVADisplay::onHasTitle() {
	this->hasTitle = true;
	this->update = false;
}

/**
 * This private method desactive attribute hasTitle
 *
 *
 *
 *
 */
void DiVADisplay::offHasTitle() {
	this->hasTitle = false;
	this->update = false;
}

/**
 * This private method create a image of display
 *
 *
 *
 *
 */
void DiVADisplay::CreateImage() {
	unsigned int x = 0, y = 0;
	unsigned int baseSize = MINGAP;
	DiVAImage* newImage = NULL;
	DiVASCALAR black = DiVAScalarAll(0);

	x = (this->getColumns() * this->getImWidthInput()) + (this->getGapColumns() * (this->getColumns()+1));
	y = (this->getRows() * this->getImHeightInput()) + (this->getGapRows() * (this->getRows()+1));
	if (this->getHasTitle())
		y +=  (((int)(baseSize * this->getSizeFont()) * this->getRows())) + 
		      (2*((((int)(baseSize * this->getSizeFont())) * this->getRows())/8));

	newImage = new DiVAImage(x,y,3,8);
	//newImage->setVAlign(1);
	newImage->setVAlign(0);
	newImage->setPixels(black);
	this->setDiVAImage(newImage);
	delete newImage;
	this->setGlobalWidht(x);
	this->setGlobalHeight(y);
}

/**
 *  Private method to create a font of OpenCv
 *
 *
 *
 *
 */
void DiVADisplay::createFont() {
	this->font = new CvFont();
	cvInitFont(this->font,CV_FONT_HERSHEY_SIMPLEX,this->getSizeFont(),1.0,0.0,1,8);
}

/**
 * Private method to destroy a display's font
 *
 *
 *
 *
 */
void DiVADisplay::destroyFont() {
	delete this->getFont();
}

/**
 * This private method give calculate position in arrays
 *
 * \param n Row of position
 * \param m Column of position
 *
 *
 * \return Size of font
 */
int DiVADisplay::calcule(int n, int m) {
	return (((n-1)*this->columns)+(m-1));
}

/**
 * This private method give initial position in display
 *
 * \param n Row of position
 * \param m Column of position
 *
 *
 * \return Size of font
 */
void DiVADisplay::initializate(int row, int column) {
	int position = 0, x = 0, y = 0, gapText = 0;
	unsigned int i = 0, j = 0, sizefont = 0;

	/// Obtain the width,height of the input image
	position = this->calcule(row,column);
	this->setImWidthInput(this->getDiVAImageN(row,column)->getWidth());
	this->setImHeightInput(this->getDiVAImageN(row,column)->getHeight());

	/// Create a base image
	this->CreateImage();

	/// Calcule the position
	for(j = 1; j <= this->getColumns(); j++) {
		for(i = 1; i <= this->getRows(); i++) {
			x = (this->getImWidthInput() * (j-1)) + (this->getGapColumns() * j);
			y = this->getGlobalHeight() - (this->getImHeightInput() * i);
				if (this->getHasTitle()) {
				sizefont = MINGAP;
				gapText = (int)(sizefont * this->getSizeFont()) + (int)(2*(((sizefont * this->getSizeFont())/8)));
				y -= (gapText * i);
			}
			else
				y -= (this->getGapRows() * i);
			this->setPositionN(x,y,i,j);
		}
	}
}
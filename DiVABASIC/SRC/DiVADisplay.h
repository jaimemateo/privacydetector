#ifndef DiVADISPLAY_H
#define DiVADISPLAY_H

/**
 *	\def MINGAP
 *	Minimal gap to include text
 */
#define MINGAP 60;

// Include need librarys
#include "DiVAImage.h"
#include "OpenCVConverter.h"
#include "stdlib.h"
#include "stdio.h"
//#include "cv.h"
//#include "opencv\cv.h"
#include "include\opencv\cv.h"
//#include "cvaux.h"
//#include "opencv\cvaux.h"
#include "include\opencv\cvaux.h"
//#include "cxcore.h"
//#include "core.hpp"
#include "include\opencv2\core\core.hpp"
//#include "highgui.h"
#include "opencv2\highgui\highgui.hpp"

class DiVADisplay  
{
// Indicate private attributes
private:
	/// Name of display's window (max size 256 characters)
	char name[256];
	/// Image of display
	DiVAImage* image;
	/// Images array
	DiVAImage** imageArray;
	/// Titles array
	char** text;
	/// Internal position images
	int** position;
	/// Base height image output
	unsigned int imHeightOutput;
	/// Base width image output
	unsigned int imWidthOutput;
	/// Base height image input
	unsigned int imHeightInput;
	/// Base width image input
	unsigned int imWidthInput;
	/// Base height window
	unsigned int wHeight;
	/// Base width window
	unsigned int wWidth;
	/// Global heitht image
	unsigned int globalHeight;
	/// Global width image
	unsigned int globalWidth;
	/// Number rows
	unsigned int rows;
	/// Number columns
	unsigned int columns;
	/// Black frame of image inter rows
	unsigned int gapRows;
	/// Black frame of image inter columns
	unsigned int gapColumns;
	/// Font of titles
	CvFont* font;
	/// Size of font
	double size;
	/// Active display
	bool active;
	/// Visible display
	bool visible;
	/// Activator of titles 
	bool hasTitle;
	/// Change size image<->windows
	bool sizeWindow;
	/// Internal display variable
	bool update;
	/// Initial phase of display
	bool initialization;
	

	// Show private method
	/// Set image
	void setDiVAImage(DiVAImage* image);
	/// Set image N
	void setDiVAImageN(DiVAImage* image, int n, int m);
	/// Set Title N
	void setTitleN(char* text, int n, int m);
	/// Set PositionN
	void setPositionN(int x, int y, int n, int m);
	/// Set Positions
	void setPositions(int** position);
	/// Set image height output 
	void setImHeightOutput(unsigned int imHeightOutput);
	/// Set image widht output
	void setImWidthOutput(unsigned int globalWidthOutput);
	/// Set image height input
	void setImHeightInput(unsigned int imHeightInput);
	/// Set image widht input
	void setImWidthInput(unsigned int globalWidthInput);
	/// Set global height
	void setGlobalHeight(unsigned int globalHeight);
	/// Set global widht
	void setGlobalWidht(unsigned int globalWidth);
	/// Set window height
	void setWindowHeight(unsigned int windowHeight);
	/// Set window widht
	void setWindowWidht(unsigned int windowWidth);
	/// Set rows
	void setRows(unsigned int rows);
	/// Set columns
	void setColumns(unsigned int columns);
	/// Set gap row
	void setGapRows(unsigned int gapR);
	/// Set gap columns
	void setGapColumns(unsigned int gapC);
	/// Set font of display
	void setFont(CvFont* font);
	/// Set size font
	void setSizeFont(double size);
	 
	/// Get image array n
	DiVAImage* getDiVAImageN(int n, int m);
	/// Get title array n
	char* getTitleN(int n, int m);
	/// Get positions
	int** getPositions();
	/// Get Position x of N
	int getPositionxN(int n, int m);
	/// Get Position y of N
	int getPositionyN(int n, int m);
	/// Get image output height
	unsigned int getImHeightOutput();
	/// Get image output width
	unsigned int getImWidthOutput();
	/// Get image input height
	unsigned int getImHeightInput();
	/// Get image input width
	unsigned int getImWidthInput();
	/// Get window height
	unsigned int getWindowHeight();
	/// Get window width
	unsigned int getWindowWidth();
	/// Get global height
	unsigned int getGlobalHeight();
	/// Get global width
	unsigned int getGlobalWidth();
	/// Get number rows 
	unsigned int getRows();
	/// Get number columns
	unsigned int getColumns();
	/// Get gap rows
	unsigned int getGapRows();
	/// Get gap columns
	unsigned int getGapColumns();
	/// Get active state
	bool getActive();
	/// Get hastitle state
	bool getHasTitle();
	/// Get font
	CvFont* getFont();
	/// Get size font
	double getSizeFont();

	/// Active window
	void onActive();
	/// Desacrive window
	void offActive();
	/// Active that windows have titles
	void onHasTitle();
	/// Desactive that windows have titles
	void offHasTitle();
	
	/// Method for create a new image
	void CreateImage();
	/// Create a new font
	void createFont();
	/// Destroy font
	void destroyFont();	
	/// Get calcule position
	int calcule(int n, int m);
	/// Initialize display variables
	void initializate(int row, int column);

// Show public method
public:

	/// Default constructor
	DiVADisplay();
	/// Constructor of class
	DiVADisplay(char* name, unsigned int imH = 500, unsigned int imW = 500,
			   unsigned int rows=1, unsigned int columns=1,unsigned int gapR= 10, 
			   unsigned int gapC= 10, bool titles=false, double size= 1.0, 
			   bool visible= false, bool allEquals=true, bool sizeWindow = false);

	/// Destructor of class
	virtual ~DiVADisplay();
	/// Set window's name
	void setWindowsName(char* name);	

	/// Get display's name
	char* getWindowName();
	/// Get display's image
	DiVAImage* getDiVAImage();
	/// Get visible state
	bool getVisible();

	/// Active visible window
	void onVisible();
	/// Desacrive visible window
	void offVisible();
	/// Active the windows size
	void onSizeWindow(unsigned int wHeight = 480, unsigned int wWidth = 600);
	/// Desacrive the windows size
	void offSizeWindow();
	/// Method to visualize the window
	void onWindows();
	/// Method to clos the window
	void offWindows();
	
	/// Method to insert a image in the display
	void insertImage(DiVAImage* image, unsigned int row= 1, unsigned int column= 1, char* title = NULL);
	/// Method to insert a text
	void insertText(char* text, unsigned int row= 1, unsigned int column= 1);
	/// Method to show web
	void showWeb();
	/// Method to show image
	void windowVisible();
};

#endif 

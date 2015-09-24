//file description
/**
* \file DiVAImage.h
* \author Juan Carlos San Miguel Avedillo
* \date 15-11-06
* \version 1.2
* \brief Class to describe an image in this framework
* 
* This file is the header of the DiVAImage class.
* Actually use the header file cxcore.h from OpenCV libraries
*
*/	


//class description
/**
 * \class DiVAImage
 * \brief To represent a image
 *
 * Write detailed description here.
 */
				
// Avoid multiple inclusion
#ifndef DiVAIMAGE_H
#define DiVAIMAGE_H

#if _MSC_VER > 1000
#pragma once
#endif 
/**
 *	\def DEPTH_8U
 *	Depth for pixels that are unsigned 8-bit integers
 */
#define DEPTH_8U 8

/**
 *	\def DEPTH_32F
 *	Depth for pixels that are single precision floating-point numbers
 */
#define DEPTH_32F 32

// Include need librarys
//#include "cxcore.h"
#include "DiVAstructures.h"
//#include "cv.h"
//#include "opencv2/core/core.hpp"
#include "opencv2\core\core.hpp"

// Class definition
class DiVAImage  
{

// Indicate private attributes
private:
	///image identification
	long idImage;
	///image timestamp
	long timestamp;
	///image
	IplImage *image;


// Show public method
public:

	///Default constructor
	DiVAImage();
	///Constructor with basic parameters
	DiVAImage(long sizeX, long sizeY, int nChannel ,int Type);
	///Constructor with a image in disk
	DiVAImage(char *filename,int iscolor=1);

	///Class's Destructor
	virtual ~DiVAImage();	

	///Method to access to image pixels
	DiVASCALAR getPixel(int x, int y);

	///Method to set one image pixel to a value
	void setPixel(int x, int y, DiVASCALAR valor);

	///Set all pixels in the image to a reference value
	void setPixels(DiVASCALAR value);	
	///Set all pixels in the image to a reference matrix
	void setPixels(void *value);		
	///Gives direct access to pixels in the image
	void *getPixels();	
	
	///Set identification number
	void setId(long id);

	///Set time stamp
	void setTimeStamp( long timeStamp);	

	///Set origin of pixels data
	void setVAlign(int value);

	///Get the indentification number
	long getId();

	///Get the time stamp
	int getTimeStamp();	

	///Get image width in pixels		
	int getWidth();
	
	///Get image height in pixels		
	int getHeight();	
		
	///Get number of channels		
	int getNChannels();

	///Get type of image data		
	int getDepth();
	
	///Get image color model (RGB,HSV,...)
	char *getColorModel();

	///Get origin of pixels data
	int getVAlign();

	///Get Image data size in bytes
	int getDataSize();

	///Flips image
	void flip(int flip_mode);

	///Method to save a image in disk
	int saveImage(char *filename);

	///Method to load a image from disk
	int loadImage(char *filename,int iscolor=1);

	///Method to obtain a equal copy of the actual image
	DiVAImage *clone();

	///Method to set the image pixels with the pixels content for the input image 
	int copy(DiVAImage *image);

	///Converts the image from RGB to Gray
	int RGB2gray();

	/// Method to encode the image
	int encode(const char* ext, std::vector<int> parameters, std::vector<uchar> &output);

	/// Method to decode the image
	int decode(std::vector<uchar> input,int flags);
};

#endif
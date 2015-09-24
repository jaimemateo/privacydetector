//file description
/**
* \file DiVAImage.cpp 
* \author Juan Carlos San Miguel Avedillo
* \date 15-11-06
* \version 1.3						
* \brief Implementation of DiVAImage class
* 
* This file is the implementation of the DiVAImage class.
* Actually most of the function are based in OpenCv functions
* but we are working to obtain our functions
*
*/

//Needed libraries
#include "DiVAImage.h"
#include "opencv2\highgui\highgui.hpp"
//#include "include/opencv2/highgui/highgui.hpp"
#include "opencv2\core\core.hpp"
//#include "include\opencv2\core\core.hpp"
//#include "opencv\cv.h"
#include "include\opencv\cv.h"
#include <vector>
//default constructor
DiVAImage::DiVAImage() {
	this->idImage=-1;
	this->timestamp=0;	
	this->image = NULL;
}

/**
 *	Constructor that creates a black image with the width and height proportioned for the parameters.
 *  Adittionally can be created a multichannel image (1-4) with different pixel depth.
 *
 * \param sizeX image width in pixels
 * \param sizeY image height in pixels
 * \param nChannels number of image channels (1-4)	
 * \param Type type of depth for pixels data.\n Possible values are:
 *			- \c DEPTH_8U (unsigned 8-bit integers)	
 *			- \c DEPTH_32F(single precision floating-point numbers) 
 *
 * \return Return a DiVAImage class
 */
DiVAImage::DiVAImage(long sizeX, long sizeY, int nChannels, int Type )
{
	CvSize Size;
	Size.width=sizeX;
	Size.height=sizeY;
	if(Type == 8 || Type == 32)
		this->image= cvCreateImage(Size,Type, nChannels);
	else 
		this->image=NULL;

	this->idImage=-1;
	this->timestamp=0;	

}

/**
 *	This constructor load an image from disk and create a DiVAImage class.
 *  DiVAImage created has the same properties that the disk image (width, height, depth,...)
 *	
 *
 * \param filename path of the image in the disk.\n
 *		  Formats supported are:
 *					 - Windows bitmaps - BMP, DIB;			
 *					 - JPEG files - JPEG, JPG, JPE;			
 *					 - Portable Network Graphics - PNG;		
 *					 - Portable image format - PBM, PGM, PPM
 *					 - Sun rasters - SR, RAS;				
 *					 - TIFF files - TIFF, TIF.
 *
 * \return Return a DiVAImage class
 */
DiVAImage::DiVAImage(char *filename,int iscolor)
{
	this->loadImage(filename,iscolor);
	this->idImage=-1;
	this->timestamp=0;	

}

//destructor of the class
DiVAImage::~DiVAImage(){

	if (this->image!=NULL)
		cvReleaseImage(&(this->image));
}

/**
 * This method access to the matrix that contain image pixels and return the requested pixel.
 * The returned value is always the same structure but depending on 
 * the number of image channels, DiVASCALAR fields are filled (1,2,3
 * or 4 channels).
 *
 * \param x width of the pixel
 * \param y height of the pixel
 * \return Return the pixel value or -1 if the coordinates are out of bounds
 */
DiVASCALAR DiVAImage::getPixel(int _width, int _height){
 
	int i=0;
	DiVASCALAR value;

	if (_width > this->image->width || _height > this->image->height)
		return DiVAScalar(-1,-1,-1,-1);

	//unsigned char data
	if (this->image->depth == IPL_DEPTH_8U)
		for (i=0;i<image->nChannels;i++)
			value.val[i] = ((uchar *)(this->image->imageData + _height*image->widthStep))[_width*image->nChannels + i];
		
	//floating data
	if (this->image->depth == IPL_DEPTH_32F)		
		for (i=0;i<image->nChannels;i++)
			value.val[i] = ((float *)(this->image->imageData + _height*image->widthStep))[_width*image->nChannels + i];
		
	//??? data
	if(this->image->depth != IPL_DEPTH_8U  &&  this->image->depth != IPL_DEPTH_32F){

			//add the code for the default option
			return DiVAScalar(-1,-1,-1,-1);
	}	
		
	return value;

}

/**
 * This method access to the matrix that contain image pixels and
 * set the selected pixel with the param \a value. If the coordinates
 * are out of bounds, no insertion is done. 
 *
 * \param x width of the pixel
 * \param y height of the pixel
 * \param value contains the pixel value to insert in the image 
 */
void DiVAImage::setPixel(int _width, int _height, DiVASCALAR value){

	int i=0;

	if (_width < this->image->width && _height< this->image->height){
		//unsigned char data
		if (this->image->depth == IPL_DEPTH_8U)
			for (i=0;i<image->nChannels;i++)
				 ((uchar *)(this->image->imageData + _height*image->widthStep))[_width*image->nChannels + i]=(uchar)value.val[i];
			
		//floating data
		if (this->image->depth == IPL_DEPTH_32F)		
			for (i=0;i<image->nChannels;i++)
				((float *)(this->image->imageData + _height*image->widthStep))[_width*image->nChannels + i]=(float)value.val[i];
			
		//??? data
		if(this->image->depth != IPL_DEPTH_8U  &&  this->image->depth != IPL_DEPTH_32F){

				//add the code for the default option
		}	
	}
}


/**
 * This method access to the matrix that contain image pixels and
 * set all the pixels matrix to the param \a value. 
 *
 * \param value contains the pixel value
 */
void DiVAImage::setPixels(DiVASCALAR value){
	
	CvScalar a = cvScalar (value.val[0],value.val[1],value.val[2],value.val[3]);
	cvSet(this->image, a );

}

/**
 * Set all pixels in the image with matrix.	Matrix elements	can be uchar * (for DEPTH_8U) or float * (for DEPTH_32F) 
 * and it will have the same size of the image. 
 *
 * \param value pointer to values. Values are in a unidimensional array.		
 *						For images in RGB, the order in the matrix 
 *						is BGR for each pixel	
 */
void DiVAImage::setPixels(void *value){

	memcpy(image->imageData, value, image->imageSize);

}

/**
 *
 * \param id identification number for the image
 *
 */
void DiVAImage::setId(long id){

	this->idImage=id;

}

/**
 *
 * \param timeStamp time Stamp mark for the image
 *
 */
void DiVAImage::setTimeStamp( long timeStamp){

	this->timestamp=timeStamp;

}

/**
 *
 * \return Return the time Stamp of the image
 */
int DiVAImage::getTimeStamp(){

	return this->timestamp;
}

/**
 *
 * \return Return the identification number of the image
 */
long DiVAImage::getId(){

	return this->idImage;
}

/**
 *
 * \return Return the width of the image in pixels
 */
int DiVAImage::getWidth(){

	return this->image->width;
}

/**
 *
 * \return Return the height of the image in pixels
 */
int DiVAImage::getHeight(){

	return this->image->height;

}

/**
 * 
 * \return Return the pixel's depth \n Possible values returned are:
 *			- \c DEPTH_8U (unsigned 8-bit integers)	
 *			- \c DEPTH_32F(single precision floating-point numbers) 
 *
 */
int DiVAImage::getDepth(){

	return this->image->depth;
}

/**
 * DiVAImage can have 1,2,3 or 4 channels 
 *
 *
 * \return Return number of channels in the image
 */ 
int DiVAImage::getNChannels(){

	return this->image->nChannels;

}
		
/**
 * "Add more comments about color models..."
 *
 */
char *DiVAImage::getColorModel(){

	return this->image->colorModel;
}

/**
 *	This method save an image int the disk. 
 *	
 *
 * \param filename path to save the image in the disk.\n
 *		  Formats supported are:
 *					 - Windows bitmaps - BMP, DIB;			
 *					 - JPEG files - JPEG, JPG, JPE;			
 *					 - Portable Network Graphics - PNG;		
 *					 - Portable image format - PBM, PGM, PPM
 *					 - Sun rasters - SR, RAS;				
 *					 - TIFF files - TIFF, TIF.
 *
 * \return Verification code
 */
int DiVAImage::saveImage(char *filename){

	int i;
	i=cvSaveImage( filename, this->image);

	return i;
}

/**
 * This method load an image from disk. An error can be produced if the 
 * DiVAImage was created with differente parameters that image in disk.
 *	
 *
 * \param filename path of the image in the disk.\n
 *		  Formats supported are:
 *					 - Windows bitmaps - BMP, DIB;			
 *					 - JPEG files - JPEG, JPG, JPE;			
 *					 - Portable Network Graphics - PNG;		
 *					 - Portable image format - PBM, PGM, PPM
 *					 - Sun rasters - SR, RAS;				
 *					 - TIFF files - TIFF, TIF.
 *
 * \return Verification code
 */
int DiVAImage::loadImage(char *filename,int iscolor){

	this->image = cvLoadImage(filename,iscolor);
	if (this->image->origin ==0)
	{
		cvFlip(this->image, NULL, 0);
		this->setVAlign(1);
	}
	if (this->image==NULL)
		return -1;
	else
		return 1;

}

/**
 * Method to get a new image identically to the actual image.
 * No parameters of the current image are modificated. The destruction
 * of this new image created must be done by user to avoid memory leaks
 *
 * \return DiVAImage class
 */
DiVAImage *DiVAImage::clone(){

	DiVAImage *img;

	img = new DiVAImage(this->image->width, this->image->height, this->image->nChannels, this->image->depth);
	img->copy(this);

	return img;
}


/**
 * This method copies the pixels values from the input image to the current image.
 * 	None of the parameters of the input image are edited.
 *  All the parameters in the two images have to be the same. If not, the copy operation
 *  is interrupted and nothing is done.
 *
 * \param img DiVAImage
 * \return Verification code
 */
int DiVAImage::copy(DiVAImage *img){

	if(this->getWidth() != img->getWidth() || this->getHeight() != img->getHeight() 
		|| this->getDepth() != img->getDepth()|| this->getNChannels() != img->getNChannels() )
		return -1;

	setPixels(img->getPixels());

	setId(img->getId());
	setTimeStamp(img->getTimeStamp());	
	setVAlign(img->getVAlign());

	return 1;
}

/**
 * This method gives direct access to pixels in the image to work with the pixels
 *
 * 
 * \return A pointer to the pixels data. This matrix is unidimensional and it depends on the number of channel, depth,...
 */
void *DiVAImage::getPixels()
{
	return image->imageData;
}

/**           
 *
 * 
 * \return Return the origin of the pixels data. Possibilities are:
 *				- 0 for TOP-LEFT origin
 *              - 1 for BOTTOM-LEFT origin (Windows bitmaps style) 
 */
int DiVAImage::getVAlign()
{
	return image->origin;
}

/**           
 * 
 * \param value The origin of the pixels data. Possibilities are:
 *				- 0 for TOP-LEFT origin
 *              - 1 for BOTTOM-LEFT origin (Windows bitmaps style) 
 */
void DiVAImage::setVAlign(int value)
{
	if (value == 0 || value == 1)
		image->origin = value;
}


/**           
 * 
 * Return the size (in bytes) of the data image (it depends on pixels depth and image size)
 *
 * \return Size in bytes
 */
int DiVAImage::getDataSize(){

	return image->imageSize;
}
	
/**           
 * 
 * Return the size (in bytes) of the data image (it depends on pixels depth and image size)
 *
 * \param flip_mode Specifies how to flip the array.\n
 * flip_mode = 0 means flipping around x-axis, flip_mode > 0 (e.g. 1) means flipping around y-axis and flip_mode < 0 (e.g. -1) means flipping around both axises
 *
 * \return Size in bytes
 */
void DiVAImage::flip(int flip_mode){
	cvFlip(image,NULL,flip_mode);
}

int DiVAImage::RGB2gray()
{
	if (strcmp(this->getColorModel(),"RGB"))
		return -1;

	IplImage* cimg = this->image;	
	CvSize Size;
	Size.width=this->getWidth();
	Size.height=this->getHeight();
	long id = this->getId();
	long timestamp  = this->getTimeStamp();
	int depth = this->getDepth();
	int nChannels = 1; //For a gray image we want a only channel
	int origin = this->getVAlign();
	IplImage* gimg  = cvCreateImage(Size,depth,nChannels);
	if (!gimg)
		return -1;
	cvCvtColor(cimg,gimg,CV_BGR2GRAY); // cimg -> gimg
	
	if (this->image!=NULL)
		cvReleaseImage(&(this->image));
	this->image = gimg;
	this->setId(id);
	this->setTimeStamp(timestamp);
	this->setVAlign(origin);
	return 0;	
}
/**           
 * 
 * Encodes an image into a memory buffer.
 *
 * \param ext File extension that defines the output format.
 *      Windows bitmaps - *.bmp, *.dib (always supported)
 *      JPEG files - *.jpeg, *.jpg, *.jpe (see the Notes section)
 *      JPEG 2000 files - *.jp2 (see the Notes section)
 *      Portable Network Graphics - *.png (see the Notes section)
 *      Portable image format - *.pbm, *.pgm, *.ppm (always supported)
 *      Sun rasters - *.sr, *.ras (always supported)
 *      TIFF files - *.tiff, *.tif (see the Notes section)
 * \param parameters Format-specific parameters.
 * 
 * \return Encoded image.
 */
int DiVAImage::encode(const char* ext, std::vector<int> parameters, std::vector<uchar> &output)
{
	if(!cv::imencode(ext, cv::Mat(this->image),  output, parameters))
	{
		return -1;
	}
	return 0;
}
/**           
 * 

 */
int DiVAImage::decode(std::vector<uchar> input, int flags)
{
	
	cv::Mat mat(this->getHeight(),this->getWidth(),this->getDepth());
	mat = cv::imdecode( input, flags);
	IplImage img = mat;
	this->image->imageSize = img.imageSize;
	this->setPixels(img.imageData);


	this->image->widthStep = img.widthStep;

	return 0;
}
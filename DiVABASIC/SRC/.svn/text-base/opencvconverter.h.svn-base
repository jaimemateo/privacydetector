/**
* \file OpenCVConverter.h
* \author Alvaro Garcia Martin
* \date 15-11-06
* \version 1.0
* \brief Class to describe an shared buffer in this framework
* 
* This file is the header of the OpenCVConverter class.
* This class allow to convert images between image format in opencv
* and format in GTI framework
*
*/	

//class description
/**
 * \class OpenCVConverter
 * \brief Header file of OpenCVConverter class
 *
 * Write detailed description here.
 */

#if !defined(AFX_OPENCVCONVERTER_H_INCLUDED_)
#define AFX_OPENCVCONVERTER_H_INCLUDED_

//#include "cxcore.h"
#include "DiVAImage.h"
//#include "opencv2\core\core_c.h"
#include "include\opencv2\core\core_c.h"
//#include "core.hpp"
class OpenCVConverter  
{
public:

	///Default constructor
	OpenCVConverter();

	///Default destructor
	virtual ~OpenCVConverter();

	///Method to obtain the IplImage contained in GTIImage
	static IplImage *getIplImage(DiVAImage *origen);

	///Method to obtain the IplImage contained in GTIImage
	static DiVAImage *getDiVAImage(IplImage *origen);

	///Method to copy IplImage in GTIImage
	static int Ipl2DiVAImage(IplImage *origen, DiVAImage *destino);

	///Method to copy GTIImage in IplImage
	static int DiVA2IplImage(DiVAImage *origen, IplImage *destino);
};

#endif // !defined(AFX_OPENCVCONVERTER_H_INCLUDED_)

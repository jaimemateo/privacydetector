/**
* \file OpenCVConverter.cpp
* \author Alvaro Garcia Martin
* \date 15-11-06
* \version 1.0
* \brief Class to describe an shared buffer in this framework
* 
* This file is the header of the OpenCVConverter class.
* This class allow to convert images between image format in opencv
* and format in DiVA framework
*
*/	

// Need Libraries
#include "OpenCVConverter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OpenCVConverter::OpenCVConverter()
{

}

OpenCVConverter::~OpenCVConverter()
{

}

/************************************************************/
/* Method: getIplImage										*/
/*															*/
/* Author: agm												*/
/*															*/
/* Description:												*/
/*		Creates an IplImage with contents of input			*/
/*															*/
/* Parameters:												*/
/*		Input:												*/
/*			origen : Image to be converted					*/
/*															*/
/* Return:													*/
/*		new IplImage with contents of input					*/
/************************************************************/

/**
 *	Creates an IplImage with contents of input
 *
 * \param origen Image to be converted
 *
 * \return new IplImage with contents of input
 */
IplImage *OpenCVConverter::getIplImage(DiVAImage *origen)
{
	if (origen == NULL)
		return NULL;

	// Crear IplImage e inicializar cabecera
	IplImage *iplImg = cvCreateImage(cvSize(origen->getWidth(), origen->getHeight()), origen->getDepth(), origen->getNChannels());

	// Copiar pixels (pendiente)
	memcpy(iplImg->imageData, origen->getPixels(), iplImg->imageSize);

	// Asignar origen (top-down o bottom-up)
	iplImg->origin = origen->getVAlign();

	return iplImg;
}

/************************************************************/
/* Method: getDiVAImage										*/
/*															*/
/* Author: agm												*/
/*															*/
/* Description:												*/
/*		Creates an DiVAImage with contents of input			*/
/*															*/
/* Parameters:												*/
/*		Input:												*/
/*			origen : Image to be converted					*/
/*															*/
/* Return:													*/
/*		new DiVAImage with contents of input					*/
/************************************************************/

/**
 *	Creates an DiVAImage with contents of input
 *
 * \param origen Image to be converted
 *
 * \return new IplImage with contents of input
 */
DiVAImage *OpenCVConverter::getDiVAImage(IplImage *origen)
{
	if (origen == NULL)
		return NULL;

	// Crear DiVAImage e inicializar cabecera
	DiVAImage *DiVAImg = new DiVAImage(origen->width, origen->height, origen->nChannels, origen->depth);

	// Copiar pixels (pendiente)
	DiVAImg->setPixels(origen->imageData);

	// Asignar origen (top-down o bottom-up)
	DiVAImg->setVAlign(origen->origin);

	return DiVAImg;
}

/************************************************************/
/* Method: Ipl2DiVAImage										*/
/*															*/
/* Author: agm												*/
/*															*/
/* Description:												*/
/*		Copies contents of IplImage into DiVAImage			*/
/*															*/
/* Parameters:												*/
/*		Input:												*/
/*			origen : Source IplImage						*/
/*															*/
/*		Output:												*/
/*			destino : Destination DiVAImage					*/
/*															*/
/* Return:													*/
/*		0  : Conversion OK									*/
/*		-1 : Error											*/
/************************************************************/

/**
 *	Copies contents of IplImage into DiVAImage
 *
 * \param origen  Source IplImage						
 * \param destino  Destination DiVAImage
 *
 * \return Return code (0 -> Conversion OK  or -1 -> Error)
 */
int OpenCVConverter::Ipl2DiVAImage(IplImage *origen, DiVAImage *destino)
{
	// Comprobar que width, height, nChannels y depth son correctos. Si no, retornar error (-1)
	if ((destino->getWidth() != origen->width)
		|| (destino->getHeight() != origen->height)
		|| (destino->getNChannels() != origen->nChannels)
		|| (destino->getDepth() != origen->depth))
		return -1;

	// Copiar pixels (pendiente)
	destino->setPixels(origen->imageData);

	// Asignar origen (top-down o bottom-up)
	destino->setVAlign(origen->origin);

	return 0;
}

/************************************************************/
/* Method: DiVA2IplImage										*/
/*															*/
/* Author: agm												*/
/*															*/
/* Description:												*/
/*		Copies contents of DiVAImage into IplImage			*/
/*															*/
/* Parameters:												*/
/*		Input:												*/
/*			origen : Source DiVAImage						*/
/*															*/
/*		Output:												*/
/*			destino : Destination IplImage					*/
/*															*/
/* Return:													*/
/*		0  : Conversion OK									*/
/*		-1 : Error											*/
/************************************************************/

/**
 *	Copies contents of DiVAImage into IplImage
 *
 * \param origen  Source DiVAImage						
 * \param destino  Destination IplImage
 *
 * \return Return code (0 -> Conversion OK  or -1 -> Error)
 */
int OpenCVConverter::DiVA2IplImage(DiVAImage *origen, IplImage *destino)
{
	// Comprobar que width, height, nChannels y depth son correctos. Si no, retornar error (-1)
	if ((origen->getWidth() != destino->width)
		|| (origen->getHeight() != destino->height)
		|| (origen->getNChannels() != destino->nChannels)
		|| (origen->getDepth() != destino->depth))
		return -1;

	// Copiar pixels (pendiente)
	memcpy(destino->imageData, origen->getPixels(), destino->imageSize);

	// Asignar origen (top-down o bottom-up)
	destino->origin = origen->getVAlign();

	return 0;
}

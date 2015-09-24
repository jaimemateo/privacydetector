//file description
/**
* \file DiVAClientBuffer.h
* \author Juan Carlos San Miguel
* \date 16-03-07
* \version 1.0
* \brief Buffer to store frames at client side
* 
* This file is the header of the DiVAClientBuffer class.
*
*/	

/** 
 * \class DiVAClientBuffer
 * \brief To store frames from FrameServer
 *
 */

#if !defined(AFX_DiVACLIENTBUFFER_H)
#define AFX_DiVACLIENTBUFFER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_BUFFERLENGTH 15 //Frames
#include "DiVAImage.h"
#include "DiVAThread.h"
#include "DiVAImageBuffer.h"
#include "DiVAClient.h"
#include "DiVACrono.h"

class DiVAClientBuffer:public DiVAThread
{
public:

	///Constructor
	DiVAClientBuffer(char *frameServername, int portnumber=20248, int bufferLength=DEFAULT_BUFFERLENGTH);
	///Destructor
	~DiVAClientBuffer();	
	int process();
	int start();
	int getFromBuffer(DiVAImage** pImage,long idFrame);

	char *getSourceId();
	int setEnd();

private:
	DiVAImage *_frame;
	DiVAImageBuffer*	_pimageBuffer;
	int				_bufferLength;
	int				_idClient;
	long			_idFrame;

	DiVAClient*	_pFrameClient;
	char	_frameServername[100];
	int		_portnumber;

	DiVACrono*		_pCrono;	

	int receiveFrame(DiVAImage** pImage,long idFrame);
	int writeInBuffer(DiVAImage* pImage);
	


};

#endif // !defined(AFX_DiVACLIENTBUFFER_H)

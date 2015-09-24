#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2\opencv_modules.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iomanip> //Para poner más ceros en cout

using namespace cv;
using namespace std;

#ifndef DETECTOR_H
#define DETECTOR_H

// Parámetros
#define	COLOR	1
#define	SCRAM	2

// Valores parámetros
#define PRESCRAM 0
#define POSTSCRAM 1

#define	BGR		1
#define	GR		2
#define	YCRCB	3
#define	Y		4

class detector{
public:
	detector();
	~detector();
	void * process(IplImage *frame);
	int setParam(int parametro, int valor_int=0);
	int getParam(int parametro, int *valor_int);
	void showresults();
	int color;
	int scram;
	Mat predetection(Mat &frame);
	Mat scrambling(Mat &framedetec, int parametro);
	Mat dctcall(Mat &image, Rect &roi);
	Mat idctcall(Mat &roidct);
	Mat imagescram(Mat &blockdct);
	Mat dctcall2(Mat &image, Rect &roi);
	Mat postdetection(Mat &framedetec);


public:
	Mat framepre, framepost, framescram;
	vector<Rect> found, found_filtered;
	HOGDescriptor hog;
	Rect r1,r2;

	// Scrambling
	Mat imageroi;
	Mat channel[3], roiR, roiG, roiB;
	Mat rois;
	Mat frametemp, frameidct, Bframedct, Bframeidct, Gframedct, Gframeidct, Rframedct, Rframeidct;
	Mat ycbcr, chan[3], y, cb, cr;


private:
	
};
#endif

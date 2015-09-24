#include "DiVACrono.h"

#include <stdio.h>
#include <time.h>
//#include "opencv\cv.h"
#include "include\opencv\cv.h"
DiVACrono::DiVACrono(){}
DiVACrono::~DiVACrono(){}



void DiVACrono::start() {
	this->inicio =	((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
}
double DiVACrono::stop(){
	this->fin = ((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );	
	
	//printf("\nCRONO STOP: %lu milisegundos",(unsigned long)fin-inicio);
	return fin-inicio;
}

double DiVACrono::stop(char *cadena){
	this->fin = ((double)cvGetTickCount()/(cvGetTickFrequency()*1000.) );
	//printf("\n%s -> CRONO STOP: %lu milisegundos", cadena, (unsigned long)fin-inicio);
	return fin-inicio;
}

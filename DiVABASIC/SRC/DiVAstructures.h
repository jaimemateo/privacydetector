#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct{

	
	double val[4];

}DiVASCALAR;

//constructor para un escalar
DiVASCALAR DiVAScalar(double val0, double val1, double val2, double val3);
DiVASCALAR DiVAScalarAll(double val0123);

#endif
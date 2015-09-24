
#if !defined(AFX_PROPIEDADES_H_INCLUDED_)
#define AFX_PROPIEDADES_H_INCLUDED_

#include <stdio.h>

#define PROPS_OK		1
#define PROPS_NO_EXISTE	0
#define PROPS_ERROR		-1


class DiVAFileReader  
{
	char filename[50];
	FILE *filePtr;

public:
	DiVAFileReader(char *filename, bool create=false);
	virtual ~DiVAFileReader();

	int getPropiedad(char *nombre, char *valor);
	int getPropiedad(char *nombre, int *valor);
	int getPropiedad(char *nombre, long *valor);
	int getPropiedad(char *nombre, double *valor);

	void leerLineas();

	int setPropiedad(char *nombre, char *valor);
	int setPropiedad(char *nombre, int valor);
	int setPropiedad(char *nombre, long valor);
	int setPropiedad(char *nombre, double valor);
};

#endif // !defined(AFX_PROPIEDADES_H__0FA8A235_42E4_417F_9BCF_DE127F641AF1__INCLUDED_)

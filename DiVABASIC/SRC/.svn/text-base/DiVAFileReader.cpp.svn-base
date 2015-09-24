// Propiedades.cpp: implementation of the Propiedades class.
//
//////////////////////////////////////////////////////////////////////

#include "DiVAFileReader.h"
#include <string.h>
#include <stdlib.h>

#define MAX_LINEA 255

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
void Propiedades::buscarPropiedad(FILE *archivo, char *nombre, char *valor)
{

}
*/

DiVAFileReader::DiVAFileReader(char *filename, bool create)
{
	if (filename == NULL)
	{
		printf("ERROR: Parametros incorrectos en creacion de objeto Propiedades\n");
		return;
	}

	sprintf(this->filename, "%s", filename);

	// Abrir el fichero para actualizacion
	filePtr = fopen(filename,"r");

	if (filePtr == NULL)
	{
		if (create)
		{
			filePtr = fopen(filename,"w+");			
		}
	}

	
	if (filePtr == NULL)
	{
		printf("Propiedades --> No se pudo abrir el fichero de propiedades '%s'\n", filename);
		return;
	}
}


DiVAFileReader::~DiVAFileReader()
{
	if (filePtr != NULL)
		fclose(filePtr);
}

int DiVAFileReader::getPropiedad(char *nombre, char *valor)
{
	if ((filePtr == NULL) || (nombre == NULL) || (filePtr == NULL))
		return PROPS_ERROR;

	char linea[MAX_LINEA];
	char nombreAux[80];
	char valorAux[80];

	fseek(filePtr, 0, SEEK_SET);

	while (fgets(linea, MAX_LINEA, filePtr) != NULL)
	{
		// Saltar comentarios
		if (strncmp(linea, "//", 2) == 0)
			continue;

		// Saltar lineas en blanco
		if (strcmp(linea, "\n") == 0)
			continue;

		sscanf(linea, "%[^'=']=%s\n", nombreAux, valorAux);

		if (strcmp(nombre, nombreAux) == 0)
		{
			strcpy(valor, valorAux);
			return PROPS_OK;
		}
	}

	// Si no ha encontrado la propiedad, pone el valor a NULL
	return PROPS_NO_EXISTE;
}

int DiVAFileReader::getPropiedad(char *nombre, int *valor)
{
	char valorStr[80];
	int res = getPropiedad(nombre, valorStr);
	if (res == PROPS_OK)
		*valor = atoi(valorStr);

	return res;
}

int DiVAFileReader::getPropiedad(char *nombre, long *valor)
{
	char valorStr[80];
	int res = getPropiedad(nombre, valorStr);
	if (res == PROPS_OK)
		*valor = atol(valorStr);

	return res;
}

int DiVAFileReader::getPropiedad(char *nombre, double *valor)
{
	char valorStr[80];
	int res = getPropiedad(nombre, valorStr);
	if (res == PROPS_OK)
		*valor = strtod(valorStr, NULL);

	return res;
}


void DiVAFileReader::leerLineas()
{
	char linea[MAX_LINEA];
	char nombreAux[80];
	char valorAux[80];

	fseek(filePtr, 0, SEEK_SET);

	while (fgets(linea, MAX_LINEA, filePtr) != NULL)
	{
		// Saltar comentarios
		if (strncmp(linea, "//", 2) == 0)
			continue;

		// Saltar lineas en blanco
		if (strcmp(linea, "\n") == 0)
			continue;

		sscanf(linea, "%s\t%s\n", nombreAux, valorAux);
		printf("'%s'='%s'\n", nombreAux, valorAux);
	}
}

int DiVAFileReader::setPropiedad(char *nombre, char *valor)
{
	if ((filePtr == NULL) || (nombre == NULL) || (filePtr == NULL))
		return PROPS_ERROR;

	char linea[MAX_LINEA];
	char nombreAux[80];
	char valorAux[80];
	fpos_t posicion=0;

	fseek(filePtr, 0, SEEK_SET);

	while (fgets(linea, MAX_LINEA, filePtr) != NULL)
	{
		// Guardar la posicion por si es la linea buena
		fgetpos(filePtr, &posicion);

		// Saltar comentarios
		if (strncmp(linea, "//", 2) == 0)
			continue;

		// Saltar lineas en blanco
		if (strcmp(linea, "\n") == 0)
			continue;

//		sscanf(linea, "%s=%s\n", nombreAux, valorAux);
		sscanf(linea, "%[^'=']=%s\n", nombreAux, valorAux);

		// Si encuentra el nombre de la propiedad, reescribe la linea
		if (strcmp(nombre, nombreAux) == 0)
		{
			fsetpos(filePtr, &posicion);
			fprintf(filePtr,"%s=%s\n", nombre, valor);
			return PROPS_OK;
		}
	}

	// Si no ha encontrado la propiedad, la escribe
	fprintf(filePtr,"%s=%s\n", nombre, valor);
	return PROPS_OK;
}


int DiVAFileReader::setPropiedad(char *nombre, int valor)
{
	char valorStr[20];
	sprintf(valorStr, "%d\0", valor);
	return setPropiedad(nombre, valorStr);
}

int DiVAFileReader::setPropiedad(char *nombre, long valor)
{
	char valorStr[20];
	sprintf(valorStr, "%d\0", valor);
	return setPropiedad(nombre, valorStr);
}

int DiVAFileReader::setPropiedad(char *nombre, double valor)
{
	char valorStr[20];
	sprintf(valorStr, "%f\0", valor);
	return setPropiedad(nombre, valorStr);
}

#include "camaras.h"
#include "stdio.h"

cam_list::cam_list(){
	char archivo_default[] = "camaras.conf";
	this->archivo_camaras = (char *)malloc((strlen(archivo_default)+1)*sizeof(char));
	strcpy(archivo_camaras,archivo_default);
	return;
}
cam_list::cam_list(char *archivo){
	this->archivo_camaras = (char *)malloc((strlen(archivo)+1)*sizeof(char));
	strcpy(archivo_camaras,archivo);
	return;
}
cam_list::~cam_list(){
	this->updateFileCameras();
	free(this->archivo_camaras);
	this->listado.~vector();
	return;
}
int cam_list::readFileCameras(){
	FILE *readfile;
	char linea[250],nombreAux[50],valorAux[250];
	camara camara_aux;
	int name = 0,port = 0,ip = 0;

	readfile = fopen(this->archivo_camaras,"r");
	if(!readfile)
	{
		printf("Error al leer el archivo de camaras.\n");
		return -1;
	}
	else
	{
		while(fgets(linea,250,readfile) != NULL)
		{
			// Saltar comentarios
			if (strncmp(linea, "//", 2) == 0)
				continue;

			// Saltar lineas en blanco
			if (strcmp(linea, "\n") == 0)
				continue;

			sscanf(linea, "%[^'=']=%250[^\n]", nombreAux, valorAux);

			if (strcmp("NAME", nombreAux) == 0)
			{
				strcpy(camara_aux.nombre, valorAux);
				name = 1;
			}
			else if (strcmp("IP", nombreAux) == 0)
			{
				strcpy(camara_aux.ip, valorAux);
				ip = 1;
			}
			else if (strcmp("PORT", nombreAux) == 0)
			{
				camara_aux.puerto =  atoi(valorAux);
				port = 1;
			}
			if (name && port && ip)
			{
				camara_aux.nueva = 0;
				name = 0;
				port = 0;
				ip = 0;
				this->listado.push_back(camara_aux);
			}
		}
		fclose(readfile);
		return 1;
	}
	
}
int cam_list::updateFileCameras(){
	FILE *writefile;
	char linea_aux[250];
	int i;
	writefile = fopen(archivo_camaras,"a");
	
	for(i=0;i<this->listado.size();i++)
	{
		if(this->listado[i].nueva)
		{
			fprintf(writefile,"\n");
			sprintf(linea_aux,"IP=%s\n",this->listado[i].ip);
			fprintf(writefile,"%s",linea_aux);
			sprintf(linea_aux,"PORT=%d\n",this->listado[i].puerto);
			fprintf(writefile,"%s",linea_aux);
			sprintf(linea_aux,"NAME=%s\n",this->listado[i].nombre);
			fprintf(writefile,"%s",linea_aux);

		}
	}
	fclose(writefile);
	return 1;
}
void cam_list::addCamera(char *ip,int puerto,char *nombre){

	camara cam_aux;
	strcpy(cam_aux.ip,ip);
	strcpy(cam_aux.nombre,nombre);
	cam_aux.puerto = puerto;
	cam_aux.nueva = 1;

	this->listado.push_back(cam_aux);
	return;
}
void cam_list::delCamera(char *nombre){

	unsigned int i;
	for(i=0;i<this->listado.size();i++)
	{
		if(!strcmp(this->listado[i].nombre,nombre))
		{
			this->listado.erase(this->listado.begin()+i);
			this->writeFileCameras();
			return;
		}

	}
	return;
}

int cam_list::writeFileCameras(){
	FILE *archivo;
	archivo = fopen(this->archivo_camaras,"w");
	if(!archivo)
	{
		printf("Error al abrir el archivo de camaras.\n");
		return -1;
	}
	unsigned int i;
	for(i=0;i < this->listado.size();i++)
	{
		fprintf(archivo,"\n\nIP=%s\nPORT=%d\nNAME=%s\n\n",this->listado[i].ip,this->listado[i].puerto,this->listado[i].nombre);
	}
	fclose(archivo);
	return 1;
}

void cam_list::selectDefaultCam(unsigned int posicion)
{
	camara cam_aux;
	strcpy(cam_aux.ip,this->listado[posicion].ip);
	strcpy(cam_aux.nombre,this->listado[posicion].nombre);
	cam_aux.puerto = this->listado[posicion].puerto;
	cam_aux.nueva = 0;
	this->listado.insert(this->listado.begin(),cam_aux);
	this->listado.erase(this->listado.begin()+posicion+1);
	this->writeFileCameras();
}


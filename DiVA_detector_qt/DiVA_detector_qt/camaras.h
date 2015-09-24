#include <vector>
#include <stdio.h>

struct camara{
	int puerto;
	char ip[50];
	char nombre[255];
	int nueva;
};
#ifndef CAM_LIST
#define CAM_LIST
class cam_list{
public:
	cam_list();
	cam_list(char *archivo);
	~cam_list();
	std::vector<camara> listado;
	int updateFileCameras();
	int readFileCameras();
	void addCamera(char *ip,int puerto,char *nombre);
	void delCamera(char *nombre);
	void selectDefaultCam(unsigned int posicion);
private:
	char *archivo_camaras;
	int writeFileCameras();
};

#endif
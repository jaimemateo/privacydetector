#include "DiVAAlgorithm.h"

DiVADataClient::DiVADataClient(void* pAlgorithm)
{
		if (pAlgorithm==NULL)
			return;
		_pAlgorithm = pAlgorithm;
		if(((DiVAAlgorithm*)pAlgorithm)->getdataServerName())
		{
			_dataServerName = new char[strlen(((DiVAAlgorithm*)pAlgorithm)->getdataServerName())+1];
			strcpy(_dataServerName,((DiVAAlgorithm*)pAlgorithm)->getdataServerName());
		}
		else
		{
			_dataServerName=NULL;
		}
			
}

DiVADataClient::~DiVADataClient()
{
	if(_dataServerName)
	{
		delete[]_dataServerName;
	}

}
DiVAImage* DiVADataClient::getEdges(long id,char* feedingAlgorithmName)
{
	char aux[1024];
	char aux2[1024];
	aux[0]=0;
	char* dumpingPathRoot = ((DiVAAlgorithm*)_pAlgorithm)->getDumpingpathRoot();
	
	strcpy(aux,dumpingPathRoot);
	strcat(aux,"/");
	strcat(aux,feedingAlgorithmName);
	strcat(aux,"/");
	sprintf(aux2,EDGES_FILE_FORMAT,id);
	strcat(aux,aux2);
	int ret = fileExists(aux);
	if (ret==0) //Not yet...
	{
		return NULL;
	}
	DiVAImage* pedges = new DiVAImage(aux,0);
	pedges->setId(id);
	return pedges;
}


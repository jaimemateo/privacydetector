//file DiVADataClient
/**
* \file DiVADataClient.h
* \author Javier Molina
* \date 15-11-07
* \version 1.2
* \brief Defines a connector to DiVA dataservers (in DiVaServer). Implementation based on windows threads (DiVAThread).
* 
* This file is the header of the DiVaClient class.
*
*/	


//class DiVADataClient
/**
 * \class DiVADataClient
 * \brief Defines a connector to DiVA dataservers (in DiVaServer). Implementation based on windows threads (DiVAThread).
 *
 */

#include "DiVAImage.h"

#if !defined(AFX_DIVADATACLIENT_H_INCLUDED_)
#define AFX_DIVADATACLIENT_H_INCLUDED_

class DiVADataClient
{
	private:
	char* 			_dataServerName;
	void* 			_pAlgorithm;
	public:
	DiVADataClient(void* pAlgorithm);
	~DiVADataClient();
	DiVAImage* getEdges(long id,char* feedingAlgorithmName);
	private:
	int connect(){return 0;};
};

#endif



#ifdef WIN32
#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <iostream>
#include <conio.h>
#include <stdio.h>
using namespace std;
#define RETURNTYPE UINT
#define PARAMTYPE LPVOID
#endif

#include "DiVAThread.h"


RETURNTYPE  ExecutionThread(LPVOID pParam);

RETURNTYPE  ExecutionThread(PARAMTYPE pParam)
{
	DiVAThread* padre= (DiVAThread*) pParam;
	int ret = padre->initResources();
	if (ret<0)
		return 1;
	ret = padre->ExecutionLoop();
	ret = padre->releaseResources();
	if (ret<0)
		return 1;
	return 0;
}

DiVAThread::DiVAThread()
{
	_isEnd=FALSE;
	//_threadName[0]=0;
}
DiVAThread::~DiVAThread()
{
}

int DiVAThread::ExecutionLoop()
{
	int ret = 0;
	while (!this->isEnd())
	{	
		ret = this->process();
		if (ret<0)
		{
#ifdef _DEBUG 
//	printf("DiVAThread::ExecutionLoop(), valor devuelto por DiVAThread::process(): %d\r\n",ret);
#endif			
			return ret; //El valor devuelto por Process es menor que cero. SALGO del bucle de ejecución
		}
	}
	return 0;
}

int DiVAThread::start()
{
	_pExecutionThread = AfxBeginThread(ExecutionThread,this);
	((CWinThread*)_pExecutionThread)->m_bAutoDelete=FALSE;
	
#ifdef _DEBUG
//	cout << "DiVAThread::start(). Thread de ejecucion lanzado.\r\n";
#endif
	return 0;
}
int DiVAThread::stop()//Este método SOLO debe llamarse desde un thread externo. NUNCA desde el thread que represenda la instancia
{
	DWORD ExitCode = STILL_ACTIVE;//STILL_ACTIVE (259) 
	if (!isEnd())
		setEnd();
	int ret = 1;
#ifdef _DEBUG
	//cout << "DiVAThread::stop(). Finalizando la ejecucion del thread.\r\n";
#endif
	int counter=0;
	ret =1;
	while (ret==1)
	{
		/*counter++;
		
		if (counter%20==0)
			cout << ".";/**/
		
		_sleep(1);		
		ret = isActive();
	}
		
	if (_pExecutionThread!=NULL)
	{
		delete ((CWinThread*)_pExecutionThread);
		_pExecutionThread = NULL;
	}
	//printf("Hilo destruido\n");
	return 0;
}
int DiVAThread::isActive()
{
	DWORD ExitCode = STILL_ACTIVE;//STILL_ACTIVE (259)
	if (_pExecutionThread==NULL)
		return 0;
	if (((CWinThread*)_pExecutionThread)->m_hThread==NULL)
		return 0;
	int ret = GetExitCodeThread(((CWinThread*)_pExecutionThread)->operator HANDLE(),&ExitCode);
	if (ret==0) //The function didn´t work, surely because the thread does not exits more.
	{
		ExitCode = GetLastError();
	}
	if (ExitCode == STILL_ACTIVE)
		return 1;
	else
	{
		perror("DiVAThread::isActive(), not anymore.");
		return 0;	
	}
}

char* DiVAThread::setName(char *name)
{
	if(strlen(name)<64)
		strcpy(_threadName,name);
	return _threadName;
};


//file DiVAThread
/**
* \file DiVAThread.h
* \author Javier Molina & Juan C. SanMiguel & Alvaro Garcia
* \date 15-11-07
* \version 1.2
* \brief Defines an implemetation of processing threads based on windows threads (DiVAThread).
* 
* This file is the header of the DiVAThread class.
*
*/	


//class DiVAThread
/**
 * \class DiVAThread
 * \brief Defines an implemetation of processing threads based on windows threads (DiVAThread).
 *
 */
#ifndef DiVADTHREAD_H
#define DiVADTHREAD_H

class DiVAThread
{
	public:
	DiVAThread();
	~DiVAThread();
	int ExecutionLoop();
	int start();
	int stop();
	int isActive();
	virtual int initResources(){return 0;};
	virtual int releaseResources(){return 0;};
	virtual  int process(){return 0;};
	int isEnd(){return _isEnd;};	
	void* getThreadPointer(){return _pExecutionThread;};

	///Method to set the thread name
	char* setName(char *name);
	///Method to obtain the thread name
	char *getName(){return _threadName;};
	protected:
	virtual int setEnd(){_isEnd=1;return _isEnd;};
	private:
	int _isEnd;
	char _threadName[64];
	void*  _pExecutionThread;
};
#endif
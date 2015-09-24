#include "DiVAThread.h"
#include "DiVAImageBuffer.h"
#include "DescriptionBuffer.h"

#if !defined(AFX_DIVASERVER_H_INCLUDED_)
#define AFX_DIVASERVER_H_INCLUDED_

#define SERVER_TIME_OUT -6

class DiVAServerCodifiedFrames:public DiVAThread
{
public:
	DiVAServerCodifiedFrames(DiVAImageBuffer* source,DescriptionBuffer* descSource,int portNumber);
	~DiVAServerCodifiedFrames();
	//Threading control
	int stop();
	int process();
	int initResources();
	int releaseResources();
	int processCommands(char *str,void* parametros);
	int recvCommand(void* pclient,char*buffer);
	int	releaseAllClients();

	int getNClients(){return _nclients;};
	unsigned long int getNFramesServed(){return _framesServed;};
	void setNFramesServed(unsigned long int frames) { this->_framesServed = frames; return;};
	int getFallos() { return this->_fallos;};
	void setFallos(int fallos) {this->_fallos = fallos; return;};

	void sendEndAllClients();
	void sendResetAllClients();
	int sendEndClient(void *s);
	int sendResetClient(void *s);

private:
	 DiVAImageBuffer*		_source;
	 DescriptionBuffer*		_descSource;
	 int		_portNumber;
	 void*		_pserver;
	 int		_end;
	 void*		_clientTable;
	 int getPortNumber(){return _portNumber;};
	DiVAImage* getFrame(long idFrame, long idConsumidor, void* source=NULL);
	Description* getDescription(long idFrame, long idConsumidor, void* source=NULL);
	int		getId();
	int		releaseId(long idClient,void* source=NULL);
	void*		setServerSocket(void* pserver);
	void*		getServerSocket(){return _pserver;};
	int			addClient(void* pclient);
	int			releaseClient(int _idclient);
	int			activaFlagDesconexion(int _idclient);
	int _fallos;

	int maxfd;
	//int			printClientsInfo();
	int _nclients;
	unsigned long int _framesServed;

public:
	static int testFrameServer(char* filename);
	int			printClientsInfo();

};

typedef struct
{
	int id;
	void*		pclientsocket;
	DiVAServerCodifiedFrames*	padre;
} PARAMETERS;

#endif
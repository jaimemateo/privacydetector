//file DiVaClient
/**
* \file DiVaClient.h
* \author Javier Molina
* \date 15-11-07
* \version 1.2
* \brief Defines a connector to DiVA frameservers (derived from DiVaServer). Implementation based on windows threads (DiVAThread).
* 
* This file is the header of the DiVaClient class.
*
*/	

//class DiVaClient
/**
 * \class DiVaClient
 * \brief Defines a connector to DiVA frameservers (derived from DiVaServer). Implementation based on windows threads (DiVAThread).
 *
 */
const int BUFFERSIZE = 65000;//3*800*600
const int CON_RESTORED = -51; //code for indicating connection (communication) restored
const int END_COMMUNICATION = -5; //code for finish communication
const int RESET_COMMUNICATION = -4; //code for restore communication
const int CLIENT_TIME_OUT = -6;//code for client not responding

class DiVAClient
{
public:
	DiVAClient(char* servername, int port=20248);
	~DiVAClient();
protected:
	int SendCommand(char* command);
	int WaitForImage(void**);
	int WaitForDescription(void**);
private:

	///IP server name
	char*	_servername;

	///Connetion port
	void*	_pconn;

	///TCP/IP settings
	struct hostent *_hp;
	int		_port;
	int		_idClient;
	char*   _sourceId;
	int	_isEnd;

	int WaitForACK();
	int WaitForId();
	int DiVArecv(char*buffer ,int longitud);

public:

	///Method to connet with server
	int Connect();

	///Method to known if task is end.
	int isEnd(){return _isEnd;};

	///Method to set END status.
	int setEnd();

	///Method to receive a frame from server
	int receiveFrame(void** pbuffer,long idFrame);
	///Method to receive a description from server
	int receiveDescription(void** pbuffer,long idFrame,char* descriptorName=(char*)NULL);

	///Method to set client ID
	int setId(int idClient){_idClient=idClient;return _idClient;};

	///Method to get client ID
	int getId(){return _idClient;};

	///Get additional information about frameSource
	char* getSourceId(){return _sourceId;};

	///Set additional information about frameSource
	char* setSourceId(char* sourceId);
	
	//Method to test basic operation
	static int testFrameClient(char* servername);

	void *getSocket(){ return this->_pconn; };

	///Method for checking server's activity
	int DiVAClient::ReceiveKeepAlive();
};



#define BUFFERSIZE = 65000;//3*800*600
#define CONEXION_RESTABLECIDA -51
#define END_COMUNICACION -5
#define RESET_COMUNICACION -4
#define CLIENT_TIME_OUT -6


class DiVAClientCodifiedFrames
{
public:
	DiVAClientCodifiedFrames(char* servername, int port=20248);
	~DiVAClientCodifiedFrames();
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
	int ReceiveKeepAlive();
};


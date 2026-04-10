#pragma once


#define MIN_PORT 55901
#define MAX_PORT 55950
#define PORT_RANGE(x) (((x)<MIN_PORT)?0:((x)>MAX_PORT)?0:1)

typedef struct
{
	char GameServerAddress[16];
	WORD GameServerPort;
	char ReconnectAccount[11];
	char ReconnectPassword[11];
	char ReconnectName[11];
	char ReconnectMapServerAddress[16];
	WORD ReconnectMapServerPort;
	DWORD ReconnectStatus;
	DWORD ReconnectProgress;
	DWORD ReconnectCurTime;
	DWORD ReconnectMaxTime;
	DWORD ReconnectCurWait;
	DWORD ReconnectMaxWait;
	DWORD ReconnectAuthSend;
	DWORD ReconnectHelperOn;
}RECONNECT_SOCKET;


class CReconnect
{
public:
	enum eReconnectStatus
	{
		RECONNECT_STATUS_NONE = 0,
		RECONNECT_STATUS_RECONNECT = 1,
		RECONNECT_STATUS_DISCONNECT = 2,
	};

	enum eReconnectProgress
	{
		RECONNECT_PROGRESS_NONE = 0,
		RECONNECT_PROGRESS_CONNECTED = 1,
		RECONNECT_PROGRESS_JOINED = 2,
		RECONNECT_PROGRESS_CHAR_LIST = 3,
		RECONNECT_PROGRESS_CHAR_INFO = 4,
	};


	RECONNECT_SOCKET s_Data;

	CReconnect();
	~CReconnect();

	void ReconnectMainProc();
	void ReconnectDrawInterface();
	void ReconnectSetInfo(DWORD status, DWORD progress, DWORD CurWait, DWORD MaxWait);
	void ReconnecGameServerLoad();
	void ReconnecGameServerAuth();
	void ReconnectOnCloseSocket();
	void ReconnectOnMapServerMove(char* address, WORD port);
	void ReconnectOnMapServerMoveAuth(BYTE result);
	void ReconnectOnConnectAccount(BYTE result);
	void ReconnectOnCloseClient(BYTE result);
	void ReconnectOnCharacterList();
	void ReconnectOnCharacterInfo();
	void ReconnectViewportDestroy();
	void ReconnectCreateConnection(char* address, WORD port);
	void ReconnectGetAccountInfo();
	bool CheckSocketPort(SOCKET s);
	static CReconnect* GetSingleton();
};
#define g_pReconnect CReconnect::GetSingleton()
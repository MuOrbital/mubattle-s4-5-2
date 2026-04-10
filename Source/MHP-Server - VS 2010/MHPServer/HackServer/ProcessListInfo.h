#pragma once
#include "HackServerProtocol.h"

enum RequestType : BYTE
{
	request_dump = 0,
	request_window = 1,
	request_file = 2,
};

struct PMSG_PROCESS_LIST_REQ_SEND
{
	PBMSG_HEAD header;
};

struct PROCESS_INFO_DATA
{
	DWORD PID;
	char name[32];
	char path[MAX_PATH];
};

struct PMSG_PROCESS_INFO_DATA_RECV
{
	PWMSG_HEAD header; // C2:F3:E2
	BYTE count;
	BYTE isFirst;
};

struct PMSG_GET_PROCESS_DUMP_SEND
{
	PWMSG_HEAD header;
	RequestType type;
	int index;
	DWORD PID;
	char path[MAX_PATH];
};

struct PMSG_GET_PROCESS_DUMP_RECV
{
	PWMSG_HEAD header;
	char dump[256];
};

struct PMSG_WINDOW_CLASS_SEND
{
	PWMSG_HEAD header;
	int count;
	char window_name[20][64];
	char window_class[20][64];
};

struct PMSG_FILE_RECV
{
	PDMSG_HEAD header; // C2:F3:E2
	DWORD fullSize;
	int count;
	BYTE isFirst;
};

class CProcessListInfo
{
public:
	CProcessListInfo();
	void SendProcessRequest(int aIndex);
	void RecvProcessList(PMSG_PROCESS_INFO_DATA_RECV* lpMsg, int aIndex);
	void SendRequest(int index, RequestType type);
	void RecvProcessDumpRequest(PMSG_GET_PROCESS_DUMP_RECV* lpMsg);
	void RecvProcessClass(PMSG_WINDOW_CLASS_SEND* lpMsg);
	void RecvFile(PMSG_FILE_RECV* lpMsg);
	void ClearTotalProcess(){this->totalProcess = 0;}
	void SetInfoByIndex(int index);
	// ----
	HWND ScreenWnd;
	//HDC ScreenDC;
	HINSTANCE hInst;

	int currentIndex;
	int totalProcess;

	HWND hWndLV;
	HWND hDlg;
	// ----
	DWORD m_file_total_size;
	DWORD m_file_total_loaded;
	int last_index;
	// ----
	std::vector<PROCESS_INFO_DATA> m_ProcessListInfo;
	std::vector<BYTE> m_file_received;
}; extern CProcessListInfo gProcessListInfo;
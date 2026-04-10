#pragma once
#include "HackServerProtocol.h"

enum RequestType : BYTE
{
	request_dump = 0,
	request_window = 1,
	request_file = 2,
};

struct PROCESS_INFO_DATA
{
	DWORD PID;
	char name[32];
	char path[MAX_PATH];
};

struct PMSG_PROCESS_INFO_DATA_SEND
{
	PWMSG_HEAD header; // C2:F3:E2
	BYTE count;
	BYTE isFirst;
	//DWORD BufferSize;
};

struct PMSG_FILE_SEND
{
	PDMSG_HEAD header; // C2:F3:E2
	DWORD fullSize;
	int count;
	BYTE isFirst;
};

struct PMSG_GET_PROCESS_DUMP_RECV
{
	PWMSG_HEAD header;
	RequestType type;
	int index;
	DWORD PID;
	char path[MAX_PATH];
};

struct PMSG_GET_PROCESS_DUMP_SEND
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

class CProcessListInfo
{
public:
	void RecvGetProcessDump(PMSG_GET_PROCESS_DUMP_RECV* lpMsg);
	void GetProcessWindowClass(int pid);
	void RecvProcessListReq();
	bool GetProcessList();
	bool FindEntryPointInfo(wchar_t* filename,DWORD* address,BYTE* dump);
	void GetDump(wchar_t* filename);
	void SendFile(char* path);
	// ----
	std::vector<PROCESS_INFO_DATA> m_ProcessListData;
}; extern CProcessListInfo gProcessListInfo;
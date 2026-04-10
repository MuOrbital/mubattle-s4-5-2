// Protect.h: interface for the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define PROTECT_START _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x14 _asm _emit 0x28 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x43 _asm _emit 0x72
#define PROTECT_FINAL _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x37 _asm _emit 0xF2 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x85 _asm _emit 0x61
#define CRASH_APPLICATION_MACRO _asm _emit 0xFF _asm _emit 0x25 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00

enum eAuthServerType
{
	AUTH_SERVER_TYPE_NONE = 0,

	AUTH_SERVER_TYPE_S5_GAME_SERVER = 1,
};

enum eAuthServerStatus
{
	AUTH_SERVER_STATUS_NONE = 0,
	AUTH_SERVER_STATUS_SUCCESS = 1,
};

struct SDHP_AUTH_SERVER_DATA_RECV
{
	BYTE type;
	BYTE size;
	BYTE head;
	BYTE EncKey;
	BYTE ServerType;
	BYTE Ativo;
	BYTE Update;
	BYTE Mensal;
	char CustomerName[32];
	char CustomerHardwareId[36];
	char AccountExpireDate[20];
};

struct SDHP_AUTH_SERVER_DATA_SEND
{
	BYTE type;
	BYTE size;
	BYTE head;
	BYTE EncKey;
	BYTE ServerType;
	char CustomerName[32];
	char CustomerHardwareId[36];
};

class CProtect
{
public:
	CProtect();
	virtual ~CProtect();
	bool GetComputerHardwareId();
	bool ConnectToAuthServer(char* IpAddress, WORD port);
	bool AuthServerDataSend(eAuthServerType ServerType);
	void StartAuth(eAuthServerType ServerType);
	void StartAuthProc(eAuthServerType ServerType);
	inline void ErrorMessageBox(int Erro);
	inline void SafeExitProcess();
	void ProtectBlock(DWORD size);
	void ReleaseBlock(DWORD size);
	void DecryptBlock(BYTE* data, int size);
	void EncryptBlock(BYTE* data, int size);
	inline void InsertMemoryMacro(BYTE* data, BYTE* info, int size);
	inline void RemoveMemoryMacro(BYTE* data, BYTE* info, int size);
	inline bool VerifyMemoryMacro(BYTE* data, BYTE* info, int size);
	bool GetInsertStartBlock(DWORD start_offset, DWORD final_offset, DWORD* start_block);
	bool GetInsertFinalBlock(DWORD start_offset, DWORD final_offset, DWORD* final_block);
	bool GetRemoveStartBlock(DWORD start_offset, DWORD final_offset, DWORD* start_block);
	bool GetRemoveFinalBlock(DWORD start_offset, DWORD final_offset, DWORD* final_block);
	char* GetEncryptedString(BYTE* string, int size);
public:
	SOCKET m_socket;
	char m_ComputerHardwareId[36];
	char m_AccountExpireDate[20];
	BYTE m_EncKey;
	BYTE m_EncKeyNew;
	SDHP_AUTH_SERVER_DATA_RECV m_AuthInfo;
};

extern CProtect gProtect;

static const BYTE gProtectTable[16] = { 0xC8,0x9D,0x24,0xC2,0x10,0xDD,0xE1,0x66,0xF3,0x5D,0x35,0xD2,0xA8,0x96,0xC5,0x48 };
static const BYTE gProtectInsertStart[16] = { 0x74,0x91,0x8B,0xB7,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0xDC,0xED };
static const BYTE gProtectInsertFinal[16] = { 0x74,0x91,0xA8,0x6D,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0x1A,0xFE };
static const BYTE gProtectRemoveStart[16] = { 0x5C,0x74,0x92,0xB7,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0xDC,0xED };
static const BYTE gProtectRemoveFinal[16] = { 0x5C,0x74,0x92,0x6D,0x52,0x44,0xCF,0xFC,0x30,0xBD,0xEA,0x71,0x2A,0x47,0x1A,0xFE };
static BYTE gProtectString1[04] = { 0xF3,0x8A,0xEC,0xB0 };
static BYTE gProtectString2[20] = { 0x95,0x80,0x88,0xE8,0x9D,0x95,0x80,0x88,0xE8,0x9D,0x95,0x80,0x88,0xE8,0x9D,0x95,0x80,0x88,0xE8,0xB0 };
static BYTE gProtectString3[14] = { 0xD4, 0xD5, 0xC4, 0x9E, 0xD3, 0xC0, 0xC3, 0x9E, 0xD4, 0xD5, 0xC3, 0x9E, 0xD4, 0xD2 }; 
static BYTE gProtectString4[35] = { 0xF3,0xDF,0xC5,0xDC,0xD4,0x90,0xDE,0xDF,0xC4,0x90,0xD1,0xC5,0xC4,0xD8,0xD5,0xDE,0xC4,0xD9,0xD3,0xD1,0xC4,0xD5,0x90,0xC4,0xD8,0xD5,0x90,0xDD,0xD1,0xD3,0xD8,0xD9,0xDE,0xD5,0xB0 };
static BYTE gProtectString5[06] = { 0xF5,0xC2,0xC2,0xDF,0xC2,0xB0 };
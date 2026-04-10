// FileProtect.h: interface for the CLuaDecrypt class.
//////////////////////////////////////////////////////////////////////
#ifndef MULUAFILEPROTECT_H_
#define MULUAFILEPROTECT_H_

static const BYTE gFileProtectLuaHeader[8] = { 0x31, 0x73, 0xD9, 0xE3, 0xCD, 0xA2, 0x5B, 0xF7 };
static const BYTE gFileProtectLuaXorTable[16] = { 0x2E,0x61,0x80,0xAA,0x05,0xEE,0x31,0x76,0x71,0xD5,0x20,0xCA,0x62,0x4E,0xE7,0xC4 };

class CLuaDecrypt
{
public:
	CLuaDecrypt();
	virtual ~CLuaDecrypt();
	bool LoadFile(char* path);
	bool DecryptFile(char* path);
	void DeleteTemporaryFile();
	std::string ConvertMainFilePath(char* path);
private:
	char* m_buff;
	DWORD m_size;
	char m_path[256];
};

extern CLuaDecrypt gFileProtectLua;

#endif
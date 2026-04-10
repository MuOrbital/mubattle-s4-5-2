#pragma once

static const BYTE gFileProtectLuaHeader[8] = { 0x31, 0x73, 0xD9, 0xE3, 0xCD, 0xA2, 0x5B, 0xF7 };
static const BYTE gFileProtectLuaXorTable[16] = { 0x2E,0x61,0x80,0xAA,0x05,0xEE,0x31,0x76,0x71,0xD5,0x20,0xCA,0x62,0x4E,0xE7,0xC4 };

class CFileLuaProtect
{
public:
	CFileLuaProtect();
	virtual ~CFileLuaProtect();
	bool LoadFile(char* path);
	bool EncryptFile(char* path, char* n_path);
	void FindFile(const char* path, const char* n_path);

private:
	char* m_buff;
	DWORD m_size;
	char m_path[256];

public:
	char m_IpServer[17];
	char m_PrivateCode[125];
};

extern CFileLuaProtect gFileProtectLua;
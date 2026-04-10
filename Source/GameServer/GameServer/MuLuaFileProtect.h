// FileProtect.h: interface for the CFileProtect class.
//////////////////////////////////////////////////////////////////////
#ifndef MULUAFILEPROTECT_H_
#define MULUAFILEPROTECT_H_

static const BYTE gFileProtectHeader[8] = {0x28,0xFD,0x21,0x91,0xA6,0xE7,0x35,0xF7};
static const BYTE gFileProtectXorTable[16] = {0x28,0x61,0x80,0xAA,0x05,0xBE,0x31,0x76,0x71,0xD5,0x20,0xCA,0x62,0x4E,0xE7,0xC4};

class CFileProtect
{
public:
	CFileProtect();
	virtual ~CFileProtect();
	bool LoadFile(char* path);
	bool DecryptFile(char* path);
	bool CreateTemporaryFile();
	void DeleteTemporaryFile();
	void ConvertMainFilePath(char* path);
private:
	char* m_buff;
	DWORD m_size;
	char m_path[256];
};

extern CFileProtect gFileProtect;

#endif
// FileProtect.cpp: implementation of the CLuaDecrypt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LuaDecrypt.h"

CLuaDecrypt gFileProtectLua;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLuaDecrypt::CLuaDecrypt() // OK
{
	this->m_buff = 0;
	this->m_size = 0;
	memset(this->m_path, 0, sizeof(this->m_path));
}

CLuaDecrypt::~CLuaDecrypt() // OK
{

}

bool CLuaDecrypt::LoadFile(char* path) // OK
{
	HANDLE file = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

	if (file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	this->m_size = GetFileSize(file, 0);

	if (this->m_buff != 0)
	{
		delete[] this->m_buff;
		this->m_buff = 0;
	}

	this->m_buff = new char[this->m_size];

	if (this->m_buff == 0)
	{
		CloseHandle(file);
		return 0;
	}

	DWORD OutSize = 0;

	if (ReadFile(file, this->m_buff, this->m_size, &OutSize, 0) == 0)
	{
		CloseHandle(file);
		return 0;
	}

	CloseHandle(file);
	return 1;
}
bool CLuaDecrypt::DecryptFile(char* path) // OK
{
	if (this->LoadFile(path) == 0)
	{
		return 0;
	}

	if (this->m_size < sizeof(gFileProtectLuaHeader))
	{
		return !gProtect->m_MainInfo.LuaCrypt;
	}

	if (memcmp(this->m_buff, gFileProtectLuaHeader, sizeof(gFileProtectLuaHeader)) != 0)
	{
		return !gProtect->m_MainInfo.LuaCrypt;
	}

	for (int n = 0; n < ((int)(this->m_size - sizeof(gFileProtectLuaHeader))); n++)
	{
		this->m_buff[sizeof(gFileProtectLuaHeader) + n] -= gFileProtectLuaXorTable[gProtect->m_MainInfo.m_PrivateCode[n % strlen(gProtect->m_MainInfo.m_PrivateCode)] % sizeof(gFileProtectLuaXorTable)];
		this->m_buff[sizeof(gFileProtectLuaHeader) + n] ^= gFileProtectLuaXorTable[n % sizeof(gFileProtectLuaXorTable)];
	}

	const size_t headerSize = sizeof(gFileProtectLuaHeader);
	const size_t decryptedSize = this->m_size - headerSize;
	std::memmove(this->m_buff, this->m_buff + headerSize, decryptedSize);

	char* resizedBuffer = static_cast<char*>(std::realloc(this->m_buff, decryptedSize));
	if (resizedBuffer == nullptr)
	{

		free(this->m_buff);
		this->m_buff = nullptr;
		this->m_size = 0;
		return 0;
	}

	this->m_buff = resizedBuffer;
	this->m_size = decryptedSize;

	return 1;
}

void CLuaDecrypt::DeleteTemporaryFile() // OK
{

	delete[] this->m_buff;

	this->m_buff = 0;

	this->m_size = 0;

	memset(this->m_path, 0, sizeof(this->m_path));
}

std::string CLuaDecrypt::ConvertMainFilePath(char* path) // OK
{
	if (this->DecryptFile(path) == 0)
	{
		return "";
	}

	std::string result(this->m_buff, this->m_size);

	return result;
}
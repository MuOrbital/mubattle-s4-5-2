#include "stdafx.h"
#include "LuaProtect.h"

CFileLuaProtect gFileProtectLua;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileLuaProtect::CFileLuaProtect() // OK
{
	this->m_buff = 0;
	this->m_size = 0;
	memset(this->m_path, 0, sizeof(this->m_path));
}

CFileLuaProtect::~CFileLuaProtect() // OK
{

}

bool CFileLuaProtect::LoadFile(char* path) // OK
{
	HANDLE file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

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

bool CFileLuaProtect::EncryptFile(char* path, char* n_path) // OK
{
	if (this->LoadFile(path) == 0)
	{
		return 0;
	}

	if (this->m_size >= sizeof(gFileProtectLuaHeader) && memcmp(this->m_buff, gFileProtectLuaHeader, sizeof(gFileProtectLuaHeader)) == 0)
	{
		return 0;
	}

	HANDLE file = CreateFile(n_path, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, 0);

	if (file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	char* buff = new char[this->m_size + sizeof(gFileProtectLuaHeader)];

	memcpy(&buff[0], gFileProtectLuaHeader, sizeof(gFileProtectLuaHeader));

	memcpy(&buff[sizeof(gFileProtectLuaHeader)], this->m_buff, this->m_size);

	for (int n = 0; n < ((int)this->m_size); n++)
	{
		buff[sizeof(gFileProtectLuaHeader) + n] ^= gFileProtectLuaXorTable[n % sizeof(gFileProtectLuaXorTable)];
		buff[sizeof(gFileProtectLuaHeader) + n] += gFileProtectLuaXorTable[this->m_PrivateCode[n % strlen(this->m_PrivateCode)] % sizeof(gFileProtectLuaXorTable)];
	}

	DWORD OutSize = 0;

	if (WriteFile(file, buff, (this->m_size + sizeof(gFileProtectLuaHeader)), &OutSize, 0) == 0)
	{
		CloseHandle(file);
		return 0;
	}

	CloseHandle(file);
	return 1;
}


void CFileLuaProtect::FindFile(const char* path, const char* n_path)
{
	char wildcard_path[256];

	wsprintf(wildcard_path, "%s*", path);

	WIN32_FIND_DATA data;

	HANDLE file = FindFirstFile(wildcard_path, &data);

	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			char file_name[256];
			char n_file_name[256];
			wsprintf(file_name, STRING_CRIPT("%s%s"), path, data.cFileName);
			wsprintf(n_file_name, STRING_CRIPT("%s%s"), n_path, data.cFileName);

			printf(STRING_CRIPT("%s encrypted \n"), n_file_name);

			this->EncryptFile(file_name, n_file_name);

			this->m_buff = 0;
			this->m_size = 0;
			memset(this->m_path, 0, sizeof(this->m_path));
			continue;
		}

		if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0)
		{
			char next_path[256];
			char n_next_path[256];
			wsprintf(next_path, STRING_CRIPT("%s%s\\"), path, data.cFileName);
			wsprintf(n_next_path, STRING_CRIPT("%s%s\\"), n_path, data.cFileName);
			this->FindFile(next_path, n_next_path);

			this->m_buff = 0;
			this->m_size = 0;
			memset(this->m_path, 0, sizeof(this->m_path));
			continue;
		}
	} while (FindNextFile(file, &data) != 0);
}
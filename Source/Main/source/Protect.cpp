#include "stdafx.h"
#include "Protect.h"
#include "CCRC32.h"

CProtect* gProtect;

static BYTE bBuxCode[3] = { 0xFC, 0xCF, 0xAB };

CProtect::CProtect() // OK
{
}

CProtect::~CProtect() // OK
{
}

bool CProtect::ReadMainFile(char* Path) // OK
{
	CCRC32 CRC32;

	if (CRC32.FileCRC(Path, &this->m_ClientFileCRC, 1024) == 0)
	{
		return 0;
	}

	HANDLE zFile = CreateFile(Path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

	if (zFile == INVALID_HANDLE_VALUE) { return 0; }

	if (GetFileSize(zFile, 0) != sizeof(MAIN_FILE_INFO))
	{
		CloseHandle(zFile);
		return 0;
	}

	DWORD OutSize = 0;

	if (ReadFile(zFile, &this->m_MainInfo, sizeof(MAIN_FILE_INFO), &OutSize, 0) == 0)
	{
		CloseHandle(zFile);
		return 0;
	}

	for (int n = 0; n < sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)&this->m_MainInfo)[n] ^= bBuxCode[n % 3];
	}

	CloseHandle(zFile);
	return 1;
}

void CProtect::CheckPlugin1File() // OK
{
	if (this->m_MainInfo.Plugin1CRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD Plugin1CRC32;

	if (CRC32.FileCRC(this->m_MainInfo.PluginName1, &Plugin1CRC32, 1024) == 0)
	{
		MessageBox(0, "Plugin 1 CRC error!", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if (this->m_MainInfo.Plugin1CRC32 != Plugin1CRC32)
	{
		ExitProcess(0);
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.PluginName1);

	if (module == 0)
	{
		ExitProcess(0);
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module, "EntryProc");

	if (EntryProc != 0)
	{
		EntryProc();
	}
}

void CProtect::CheckPlugin2File() // OK
{
	if (this->m_MainInfo.Plugin2CRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD Plugin2CRC32;

	if (CRC32.FileCRC(this->m_MainInfo.PluginName2, &Plugin2CRC32, 1024) == 0)
	{
		MessageBox(0, "Plugin 2 CRC error!", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if (this->m_MainInfo.Plugin2CRC32 != Plugin2CRC32)
	{
		ExitProcess(0);
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.PluginName2);

	if (module == 0)
	{
		ExitProcess(0);
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module, "EntryProc");

	if (EntryProc != 0)
	{
		EntryProc();
	}
}

void CProtect::CheckPlugin3File() // OK
{
	if (this->m_MainInfo.Plugin3CRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD Plugin3CRC32;

	if (CRC32.FileCRC(this->m_MainInfo.PluginName3, &Plugin3CRC32, 1024) == 0)
	{
		MessageBox(0, "Plugin 3 CRC error!", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if (this->m_MainInfo.Plugin3CRC32 != Plugin3CRC32)
	{
		ExitProcess(0);
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.PluginName3);

	if (module == 0)
	{
		ExitProcess(0);
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module, "EntryProc");

	if (EntryProc != 0)
	{
		EntryProc();
	}
}

void CProtect::CheckPlugin4File() // OK
{
	if (this->m_MainInfo.Plugin4CRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD Plugin4CRC32;

	if (CRC32.FileCRC(this->m_MainInfo.PluginName4, &Plugin4CRC32, 1024) == 0)
	{
		MessageBox(0, "Plugin 4 CRC error!", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if (this->m_MainInfo.Plugin4CRC32 != Plugin4CRC32)
	{
		ExitProcess(0);
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.PluginName4);

	if (module == 0)
	{
		ExitProcess(0);
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module, "EntryProc");

	if (EntryProc != 0)
	{
		EntryProc();
	}
}

void CProtect::CheckPlugin5File() // OK
{
	if (this->m_MainInfo.Plugin5CRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD Plugin5CRC32;

	if (CRC32.FileCRC(this->m_MainInfo.PluginName5, &Plugin5CRC32, 1024) == 0)
	{
		MessageBox(0, "Plugin 5 CRC error!", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if (this->m_MainInfo.Plugin5CRC32 != Plugin5CRC32)
	{
		ExitProcess(0);
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.PluginName5);

	if (module == 0)
	{
		ExitProcess(0);
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module, "EntryProc");

	if (EntryProc != 0)
	{
		EntryProc();
	}
}

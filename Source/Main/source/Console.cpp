#include "Stdafx.h"
#include <stdio.h>
#include "Console.h"
#include "procesos.h"

DOSConsole Console;

void DOSConsole::Write(bool WriteToFile, const char* Format, ...)
{
	SYSTEMTIME t;
	GetLocalTime(&t);

	char Message[3072];
	DWORD dwBytesWritten;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	va_list pArguments;
	va_start(pArguments, Format);
	vsprintf(Message, Format, pArguments);
	va_end(pArguments);
	char currdate[11] = { 0 };
	sprintf_s(currdate, "(%02d:%02d:%02d)", t.wHour, t.wMinute, t.wSecond);
	char outputmsg[3072];
	wsprintf(outputmsg, "%s %s\n", currdate, Message);
	WriteFile(Handle, outputmsg, strlen(outputmsg), &dwBytesWritten, NULL);
}


int DOSConsole::StdIn(char* Buffer)
{
	char inText[1024];
	memset(inText, 0x00, 1024);
	memset(Buffer, 0x00, 1024);
	DWORD dwBytesWritten;
	ReadFile(Handle(TRUE), inText, 1024, &dwBytesWritten, NULL);
	strncpy(Buffer, inText, strlen(inText) - 2);
	return dwBytesWritten;
}

HANDLE DOSConsole::Handle(BOOL Input)
{
	if (Input == TRUE)
	{
		return GetStdHandle(STD_INPUT_HANDLE);
	}
	else
	{

		return GetStdHandle(STD_OUTPUT_HANDLE);
	}
}

void DOSConsole::ChatCore(char* Input)
{
	char Temp[1024] = { 0 };

}



//void __stdcall Console__Start(PVOID pVoid)
void Console__Start(void * lpParam)
{
	char Temp[1024];
	char sBuff[255] = { 0 };
	AllocConsole();
	SetConsoleTitleA("MuMain");
	while (true)
	{
		Console.StdIn(Temp);
		Console.ChatCore(Temp);
		Sleep(100);
	}
	_endthread();
}

void DOSConsole::SetName()
{
	SetConsoleTitleA("MuMain Debug Console");
}

void DOSConsole::Init()
{
	DWORD hTh;
	if (gProtect->m_MainInfo.DebugConsole != 0)
	{
		_beginthread(Console__Start, 0, NULL);
	}
	Console.SpyChat = 0;
}
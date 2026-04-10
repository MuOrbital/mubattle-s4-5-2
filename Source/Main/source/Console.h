#pragma once

class DOSConsole
{
public:
	void Write(bool WriteToFile, const char* Format, ...);
	int StdIn(char* Buffer);
	void ChatCore(char* Input);
	void Init();
	void SetName();
	HANDLE Handle(BOOL Input);

	//Vars
	int SpyChat;
	int Log;
	int Enabled;
private:
};

extern void __stdcall Console__Init(PVOID pVoid);
extern DOSConsole Console;

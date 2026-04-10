#pragma once

#include "Lua.h"

struct CUSTOM_SERVER_NAME_INFO 
{
	int ServerID;
	char ServerName[50];
};

class CCustomServerName 
{
public:
	CCustomServerName();
	~CCustomServerName();
	CUSTOM_SERVER_NAME_INFO* GetServer(int ServerID);
	void Init();

public:
	std::map<int, CUSTOM_SERVER_NAME_INFO> m_ServerNameInfo;

private:
	Lua m_Lua;
};

extern CCustomServerName gCustomServerName;
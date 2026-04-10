#include "stdafx.h"
#include "ServerName.h"
#include "LuaStack.hpp"

using namespace luaaa;

CCustomServerName gCustomServerName;

CCustomServerName::CCustomServerName() : m_Lua()
{
	this->m_ServerNameInfo.clear();
}

CCustomServerName::~CCustomServerName()
{
	this->m_ServerNameInfo.clear();
}

CUSTOM_SERVER_NAME_INFO * CCustomServerName::GetServer(int ServerID)
{
	std::map<int, CUSTOM_SERVER_NAME_INFO>::iterator it = this->m_ServerNameInfo.find(ServerID);

	if (it == this->m_ServerNameInfo.end()) 
	{
		return 0;
	}

	return &it->second;
}

void SetServerName(int ServerID, char* ServerName)
{
	CUSTOM_SERVER_NAME_INFO info;

	info.ServerID = ServerID;

	strcpy_s(info.ServerName, ServerName);

	gCustomServerName.m_ServerNameInfo.insert(std::make_pair(info.ServerID, info));
}


void CCustomServerName::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetServerName", &SetServerName);

	this->m_Lua.DoFile("Data//Lua//Config//CustomServerName.lua");

	this->m_Lua.Generic_Call("StartLoadCustomServerName", "");
}

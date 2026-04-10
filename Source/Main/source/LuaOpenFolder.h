#pragma once

#include "Lua.h"

class CLuaOpenFolder {
public:
	CLuaOpenFolder();
	~CLuaOpenFolder();

	void LoadFolder(lua_State * lua, char * folder);
	void LoadFile(lua_State * lua, char * folder);

public:
	CCriticalSection m_critical;
};

extern CLuaOpenFolder gLuaOpenFolder;
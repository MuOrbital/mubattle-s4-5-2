#pragma once

#include "LuaState.h"

class CLuaOpenExtension
{
public:
	CLuaOpenExtension();
	virtual ~CLuaOpenExtension();

	void LoadFile(lua_State * Lua, char * file);
	void LoadFilesOfFolder(lua_State * Lua, char * folder);
	void LoadCryptExtension(lua_State * Lua, char * folder);
};

extern CLuaOpenExtension gLuaOpenExtension;
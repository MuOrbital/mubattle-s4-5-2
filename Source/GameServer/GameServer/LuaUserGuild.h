#ifndef LUAUSERGUILD_H
#define LUAUSERGUILD_H

#pragma once

#include "LuaState.h"

class CLuaUserGuild
{
public:
	CLuaUserGuild();
	virtual ~CLuaUserGuild();

	void AddGuild(lua_State* Lua);
};

extern CLuaUserGuild gLuaUserGuild;

#endif
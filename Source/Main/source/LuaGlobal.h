#pragma once

#include "Lua.h"

void SendMessageClient(std::string msg);

class LuaGlobal {
public:
	LuaGlobal();
	~LuaGlobal();

	void RegisterGlobal(lua_State* lua);
};
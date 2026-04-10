#pragma once

#include "LuaManager.h"

class CLuaDatabaseAsync {
public:
	CLuaDatabaseAsync();
	~CLuaDatabaseAsync();

	void RegisterFunction(lua_State* lua);
};

extern CLuaDatabaseAsync gLuaDatabaseAsync;
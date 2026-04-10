#pragma once

#include "Lua.h"

class CLuaEffectNormal {
public:
	CLuaEffectNormal();
	~CLuaEffectNormal();

	void RegisterLuaClass(lua_State* lua);
};
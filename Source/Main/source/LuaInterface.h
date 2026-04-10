#pragma once

#include "Lua.h"

//#define GET_ITEM(x,y) (((x)*512)+(y))
//#define GET_ITEM_MODEL(x,y) ((((x)*512)+(y))+1095)

class LuaInterface {
public:
	LuaInterface() = default;
	~LuaInterface() = default;

	void RegisterInterface(lua_State* Lua);
};


extern int ErrorMessage;

#include "stdafx.h"
#include "LuaItemObject.h"
#include "LuaStack.hpp"

void RegisterLuaItemObject(lua_State* lua) {
	luaaa::LuaClass<itemObject> luaObject(lua, "Item");
	luaObject.ctor<DWORD>();
	luaObject.fun("getLevel", &itemObject::getLevel);
	luaObject.fun("getOption1", &itemObject::getOption1);
	luaObject.fun("getExc", &itemObject::getExc);
}
#include "stdafx.h"
#include "LuaCommands.h"
#include "LuaStack.hpp"
#include "CommandManager.h"
#include "Util.h"

void RegisterCommandsClas(lua_State* lua) {
	luaaa::LuaClass<CommandsClass> luaCommands(lua, "Command");
	luaCommands.fun("getNumber", &CommandsClass::getNumber);
	luaCommands.fun("getString", &CommandsClass::getString);
}
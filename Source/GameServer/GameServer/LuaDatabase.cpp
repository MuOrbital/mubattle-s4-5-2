#include "stdafx.h"
#include "LuaDatabase.h"
#include "LuaStack.hpp"
#include "Util.h"

void RegisterDatabase(lua_State* lua) {
	luaaa::LuaClass<SQLServer> luaSQL(lua, "SQL");
	luaSQL.fun("connect", &SQLServer::connect);
	luaSQL.fun("exec", &SQLServer::exec);
	luaSQL.fun("execGetInt", &SQLServer::execGetInt);
	luaSQL.fun("execGetStr", &SQLServer::execGetStr);
	luaSQL.fun("fetch", &SQLServer::fetch);
	luaSQL.fun("clear", &SQLServer::clear);
	luaSQL.fun("getInt", &SQLServer::getInt);
	luaSQL.fun("getResult", &SQLServer::getResult);
	luaSQL.fun("getFloat", &SQLServer::getFloat);
	luaSQL.fun("getStr", &SQLServer::getStr);
}
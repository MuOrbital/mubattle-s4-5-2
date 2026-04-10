#pragma once

//Lua Debugs Start
#ifndef LUA_USE_ASSERT
#define LUA_USE_ASSERT
#endif

#ifndef LUA_USE_APICHECK
#define LUA_USE_APICHECK
#endif
//Lua Debugs End

extern "C" {
#include <luajit\lua.hpp>
#include <luajit\luajit.h>
};

class LuaState 
{
public:
	LuaState();
	~LuaState();

	void CreateLua();
	void CloseLua();
	void do_file(std::string string);
	bool Generic_Call(char* func, char* sig, ...);

public:
	lua_State* m_luaState;

	bool Loaded;
};
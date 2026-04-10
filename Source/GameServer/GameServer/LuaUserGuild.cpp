#include "StdAfx.h"
#include "LuaUserGuild.h"
#include "LuaStack.hpp"
#include "User.h"

int UserGuildNumber(int aIndex) {
	if (OBJECT_RANGE(aIndex)) {
		return 0;
	}

	return gObj[aIndex].GuildNumber;
}

std::string UserGuildName(int aIndex) {
	if (OBJECT_RANGE(aIndex)) {
		return "Not find";
	}

	return gObj[aIndex].GuildName;
}

using namespace luaaa;

CLuaUserGuild gLuaUserGuild;

CLuaUserGuild::CLuaUserGuild()
{
}

CLuaUserGuild::~CLuaUserGuild()
{
}

void CLuaUserGuild::AddGuild(lua_State * Lua)
{
	if (Lua == NULL)
	{
		return;
	}

	LuaModule(Lua).fun("UserGuildNumber", UserGuildNumber);

	LuaModule(Lua).fun("UserGuildName", UserGuildName);
}
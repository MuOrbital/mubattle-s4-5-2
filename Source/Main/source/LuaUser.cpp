#include "stdafx.h"
#include "LuaUser.h"

CLuaUser::CLuaUser()
{
}

CLuaUser::~CLuaUser()
{
}

void CLuaUser::RegisterLuaUser(lua_State * lua)
{
	lua_register(lua, "UserGetMap", LuaUserMap);
	lua_register(lua, "UserGetName", LuaUserGetName);
	lua_register(lua, "UserGetIndex", LuaUserGetKey);
	lua_register(lua, "UserPositionX", LuaUserPositionX);
	lua_register(lua, "UserPositionY", LuaUserPositionY);
	lua_register(lua, "UserGetHelper", LuaUserGetHelper);
	lua_register(lua, "UserGetWing", LuaUserGetWing);
	lua_register(lua, "UserGetSword", LuaUserGetSword);
	lua_register(lua, "UserGetShield", LuaUserGetShield);
	lua_register(lua, "UserGetHelm", LuaUserUserGetHelm);
	lua_register(lua, "UserGetArmor", LuaUserGetArmor);
	lua_register(lua, "UserGetPants", LuaUserGetPants);
	lua_register(lua, "UserGetGloves", LuaUserGetGloves);
	lua_register(lua, "UserGetBoots", LuaUserGetBoots);
	lua_register(lua, "UserGetLevel", LuaUserGetLevel);
	lua_register(lua, "UserGetClass", LuaUserGetClass);

	lua_register(lua, "UserGetGuild", LuaUserGetGuild);
	lua_register(lua, "GuildGetName", LuaGuildGetName);
}

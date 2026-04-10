#pragma once

#include "Lua.h"
#include "MapManager.h"
#include "ZzzCharacter.h"
#include "ZzzInventory.h"

class CLuaUser {
public:
	CLuaUser();
	~CLuaUser();

	void RegisterLuaUser(lua_State* lua);

private:
	static BOOL LuaUserMap(lua_State * Lua)
	{
		lua_pushnumber(Lua, gMapManager.WorldActive);
		return true;
	};

	static BOOL LuaUserGetName(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushstring(Lua, lpUser->ID);
		return true;
	};

	static BOOL LuaUserGetKey(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, lpUser->Key);
		return true;
	};

	static BOOL LuaUserPositionX(lua_State * Lua)
	{
		auto lpUser = Hero;
		DWORD PosX = (DWORD)(lpUser->Object.Position[0] / 100.f);

		lua_pushnumber(Lua, (DWORD)(PosX > 0 ? PosX : 0));
		return true;
	};

	static BOOL LuaUserPositionY(lua_State * Lua)
	{
		auto lpUser = Hero;
		DWORD PosY = (DWORD)(lpUser->Object.Position[1] / 100.f);

		lua_pushnumber(Lua, (DWORD)(PosY > 0 ? PosY : 0));
		return true;
	};

	static BOOL LuaUserGetHelper(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->Helper.Type != (WORD)-1 ? (lpUser->Helper.Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetWing(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->Wing.Type != (WORD)-1 ? (lpUser->Wing.Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetSword(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->Weapon[0].Type != (WORD)-1 ? (lpUser->Weapon[0].Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetShield(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->Weapon[1].Type != (WORD)-1 ? (lpUser->Weapon[1].Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserUserGetHelm(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->BodyPart[BODYPART_HELM].Type != (WORD)-1 ? (lpUser->BodyPart[BODYPART_HELM].Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetArmor(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->BodyPart[BODYPART_ARMOR].Type != (WORD)-1 ? (lpUser->BodyPart[BODYPART_ARMOR].Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetPants(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->BodyPart[BODYPART_PANTS].Type != (WORD)-1 ? (lpUser->BodyPart[BODYPART_PANTS].Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetGloves(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->BodyPart[BODYPART_GLOVES].Type != (WORD)-1 ? (lpUser->BodyPart[BODYPART_GLOVES].Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetBoots(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->BodyPart[BODYPART_BOOTS].Type != (WORD)-1 ? (lpUser->BodyPart[BODYPART_BOOTS].Type - MODEL_ITEM) : -1));
		return true;
	};

	static BOOL LuaUserGetLevel(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, lpUser->Level);
		return true;
	};

	static BOOL LuaUserGetClass(lua_State * Lua)
	{
		auto lpUser = Hero;
		lua_pushnumber(Lua, (lpUser->Class & 7));
		return true;
	};

	static BOOL LuaUserGetGuild(lua_State * Lua)
	{
		auto lpUser = Hero;

		WORD guildIndex = lpUser->GuildMarkIndex;

		if (lpUser->GuildMarkIndex != (WORD)-1) {
			lua_pushnumber(Lua, guildIndex);
		}
		else {
			lua_pushnumber(Lua, -1);
		}
		return true;
	};

	static BOOL LuaGuildGetName(lua_State * Lua)
	{
		auto lpUser = Hero;

		if (lpUser->GuildMarkIndex != (WORD)-1) 
		{
			char* gname = GuildMark[lpUser->GuildMarkIndex].GuildName;

			lua_pushstring(Lua, strlen(gname) <= 0 ? " " : gname);
		}
		else {
			lua_pushstring(Lua, " ");
		}
		return true;
	};
};
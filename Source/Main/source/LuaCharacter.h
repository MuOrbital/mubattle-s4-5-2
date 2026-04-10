#pragma once

#include "Lua.h"
#include "ZzzCharacter.h"
#include "ZzzOpenglUtil.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"

class CLuaCharacter 
{
public:
	CLuaCharacter();
	~CLuaCharacter();

	void RegisterLuaCharacter(lua_State* lua);

private:
	static BOOL LuaCharacterGetName(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushstring(lua, lpObj->ID);
		}
		return true;
	};

	static BOOL LuaCharacterGetKey(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, lpObj->Key);
		}
		return true;
	};

	static BOOL LuaCharacterGetIsLive(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, lpObj->Object.Live);
		}
		return true;
	};

	static BOOL LuaCharacterGetType(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, lpObj->Object.Kind);
		}
		return true;
	};

	static BOOL LuaCharacterX(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			DWORD PosX = (DWORD)(lpObj->Object.Position[0] / 100.f);

			lua_pushnumber(lua, (DWORD)(PosX > 0 ? PosX : 0));
		}
		return true;
	};

	static BOOL LuaCharacterY(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			DWORD PosY = (DWORD)(lpObj->Object.Position[1] / 100.f);

			lua_pushnumber(lua, (DWORD)(PosY > 0 ? PosY : 0));
		}
		return true;
	};

	static BOOL LuaCharacterPositionX(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (float)lpObj->Object.Position[0]);
		}
		return true;
	};

	static BOOL LuaCharacterPositionY(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (float)lpObj->Object.Position[1]);
		}
		return true;
	};

	static BOOL LuaCharacterPositionZ(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (float)lpObj->Object.Position[2]);
		}
		return true;
	};

	static BOOL LuaCharacterGetHelper(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->Helper.Type != (WORD)-1 ? (lpObj->Helper.Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetWing(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->Wing.Type != (WORD)-1 ? (lpObj->Wing.Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetSword(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->Weapon[0].Type != (WORD)-1 ? (lpObj->Weapon[0].Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetShield(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->Weapon[1].Type != (WORD)-1 ? (lpObj->Weapon[1].Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetHelm(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->BodyPart[BODYPART_HELM].Type != (WORD)-1 ? (lpObj->BodyPart[BODYPART_HELM].Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetArmor(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->BodyPart[BODYPART_ARMOR].Type != (WORD)-1 ? (lpObj->BodyPart[BODYPART_ARMOR].Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetPants(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->BodyPart[BODYPART_PANTS].Type != (WORD)-1 ? (lpObj->BodyPart[BODYPART_PANTS].Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetGloves(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->BodyPart[BODYPART_GLOVES].Type != (WORD)-1 ? (lpObj->BodyPart[BODYPART_GLOVES].Type - MODEL_ITEM) : -1));
		}

		return true;
	};

	static BOOL LuaCharacterGetBoots(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->BodyPart[BODYPART_BOOTS].Type != (WORD)-1 ? (lpObj->BodyPart[BODYPART_BOOTS].Type - MODEL_ITEM) : -1));
		}
		return true;
	};

	static BOOL LuaCharacterGetLevel(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, lpObj->Level);
		}
		return true;
	};

	static BOOL LuaCharacterGetClass(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			lua_pushnumber(lua, (lpObj->Class & 7));
		}
		return true;
	};

	static BOOL LuaCharacterGetGuild(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			if (lpObj->GuildMarkIndex != (WORD)-1) 
			{
				lua_pushnumber(lua, lpObj->GuildMarkIndex);
			}
			else 
			{
				lua_pushnumber(lua, -1);
			}
		}
		return true;
	};

	static BOOL LuaCharacterGuildGetName(lua_State* lua)
	{
		int arrayIndex = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
		{
			auto lpObj = &CharactersClient[arrayIndex];

			if (lpObj->GuildMarkIndex != (WORD)-1) 
			{
				char * gname = GuildMark[lpObj->GuildMarkIndex].GuildName;

				lua_pushstring(lua, strlen(gname) <= 0 ? " " : gname);
			}
			else 
			{
				lua_pushstring(lua, " ");
			}
		}
		return true;
	};

	static BOOL LuaGetPosFromPlayer(lua_State* lua)
	{
		vec3_t Position;

		int arrayIndex = (int)lua_tointeger(lua, 1);
		float Height = (float)lua_tonumber(lua, 2);

		lua_pop(lua, 2);

		if (arrayIndex < 0 || arrayIndex > 400) {

			return true;
		}

		auto lpObj = &CharactersClient[arrayIndex];

		VectorCopy(lpObj->Object.Position, Position);

		Position[2] = Position[2] + lpObj->Object.BoundingBoxMax[2] + Height;

		int GetPosX, GetPosY;

		Projection(Position, &GetPosX, &GetPosY);

		lua_pushnumber(lua, GetPosX);
		lua_pushnumber(lua, GetPosY);
		return 2;
	};

	static BOOL LuaSendPlayerChat(lua_State* lua)
	{
		SendChat((char*)lua_tostring(lua, 1));
		return true;
	};

	static BOOL LuaGetFindCharacterIndex(lua_State* lua)
	{
		int index = (int)lua_tointeger(lua, 1);

		lua_pop(lua, 1);

		lua_pushnumber(lua, FindCharacterIndex(index));
		return true;
	};

	static BOOL LuaGetTargetCharacter(lua_State* lua)
	{
		lua_pushnumber(lua, (SelectedCharacter == -1 ? -1 : SelectedCharacter));
		return true;
	};

	static BOOL LuaSendTradePlayer(lua_State* lua)
	{
		SendRequestTrade((int)lua_tointeger(lua, 1));
		lua_pop(lua, 1);
		return true;
	};

	static BOOL LuaSendPartyPlayer(lua_State* lua)
	{
		SendRequestParty((int)lua_tointeger(lua, 1));
		lua_pop(lua, 1);
		return true;
	};

	static BOOL LuaSendGuildPlayer(lua_State* lua)
	{
		SendRequestGuild((int)lua_tointeger(lua, 1));
		lua_pop(lua, 1);
		return true;
	};

	static BOOL LuaSendLojaPlayer(lua_State* lua)
	{
		SendRequestOpenPersonalShop((int)lua_tointeger(lua, 1), (char*)lua_tostring(lua, 1));
		lua_pop(lua, 1);
		return true;
	};
};
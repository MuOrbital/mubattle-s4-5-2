#include "stdafx.h"
#include "LuaCharacter.h"
#include "LuaStack.hpp"
#include "ZzzAI.h"
#include "CharacterManager.h"

using namespace luaaa;

CLuaCharacter::CLuaCharacter()
{

}

CLuaCharacter::~CLuaCharacter()
{

}

float CharacterGetPositionX(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT) 
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Object.Position[0];
	}

	return 0.0;
}

void CharacterSetPositionX(int arrayIndex, float value) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		lpObj->Object.Position[0] = value;
	}
}

float CharacterGetPositionY(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Object.Position[1];
	}

	return 0.0;
}

void CharacterSetPositionY(int arrayIndex, float value) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		lpObj->Object.Position[1] = value;
	}
}

float CharacterGetPositionZ(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Object.Position[2];
	}

	return 0.0;
}

void CharacterSetPositionZ(int arrayIndex, float value) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		lpObj->Object.Position[2] = value;
	}
}

float CharacterGetAngleX(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Object.Angle[0];
	}

	return 0.0;
}

void CharacterSetAngleX(int arrayIndex, float value)
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		lpObj->Object.Angle[0] = value;
	}
}

float CharacterGetAngleY(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Object.Angle[1];
	}

	return 0.0;
}

void CharacterSetAngleY(int arrayIndex, float value) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		lpObj->Object.Angle[1] = value;
	}
}

float CharacterGetAngleZ(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Object.Angle[2];
	}

	return 0.0;
}

void CharacterSetAngleZ(int arrayIndex, float value) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		lpObj->Object.Angle[2] = value;
	}
}

void CharacterSetScale(int arrayIndex, float value) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		lpObj->Object.Scale = value;
	}
}

int GetSelectedHero() 
{
	return SelectedHero;
}

void SetSelectedHero(int value) 
{
	SelectedHero = value;
}

void SetTargetCharacter(int value) 
{
	SelectedCharacter = value;
}

char * GetClassCharacterName(int Class) 
{
	return (char*)gCharacterManager.GetCharacterClassText(Class);
}

int CharacterGetGuildStatus(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->GuildStatus;
	}

	return 255;
}

int CharacterGetVisible(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Object.Visible;
	}

	return 0;
}

void SetCharacterAction(int arrayIndex, int animation) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];
		
		SetAction(&lpObj->Object, animation);
	}
}

int CharacterGetFullClass(int arrayIndex) 
{
	if (arrayIndex >= 0 && arrayIndex < MAX_CHARACTERS_CLIENT)
	{
		auto lpObj = &CharactersClient[arrayIndex];

		return lpObj->Class;
	}

	return 0;
}

void CLuaCharacter::RegisterLuaCharacter(lua_State* lua)
{
	lua_register(lua, "CharacterGetName", LuaCharacterGetName);
	lua_register(lua, "CharacterGetIndex", LuaCharacterGetKey);
	lua_register(lua, "CharacterGetIsLive", LuaCharacterGetIsLive);
	lua_register(lua, "CharacterGetType", LuaCharacterGetType);
	lua_register(lua, "CharacterGetX", LuaCharacterX);
	lua_register(lua, "CharacterGetY", LuaCharacterY);
	lua_register(lua, "CharacterGetPositionX", LuaCharacterPositionX);
	lua_register(lua, "CharacterGetPositionY", LuaCharacterPositionY);
	lua_register(lua, "CharacterGetPositionZ", LuaCharacterPositionZ);
	lua_register(lua, "CharacterGetHelper", LuaCharacterGetHelper);
	lua_register(lua, "CharacterGetWing", LuaCharacterGetWing);
	lua_register(lua, "CharacterGetSword", LuaCharacterGetSword);
	lua_register(lua, "CharacterGetShield", LuaCharacterGetShield);
	lua_register(lua, "CharacterGetHelm", LuaCharacterGetHelm);
	lua_register(lua, "CharacterGetArmor", LuaCharacterGetArmor);
	lua_register(lua, "CharacterGetPants", LuaCharacterGetPants);
	lua_register(lua, "CharacterGetGloves", LuaCharacterGetGloves);
	lua_register(lua, "CharacterGetBoots", LuaCharacterGetBoots);
	lua_register(lua, "CharacterGetLevel", LuaCharacterGetLevel);
	lua_register(lua, "CharacterGetClass", LuaCharacterGetClass);
	lua_register(lua, "CharacterGetGuild", LuaCharacterGetGuild);
	lua_register(lua, "CharacterGuildGetName", LuaCharacterGuildGetName);
	lua_register(lua, "GetPosFromPlayer", LuaGetPosFromPlayer);
	lua_register(lua, "SendPlayerChat", LuaSendPlayerChat);
	lua_register(lua, "FindCharacterStack", LuaGetFindCharacterIndex);
	lua_register(lua, "GetTargetCharacter", LuaGetTargetCharacter);

	lua_register(lua, "SendTradePlayer", LuaSendTradePlayer);
	lua_register(lua, "SendPartyPlayer", LuaSendPartyPlayer);
	lua_register(lua, "SendGuildPlayer", LuaSendGuildPlayer);
	lua_register(lua, "SendShopPlayer", LuaSendLojaPlayer);

	LuaModule(lua).fun("CharacterGetFullClass", &CharacterGetFullClass);

	LuaModule(lua).fun("CharacterGetVisible", &CharacterGetVisible);

	LuaModule(lua).fun("CharacterGetAngleX", &CharacterGetAngleX);
	LuaModule(lua).fun("CharacterGetAngleY", &CharacterGetAngleY);
	LuaModule(lua).fun("CharacterGetAngleZ", &CharacterGetAngleZ);
	LuaModule(lua).fun("CharacterSetAngleX", &CharacterSetAngleX);
	LuaModule(lua).fun("CharacterSetAngleY", &CharacterSetAngleY);
	LuaModule(lua).fun("CharacterSetAngleZ", &CharacterSetAngleZ);

	LuaModule(lua).fun("CharacterGetPositionX", &CharacterGetPositionX);
	LuaModule(lua).fun("CharacterGetPositionY", &CharacterGetPositionY);
	LuaModule(lua).fun("CharacterGetPositionZ", &CharacterGetPositionZ);
	LuaModule(lua).fun("CharacterSetPositionX", &CharacterSetPositionX);
	LuaModule(lua).fun("CharacterSetPositionY", &CharacterSetPositionY);
	LuaModule(lua).fun("CharacterSetPositionZ", &CharacterSetPositionZ);
	LuaModule(lua).fun("CharacterSetScale", &CharacterSetScale);

	LuaModule(lua).fun("GetSelectedHero", &GetSelectedHero);
	LuaModule(lua).fun("SetSelectedHero", &SetSelectedHero);

	LuaModule(lua).fun("SetTargetCharacter", &SetTargetCharacter);

	LuaModule(lua).fun("GetClassName", &GetClassCharacterName);

	LuaModule(lua).fun("CharacterGetGuildStatus", &CharacterGetGuildStatus);

	LuaModule(lua).fun("SetCharacterAction", &SetCharacterAction);
}

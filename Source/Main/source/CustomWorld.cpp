#include "stdafx.h"
#include "CustomWorld.h"

CCustomWorld gCustomWorld;

CCustomWorld::CCustomWorld() : m_Lua()
{
	this->m_WorldInfo.clear();
}

CCustomWorld::~CCustomWorld()
{
	this->m_WorldInfo.clear();
}

static BOOL LuaLoadCustomWorld(lua_State* lua)
{
	CUSTOM_WORLD_INFO info;
	ZeroMemory(&info, sizeof(info));

	int top = lua_gettop(lua);
	int arg = 1;

	if (top < 6)
	{
		lua_pop(lua, top);
		return false;
	}

	info.MapIndex = (int)lua_tointeger(lua, arg++);

	if (top >= 7)
	{
		info.WorldFolder = (int)lua_tointeger(lua, arg++);
	}
	else
	{
		info.WorldFolder = info.MapIndex + 1;
	}

	info.Effect = (int)lua_tointeger(lua, arg++);
	info.TextIndex = (int)lua_tointeger(lua, arg++);
	info.TerrainExt = (int)lua_tointeger(lua, arg++);

	const char* imageName = lua_tostring(lua, arg++);
	const char* mp3Name = lua_tostring(lua, arg++);

	strcpy_s(info.ImageName, (imageName == NULL) ? "" : imageName);
	strcpy_s(info.Mp3Name, (mp3Name == NULL || mp3Name[0] == '\0') ? "None" : mp3Name);

	if (info.WorldFolder <= 0)
	{
		info.WorldFolder = info.MapIndex + 1;
	}

	gCustomWorld.m_WorldInfo[info.MapIndex] = info;

	lua_pop(lua, top);
	return true;
}

void CCustomWorld::Init()
{
	lua_register(this->m_Lua.GetState(), "LoadCustomWorld", LuaLoadCustomWorld);
	this->m_Lua.DoFile("Data//Lua//Config//CustomMaps.lua");
	this->m_Lua.Generic_Call("StartLoadCustomWorld", "");
}

void CCustomWorld::RestartLua()
{
	this->m_Lua.CloseLua();
	this->m_Lua.RegisterLua();
	this->m_WorldInfo.clear();
	this->Init();
}

CUSTOM_WORLD_INFO* CCustomWorld::GetInfo(int mapIndex)
{
	std::map<int, CUSTOM_WORLD_INFO>::iterator it = this->m_WorldInfo.find(mapIndex);
	return (it == this->m_WorldInfo.end()) ? NULL : &it->second;
}

bool CCustomWorld::HasEffect(int mapIndex, int effect)
{
	CUSTOM_WORLD_INFO* info = this->GetInfo(mapIndex);

	if (info == NULL || info->Effect == CUSTOM_WORLD_EFFECT_NONE)
	{
		return false;
	}

	return (effect == CUSTOM_WORLD_EFFECT_NONE || info->Effect == effect);
}

bool CCustomWorld::IsTerrainHeightExtMap(int mapIndex)
{
	CUSTOM_WORLD_INFO* info = this->GetInfo(mapIndex);
	return (info != NULL && info->TerrainExt != 0);
}

void CCustomWorld::UpdateMusic(int mapIndex)
{
	for (std::map<int, CUSTOM_WORLD_INFO>::iterator it = this->m_WorldInfo.begin(); it != this->m_WorldInfo.end(); ++it)
	{
		CUSTOM_WORLD_INFO& world = it->second;
		if (world.Mp3Name[0] == '\0' || _stricmp(world.Mp3Name, "None") == 0)
		{
			continue;
		}

		if (world.MapIndex == mapIndex)
		{
			PlayMp3(world.Mp3Name);
		}
		else
		{
			StopMp3(world.Mp3Name);
		}
	}
}

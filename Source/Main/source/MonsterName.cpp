#include "stdafx.h"
#include "MonsterName.h"
#include "LuaStack.hpp"
#include <MapManager.h>

using namespace luaaa;

CMonsterName gMonsterName;

CMonsterName::CMonsterName() : m_Lua()
{
	this->m_CustomMonsterNameInfo.clear();
}

CMonsterName::~CMonsterName()
{
}

void SetCustomMonsterName(int Class, int Map, int X, int Y, std::string name) 
{
	CUSTOM_MONSTER_NAME_INFO info;

	info.Class = Class;

	info.Map = Map;

	info.X = X;

	info.Y = Y;

	info.name = name;

	gMonsterName.m_CustomMonsterNameInfo.push_back(info);
}

void CMonsterName::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetCustomMonsterName", SetCustomMonsterName);

	this->m_Lua.DoFile("Data//Configs//Lua//Monster//CustomMonsterName.lua");
}

CUSTOM_MONSTER_NAME_INFO * CMonsterName::getMonster(int Class, int Map, int X, int Y)
{
	for (auto &monster : this->m_CustomMonsterNameInfo)
	{
		if (monster.Class != Class) {
			continue;
		}

		if (monster.Map != Map) {
			continue;
		}

		if ((monster.X == -1 || monster.X == X) || (monster.Y == -1 || monster.Y == Y)) {
			return &monster;
		}
	}

	return nullptr;
}

void CMonsterName::SetMonsterAttribute(CHARACTER* ObjectMonster, int MonsterID, int PositionX, int PositionY)
{
	auto monster = gMonsterName.getMonster(MonsterID, gMapManager.WorldActive, PositionX, PositionY);

	if (monster) 
	{
		sprintf_s(ObjectMonster->ID, monster->name.c_str());
	}
}

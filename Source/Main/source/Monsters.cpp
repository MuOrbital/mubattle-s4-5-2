#include "stdafx.h"
#include "Monsters.h"
#include "LuaStack.hpp"
#include "LoadData.h"
#include "ZzzOpenData.h"
#include "ZzzCharacter.h"
#include "MapManager.h"

using namespace luaaa;

Monsters gMonsters;

Monsters::Monsters() : m_Lua()
{
	this->m_CustomMonsterInfo.clear();
}

Monsters::~Monsters() // OK
{
	this->m_CustomMonsterInfo.clear();
}

bool Monsters::CheckCustomMonster(int MonsterID)
{
	for (std::map<int, CUSTOM_MONSTER_INFO>::iterator it = this->m_CustomMonsterInfo.begin(); it != this->m_CustomMonsterInfo.end(); ++it)
	{
		if (it != this->m_CustomMonsterInfo.end())
		{
			if (MonsterID = (it->second.MonsterID - 574))
			{
				return true;
			}
		}
	}

	return false;
}

void SetMonsterInfo(int MonsterID, int MonsterType)
{
	CUSTOM_MONSTER_INFO info;

	info.MonsterID = MonsterID;

	info.Type = MonsterType;

	gMonsters.m_CustomMonsterInfo.insert(std::make_pair(MonsterID, info));
}

int getMapNumber()
{
	return gMapManager.WorldActive;
}

CHARACTER* Monsters::CreateMonster(int Type, int PositionX, int PositionY, int Key)
{
	CHARACTER* c = NULL;
	int returnValue = 0;

	DWORD ObjectClass = 0;

	gMonsters.m_Lua.Generic_Call("CreateMonster", "iiii>ij", Type, PositionX, PositionY, Key, &returnValue, &ObjectClass);

	if (returnValue == 1)
	{
		return (CHARACTER*)ObjectClass;
	}

	return c;
}

void LoadMonster(int MonsterID, int Type, std::string Dir, std::string ModelName)
{
	gMonsters.LoadMonsterModel(MonsterID + 574, (char*)Dir.c_str(), (char*)ModelName.c_str(), -1);

	gMonsters.LoadMonsterTexture(MonsterID + 574, (char*)Dir.c_str());

	int Index = 574 + MonsterID;

	BMD* BMDStruct = &Models[Index];

	if (BMDStruct != 0)
	{
		for (int m = 0; ; ++m)
		{
			if (m >= BMDStruct->NumActions)
			{
				break;
			}

			BMDStruct->Actions[m].PlaySpeed = 0.25;
		}
	}

}

extern int HeroIndex;

DWORD CreateMonsterStruct(int Key, int MonsterID, int PosX, int PosY, int MonsterType, float Size, std::string name)
{
	auto Object = CreateCharacter(Key, MonsterID, PosX, PosY);

	if (Object)
	{
		std::memcpy(Object->ID, name.c_str(), sizeof(Object->ID));

		switch (MonsterType)
		{
		case 0:
			Object->Object.Kind = 4;
			break;
		case 1:
			Object->Object.Kind = 2;
			break;
		default:
			Object->Object.Kind = 8;
			break;
		}

		Object->Object.Scale = Size;

		Object->MonsterIndex = MonsterID;

		Object->Object.ExtState = 0;

		Object->TargetCharacter = HeroIndex;

		return (DWORD)(Object);
	}

	return 0;
}


void LoadItemModel(int index, char* folder, char* name, int value) // OK
{
	if (name[0] == 0)
	{
		return;
	}

	char path[MAX_PATH] = { 0 };

	wsprintf(path, "Data\\%s", folder);

	gLoadData.AccessModel(index, path, name, value);
}

void LoadItemTexture(int index, char* folder, char* name, int value) // OK
{
	if (name[0] == 0)
	{
		return;
	}

	gLoadData.OpenTexture(index, folder);
}

void Monsters::LoadMonsterModel(int ModelID, char* a2, char* a3, int a4)
{
	int returnValue = 0;

	gMonsters.m_Lua.Generic_Call("LooadModelMonster", "i>i", ModelID, &returnValue);

	if (returnValue == 1)
	{
		return;
	}

	gLoadData.AccessModel(ModelID, a2, a3, a4);
}

void Monsters::LoadMonsterTexture(int ModelID, char* a2)
{
	int returnValue = 0;

	gMonsters.m_Lua.Generic_Call("LooadTextureMonster", "i>i", ModelID, &returnValue);

	if (returnValue == 1)
	{
		return;
	}

	gLoadData.OpenTexture(ModelID, a2);
}

bool Monsters::MonsterType(int MonsterIndex, int Type)
{
	for (std::map<int, CUSTOM_MONSTER_INFO>::iterator it = this->m_CustomMonsterInfo.begin(); it != this->m_CustomMonsterInfo.end(); ++it)
	{

		if (it == this->m_CustomMonsterInfo.end())
		{
			return false;
		}

		if (MonsterIndex == (it->second.MonsterID - 574) && Type == it->second.Type)
		{
			return true;
		}
	}

	return false;
}

void Monsters::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetMonsterInfo", SetMonsterInfo);

	this->m_Lua.DoFile("Data//Lua//Monster//CustomMonster.lua");

	LuaModule(this->m_Lua.GetState()).fun("LoadMonster", LoadMonster);
	LuaModule(this->m_Lua.GetState()).fun("CreateMonsterStruct", CreateMonsterStruct);
	LuaModule(this->m_Lua.GetState()).fun("LoadModel", LoadItemModel);
	LuaModule(this->m_Lua.GetState()).fun("LoadTexture", LoadItemTexture);
	LuaModule(this->m_Lua.GetState()).fun("getMapNumber", getMapNumber);
}

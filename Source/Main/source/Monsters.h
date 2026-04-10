#pragma once

#include "Lua.h"

struct CUSTOM_MONSTER_INFO
{
	int MonsterID;
	int Type;
};

class Monsters
{
public:
	Monsters();
	~Monsters();
	void Init();
	bool CheckCustomMonster(int MonsterID);
	CHARACTER* CreateMonster(int Type, int PositionX, int PositionY, int Key);
	void LoadMonsterModel(int ModelID, char* a2, char* a3, int a4);
	void LoadMonsterTexture(int ModelID, char* a2);
	bool MonsterType(int MonsterIndex, int Type);
public:
	std::map<int, CUSTOM_MONSTER_INFO> m_CustomMonsterInfo;
private:
	Lua m_Lua;
};

extern Monsters gMonsters;
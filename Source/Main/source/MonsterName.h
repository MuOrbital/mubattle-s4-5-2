#pragma once

#include "Lua.h"
#include "ZzzBMD.h"

struct CUSTOM_MONSTER_NAME_INFO {
	int Class;
	int Map;
	int X;
	int Y;
	std::string name;
};

class CMonsterName {
public:
	CMonsterName();
	~CMonsterName();

	void Init();
	CUSTOM_MONSTER_NAME_INFO * getMonster(int Class, int Map, int X, int Y);
	static void SetMonsterAttribute(CHARACTER* ObjectMonster, int MonsterID, int PositionX, int PositionY);

public:
	Lua m_Lua;

	std::vector<CUSTOM_MONSTER_NAME_INFO> m_CustomMonsterNameInfo;
};

extern CMonsterName gMonsterName;
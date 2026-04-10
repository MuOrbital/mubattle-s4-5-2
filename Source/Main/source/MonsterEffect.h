#pragma once

#include "Lua.h"
#include "ZzzBMD.h"

struct CUSTOM_MONSTER_EFFECT_INFO
{
	int Type;
	int EffectID;
	int EffectLv;
	int Bone;
	float Size;
	float R;
	float G;
	float B;
	float Black;
	int Rand;
};

class CUSTOM_MONSTER_EFFECT
{
public:
	CUSTOM_MONSTER_EFFECT()
	{
		this->m_Effects.clear();
	};

	~CUSTOM_MONSTER_EFFECT()
	{
		this->m_Effects.clear();
	};

	int MonsterID;
	std::vector<CUSTOM_MONSTER_EFFECT_INFO> m_Effects;
};

class CMonsterEffect
{
public:
	CMonsterEffect();
	~CMonsterEffect();

	void Init();
	CUSTOM_MONSTER_EFFECT* getMonsterEffect(int Type);
	void CreateEffectMonster(OBJECT* ObjectStruct, BMD* pModel);

public:
	std::map<int, CUSTOM_MONSTER_EFFECT> m_MonsterEffect;

private:
	Lua m_Lua;
};

extern CMonsterEffect gMonsterEffect;
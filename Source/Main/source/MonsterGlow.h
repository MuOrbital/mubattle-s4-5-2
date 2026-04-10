#pragma once

#include "Lua.h"
#include "ZzzBMD.h"

struct CUSTOM_MONSTER_GLOW_INFO 
{
	int MonsterID;
	int Layer;
	int MeshType;
	int R;
	int G;
	int B;
};

class CMonsterGlow 
{
public:
	CMonsterGlow();

	~CMonsterGlow();

	void Init();
	CUSTOM_MONSTER_GLOW_INFO* getMonsterGlow(int Type);

	static bool RenderMonsterCustomObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon);

public:
	std::map<int, CUSTOM_MONSTER_GLOW_INFO> m_CustomMonsterGlow;

private:
	Lua m_Lua;
};

extern CMonsterGlow gMonsterGlow;
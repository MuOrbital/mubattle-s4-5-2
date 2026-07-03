#pragma once

#include "Lua.h"

enum CUSTOM_WORLD_EFFECT
{
	CUSTOM_WORLD_EFFECT_NONE = -1,
	CUSTOM_WORLD_EFFECT_ACHERON = 91,
	CUSTOM_WORLD_EFFECT_DEVENTER = 95,
	CUSTOM_WORLD_EFFECT_URUK_MOUNTAIN = 100,
	CUSTOM_WORLD_EFFECT_NARS = 110,
	CUSTOM_WORLD_EFFECT_FEREA = 112,
	CUSTOM_WORLD_EFFECT_NIXIES_LAKE = 113,
};

struct CUSTOM_WORLD_INFO
{
	int MapIndex;
	int WorldFolder;
	int Effect;
	int TextIndex;
	int TerrainExt;
	char ImageName[32];
	char Mp3Name[MAX_PATH];
};

class CCustomWorld
{
public:
	CCustomWorld();
	~CCustomWorld();

	void Init();
	void RestartLua();
	CUSTOM_WORLD_INFO* GetInfo(int mapIndex);
	bool HasEffect(int mapIndex, int effect = CUSTOM_WORLD_EFFECT_NONE);
	bool IsTerrainHeightExtMap(int mapIndex);
	void UpdateMusic(int mapIndex);

public:
	std::map<int, CUSTOM_WORLD_INFO> m_WorldInfo;

private:
	Lua m_Lua;
};

extern CCustomWorld gCustomWorld;

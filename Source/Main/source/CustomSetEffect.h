#pragma once

#include "Lua.h"

class CCustomSetEffect {
public:
	CCustomSetEffect();
	~CCustomSetEffect();
	void CreateEffectSetPlayer(DWORD ObjectStruct, DWORD ItemIndex);

	void Init();

public:
	Lua m_Lua;
};

extern CCustomSetEffect gCCustomSetEffect;
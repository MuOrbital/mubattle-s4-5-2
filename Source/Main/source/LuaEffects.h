#pragma once

#include "Lua.h"
#include "ZzzBMD.h"

void FuncCreateParticle(DWORD BMDStruct, int Bitmap, int SubType, int Link, float Scale, vec3_t Light, DWORD ObjectStruct);

class CLuaEffects {
public:
	CLuaEffects();
	~CLuaEffects();

	void RegisterLuaClass(lua_State* lua);
public:
	BMD* m_Struct;
};
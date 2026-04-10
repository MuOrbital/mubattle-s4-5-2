#pragma once
#pragma once

#include <set>
#include "Lua.h"
#include "PhysicsManager.h"

class ClothClass {
public:
	ClothClass() {
	};

	ClothClass(DWORD Struct) {
		this->m_Struct = reinterpret_cast<CPhysicsCloth*>(Struct);
	};

	~ClothClass() {

	};

	void Create(int numCloth, DWORD ObjectStruct, int iBone, float fxPos, float fyPos, float fzPos, int iNumHor, int iNumVer, float fWidth, float fHeight, int iTexFront, int iTexBack, DWORD dwType)
	{
		OBJECT* o = (OBJECT*)ObjectStruct;
		this->m_Struct[numCloth].Create(o, iBone, fxPos, fyPos, fzPos, iNumHor, iNumVer, fWidth, fHeight, iTexFront, iTexBack, dwType);
	}

	void SetWindMax(int numCloth, int min, int max)
	{
		this->m_Struct[numCloth].SetWindMinMax(min, max);
	}


	void AddCollisionSphere(int numCloth, float fXPos, float fYPos, float fZPos, float fRadius, int iBone)
	{
		this->m_Struct[numCloth].AddCollisionSphere(fXPos, fYPos, fZPos, fRadius, iBone);
	}

public:
	CPhysicsCloth* m_Struct;
};

class LuaCloth {
public:
	LuaCloth();
	~LuaCloth();

	void RegisterClassCloth(lua_State* lua);
};
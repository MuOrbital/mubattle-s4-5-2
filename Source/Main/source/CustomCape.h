#pragma once

#include "Lua.h"
#include "LuaLoadImage.h"
#include "LuaBMD.h"
#include "PhysicsManager.h"
#include "LuaCloth.h"

class CCustomCape
{
public:
	CCustomCape();
	~CCustomCape();

	void Init();
	void RestartLua();
	void LoadImageCape();
	void CreateCape(CPhysicsCloth* pCloth, CHARACTER* character, WORD ItemIndex, BYTE Class);
	void RenderModel(BMD* BMDStruct, OBJECT* Object, DWORD ItemIndex);
	void CapeModelPosition(DWORD ItemIndex, float& posX, float& posY, float& posZ, float& matrixX, float& matrixY, float& matrixZ);

public:
	Lua m_Lua;

private:
	LuaLoadImage m_LuaLoadImage;
	LuaBMD m_LuaBMD;
	LuaCloth m_LuaCloth;
};

extern CCustomCape gCustomCape;

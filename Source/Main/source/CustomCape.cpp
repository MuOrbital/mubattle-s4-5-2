#include "stdafx.h"
#include "CustomCape.h"
#include "CustomWing.h"
#include "LuaStack.hpp"
#include "LuaBMD.h"
#include "LuaObject.h"
#include "Console.h"
#include "PhysicsManager.h"

using namespace luaaa;

CCustomCape gCustomCape;

CCustomCape::CCustomCape() : m_Lua()
{
}

CCustomCape::~CCustomCape()
{
}

void CCustomCape::RenderModel(BMD* b, OBJECT* o, DWORD ItemIndex)
{
	int renderCharacter = 1;

	if (b->BodyLight[0] == 1.0 && b->BodyLight[1] == 1.0 && b->BodyLight[2] == 1.0)
	{
		renderCharacter = 0;
	}

	gCustomCape.m_Lua.Generic_Call("RenderCapeModel", "iiii>", (DWORD)b, (DWORD)o, ItemIndex, renderCharacter);
}

void CCustomCape::CapeModelPosition(DWORD ItemIndex, float& posX, float& posY, float& posZ, float& matrixX, float& matrixY, float& matrixZ)
{
	float m_posX = 0.0;
	float m_posY = 0.0;
	float m_posZ = 0.0;
	float m_matrixX = 0.0;
	float m_matrixY = 0.0;
	float m_matrixZ = 0.0;

	gCustomCape.m_Lua.Generic_Call("CapeModelPosition", "i>ffffff", ItemIndex, &m_posX, &m_posY, &m_posZ, &m_matrixX, &m_matrixY, &m_matrixZ);

	posX = m_posX;
	posY = m_posY;
	posZ = m_posZ;
	matrixX = m_matrixX;
	matrixY = m_matrixY;
	matrixZ = m_matrixZ;
}

void CCustomCape::Init()
{
	this->m_LuaCloth.RegisterClassCloth(this->m_Lua.GetState());

	this->m_LuaLoadImage.SetFunctions(this->m_Lua.GetState());

	this->m_Lua.DoFile("Data//Lua//ItemManager//CharacterCreateCape.lua");

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());
}

void CCustomCape::RestartLua()
{
	this->m_Lua.CloseLua();
	this->m_Lua.RegisterLua();
	this->Init();
}

void CCustomCape::LoadImageCape()
{
	this->m_Lua.Generic_Call("LoadImageCape", ">");
}

void CCustomCape::CreateCape(CPhysicsCloth* pCloth, CHARACTER* character, WORD ItemIndex, BYTE Class)
{
	this->m_Lua.Generic_Call("CreateCapePlayer", "iiii>", (DWORD)pCloth, (DWORD)&character->Object, ItemIndex, Class);
}

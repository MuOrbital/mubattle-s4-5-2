#include "stdafx.h"
#include "MonsterGlow.h"
//#include "CustomMonsterEffect.h"
#include "Console.h"
#include "LuaStack.hpp"
#include "GMAida.h"

using namespace luaaa;

CMonsterGlow gMonsterGlow;

CMonsterGlow::CMonsterGlow() : m_Lua()
{
	this->m_CustomMonsterGlow.clear();
}

CMonsterGlow::~CMonsterGlow()
{
	this->m_CustomMonsterGlow.clear();
}

void SetMonsterGlow(int MonsterID, int Layer, int MeshType, int R, int G, int B) 
{
	CUSTOM_MONSTER_GLOW_INFO info;

	info.MonsterID = MonsterID;

	info.Layer = Layer;

	info.MeshType = MeshType;

	info.R = R;

	info.G = G;

	info.B = B;

	gMonsterGlow.m_CustomMonsterGlow.insert(std::make_pair(info.MonsterID, info));
}

void CMonsterGlow::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetMonsterGlow", SetMonsterGlow);

	this->m_Lua.DoFile("Data//Configs//Lua//Monster//CustomMonsterGlow.lua");
}

CUSTOM_MONSTER_GLOW_INFO* CMonsterGlow::getMonsterGlow(int Type)
{
	auto it = this->m_CustomMonsterGlow.find(Type);

	if (it == this->m_CustomMonsterGlow.end()) 
	{
		return nullptr;
	}

	return &it->second;
}

bool CMonsterGlow::RenderMonsterCustomObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	auto monsterGlow = gMonsterGlow.getMonsterGlow(pObject->Type - 574);
	
	if (monsterGlow) 
	{
		pModel->BeginRender(1.f);


		pModel->BodyLight[0] = (float)(monsterGlow->R / 255.0f);

		pModel->BodyLight[1] = (float)(monsterGlow->G / 255.0f);

		pModel->BodyLight[2] = (float)(monsterGlow->B / 255.0f);

		for (int n = 0; n < 8; n++)
		{
			pModel->RenderMesh(n, 2, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
		}
	
		pModel->RenderMesh(monsterGlow->Layer, monsterGlow->MeshType, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
	
		pModel->EndRender();

		pModel->StreamMesh = -1;
	
		return true;
	}
	
	return M33Aida::RenderAidaMonsterObjectMesh(pObject, pModel, ExtraMon);
}

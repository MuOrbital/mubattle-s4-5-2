#include "stdafx.h"
#include "CustomSetEffect.h"
#include "LuaStack.hpp"
#include "ZzzCharacter.h"
#include <ZzzEffect.h>

using namespace luaaa;

CCustomSetEffect gCCustomSetEffect;

CCustomSetEffect::CCustomSetEffect() : m_Lua()
{

}

CCustomSetEffect::~CCustomSetEffect()
{

}

void CCustomSetEffect::CreateEffectSetPlayer(DWORD ObjectStruct, DWORD ItemIndex)
{
	BMD* BMDStruct = &Models[ItemIndex];

	if (BMDStruct)
	{
		gCCustomSetEffect.m_Lua.Generic_Call("CreateEffectSetPlayer", "jjii>", BMDStruct, ObjectStruct, ItemIndex, EquipmentLevelSet);
	}
}

void CreateSetEffectSprite(DWORD BMDStruct, DWORD ObjectStruct, int BonePlayer, int EffectID, float Scale, float LightR, float LightB, float LightG, float PosX, float PosY, float PosZ)
{
	if (BMDStruct == 0 || ObjectStruct == 0)
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;
	auto b = (BMD*)BMDStruct;

	if (Object != nullptr)
	{
		vec3_t p, Position;
		vec3_t Light;

		Vector(LightR, LightG, LightB, Light);
		Vector(PosX, PosY, PosZ, p);

		vec34_t* BoneTransform = &*(vec34_t*)(Object->BoneTransform[BonePlayer]);

		if (BoneTransform != 0)
		{
			b->TransformPosition(BoneTransform[0], p, Position, 1);

			CreateSprite(EffectID, Position, Scale, Light, (OBJECT*)ObjectStruct, 0.0, 0);
		}
	}
}

void CreateSetEffectParticle(DWORD BMDStruct, DWORD ObjectStruct, int BonePlayer, int EffectID, int EffectLv, float Scale, float LightR, float LightB, float LightG, float PosX, float PosY, float PosZ)
{
	if (BMDStruct == 0 || ObjectStruct == 0)
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;
	auto b = (BMD*)BMDStruct;

	if (Object != nullptr)
	{
		vec3_t p, Position;
		vec3_t Light;

		Vector(LightR, LightG, LightB, Light);
		Vector(PosX, PosY, PosZ, p);

		vec34_t* BoneTransform = &*(vec34_t*)(Object->BoneTransform[BonePlayer]);

		if (BoneTransform != 0)
		{
			b->TransformPosition(BoneTransform[0], p, Position, 1);

			CreateParticleFpsChecked(EffectID, Position, Object->Angle, Light, EffectLv, Scale, (OBJECT*)ObjectStruct);
		}
	}
}

void CreateSetEffectSkill(DWORD BMDStruct, DWORD ObjectStruct, int BonePlayer, int EffectID, int EffectLv, float Scale, float LightR, float LightB, float LightG, float PosX, float PosY, float PosZ)
{
	if (BMDStruct == 0 || ObjectStruct == 0)
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;
	auto b = (BMD*)BMDStruct;

	if (Object != nullptr)
	{
		vec3_t p, Position;
		vec3_t Light;

		Vector(LightR, LightG, LightB, Light);
		Vector(PosX, PosY, PosZ, p);

		vec34_t* BoneTransform = &*(vec34_t*)(Object->BoneTransform[BonePlayer]);

		if (BoneTransform != 0)
		{
			b->TransformPosition(BoneTransform[0], p, Position, 1);

			CreateParticleFpsChecked(EffectID, Position, Object->Angle, Light, EffectLv, Scale, (OBJECT*)ObjectStruct);

			CreateEffectFpsChecked(EffectID, Position, Object->Angle, Light, EffectLv, Object, -1, 0, 0, 0, 0.0, -1);
		}
	}
}

void CCustomSetEffect::Init()
{
	this->m_Lua.DoFile("Data//Lua//EffectSystem//CharacterSetEffect.lua");

	LuaModule(this->m_Lua.GetState()).fun("CreateSprite", &CreateSetEffectSprite);

	LuaModule(this->m_Lua.GetState()).fun("CreateParticle", &CreateSetEffectParticle);

	LuaModule(this->m_Lua.GetState()).fun("CreateSkill", &CreateSetEffectSkill);
}

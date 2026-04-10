#include "stdafx.h"
#include "LuaEffects.h"
#include "LuaStack.hpp"
#include "Console.h"
#include "ZzzBMD.h"
#include "ZzzEffect.h"

using namespace luaaa;

void FuncCreateParticle(DWORD BMDStruct, int Bitmap, int SubType, int Link, float Scale, vec3_t Light, DWORD ObjectStruct) 
{
	if (BMDStruct == 0 || ObjectStruct == 0) 
	{
		return;
	}

	vec3_t Position, p;

	p[0] = 0.f; p[1] = 0.f; p[2] = 0.f;

	auto Object = &*(OBJECT*)ObjectStruct;

	BMD* b = &*(BMD*)BMDStruct;

	if (Object != nullptr) 
	{
		if (Object->Type >= gItemManager->GetItemModel(13, 150) && Object->Type <= gItemManager->GetItemModel(13, 500) || Object->Type >= gItemManager->GetItemModel(14, 150) && Object->Type <= gItemManager->GetItemModel(16, 0))
		{
			b->TransformPosition(BoneTransform[Link], p, Position, 0);
		}
		else if (Object->Type >= gItemManager->GetItemModel(16, 250) && Object->Type <= gItemManager->GetItemModel(17, 0))
		{
			b->TransformPosition(Object->BoneTransform[Link], p, Position, 0);
		}
		else 
		{
			b->TransformPosition(BoneTransform[Link], p, Position, 1);
		}
	}
	else 
	{
		b->TransformPosition(BoneTransform[Link], p, Position, 1);
	}

	CreateParticle(Bitmap, Position, Object->Angle, Light, SubType, Scale, (OBJECT*)ObjectStruct);
}

void FuncCreateSprite(DWORD BMDStruct, int Bitmap, int Link, float Scale, vec3_t Light, DWORD ObjectStruct) 
{
	if (BMDStruct == 0 || ObjectStruct == 0) 
	{
		return;
	}

	vec3_t Position, p;

	p[0] = 0.f; p[1] = 0.f; p[2] = 0.f;

	auto Object = &*(OBJECT*)ObjectStruct;

	BMD* b = &*(BMD*)BMDStruct;

	if (Object != nullptr) 
	{
		if (Object->Type >= gItemManager->GetItemModel(13, 150) && Object->Type <= gItemManager->GetItemModel(13, 500) || Object->Type >= gItemManager->GetItemModel(14, 150) && Object->Type <= gItemManager->GetItemModel(16, 0))
		{
			b->TransformPosition(BoneTransform[Link], p, Position, 0);
		}
		else if (Object->Type >= gItemManager->GetItemModel(16, 250) && Object->Type <= gItemManager->GetItemModel(17, 0))
		{
			b->TransformPosition(Object->BoneTransform[Link], p, Position, 0);
		}
		else 
		{
			b->TransformPosition(BoneTransform[Link], p, Position, 1);
		}
	}
	else 
	{
		b->TransformPosition(BoneTransform[Link], p, Position, 1);
	}

	CreateSprite(Bitmap, Position, Scale, Light, (OBJECT*)ObjectStruct, 0.0, 0);
}

void CreateSprites(DWORD BMDStruct, int Bitmap, int Link, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct) 
{
	vec3_t Light;

	Vector(LightR, LightG, LightB, Light);

	FuncCreateSprite(BMDStruct, Bitmap, Link, Scale, Light, ObjectStruct);
}

void CreateParticles(DWORD BMDStruct, int Bitmap, int SubType, int Link, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct) 
{
	vec3_t Light;

	Vector(LightR, LightG, LightB, Light);

	FuncCreateParticle(BMDStruct, Bitmap, SubType, Link, Scale, Light, ObjectStruct);
}

CLuaEffects::CLuaEffects()
{
}

CLuaEffects::~CLuaEffects()
{
}

void CLuaEffects::RegisterLuaClass(lua_State * lua)
{
	LuaModule(lua).fun("CreateSprite", &CreateSprites);
	LuaModule(lua).fun("CreateParticle", &CreateParticles);
}

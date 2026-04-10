#include "stdafx.h"
#include "CustomEffects.h"
#include "ZzzBMD.h"

CCustomEffects gCustomEffects;

CCustomEffects::CCustomEffects() : m_Lua()
{

}

CCustomEffects::~CCustomEffects()
{

}

void CCustomEffects::Init()
{
	this->m_Lua.DoFile("Data//Lua//EffectSystem//CharacterEffectItens.lua");

	this->m_LuaEffects.RegisterLuaClass(this->m_Lua.GetState());
}


void CCustomEffects::SetInfo(DWORD ObjectStruct, void* BMDStruct, int Type)
{
	gCustomEffects.m_Lua.Generic_Call("CharacterItensEffect", "jjj>", ObjectStruct, BMDStruct, Type);
}
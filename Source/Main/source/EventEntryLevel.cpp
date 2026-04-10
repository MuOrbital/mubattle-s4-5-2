#pragma once
#include "stdafx.h"
#include "EventEntryLevel.h"
#include "LuaStack.hpp"

using namespace luaaa;

CEventEntryLevel gEventEntryLevel;

CEventEntryLevel::CEventEntryLevel() : m_Lua()
{
	for (int i = 0; i < 7; i++) {
		for (int r = 0; r < 2; r++) {
			this->m_BloodCastleEntryLevelCommon[i][r] = 0;
			this->m_BloodCastleEntryLevelSpecial[i][r] = 0;
			this->m_DevilSquareEntryLevelCommon[i][r] = 0;
			this->m_DevilSquareEntryLevelSpecial[i][r] = 0;
			this->m_ChaosCastleEntryLevelCommon[i][r] = 0;
			this->m_ChaosCastleEntryLevelSpecial[i][r] = 0;
		}
	}
	for (int i = 0; i < 5; i++) {
		this->m_IllusionTempleEntryLevelMin[i] = 0;
		this->m_IllusionTempleEntryLevelMax[i] = 0;
	}
}

void SetBloodCastleInfo(int Index, int CommonMinLevel, int CommonMaxLevel, int SpecialMinLevel, int SpecialMaxLevel)
{
	gEventEntryLevel.m_BloodCastleEntryLevelCommon[Index][0] = CommonMinLevel;
	gEventEntryLevel.m_BloodCastleEntryLevelCommon[Index][1] = CommonMaxLevel;
	gEventEntryLevel.m_BloodCastleEntryLevelSpecial[Index][0] = SpecialMinLevel;
	gEventEntryLevel.m_BloodCastleEntryLevelSpecial[Index][1] = SpecialMaxLevel;
}

void SetDevilSquareInfo(int Index, int CommonMinLevel, int CommonMaxLevel, int SpecialMinLevel, int SpecialMaxLevel)
{
	gEventEntryLevel.m_DevilSquareEntryLevelCommon[Index][0] = CommonMinLevel;
	gEventEntryLevel.m_DevilSquareEntryLevelCommon[Index][1] = CommonMaxLevel;
	gEventEntryLevel.m_DevilSquareEntryLevelSpecial[Index][0] = SpecialMinLevel;
	gEventEntryLevel.m_DevilSquareEntryLevelSpecial[Index][1] = SpecialMaxLevel;
}

void SetChaosCastleInfo(int Index, int CommonMinLevel, int CommonMaxLevel, int SpecialMinLevel, int SpecialMaxLevel)
{
	gEventEntryLevel.m_ChaosCastleEntryLevelCommon[Index][0] = CommonMinLevel;
	gEventEntryLevel.m_ChaosCastleEntryLevelCommon[Index][1] = CommonMaxLevel;
	gEventEntryLevel.m_ChaosCastleEntryLevelSpecial[Index][0] = SpecialMinLevel;
	gEventEntryLevel.m_ChaosCastleEntryLevelSpecial[Index][1] = SpecialMaxLevel;
}

void SetIllusionTempleInfo(int Index, int CommonMinLevel, int CommonMaxLevel)
{
	gEventEntryLevel.m_IllusionTempleEntryLevelMin[Index] = CommonMinLevel;
	gEventEntryLevel.m_IllusionTempleEntryLevelMax[Index] = CommonMaxLevel;
}

void CEventEntryLevel::Init()
{

	LuaModule(this->m_Lua.GetState()).fun("SetBloodCastleInfo", &SetBloodCastleInfo);
	LuaModule(this->m_Lua.GetState()).fun("SetDevilSquareInfo", &SetDevilSquareInfo);
	LuaModule(this->m_Lua.GetState()).fun("SetChaosCastleInfo", &SetChaosCastleInfo);
	LuaModule(this->m_Lua.GetState()).fun("SetIllusionTempleInfo", &SetIllusionTempleInfo);

	this->m_Lua.DoFile("Data//Lua//Config//EventEntryLevel.lua");

	this->m_Lua.Generic_Call("LoadEventEntryLevel", "i>");

}
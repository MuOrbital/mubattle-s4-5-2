// CustomWing.cpp: implementation of the CCustomWing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomWing.h"

CCustomWing gCustomWing;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomWing::CCustomWing() : m_Lua() // OK
{
	this->m_CustomWingInfo.clear();
}

CCustomWing::~CCustomWing() // OK
{
	this->m_CustomWingInfo.clear();
}

static BOOL LuaLoadWing(lua_State* Lua)
{
	CUSTOM_WING_INFO info;

	info.ItemIndex = (int)lua_tointeger(Lua, 1);

	info.DefenseConstA = (int)lua_tointeger(Lua, 2);

	info.IncDamageConstA = (int)lua_tointeger(Lua, 3);

	info.IncDamageConstB = (int)lua_tointeger(Lua, 4);

	info.DecDamageConstA = (int)lua_tointeger(Lua, 5);

	info.DecDamageConstB = (int)lua_tointeger(Lua, 6);

	info.OptionIndex1 = (int)lua_tointeger(Lua, 7);

	info.OptionValue1 = (int)lua_tointeger(Lua, 8);

	info.OptionIndex2 = (int)lua_tointeger(Lua, 9);

	info.OptionValue2 = (int)lua_tointeger(Lua, 10);

	info.OptionIndex3 = (int)lua_tointeger(Lua, 11);

	info.OptionValue3 = (int)lua_tointeger(Lua, 12);

	info.NewOptionIndex1 = (int)lua_tointeger(Lua, 13);

	info.NewOptionValue1 = (int)lua_tointeger(Lua, 14);

	info.NewOptionIndex2 = (int)lua_tointeger(Lua, 15);

	info.NewOptionValue2 = (int)lua_tointeger(Lua, 16);

	info.NewOptionIndex3 = (int)lua_tointeger(Lua, 17);

	info.NewOptionValue3 = (int)lua_tointeger(Lua, 18);

	info.NewOptionIndex4 = (int)lua_tointeger(Lua, 19);

	info.NewOptionValue4 = (int)lua_tointeger(Lua, 20);

	info.ModelType = (int)lua_tointeger(Lua, 21);

	info.IsCape = (int)lua_tointeger(Lua, 22);

	strcpy_s(info.ModelName, lua_tostring(Lua, 23));

	lua_pop(Lua, 23);

	gCustomWing.m_CustomWingInfo.insert(std::make_pair(info.ItemIndex, info));
	return true;
};

void CCustomWing::Init() // OK
{
	lua_register(m_Lua.GetState(), "LoadWing", LuaLoadWing);

	this->m_Lua.DoFile("Data//Lua//ItemManager//CustomWings.lua");

	m_Lua.Generic_Call("StartLoadWings", "");
}

void CCustomWing::RestartLua() // OK
{
	this->m_Lua.CloseLua();
	this->m_Lua.RegisterLua();
	this->m_CustomWingInfo.clear();

	lua_register(m_Lua.GetState(), "LoadWing", LuaLoadWing);

	this->m_Lua.DoFile("Data//Lua//ItemManager//CustomWings.lua");

	m_Lua.Generic_Call("StartLoadWings", "");
}

CUSTOM_WING_INFO* CCustomWing::GetInfoByItem(int ItemIndex) // OK
{
	std::map<int, CUSTOM_WING_INFO>::iterator it = gCustomWing.m_CustomWingInfo.find(ItemIndex);

	if (it == gCustomWing.m_CustomWingInfo.end()) {
		return 0;
	}

	return &it->second;
}

BOOL CCustomWing::CheckCustomWingByItem(int ItemIndex) // OK
{
	if (this->GetInfoByItem(ItemIndex) != 0)
	{
		return 1;
	}

	return 0;
}

BOOL CCustomWing::CheckCustomWingByModelType(int ItemIndex, int ModelType) // OK
{
	for (std::map<int, CUSTOM_WING_INFO>::iterator it = this->m_CustomWingInfo.begin(); it != this->m_CustomWingInfo.end(); it++)
	{
		auto lpInfo = &(*it).second;

		if (lpInfo->ItemIndex == ItemIndex && lpInfo->ModelType == ModelType)
		{
			return 1;
		}
	}

	return 0;
}

int CCustomWing::GetCustomWingDefense(int ItemIndex, int ItemLevel) // OK
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->DefenseConstA * ItemLevel);
}

int CCustomWing::GetCustomWingIncDamage(int ItemIndex, int ItemLevel) // OK
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->IncDamageConstA + (ItemLevel * lpInfo->IncDamageConstB));
}

int CCustomWing::GetCustomWingDecDamage(int ItemIndex, int ItemLevel) // OK
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->DecDamageConstA - (ItemLevel * lpInfo->DecDamageConstB));
}

int CCustomWing::GetCustomWingOptionIndex(int ItemIndex, int OptionNumber) // OK
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	switch (OptionNumber)
	{
	case 0:
		return lpInfo->OptionIndex1;
	case 1:
		return lpInfo->OptionIndex2;
	case 2:
		return lpInfo->OptionIndex3;
	}

	return 0;
}

int CCustomWing::GetCustomWingOptionValue(int ItemIndex, int OptionNumber) // OK
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	switch (OptionNumber)
	{
	case 0:
		return lpInfo->OptionValue1;
	case 1:
		return lpInfo->OptionValue2;
	case 2:
		return lpInfo->OptionValue3;
	}

	return 0;
}

int CCustomWing::GetCustomWingNewOptionIndex(int ItemIndex, int OptionNumber) // OK
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	switch (OptionNumber)
	{
	case 0:
		return lpInfo->NewOptionIndex1;
	case 1:
		return lpInfo->NewOptionIndex2;
	case 2:
		return lpInfo->NewOptionIndex3;
	case 3:
		return lpInfo->NewOptionIndex4;
	}

	return 0;
}

int CCustomWing::GetCustomWingNewOptionValue(int ItemIndex, int OptionNumber) // OK
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	switch (OptionNumber)
	{
	case 0:
		return lpInfo->NewOptionValue1;
	case 1:
		return lpInfo->NewOptionValue2;
	case 2:
		return lpInfo->NewOptionValue3;
	case 3:
		return lpInfo->NewOptionValue4;
	}

	return 0;
}

int CCustomWing::CheckCustomWingIsCape(int ItemIndex)
{
	if (ItemIndex < gItemManager->GET_ITEM_MODEL(12, 0) || ItemIndex >= gItemManager->GET_ITEM_MODEL(13, 0))
	{
		return 0;
	}

	std::map<int, CUSTOM_WING_INFO>::iterator it = gCustomWing.m_CustomWingInfo.find(ItemIndex - MODEL_ITEM);

	if (it == gCustomWing.m_CustomWingInfo.end())
	{
		return 0;
	}

	return it->second.IsCape;
}
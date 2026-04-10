#include "stdafx.h"
#include "ItemPosition.h"
#include "LuaStack.hpp"

using namespace luaaa;

CCustomItemPosition gCustomItemPosition;

CCustomItemPosition::CCustomItemPosition() : m_Lua()
{
	this->m_CustomItemPositionInfo.clear();
}

CCustomItemPosition::~CCustomItemPosition()
{
	this->m_CustomItemPositionInfo.clear();
}

void SetItemPosition(int ItemIndex, float PosX, float PosY, float AngleX, float AngleY, float AngleZ)
{
	CUSTOM_ITEM_POSITION_INFO info;

	info.ItemIndex = ItemIndex;

	info.PosX = PosX;

	info.PosY = PosY;

	info.AngleX = AngleX;

	info.AngleY = AngleY;

	info.AngleZ = AngleZ;

	gCustomItemPosition.m_CustomItemPositionInfo.insert(std::make_pair(info.ItemIndex, info));
}

void CCustomItemPosition::GetPositionItem(int ItemIndex, float* PosX, float* PosY, float* AngleX, float* AngleY, float* AngleZ)
{
	auto itemPos = gCustomItemPosition.getItemPosition(ItemIndex);

	if (itemPos)
	{
		(*PosX) = itemPos->PosX;
		(*PosY) = itemPos->PosY;
		(*AngleX) = itemPos->AngleX;
		(*AngleY) = itemPos->AngleY;
		(*AngleZ) = itemPos->AngleZ;
	}
}

bool CCustomItemPosition::CheckCustomItemPosition(int ItemIndex)
{
	std::map<int, CUSTOM_ITEM_POSITION_INFO>::iterator it = this->m_CustomItemPositionInfo.find(ItemIndex);

	if (it == this->m_CustomItemPositionInfo.end())
	{
		return 0;
	}

	return 1;
}

CUSTOM_ITEM_POSITION_INFO* CCustomItemPosition::getItemPosition(int ItemIndex)
{
	std::map<int, CUSTOM_ITEM_POSITION_INFO>::iterator it = this->m_CustomItemPositionInfo.find(ItemIndex);

	if (it == this->m_CustomItemPositionInfo.end())
	{
		return 0;
	}

	return &it->second;
}

void CCustomItemPosition::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetItemPosition", &SetItemPosition);

	this->m_Lua.DoFile("Data//Lua//ItemManager//CustomItemPosition.lua");

	this->m_Lua.Generic_Call("StartLoadItemPosition", "");
}

void CCustomItemPosition::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_CustomItemPositionInfo.clear();

	LuaModule(this->m_Lua.GetState()).fun("SetItemPosition", &SetItemPosition);

	this->m_Lua.DoFile("Data//Lua//ItemManager//CustomItemPosition.lua");

	m_Lua.Generic_Call("StartLoadItemPosition", "");
}
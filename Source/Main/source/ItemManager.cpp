// ItemManager.cpp: implementation of the CItemManager class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LuaStack.hpp"
#include "ItemManager.h"
#include "ZzzObject.h"

using namespace luaaa;

//CItemManager gItemManager;

CItemManager::CItemManager() : m_Lua()  // OK
{
	this->m_CustomItemInfo.clear();
}

CItemManager::~CItemManager() // OK
{
	this->m_CustomItemInfo.clear();
}

int CItemManager::GetItem(int section, int index)
{
	return (((section) * 512) + (index));
}

int CItemManager::GetItemModel(int section, int index)
{
	return ((((section) * 512) + (index)) + MODEL_ITEM);
}

void SetLoadItem(int ItemIndex, int colorR, int colorG, int ColorB, char* ModelName)
{
	CUSTOM_ITEM_INFO info;

	info.ItemIndex = ItemIndex;

	info.ColorR = colorR;

	info.ColorG = colorG;

	info.ColorB = ColorB;

	strcpy_s(info.ModelName, ModelName);

	gItemManager->m_CustomItemInfo.insert(std::make_pair(info.ItemIndex, info));
}

void CItemManager::Init() // OK
{
	LuaModule(this->m_Lua.GetState()).fun("LoadItem", &SetLoadItem);

	this->m_Lua.DoFile("Data//Lua//ItemManager//LoadItens.lua");

	m_Lua.Generic_Call("StartLoadItens", "");
}

void CItemManager::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_CustomItemInfo.clear();

	LuaModule(this->m_Lua.GetState()).fun("LoadItem", &SetLoadItem);

	this->m_Lua.DoFile("Data//Lua//ItemManager//LoadItens.lua");

	m_Lua.Generic_Call("StartLoadItens", "");
}

BOOL CItemManager::GetCustomItemColor(int Type, float* ItemColor) // OK
{
	std::map<int, CUSTOM_ITEM_INFO>::iterator it = gItemManager->m_CustomItemInfo.find(Type);

	if (it == gItemManager->m_CustomItemInfo.end())
	{
		return 0;
	}

	auto lpInfo = &(*it).second;

	ItemColor[0] = (float)(lpInfo->ColorR / 255.0f);
	ItemColor[1] = (float)(lpInfo->ColorG / 255.0f);
	ItemColor[2] = (float)(lpInfo->ColorB / 255.0f);

	return 1;
}

int CItemManager::GET_ITEM_MODEL(int section, int index)
{
	return ((((section) * 512) + (index)) + MODEL_ITEM);
}

void CItemManager::GetItemColor(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon)
{
	if (this->GetCustomItemColor(Type, (float*)Light) == 0)
	{
		PartObjectColor(Type, Alpha, Bright, Light, ExtraMon);
	}
}
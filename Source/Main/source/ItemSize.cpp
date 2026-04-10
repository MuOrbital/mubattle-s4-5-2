#include "stdafx.h"
#include "ItemSize.h"
#include "LuaStack.hpp"
using namespace luaaa;

CCustomItemSize gCustomItemSize;

CCustomItemSize::CCustomItemSize() : m_Lua()
{
    this->m_CustomItemSizeInfo.clear();
}

CCustomItemSize::~CCustomItemSize()
{
    this->m_CustomItemSizeInfo.clear();
}

void SetItemPosition(int ItemIndex, float SizeInventory)
{
    CUSTOM_ITEM_SIZE_INFO info;
    info.ItemIndex = ItemIndex;
    info.SizeInventory = SizeInventory;

    gCustomItemSize.m_CustomItemSizeInfo.insert(std::make_pair(info.ItemIndex, info));
}

void CCustomItemSize::GetSizeItem(int ItemIndex, float& Size)
{
    auto itemPos = gCustomItemSize.getItemSize(ItemIndex);
    if (itemPos)
    {
        Size = itemPos->SizeInventory;
    }
}

bool CCustomItemSize::CheckCustomItemSize(int ItemIndex)
{
    std::map<int, CUSTOM_ITEM_SIZE_INFO>::iterator it = this->m_CustomItemSizeInfo.find(ItemIndex);
    if (it == this->m_CustomItemSizeInfo.end()) {
        return false;
    }
    return true;
}

CUSTOM_ITEM_SIZE_INFO* CCustomItemSize::getItemSize(int ItemIndex)
{
    std::map<int, CUSTOM_ITEM_SIZE_INFO>::iterator it = this->m_CustomItemSizeInfo.find(ItemIndex);
    if (it == this->m_CustomItemSizeInfo.end()) {
        return nullptr;
    }
    return &it->second;
}

void CCustomItemSize::Init()
{
    LuaModule(this->m_Lua.GetState()).fun("SetItemSize", &SetItemPosition);
    this->m_Lua.DoFile("Data//Lua//ItemManager//CustomItemSize.lua");
    this->m_Lua.Generic_Call("StartLoadItemSize", "");
}

void CCustomItemSize::RestartLua()
{
    this->m_Lua.CloseLua();
    this->m_Lua.RegisterLua();

    this->m_CustomItemSizeInfo.clear();

    LuaModule(this->m_Lua.GetState()).fun("SetItemSize", &SetItemPosition);

    this->m_Lua.DoFile("Data//Lua//ItemManager//CustomItemSize.lua");
    this->m_Lua.Generic_Call("StartLoadItemSize", "");
}
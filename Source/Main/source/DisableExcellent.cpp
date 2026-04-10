#include "stdafx.h"
#include "DisableExcellent.h"
#include "LuaStack.hpp"

using namespace luaaa;

DisableExcellent gDisableExcellent;

DisableExcellent::DisableExcellent() : m_Lua()
{
	this->m_DisableExcellent.clear();
}

DisableExcellent::~DisableExcellent()
{
}

void SetDisableExcellent(int ItemIndex)
{
	DISABLE_EXCELLENT_INFO info;

	info.ItemIndex;

	gDisableExcellent.m_DisableExcellent.insert(std::make_pair(ItemIndex, info));
}

void DisableExcellent::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetDisableExcellent", &SetDisableExcellent);

	this->m_Lua.DoFile("Data//Lua//EffectSystem//DisableExcellent.lua");
}

bool DisableExcellent::CanDisableExcellent(int ItemIndex)
{
	std::map<int, DISABLE_EXCELLENT_INFO>::iterator it = this->m_DisableExcellent.find(ItemIndex);

	if (it == this->m_DisableExcellent.end())
	{
		return 0;
	}

	return 1;
}

int DisableExcellent::CheckDisableExcellent(int ItemIndex)
{
	if (gDisableExcellent.CanDisableExcellent(ItemIndex))
	{
		return 1;
	}

	return 0;
}
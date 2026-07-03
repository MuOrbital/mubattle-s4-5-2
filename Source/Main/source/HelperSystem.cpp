#include "stdafx.h"
#include "HelperSystem.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "ZzzEffect.h"
#include "Console.h"
#include "LuaStack.hpp"
#include "LuaObject.h"

using namespace luaaa;

CHelperSystem gHelperSystem;

CHelperSystem::CHelperSystem() : m_Lua()
{
	this->m_HelperInfo.clear();
}

CHelperSystem::~CHelperSystem()
{

}

void SetHelper(int ItemIndex, int Type, int Movement, float Height, float Size, float SizeCharList, int miniature, float sizeMiniature, float velocityMiniature, std::string Model, std::string ObjectModel)
{
	HELPER_INFO info;

	info.ItemIndex = ItemIndex;

	info.Type = (Type == 1 ? 2 : Type == 2 ? 4 : Type == 3 ? 8 : Type == 4 ? 16 : Type);

	info.Movement = Movement;

	info.HeightFloor = Height;

	info.Size = Size;

	info.SizeCharList = SizeCharList;

	info.Miniature = miniature;

	info.SizeMiniature = sizeMiniature;

	info.VelocityMiniature = velocityMiniature;

	strcpy_s(info.Model, Model.c_str());

	strcpy_s(info.ObjectModel, ObjectModel.c_str());

	gHelperSystem.m_HelperInfo.insert(std::make_pair(ItemIndex, info));
};

int CheckModelById(int ItemIndex)
{
	return gHelperSystem.GetHelperModel(ItemIndex);
}

bool CHelperSystem::CheckIsHelper(int ItemIndex)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (ItemIndex == 15)	//	Fix Objects Devias
		{
			return 0;
		}

		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex) 
		{
			return 1;
		}
	}

	return 0;
}

bool CHelperSystem::CheckHelperType(int ItemIndex, int value)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex) {
			return (it->second.Type & value) == value;
		}
	}

	return 0;
}

int CHelperSystem::GetHelperModel(int ItemIndex)
{
	std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.find(ItemIndex);

	if (it == this->m_HelperInfo.end()) {
		return 0;
	}

	if (it->second.Type == 0) {
		return it->second.ItemIndex;
	}
	else {
		return it->second.ModelID;
	}
}

HELPER_INFO* CHelperSystem::GetHelper(int ItemIndex)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex)
		{
			return &it->second;
		}
	}

	return 0;
}

int CHelperSystem::CheckIsFenrirOrDino(int ItemIndex)
{
	if (gHelperSystem.CheckHelperType(ItemIndex, 2) == 1 || gHelperSystem.CheckHelperType(ItemIndex, 4) == 1)
	{
		return 1;
	}

	return 0;
}

void CHelperSystem::Init()
{
	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetHelper", &SetHelper);

	LuaModule(this->m_Lua.GetState()).fun("CheckModelById", &CheckModelById);

	this->m_Lua.DoFile("Data//Lua//ItemManager//CharacterHelper.lua");

	this->m_Lua.Generic_Call("LoadHelpers", ">");
}

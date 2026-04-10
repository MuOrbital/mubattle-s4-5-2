// CustomJewel.cpp: implementation of the CCustomJewel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomJewel.h"
#include "NewUISystem.h"
#include "LuaStack.hpp"
#include "CustomWing.h"

using namespace luaaa;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomJewel::CCustomJewel() : m_Lua() // OK
{
	this->m_CustomJewelInfo.clear();
}

CCustomJewel::~CCustomJewel() // OK
{

}

void SetCustomJewel(int Index, int ItemIndex, int EnableSlotWeapon, int EnableSlotArmor, int EnableSlotWing, int EnableSlotRing, int EnableSlotPets, int SuccessRate0, int SuccessRate1, int SuccessRate2, int SuccessRate3, int SalePrice, char* ModelName)
{
	CUSTOM_JEWEL_INFO info;

	info.Index = Index;
	info.ItemIndex = ItemIndex;
	info.EnableSlotWeapon = EnableSlotWeapon;
	info.EnableSlotArmor = EnableSlotArmor;
	info.EnableSlotWing = EnableSlotWing;
	info.EnableSlotRing = EnableSlotRing;
	info.EnableSlotPets = EnableSlotPets;
	info.SuccessRate[0] = SuccessRate0;
	info.SuccessRate[1] = SuccessRate1;
	info.SuccessRate[2] = SuccessRate2;
	info.SuccessRate[3] = SuccessRate3;
	info.SalePrice = SalePrice;
	strcpy_s(info.ModelName, ModelName);

	gCustomJewel->m_CustomJewelInfo.insert(std::make_pair(Index, info));
}

void CCustomJewel::Init() // OK
{
	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetCustomJewel", &SetCustomJewel);

	this->m_Lua.DoFile("Data//Lua//ItemManager//CustomJewel.lua");

	this->m_Lua.Generic_Call("LoadCustomJewel", "i>");
}

CUSTOM_JEWEL_INFO* CCustomJewel::GetInfo(int index) // OK
{
	auto it = this->m_CustomJewelInfo.find(index);
	if (it == this->m_CustomJewelInfo.end())
	{
		return 0;
	}

	return &it->second;
}

CUSTOM_JEWEL_INFO* CCustomJewel::GetInfoByItem(int ItemIndex) // OK
{
	for (auto& pair : this->m_CustomJewelInfo)
	{
		CUSTOM_JEWEL_INFO& info = pair.second;

		if (info.ItemIndex == ItemIndex)
		{
			return &info;
		}
	}

	return 0;
}

BOOL CCustomJewel::CheckCustomJewel(int index) // OK
{
	if (this->GetInfo(index) != 0)
	{
		return 1;
	}

	return 0;
}

BOOL CCustomJewel::CheckCustomJewelByItem(int ItemIndex) // OK
{
	if (this->GetInfoByItem(ItemIndex) != 0)
	{
		return 1;
	}

	return 0;
}

BOOL CCustomJewel::CheckCustomJewelApplyItem(int ItemIndex, int TargetItemIndex) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	if ((TargetItemIndex / MAX_ITEM_TYPE) >= 7 && (TargetItemIndex / MAX_ITEM_TYPE) <= 11 && (TargetItemIndex % MAX_ITEM_TYPE) >= 62 && (TargetItemIndex % MAX_ITEM_TYPE) <= 72)
	{
		return 0;
	}

	if (TargetItemIndex > gItemManager->GetItem(12, 6) &&
		(TargetItemIndex < gItemManager->GetItem(12, 36) || TargetItemIndex > gItemManager->GetItem(12, 43)) &&
		TargetItemIndex != gItemManager->GetItem(12, 49) &&
		TargetItemIndex != gItemManager->GetItem(12, 50) &&
		(TargetItemIndex < gItemManager->GetItem(12, 262) || TargetItemIndex > gItemManager->GetItem(12, 265)) &&
		TargetItemIndex != gItemManager->GetItem(13, 30) &&
		(TargetItemIndex < gItemManager->GetItem(13, 0) || TargetItemIndex > gItemManager->GetItem(13, 75)) && // <-- LIBERA 13
		gCustomWing.CheckCustomWingByItem(TargetItemIndex) == 0)
	{
		return 0;
	}

	if (lpInfo->EnableSlotWeapon == 0 && (TargetItemIndex >= gItemManager->GetItem(0, 0) && TargetItemIndex < gItemManager->GetItem(6, 0)))
	{
		return 0;
	}

	if (lpInfo->EnableSlotArmor == 0 && (TargetItemIndex >= gItemManager->GetItem(6, 0) && TargetItemIndex < gItemManager->GetItem(12, 0)))
	{
		return 0;
	}

	if (lpInfo->EnableSlotWing == 0 && ((TargetItemIndex >= gItemManager->GetItem(12, 0) && TargetItemIndex <= gItemManager->GetItem(12, 6)) || (TargetItemIndex >= gItemManager->GetItem(12, 36) && TargetItemIndex <= gItemManager->GetItem(12, 43)) || TargetItemIndex == gItemManager->GetItem(12, 49) || TargetItemIndex == gItemManager->GetItem(12, 50) || (TargetItemIndex >= gItemManager->GetItem(12, 262) && TargetItemIndex <= gItemManager->GetItem(12, 265)) || TargetItemIndex == gItemManager->GetItem(13, 30) || gCustomWing.CheckCustomWingByItem(TargetItemIndex) != 0))
	{
		return 0;
	}

	if (lpInfo->EnableSlotRing == 0 && ((TargetItemIndex >= gItemManager->GetItem(13, 20) && TargetItemIndex <= gItemManager->GetItem(13, 28)) || (TargetItemIndex >= gItemManager->GetItem(13, 8) && TargetItemIndex <= gItemManager->GetItem(13, 9)) || TargetItemIndex == gItemManager->GetItem(13, 12) || TargetItemIndex == gItemManager->GetItem(13, 13)))
	{
		return 0;
	}

	if (lpInfo->EnableSlotPets == 0 && ((TargetItemIndex >= gItemManager->GetItem(13, 0) && TargetItemIndex <= gItemManager->GetItem(13, 5)) || TargetItemIndex == gItemManager->GetItem(13, 37) || TargetItemIndex == gItemManager->GetItem(13, 64) || TargetItemIndex == gItemManager->GetItem(13, 65) || TargetItemIndex == gItemManager->GetItem(13, 67) || ((TargetItemIndex >= gItemManager->GetItem(13, 71) && TargetItemIndex <= gItemManager->GetItem(13, 75)))))
	{
		return 0;
	}

	return 1;
}

int CCustomJewel::GetCustomJewelSuccessRate(int ItemIndex, int AccountLevel) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	return lpInfo->SuccessRate[AccountLevel];
}

int CCustomJewel::GetCustomJewelSalePrice(int ItemIndex) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	return lpInfo->SalePrice;
}

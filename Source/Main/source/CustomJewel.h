// CustomJewel.h: interface for the CCustomJewel class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NewSingleton.h"
#include "Lua.h"
#include <unordered_map>
#include "LuaGlobal.h"

struct CUSTOM_JEWEL_INFO
{
	int Index;
	int ItemIndex;
	int EnableSlotWeapon;
	int EnableSlotArmor;
	int EnableSlotWing;
	int EnableSlotRing;
	int EnableSlotPets;
	int SuccessRate[4];
	int SalePrice;
	char ModelName[32];
};

class CCustomJewel
{
	CCustomJewel();

	virtual ~CCustomJewel();

	SingletonInstance(CCustomJewel);

public:
	void Init();

	CUSTOM_JEWEL_INFO* GetInfo(int index);

	CUSTOM_JEWEL_INFO* GetInfoByItem(int ItemIndex);

	BOOL CheckCustomJewel(int index);

	BOOL CheckCustomJewelByItem(int ItemIndex);

	BOOL CheckCustomJewelApplyItem(int ItemIndex, int TargetItemIndex);

	int GetCustomJewelSuccessRate(int ItemIndex, int AccountLevel);

	int GetCustomJewelSalePrice(int ItemIndex);

public:
	std::unordered_map<int, CUSTOM_JEWEL_INFO> m_CustomJewelInfo;

private:
	Lua m_Lua;
	LuaGlobal m_LuaGlobal;
};

#define gCustomJewel SingNull(CCustomJewel)

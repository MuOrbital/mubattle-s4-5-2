#pragma once

#include "Item.h"
#include "User.h"
#include <vector>

struct CUSTOM_PET_INFO
{
	int Index;
	int ItemIndex;
	int IsMontaria;
	int IsImortal;
	int IncLife;
	int IncMana;
	int IncDamageRate;
	int IncAttackSpeed;
	int IncDoubleDamageRate;
	int IncTripleDamageRate;
	int Experience;
	int IncResistIgnoreDefense;
	int IncResistIgnoreSD;
	int IncResistCriticalDamage;
	int IncResisteExcellentDamage;
	int IncBlockStuck;
	int IncReflectRate;
	int IncSD;
	int IncBP;
};

class CCustomPet
{
public:
	CCustomPet();
	virtual ~CCustomPet();
	void Load(char* path);
	int CheckCustomPetIsMontaria(int ItemIndex);
	bool CheckCustomPetByItem(int ItemIndex);
	int CheckCustomPetIsImortal(int ItemIndex);
	int GetCustomPetDamageRate(int ItemIndex);
	void CalcCustomPetOption(LPOBJ lpObj, bool flag);
private:
	CUSTOM_PET_INFO* GetInfoByItem(int ItemIndex);
public:
	std::vector<CUSTOM_PET_INFO> m_CustomPetInfo;
};

extern CCustomPet gCustomPet;

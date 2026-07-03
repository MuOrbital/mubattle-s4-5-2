#include "stdafx.h"
#include "Pets.h"
#include "MemScript.h"
#include "Util.h"

CCustomPet gCustomPet;

CCustomPet::CCustomPet()
{
	this->m_CustomPetInfo.clear();
}

CCustomPet::~CCustomPet()
{

}

void CCustomPet::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_CustomPetInfo.clear();

	try
	{
		int CustomItemIndexCount = 0;

		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_PET_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = CustomItemIndexCount++;
			info.ItemIndex = lpMemScript->GetNumber();
			info.IsMontaria = lpMemScript->GetAsNumber();
			info.IsImortal = lpMemScript->GetAsNumber();
			info.IncLife = lpMemScript->GetAsNumber();
			info.IncMana = lpMemScript->GetAsNumber();
			info.IncDamageRate = lpMemScript->GetAsNumber();
			info.IncAttackSpeed = lpMemScript->GetAsNumber();
			info.IncDoubleDamageRate = lpMemScript->GetAsNumber();
			info.IncTripleDamageRate = lpMemScript->GetAsNumber();
			info.Experience = lpMemScript->GetAsNumber();
			info.IncResistIgnoreDefense = lpMemScript->GetAsNumber();
			info.IncResistIgnoreSD = lpMemScript->GetAsNumber();
			info.IncResistCriticalDamage = lpMemScript->GetAsNumber();
			info.IncResisteExcellentDamage = lpMemScript->GetAsNumber();
			info.IncBlockStuck = lpMemScript->GetAsNumber();
			info.IncReflectRate = lpMemScript->GetAsNumber();
			info.IncSD = lpMemScript->GetAsNumber();
			info.IncBP = lpMemScript->GetAsNumber();

			if(info.IsMontaria < 2)
			{
				this->m_CustomPetInfo.push_back(info);
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

CUSTOM_PET_INFO* CCustomPet::GetInfoByItem(int ItemIndex)
{
	for(std::vector<CUSTOM_PET_INFO>::iterator it=this->m_CustomPetInfo.begin();it != this->m_CustomPetInfo.end();it++)
	{
		if(it->ItemIndex == ItemIndex)
		{
			return &(*it);
		}
	}

	return 0;
}

int CCustomPet::CheckCustomPetIsMontaria(int ItemIndex)
{
	CUSTOM_PET_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	return ((lpInfo == 0) ? 0 : lpInfo->IsMontaria);
}

bool CCustomPet::CheckCustomPetByItem(int ItemIndex)
{
	return (this->GetInfoByItem(ItemIndex) != 0);
}

int CCustomPet::CheckCustomPetIsImortal(int ItemIndex)
{
	CUSTOM_PET_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	return ((lpInfo == 0) ? 0 : lpInfo->IsImortal);
}

int CCustomPet::GetCustomPetDamageRate(int ItemIndex)
{
	CUSTOM_PET_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	return ((lpInfo == 0) ? 0 : lpInfo->IncDamageRate);
}

void CCustomPet::CalcCustomPetOption(LPOBJ lpObj, bool flag)
{
	if(flag != 0 || lpObj->Inventory[8].IsItem() == 0)
	{
		return;
	}

	CUSTOM_PET_INFO* lpInfo = this->GetInfoByItem(lpObj->Inventory[8].m_Index);

	if(lpInfo == 0)
	{
		return;
	}

	if(lpInfo->IncDamageRate != 0)
	{
		lpObj->PhysiDamageMinRight += (lpObj->PhysiDamageMinRight*lpInfo->IncDamageRate)/100;
		lpObj->PhysiDamageMaxRight += (lpObj->PhysiDamageMaxRight*lpInfo->IncDamageRate)/100;
		lpObj->PhysiDamageMinLeft += (lpObj->PhysiDamageMinLeft*lpInfo->IncDamageRate)/100;
		lpObj->PhysiDamageMaxLeft += (lpObj->PhysiDamageMaxLeft*lpInfo->IncDamageRate)/100;
		lpObj->MagicDamageMin += (lpObj->MagicDamageMin*lpInfo->IncDamageRate)/100;
		lpObj->MagicDamageMax += (lpObj->MagicDamageMax*lpInfo->IncDamageRate)/100;
		lpObj->CurseDamageMin += (lpObj->CurseDamageMin*lpInfo->IncDamageRate)/100;
		lpObj->CurseDamageMax += (lpObj->CurseDamageMax*lpInfo->IncDamageRate)/100;
	}

	lpObj->AddLife += lpInfo->IncLife;
	lpObj->AddMana += lpInfo->IncMana;
	lpObj->PhysiSpeed += lpInfo->IncAttackSpeed;
	lpObj->MagicSpeed += lpInfo->IncAttackSpeed;
	lpObj->DoubleDamageRate += lpInfo->IncDoubleDamageRate;
	lpObj->ResistDoubleDamageRate += lpInfo->IncDoubleDamageRate;
	lpObj->TripleDamageRate += lpInfo->IncTripleDamageRate;
	lpObj->ExperienceRate += lpInfo->Experience;
	lpObj->MasterExperienceRate += lpInfo->Experience;
	lpObj->ResistIgnoreDefenseRate += lpInfo->IncResistIgnoreDefense;
	lpObj->ResistIgnoreShieldGaugeRate += lpInfo->IncResistIgnoreSD;
	lpObj->ResistCriticalDamageRate += lpInfo->IncResistCriticalDamage;
	lpObj->ResistExcellentDamageRate += lpInfo->IncResisteExcellentDamage;
	lpObj->ResistStunRate += lpInfo->IncBlockStuck;
	lpObj->DamageReflect += lpInfo->IncReflectRate;
	lpObj->AddShield += lpInfo->IncSD;
	lpObj->AddBP += lpInfo->IncBP;
}

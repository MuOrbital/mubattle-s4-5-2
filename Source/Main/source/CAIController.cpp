#include "stdafx.h"
#include "ZzzAI.h"
#include "CComGem.h"
#include "MapManager.h"
#include "NewUICommon.h"
#include "NewUISystem.h"
#include "ZzzInterface.h"
#include "SkillManager.h"
#include "wsclientinline.h"
#include "CAIController.h"
#include "Protect.h"
#include "CharacterManager.h"

CAIController* GetMuHelperController()
{
	// Lazy construction avoids changing the legacy WinMain startup sequence.
	static CAIController controller(Hero);
	return &controller;
}

void RANGE_FIXED(MUAction_info* val, int min, int max, int condition)
{
	if (!(val->bRate >= min && val->bRate <= max))
	{
		val->bRate = condition;
	}
}

bool CAIController::IsRunning()
{
	return this->m_IsStart;
}

bool CAIController::CanUseAIController()
{
	bool success = false;

	if (CharacterAttribute->Level >= gProtect->m_MainInfo.m_HelperActiveLevel)
	{
		if (sCharacter->SafeZone || gMapManager.InChaosCastle())
		{
			SEASON3B::CreateOkMessageBox(GlobalText[3574], -1);
			success = false;
		}
		else
		{
#ifdef INVENTORY_HELPER_ACTIVE
			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY))
			{
				SEASON3B::CreateOkMessageBox(GlobalText[3575], -1);
				success = false;
			}
			else
			{
				if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MACRO_OFICIAL))
				{
					SEASON3B::CreateOkMessageBox(GlobalText[3576], -1);
					success = false;
				}
				else if (this->SkillIndex > 0
					|| this->SkillGroup[0].Index > 0
					|| this->SkillGroup[1].Index > 0
					|| gCharacterManager.GetBaseClass(sCharacter->Class) == Fairy_Elf)
				{
					success = true;
				}
				else
				{
					SEASON3B::CreateOkMessageBox(GlobalText[3577], -1);
					success = false;
				}
			}
#else
			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MACRO_OFICIAL))
			{
				SEASON3B::CreateOkMessageBox(GlobalText[3576], -1);
				success = false;
			}
			else if (this->SkillIndex > 0
				|| this->SkillGroup[0].ID > 0
				|| this->SkillGroup[1].ID > 0
				|| gCharacterManager.GetBaseClass(sCharacter->Class) == Fairy_Elf)
			{
				success = true;
			}
			else
			{
				SEASON3B::CreateOkMessageBox(GlobalText[3577], -1);
				success = false;
			}
#endif // INVENTORY_HELPER_ACTIVE
		}
	}
	else
	{
		char Src[255];
		sprintf_s(Src, GlobalText[3573], gProtect->m_MainInfo.m_HelperActiveLevel);
		SEASON3B::CreateOkMessageBox(Src, -1);
		success = false;
	}
	return success;
}

void CAIController::Start()
{
	if (!this->m_IsStart)
	{
		this->m_IsAutoAttack = g_pOption->IsAutoAttack();
		g_pOption->SetAutoAttack(!this->ShutdownCombo);

		this->m_IsStart = true;
		this->isSendPetCommand = false;
		this->sMonsterATK = NULL;
		this->sMonsterPre = NULL;
		this->cur_item = NULL;
		this->StartPositionX = sCharacter->PositionX;
		this->StartPositionY = sCharacter->PositionY;
		this->m_DelayFindTarget = 0;
		this->m_WaitTimeReturn = std::chrono::steady_clock::now();
		this->m_WaitTimeHealAT = 0;
		this->m_WaitTimeConsume = 0;
		this->m_WaitTimeBuffAT = 0;
		this->m_TimeCondition[0] = 0;
		this->m_TimeCondition[1] = 0;
		this->buffParty_Work.intervalSet(0.3);
		this->healParty_Work.intervalSet(0.3);
		this->ShutdownReturnOn = false;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < MAX_PARTYS; ++j)
			{
				this->buff_member_work[i][j] = 0;
			}
		}

		SetPartyBuff();
		g_pMacroGaugeBar->StartAccumulate();
		g_pChatListBox->AddText("", GlobalText[3578], SEASON3B::TYPE_ERROR_MESSAGE, SEASON3B::TYPE_ALL_MESSAGE);
		g_pChatListBox->AddText("", GlobalText[3582], SEASON3B::TYPE_ERROR_MESSAGE, SEASON3B::TYPE_ALL_MESSAGE); // message "Certain amount of zen is spent every 5 minutes in implementing Official MU Helper"
	}
}

void CAIController::Stop()
{
	if (this->m_IsStart == true)
	{
		this->m_IsStart = false;
		g_pOption->SetAutoAttack(this->m_IsAutoAttack);
		g_pMacroGaugeBar->StopAccumulate();
		g_pChatListBox->AddText("", GlobalText[3579], SEASON3B::TYPE_ERROR_MESSAGE, SEASON3B::TYPE_ALL_MESSAGE);
	}
}

CAIController::CAIController(CHARACTER* Character)
{
	m_IsStart = false;
	this->WorkAI.intervalSet(0.15);
	ResetConfig();
	SetActiveCharacter(Character);
}

CAIController::~CAIController()
{
}

void CAIController::SetActiveCharacter(CHARACTER* pCha)
{
	sCharacter = pCha;
	this->isSendPetCommand = false;
	this->m_CounATK = 0;
	this->sMonsterATK = NULL;
	this->sMonsterPre = NULL;
	this->cur_item = NULL;
	this->prev_item = NULL;
}

void CAIController::ResetConfig()
{
	this->AttackRange = 1;
	this->healPotion.Shutdown = false;
	this->healPotion.bRate = 50;
	this->healskillLife.Shutdown = false;
	this->healskillLife.bRate = 50;
	this->healskilldrain.Shutdown = false;
	this->healskilldrain.bRate = 50;
	m_DistanceLong = false;
	this->DistanceReturn = false;
	m_DistanceOnSec = 0;
	this->SkillIndex = 0;
	this->ShutdownCombo = false;
	this->ShutdownParty = false;
	this->healParty.Shutdown = false;
	this->healParty.bRate = 50;
	this->buff_duration_all_work = false;
	this->space_of_casting_buff = 0;
	this->ShutdownPetCmd = false;
	this->ShutdownBuff = false;
	m_PickDistance = 1;
	this->ShutdownRepair = false;
	this->Picker_work.Near = false;
	this->Picker_work.Select = true;
	this->Picker_work.Jewel = false;
	this->Picker_work.Ancient = false;
	this->Picker_work.Excellent = false;
	this->Picker_work.Zen = false;
	this->Picker_work.Extra = false;
	m_AutoFriend = false;
	m_AutoGuildMember = false;
	m_EliteManaPotion = false;

	memset(&SkillGroup, 0, sizeof(MUSkill_info));
	memset(&SkillGroup[1], 0, sizeof(MUSkill_info));
	memset(&m_SkillBuff, 0, sizeof(m_SkillBuff));

	for (int i = 0; i < MAX_PICK_EXTRA; ++i)
	{
		memset(this->Picker_work.Name[i], 0, sizeof(this->Picker_work.Name[i]));
	}
}

void CAIController::LoadDefaultConfg()
{
	this->AttackRange = 6;
	this->healPotion.Shutdown = false;
	this->healPotion.bRate = 50;
	this->healskillLife.Shutdown = false;
	this->healskillLife.bRate = 50;
	this->healskilldrain.Shutdown = false;
	this->healskilldrain.bRate = 50;
	m_DistanceLong = false;
	this->DistanceReturn = false;
	m_DistanceOnSec = 10;
	this->SkillIndex = 0;
	this->ShutdownCombo = false;
	this->ShutdownParty = false;
	this->healParty.Shutdown = false;
	this->healParty.bRate = 50;
	this->buff_duration_all_work = false;
	this->space_of_casting_buff = 0;
	this->ShutdownPetCmd = false;
	this->ShutdownBuff = false;
	m_PickDistance = 8;
	this->ShutdownRepair = false;
	this->Picker_work.Near = true;
	this->Picker_work.Select = false;
	this->Picker_work.Jewel = false;
	this->Picker_work.Ancient = false;
	this->Picker_work.Excellent = false;
	this->Picker_work.Zen = false;
	this->Picker_work.Extra = false;

	m_AutoFriend = false;
	m_AutoGuildMember = false;
	m_EliteManaPotion = false;
	memset(&this->SkillGroup[0], 0, sizeof(MUSkill_info));
	memset(&this->SkillGroup[1], 0, sizeof(MUSkill_info));
	memset(&m_SkillBuff, 0, sizeof(m_SkillBuff));

	for (int i = 0; i < MAX_PICK_EXTRA; ++i)
	{
		memset(this->Picker_work.Name[i], 0, sizeof(this->Picker_work.Name[i]));
	}
}

void CAIController::SaveConfig()
{
	BYTE byPack[256];

	if (this->Pack(byPack))
	{
		SendRequestSaveHelperSetting(byPack, sizeof(byPack));
	}
	g_pChatListBox->AddText("", GlobalText[3580], SEASON3B::TYPE_SYSTEM_MESSAGE, SEASON3B::TYPE_ALL_MESSAGE);
}

float CAIController::GetDistance(CHARACTER* pCha)
{
	float fDistance = 0.0;

	if (pCha)
	{
		float fDis_x, fDis_y;
		fDis_x = (double)(sCharacter->PositionX - pCha->PositionX);
		fDis_y = (double)(sCharacter->PositionY - pCha->PositionY);
		fDistance = sqrtf(fDis_x * fDis_x + fDis_y * fDis_y);
	}
	return fDistance;
}

float CAIController::GetDistance(OBJECT* pObject)
{
	float fDistance = 0.0;

	if (pObject)
	{
		float fDis_x = (double)(sCharacter->PositionX - pObject->Position[0] / TERRAIN_SCALE);
		float fDis_y = (double)(sCharacter->PositionY - pObject->Position[1] / TERRAIN_SCALE);
		fDistance = sqrtf(fDis_x * fDis_x + fDis_y * fDis_y);
	}
	return fDistance;
}

void CAIController::WhatToDoNext()
{
	MouseRButtonPush = 0;
	if (sMonsterATK && (!sMonsterATK->Object.Live || sMonsterATK->Object.CurrentAction == MONSTER01_DIE))
	{
		sMonsterATK = NULL;
		SelectedCharacter = -1;
	}

	auto time_current = std::chrono::steady_clock::now();

	if (this->WorkAI.hasElapsed())
	{
		this->WorkAI.intervalSet(0.15);

		if (sCharacter->SafeZone)
		{
			g_pChatListBox->AddText("", GlobalText[3581], SEASON3B::TYPE_SYSTEM_MESSAGE, SEASON3B::TYPE_ALL_MESSAGE);
			SendRequestStartHelper(TRUE);
			return;
		}
		if (
#ifndef INVENTORY_HELPER_ACTIVE
			g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY) ||
#endif // INVENTORY_HELPER_ACTIVE
			g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MACRO_OFICIAL)
			)
		{
			SendRequestStartHelper(TRUE);
			return;
		}

		if (this->ShutdownPetCmd && !this->isSendPetCommand)
		{
			SendRequestPetCommand(PET_TYPE_DARK_SPIRIT, this->PetCmdType, 0xffff);
			this->isSendPetCommand = true;
		}

		DWORD MaxLife = CharacterAttribute->LifeMax;

		if (this->healPotion.Shutdown)
		{
			if (CharacterAttribute->Life < (MaxLife * (this->healPotion.bRate / 100.f))
				&& sCharacter->Object.Live == 1
				&& CharacterAttribute->Life > 0)
			{
				this->ConsumePotion();
			}
		}

		if (!this->healskillLife.Shutdown
			|| this->GetSkillIndex(Skill_Heal) < 0 && this->GetSkillIndex(Skill_Heal_Strengthener) < 0
			|| ((CharacterAttribute->Life >= (MaxLife * (this->healskillLife.bRate / 100.f)))
				|| sCharacter->Object.Live != 1
				|| (signed int)CharacterAttribute->Life <= 0
				|| !this->runtime_healAt(sCharacter)))
		{
			if (!this->ShutdownParty
				|| !this->healParty.Shutdown
				|| PartyNumber <= 0
				|| this->GetSkillIndex(Skill_Heal) < 0 && this->GetSkillIndex(Skill_Heal_Strengthener) < 0
				|| !this->runtime_party_healing())
			{
				if (!this->healskilldrain.Shutdown
					|| this->GetSkillIndex(Skill_Drain_Life) < 0 && this->GetSkillIndex(Skill_Drain_Life_Strengthener) < 0
					|| ((CharacterAttribute->Life >= (MaxLife * (this->healskilldrain.bRate / 100.f)))
						|| sCharacter->Object.Live != 1
						|| (signed int)CharacterAttribute->Life <= 0
						|| !this->runtime_choosetarget()
						|| (this->GetSkillIndex(Skill_Drain_Life) < 0 || !this->SkillAt(Skill_Drain_Life, sMonsterATK, true))
						&& (this->GetSkillIndex(Skill_Drain_Life_Strengthener) < 0 || !this->SkillAt(Skill_Drain_Life_Strengthener, sMonsterATK, true))))
				{

					if (!this->ShutdownBuff || !this->SelfBuffAt())
					{
						if (this->CheckPickup())
						{
							this->PickupItem(cur_item);
							return;
						}
						if (this->ShutdownRepair)
							this->RepairItem();
						if (sCharacter->Movement)
						{
							if (this->ShutdownReturnOn && this->IsEnemyAttaced())
							{
								LetHeroStop(sCharacter, TRUE);
								SetPlayerStop(sCharacter);
							}
						}
						else
						{
							if (this->ShutdownCombo
								&& m_CounATK > 0
								&& this->sMonsterPre
								&& sMonsterPre->Object.Live == 1
								&& sMonsterPre->Object.CurrentAction != MONSTER01_DIE)
							{
								if ((signed int)sCharacter->Object.CurrentAction > PLAYER_STOP_RIDE_WEAPON
									&& ((signed int)sCharacter->Object.CurrentAction < PLAYER_FENRIR_STAND
										|| (signed int)sCharacter->Object.CurrentAction > PLAYER_FENRIR_WALK_WEAPON_LEFT))
								{
									return;
								}
								if (this->m_CounATK == 1)
								{
									if (this->SkillAt(SkillGroup[0].Index, sMonsterPre, true))
									{
										++this->m_CounATK;
										return;
									}
								}
								else if (this->m_CounATK == 2
									&& this->SkillAt(SkillGroup[1].Index, sMonsterPre, true))
								{
									this->m_CounATK = 0;
									return;
								}
							}
							if ((!this->ShutdownParty
								|| PartyNumber <= 0
								|| this->GetSkillIndex(Skill_Heal) < 0 && this->GetSkillIndex(Skill_Heal_Strengthener) < 0
								|| !this->runtime_party_healing())
								&& (!this->ShutdownParty
									|| PartyNumber <= 0
									|| !this->buff_duration_all_work
									|| !this->runtime_party_buff()))
							{
								if (this->DistanceReturn)
								{
									if (this->PlayerReached(StartPositionX, StartPositionY))
									{
										if (this->ShutdownReturnOn == true)
										{
											this->ShutdownReturnOn = false;
											this->sMonsterATK = NULL;
											SelectedCharacter = -1;
										}
										this->m_WaitTimeReturn = time_current;
									}
									else if ((time_current - this->m_WaitTimeReturn) > std::chrono::milliseconds((1000 * this->m_DistanceOnSec)))
									{
										m_WaitTimeReturn += std::chrono::milliseconds(5000);
										sMonsterPre = NULL;
										sMonsterATK = NULL;
										this->MoveTo(StartPositionX, StartPositionY);
										this->ShutdownReturnOn = true;
										return;
									}
								}

								if ((this->ShutdownCombo
									|| !this->UseConditionalSkill(&SkillGroup[1], &m_TimeCondition[1])
									&& !this->UseConditionalSkill(&SkillGroup[0], &m_TimeCondition[0]))
									&& this->SkillIndex
									&& runtime_choosetarget()
									&& (!this->ShutdownCombo
										|| this->m_CounATK
										|| (signed int)sCharacter->Object.CurrentAction <= PLAYER_STOP_RIDE_WEAPON
										|| (signed int)sCharacter->Object.CurrentAction >= PLAYER_FENRIR_STAND
										&& (signed int)sCharacter->Object.CurrentAction <= PLAYER_FENRIR_WALK_WEAPON_LEFT)
									&& this->SkillAt(this->SkillIndex, sMonsterATK, true))
								{
									this->m_CounATK = 1;
								}
							}
						}
					}
				}
			}
		}
	}
}

bool CAIController::ConsumePotion()
{
	DWORD cur_time = timeGetTime();
	if (cur_time - m_WaitTimeConsume < 500)
	{
		return false;
	}
	int hp_good[9] = {
		ITEM_POTION,
		ITEM_POTION + 1,
		ITEM_POTION + 2,
		ITEM_POTION + 3,
		ITEM_POTION + 38,
		ITEM_POTION + 39,
		ITEM_POTION + 40,
		ITEM_POTION + 70,
		ITEM_POTION + 94
	};
	int use_index = -1;
	for (int i = 0; i < 9; i++)
	{
		use_index = g_pMyInventory->FindItemReverseIndex(hp_good[i]);
		if (-1 != use_index)
		{
			break;
		}
	}
	if (-1 != use_index)
	{
		SendRequestUse(use_index, 0);
	}
	m_WaitTimeConsume = cur_time;
	return true;
}

int CAIController::runtime_choosetarget()
{
	int EnemyNum = 0;
	CHARACTER* pChar = NULL;
	CHARACTER* pHero = NULL;

	if (sMonsterATK && sMonsterATK->Object.Kind != KIND_MONSTER)
		sMonsterATK = NULL;

	if (sMonsterATK && !CheckWall(sCharacter->PositionX, sCharacter->PositionY, sMonsterATK->PositionX, sMonsterATK->PositionY))
	{
		sMonsterATK = 0;
	}

	if (sCharacter->Object.CurrentAction != sCharacter->Object.PriorAction)
		m_DelayFindTarget = timeGetTime();

	time_t time_current = timeGetTime();

	if ((timeGetTime() - m_DelayFindTarget) > 5000)
	{
		m_DelayFindTarget = timeGetTime();
		sMonsterATK = 0;
	}
	if (!sMonsterATK)
		SelectedCharacter = -1;
	sMonsterPre = sMonsterATK;

	for (int i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		pHero = &CharactersClient[i];

		if (pHero->Object.Live == 1
			&& pHero->Object.Kind == KIND_MONSTER
			&& pHero->Object.CurrentAction != MONSTER01_DIE
			&& pHero->Object.CurrentAction != MONSTER01_APEAR)
		{
			if (GetDistance(pHero) <= (double)this->AttackRange || m_DistanceLong && IsEnemyAttacking(pHero))
			{
				++EnemyNum;
				pChar = RankEnemy(pChar, pHero);
			}
		}
	}
	sMonsterATK = pChar;
	return EnemyNum;
}

int CAIController::GetHostileEnemyNum()
{
	int EnemyNum = 0;
	CHARACTER* pHero = NULL;

	for (int i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		pHero = &CharactersClient[i];
		if (pHero->Object.Live == 1
			&& pHero->Object.Kind == KIND_MONSTER
			&& (double)this->AttackRange >= GetDistance(pHero))
		{
			if (IsEnemyAttacking(pHero))
				++EnemyNum;
		}
	}
	return EnemyNum;
}

CHARACTER* CAIController::RankEnemy(CHARACTER* Rank1, CHARACTER* Rank2)
{
	CHARACTER* pCha;

	if (Rank2 && !CheckWall(sCharacter->PositionX, sCharacter->PositionY, Rank2->PositionX, Rank2->PositionY))
	{
		Rank2 = NULL;
	}
	if (Rank1)
	{
		if (sMonsterATK && (sMonsterATK == Rank2 || sMonsterATK == Rank1))
		{
			pCha = sMonsterATK;
		}
		else if (Rank1 && Rank2)
		{
			if (GetDistance(Rank1) <= GetDistance(Rank2))
				pCha = Rank1;
			else
				pCha = Rank2;
		}
		else if (Rank1)
		{
			pCha = Rank1;
		}
		else if (Rank2)
		{
			pCha = Rank2;
		}
		else
		{
			pCha = NULL;
		}
	}
	else
	{
		pCha = Rank2;
	}
	return pCha;
}

bool CAIController::IsEnemyAttaced()
{
	CHARACTER* pCha;
	for (int i = 0; i < 400; ++i)
	{
		pCha = &CharactersClient[i];
		if (pCha->Object.Live == 1
			&& pCha->Object.Kind == KIND_MONSTER
			&& pCha->Object.CurrentAction != MONSTER01_DIE
			&& pCha->Object.CurrentAction != MONSTER01_APEAR
			&& IsEnemyAttacking(pCha) == true)
		{
			return true;
		}
	}
	return false;
}

bool CAIController::IsEnemyAttacking(CHARACTER* Target)
{
	bool success = false;

	CHARACTER* pCha = (Target->TargetCharacter >= 0 && Target->TargetCharacter < MAX_CHARACTERS_CLIENT)
		? &CharactersClient[Target->TargetCharacter] : NULL;

	if (pCha)
	{
		switch (Target->Object.CurrentAction)
		{
		case MONSTER01_ATTACK1:
		case MONSTER01_ATTACK2:
		case MONSTER01_ATTACK3:
		case MONSTER01_ATTACK4:
		case MONSTER01_ATTACK5:
			if (pCha == sCharacter)
				success = true;
			break;
		default:
			break;
		}
	}
	return success;
}

bool CAIController::CheckPickup()
{
	int Rate = 0;
	ITEM_t* item = NULL;
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		ITEM_t* temp = &Items[i];
		if (true == temp->Object.Live && temp->Object.Visible && (double)m_PickDistance >= this->GetDistance(&temp->Object))
		{
			int cur_Weight = this->RateItem(temp);
			if (cur_Weight > Rate)
			{
				Rate = cur_Weight;
				item = temp;
			}
		}
	}
	if (!item || item == this->prev_item)
	{
		return false;
	}
	this->cur_item = item;
	return true;
}

int CAIController::RateItem(ITEM_t* item)
{
	int percent = 0;

	MUPick_info* MuPick = &this->Picker_work;

	if (MuPick->Near)
	{
		percent = 100;
	}
	else if (MuPick->Select)
	{
		int selectedItemOption = item->Item.ExtOption % 0x04;

		if (MuPick->Jewel && -1 != COMGEM::Check_Jewel(item->Item.Type))
		{
			percent = 100;
		}
		if (MuPick->Ancient && selectedItemOption > 0)
		{
			percent = 100;
		}
		if (MuPick->Excellent && (item->Item.Option1 & 0x3F || selectedItemOption > 0))
		{
			percent = 100;
		}
		if (MuPick->Zen && ITEM_POTION + 15 == item->Item.Type)
		{
			percent = 100;
		}
		if (MuPick->Extra)
		{
			for (int i = 0; i < MAX_PICK_EXTRA; i++)
			{
				if (unicode::_strlen(this->Picker_work.Name[i]) > 0)
				{
					if (this->matchesPattern(ItemAttribute[item->Item.Type].Name, this->Picker_work.Name[i]))
					{
						percent = 100;
						break;
					}
				}

			}
		}
	}
	if (percent > 0)
	{
		if (-1 == g_pMyInventory->FindEmptySlot(&item->Item) && ITEM_POTION + 15 != item->Item.Type)
		{
			percent = 0;
		}
	}
	return percent;
}

bool CAIController::PickupItem(ITEM_t* item)
{
	if (item != nullptr)
	{
		vec3_t vLength;
		SelectedItem = ((int)item - (int)&Items[0]) / sizeof(ITEM_t);
		sCharacter->MovementType = MOVEMENT_GET;

		int tx = (int)(Items[SelectedItem].Object.Position[0] / TERRAIN_SCALE);
		int ty = (int)(Items[SelectedItem].Object.Position[1] / TERRAIN_SCALE);

		vLength[0] = item->Object.Position[0] - sCharacter->Object.Position[0];
		vLength[1] = item->Object.Position[1] - sCharacter->Object.Position[1];
		vLength[2] = item->Object.Position[2] - sCharacter->Object.Position[2];

		if (VectorLength(vLength) > 200.0)
		{
			if (PathFinding2(sCharacter->PositionX, sCharacter->PositionY, tx, ty, &sCharacter->Path, 0.0, 2))
			{
				SendMove(sCharacter, &item->Object);
			}
		}
		else
		{
			ItemKey = SelectedItem;
			SendRequestGetItem(ItemKey);
			prev_item = item;
		}
		return true;
	}
	return false;
}

void CAIController::RepairItem()
{
	if (CharacterAttribute->Level >= 50)
	{
		for (int i = 0; i < MAX_EQUIPMENT; i++)
		{
			ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[i];
			if (false == IsRepairBan(pEquipmentItemSlot) && pEquipmentItemSlot->Type >= 0)
			{
				int level = (pEquipmentItemSlot->Level >> 3) & 0xF;

				ITEM_ATTRIBUTE* itemAttribute = &ItemAttribute[pEquipmentItemSlot->Type];
				unsigned short Durability = calcMaxDurability(pEquipmentItemSlot, itemAttribute, level);

				if (pEquipmentItemSlot->Durability < (7 * Durability / 10))
				{
					SendRequestRepair(i, 1);
					return;
				}
			}
		}
	}
}

int CAIController::GetSkillIndex(int SkillKey)
{
	if (SkillKey)
	{
		for (int i = 0; i < CharacterAttribute->SkillNumber; ++i)
		{
			if (CharacterAttribute->Skill[i] == SkillKey)
				return i;
		}
	}
	return -1;
}

bool CAIController::SkillAt(int SkillKey, CHARACTER* Target, bool bActive)
{
	bool success = false;

	if (Target && (!Target || bActive != true || Target->Object.Kind == KIND_MONSTER))
	{
		int index = GetSkillIndex(SkillKey);

		if (index == -1)
		{
			success = false;
		}
		else
		{
			int piMana, piDistance, piSkillMana;
			gSkillManager.GetSkillInformation(SkillKey, 1, NULL, &piMana, &piDistance, &piSkillMana);

			if ((DWORD)piMana <= CharacterAttribute->Mana
				|| (g_pMyInventory->FindManaItemIndex() != -1))
			{
				if ((DWORD)piSkillMana <= CharacterAttribute->SkillMana)
				{
					if (!this->ShutdownCombo)
					{
						if (GetDistance(Target) > (float)piDistance)
						{
							if (PathFinding2(sCharacter->PositionX, sCharacter->PositionY, Target->PositionX, Target->PositionY, &sCharacter->Path, (float)piDistance, 2))
							{
								sCharacter->Movement = 1;
								SendMove(sCharacter, &sCharacter->Object);
							}
						}
					}
					sCharacter->CurrentSkill = index;
					SelectedCharacter = (int)(Target - CharactersClient);
					MouseRButtonPush = true;
					m_CurTimeATK1 = timeGetTime();
					success = true;
				}
				else
				{
					success = false;
				}
			}
			else
			{
				success = false;
			}
		}
	}
	else
	{
		sCharacter->CurrentSkill = 0;
		SelectedCharacter = -1;
		success = false;
	}
	return success;
}

bool CAIController::UseConditionalSkill(MUSkill_info* Skill, DWORD* cTime)
{
	if (!Skill->Index)
		return false;

	if (Skill->ShutdownDelay)
	{
		if ((timeGetTime() - *cTime) > 1000 * Skill->TimeDelay
			&& runtime_choosetarget()
			&& SkillAt(Skill->Index, sMonsterATK, true))
		{
			*cTime = timeGetTime();
			return true;
		}
		return false;
	}
	if (!Skill->Counter)
		return false;

	bool Success = false;
	int EnemyNum = 0;
	int CounterPre = Skill->CounterPre;

	if (CounterPre)
	{
		if (CounterPre == 1)
			EnemyNum = runtime_choosetarget();
	}
	else
	{
		EnemyNum = runtime_choosetarget();
	}

	switch (Skill->CounterSub)
	{
	case 0:
		if (EnemyNum < 2)
			return false;
		SkillAt(Skill->Index, sMonsterATK, true);
		Success = 1;
		break;
	case 1:
		if (EnemyNum < 3)
			return false;
		SkillAt(Skill->Index, sMonsterATK, true);
		Success = 1;
		break;
	case 2:
		if (EnemyNum < 4)
			return false;
		SkillAt(Skill->Index, sMonsterATK, true);
		Success = 1;
		break;
	case 3:
		if (EnemyNum < 5)
			return false;
		SkillAt(Skill->Index, sMonsterATK, true);
		Success = 1;
		break;
	default:
		return Success;
	}
	return Success;
}

bool CAIController::IsBuffActive(CHARACTER* pCha, int SkillIndex)
{
	if (SkillIndex > 0 && SkillIndex < MAX_SKILLS)
	{
		// This MuBattle branch has no Effect field in SKILL_ATTRIBUTE.  Convert
		// master skills to their base skill and map the official persistent buffs.
		const int baseSkill = gSkillManager.MasterSkillToBaseSkillIndex(SkillIndex);
		eBuffState BuffIndex = eBuffNone;
		switch (baseSkill)
		{
		case AT_SKILL_ATTACK: BuffIndex = eBuff_Attack; break;
		case AT_SKILL_DEFENSE: BuffIndex = eBuff_Defense; break;
		case AT_SKILL_VITALITY: BuffIndex = eBuff_HpRecovery; break;
		case AT_SKILL_INFINITY_ARROW: BuffIndex = eBuff_InfinityArrow; break;
		case AT_SKILL_ALICE_BERSERKER: BuffIndex = eBuff_Berserker; break;
		case AT_SKILL_SWELL_OF_MAGICPOWER: BuffIndex = eBuff_SwellOfMagicPower; break;
		default: break;
		}

		if (pCha && BuffIndex != eBuffNone)
		{
			return g_isCharacterBuff((&pCha->Object), BuffIndex);
		}
	}

	return false;
}

bool CAIController::SelfBuffAt()
{
	DWORD GetTime = timeGetTime();

	if ((GetTime - m_WaitTimeBuffAT) >= 500)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (m_SkillBuff[i] > 0 && !IsBuffActive(sCharacter, m_SkillBuff[i]) && SkillAt(m_SkillBuff[i], sCharacter, false))
			{
				m_WaitTimeBuffAT = GetTime;
				this->WorkAI.intervalSet(0.30);
				return true;
			}
		}
	}
	return false;
}

bool CAIController::runtime_healAt(CHARACTER* Target)
{
	bool success = false;
	DWORD time_current = timeGetTime();

	if ((time_current - m_WaitTimeHealAT) >= 500)
	{
		if (GetSkillIndex(Skill_Heal) >= 0 && SkillAt(Skill_Heal, Target, false))
			success = true;
		else
			success = GetSkillIndex(Skill_Heal_Strengthener) >= 0 && SkillAt(Skill_Heal_Strengthener, Target, false);
		m_WaitTimeHealAT = time_current;
	}
	else
	{
		success = false;
	}
	return success;
}

bool CAIController::runtime_party_healing()
{
	if (this->healParty_Work.hasElapsed())
	{
		for (int i = 0; i < PartyNumber; ++i)
		{
			CHARACTER* pCha = (Party[i].index >= 0 && Party[i].index < MAX_CHARACTERS_CLIENT)
				? &CharactersClient[Party[i].index] : NULL;

			if (pCha && pCha != Hero)
			{
				if (Party[i].stepHP <= (unsigned __int8)(this->healParty.bRate / 10.f) && pCha->Object.Live == 1 && runtime_healAt(pCha))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool CAIController::runtime_party_buff()
{
	DWORD time_current = timeGetTime();

	if (this->buffParty_Work.hasElapsed())
	{
		for (int i = 0; i < PartyNumber; ++i)
		{
			CHARACTER* pCha = (Party[i].index >= 0 && Party[i].index < MAX_CHARACTERS_CLIENT)
				? &CharactersClient[Party[i].index] : NULL;

			if (pCha && pCha != Hero)
			{
				for (int j = 0; j < 3; ++j)
				{
					if (m_PartySkillBuff[j] > 0 && (time_current - this->buff_member_work[j][i] >= (1000 * this->space_of_casting_buff)
						|| !IsBuffActive(pCha, m_PartySkillBuff[j])) && pCha->Object.Live == 1 && SkillAt(m_PartySkillBuff[j], pCha, false))
					{
						this->buff_member_work[j][i] = time_current;
						return true;
					}
				}
			}
		}
	}
	return false;
}

void CAIController::MoveTo(int x, int y)
{
	if (!PlayerReached(x, y) && PathFinding2(sCharacter->PositionX, sCharacter->PositionY, x, y, &sCharacter->Path, 0.0, 2))
	{
		sCharacter->Movement = 1;
		sCharacter->MovementType = 0;
		SendMove(sCharacter, &sCharacter->Object);
	}
}

bool CAIController::PlayerReached(int x, int y)
{
	return (sCharacter->PositionX - x) * (sCharacter->PositionX - x) <= 2
		&& (sCharacter->PositionY - y) * (sCharacter->PositionY - y) <= 2;
}

bool CAIController::RecievePartyRequest(int a2)
{
	return false;
}

bool CAIController::CanUsingEliteManaPotion()
{
	if (m_IsStart)
		return m_EliteManaPotion;
	else
		return false;
}

void CAIController::SetPartyBuff()
{
	for (int i = 0; i < 3; ++i)
	{
		m_PartySkillBuff[i] = 0;
		if (m_SkillBuff[i] > 0 && m_SkillBuff[i] < MAX_SKILLS)
		{
			m_PartySkillBuff[i] = m_SkillBuff[i];
		}
	}
}

bool CAIController::Pack(BYTE* BufferUnpack)
{
	memset(BufferUnpack, 0, 256);

	*(WORD*)BufferUnpack = *(WORD*)BufferUnpack & 0xFFF8 | 1;
	*(WORD*)BufferUnpack = ((this->AttackRange & 0xF) << 8) | *(WORD*)BufferUnpack & 0xF0FF;
	*(WORD*)(BufferUnpack + 24) = this->healPotion.Shutdown & 1 | *(WORD*)(BufferUnpack + 24) & 0xFFFE;
	*(WORD*)(BufferUnpack + 22) = this->healPotion.bRate / 10 & 0xF | *(WORD*)(BufferUnpack + 22) & 0xFFF0;
	*(WORD*)(BufferUnpack + 24) = 2 * (this->healskillLife.Shutdown & 1) | *(WORD*)(BufferUnpack + 24) & 0xFFFD;
	*(WORD*)(BufferUnpack + 22) = 16 * (this->healskillLife.bRate / 10 & 0xF) | *(WORD*)(BufferUnpack + 22) & 0xFF0F;
	*(WORD*)(BufferUnpack + 24) = 4 * (this->healskilldrain.Shutdown & 1) | *(WORD*)(BufferUnpack + 24) & 0xFFFB;
	*(WORD*)(BufferUnpack + 22) = ((this->healskilldrain.bRate / 10 & 0xF) << 8) | *(WORD*)(BufferUnpack + 22) & 0xF0FF;
	*(WORD*)(BufferUnpack + 24) = 8 * (m_DistanceLong & 1) | *(WORD*)(BufferUnpack + 24) & 0xFFF7;
	*(WORD*)(BufferUnpack + 24) = 16 * (this->DistanceReturn & 1) | *(WORD*)(BufferUnpack + 24) & 0xFFEF;
	*(WORD*)(BufferUnpack + 2) = m_DistanceOnSec;
	*(WORD*)(BufferUnpack + 4) = this->SkillIndex;
	*(WORD*)(BufferUnpack + 6) = this->SkillGroup[0].Index;
	*(WORD*)(BufferUnpack + 24) = ((this->SkillGroup[0].ShutdownDelay & 1) << 11) | *(WORD*)(BufferUnpack + 24) & 0xF7FF;
	*(WORD*)(BufferUnpack + 8) = this->SkillGroup[0].TimeDelay;
	*(WORD*)(BufferUnpack + 24) = ((this->SkillGroup[0].Counter & 1) << 12) | *(WORD*)(BufferUnpack + 24) & 0xEFFF;
	*(WORD*)(BufferUnpack + 24) = ((this->SkillGroup[0].CounterPre & 1) << 13) | *(WORD*)(BufferUnpack + 24) & 0xDFFF;
	*(WORD*)(BufferUnpack + 24) = ((this->SkillGroup[0].CounterSub & 3) << 14) | *(WORD*)(BufferUnpack + 24) & 0x3FFF;
	*(WORD*)(BufferUnpack + 10) = this->SkillGroup[1].Index;
	*(WORD*)(BufferUnpack + 26) = this->SkillGroup[1].ShutdownDelay & 1 | *(WORD*)(BufferUnpack + 26) & 0xFFFE;
	*(WORD*)(BufferUnpack + 12) = this->SkillGroup[1].TimeDelay;
	*(WORD*)(BufferUnpack + 26) = 2 * (this->SkillGroup[1].Counter & 1) | *(WORD*)(BufferUnpack + 26) & 0xFFFD;
	*(WORD*)(BufferUnpack + 26) = 4 * (this->SkillGroup[1].CounterPre & 1) | *(WORD*)(BufferUnpack + 26) & 0xFFFB;
	*(WORD*)(BufferUnpack + 26) = 8 * (this->SkillGroup[1].CounterSub & 3) | *(WORD*)(BufferUnpack + 26) & 0xFFE7;
	*(WORD*)(BufferUnpack + 24) = 32 * (this->ShutdownCombo & 1) | *(WORD*)(BufferUnpack + 24) & 0xFFDF;
	*(WORD*)(BufferUnpack + 24) = ((this->ShutdownParty & 1) << 6) | *(WORD*)(BufferUnpack + 24) & 0xFFBF;
	*(WORD*)(BufferUnpack + 24) = ((this->healParty.Shutdown & 1) << 7) | *(WORD*)(BufferUnpack + 24) & 0xFF7F;
	*(WORD*)(BufferUnpack + 22) = ((this->healParty.bRate / 10 & 0xF) << 12) | *(WORD*)(BufferUnpack + 22) & 0xFFF;
	*(WORD*)(BufferUnpack + 24) = ((this->buff_duration_all_work & 1) << 8) | *(WORD*)(BufferUnpack + 24) & 0xFEFF;
	*(WORD*)(BufferUnpack + 14) = this->space_of_casting_buff;
	*(WORD*)(BufferUnpack + 24) = ((this->ShutdownPetCmd & 1) << 9) | *(WORD*)(BufferUnpack + 24) & 0xFDFF;
	*(WORD*)(BufferUnpack + 26) = ((this->PetCmdType & 3) << 8) | *(WORD*)(BufferUnpack + 26) & 0xFCFF;
	*(WORD*)(BufferUnpack + 24) = ((this->ShutdownBuff & 1) << 10) | *(WORD*)(BufferUnpack + 24) & 0xFBFF;
	*(WORD*)(BufferUnpack + 16) = m_SkillBuff[0];
	*(WORD*)(BufferUnpack + 18) = m_SkillBuff[1];
	*(WORD*)(BufferUnpack + 20) = m_SkillBuff[2];
	*(WORD*)BufferUnpack = ((m_PickDistance & 0xF) << 12) | *(WORD*)BufferUnpack & 0xFFF;
	*(WORD*)(BufferUnpack + 26) = 32 * (this->ShutdownRepair & 1) | *(WORD*)(BufferUnpack + 26) & 0xFFDF;
	*(WORD*)(BufferUnpack + 26) = ((this->Picker_work.Near & 1) << 6) | *(WORD*)(BufferUnpack + 26) & 0xFFBF;
	*(WORD*)(BufferUnpack + 26) = ((this->Picker_work.Select & 1) << 7) | *(WORD*)(BufferUnpack + 26) & 0xFF7F;
	*(WORD*)BufferUnpack = 8 * (this->Picker_work.Jewel & 1) | *(WORD*)BufferUnpack & 0xFFF7;
	*(WORD*)BufferUnpack = 16 * (this->Picker_work.Ancient & 1) | *(WORD*)BufferUnpack & 0xFFEF;
	*(WORD*)BufferUnpack = 32 * (this->Picker_work.Excellent & 1) | *(WORD*)BufferUnpack & 0xFFDF;
	*(WORD*)BufferUnpack = ((this->Picker_work.Zen & 1) << 6) | *(WORD*)BufferUnpack & 0xFFBF;
	*(WORD*)BufferUnpack = ((this->Picker_work.Extra & 1) << 7) | *(WORD*)BufferUnpack & 0xFF7F;
	*(WORD*)(BufferUnpack + 26) = ((m_AutoFriend & 1) << 10) | *(WORD*)(BufferUnpack + 26) & 0xFBFF;
	*(WORD*)(BufferUnpack + 26) = ((m_AutoGuildMember & 1) << 11) | *(WORD*)(BufferUnpack + 26) & 0xF7FF;
	*(WORD*)(BufferUnpack + 26) = ((m_EliteManaPotion & 1) << 12) | *(WORD*)(BufferUnpack + 26) & 0xEFFF;

	for (int i = 0; i < MAX_PICK_EXTRA; ++i)
	{
		memcpy(BufferUnpack + 16 * i + 64, this->Picker_work.Name[i], 0x10u);
	}
	return true;
}

bool CAIController::Unpack(BYTE* BufferPack)
{
	if (*BufferPack == 0xFF)
	{
		LoadDefaultConfg();
	}
	else
	{
		this->AttackRange = (*(WORD*)BufferPack >> 8) & 0xF;
		this->healPotion.Shutdown = *((WORD*)BufferPack + 12) & 1;
		this->healPotion.bRate = 10 * (*((WORD*)BufferPack + 11) & 0xF);
		this->healskillLife.Shutdown = (*((WORD*)BufferPack + 12) >> 1) & 1;
		this->healskillLife.bRate = 10 * ((*((WORD*)BufferPack + 11) >> 4) & 0xF);
		this->healskilldrain.Shutdown = (*((WORD*)BufferPack + 12) >> 2) & 1;
		this->healskilldrain.bRate = 10 * ((*((WORD*)BufferPack + 11) >> 8) & 0xF);
		m_DistanceLong = (*((WORD*)BufferPack + 12) >> 3) & 1;
		this->DistanceReturn = (*((WORD*)BufferPack + 12) >> 4) & 1;
		m_DistanceOnSec = *((WORD*)BufferPack + 1);
		this->SkillIndex = *((WORD*)BufferPack + 2);
		this->SkillGroup[0].Index = *((WORD*)BufferPack + 3);
		this->SkillGroup[0].ShutdownDelay = (*((WORD*)BufferPack + 12) >> 11) & 1;
		this->SkillGroup[0].TimeDelay = *((WORD*)BufferPack + 4);
		this->SkillGroup[0].Counter = (*((WORD*)BufferPack + 12) >> 12) & 1;
		this->SkillGroup[0].CounterPre = ((*((WORD*)BufferPack + 12) >> 13) & 1) % 2;
		this->SkillGroup[0].CounterSub = ((unsigned __int8)(*((WORD*)BufferPack + 12) >> 8) >> 6) % 4;
		this->SkillGroup[1].Index = *((WORD*)BufferPack + 5);
		this->SkillGroup[1].ShutdownDelay = *((WORD*)BufferPack + 13) & 1;
		this->SkillGroup[1].TimeDelay = *((WORD*)BufferPack + 6);
		this->SkillGroup[1].Counter = (*((WORD*)BufferPack + 13) >> 1) & 1;
		this->SkillGroup[1].CounterPre = ((*((WORD*)BufferPack + 13) >> 2) & 1) % 2;
		this->SkillGroup[1].CounterSub = ((*((WORD*)BufferPack + 13) >> 3) & 3) % 4;
		this->ShutdownCombo = (*((WORD*)BufferPack + 12) >> 5) & 1;
		this->ShutdownParty = (*((WORD*)BufferPack + 12) >> 6) & 1;
		this->healParty.Shutdown = (*((WORD*)BufferPack + 12) >> 7) & 1;
		this->healParty.bRate = 10 * ((*((WORD*)BufferPack + 11) >> 12) & 0xF);

		this->buff_duration_all_work = (*((WORD*)BufferPack + 12) >> 8) & 1;
		this->space_of_casting_buff = *((WORD*)BufferPack + 7);
		this->ShutdownPetCmd = (*((WORD*)BufferPack + 12) >> 9) & 1;
		this->PetCmdType = ((*((WORD*)BufferPack + 13) >> 8) & 3) % 4;
		this->ShutdownBuff = (*((WORD*)BufferPack + 12) >> 10) & 1;
		m_SkillBuff[0] = *((WORD*)BufferPack + 8);
		m_SkillBuff[1] = *((WORD*)BufferPack + 9);
		m_SkillBuff[2] = *((WORD*)BufferPack + 10);
		m_PickDistance = (unsigned __int8)(*(WORD*)BufferPack >> 8) >> 4;

		this->ShutdownRepair = (*((WORD*)BufferPack + 13) >> 5) & 1;
		this->Picker_work.Near = (*((WORD*)BufferPack + 13) >> 6) & 1;
		this->Picker_work.Select = (*((WORD*)BufferPack + 13) >> 7) & 1;
		this->Picker_work.Jewel = (*(WORD*)BufferPack >> 3) & 1;
		this->Picker_work.Ancient = (*(WORD*)BufferPack >> 4) & 1;
		this->Picker_work.Excellent = (*(WORD*)BufferPack >> 5) & 1;
		this->Picker_work.Zen = (*(WORD*)BufferPack >> 6) & 1;
		this->Picker_work.Extra = (*(WORD*)BufferPack >> 7) & 1;
		m_AutoFriend = (*((WORD*)BufferPack + 13) >> 10) & 1;
		m_AutoGuildMember = (*((WORD*)BufferPack + 13) >> 11) & 1;
		m_EliteManaPotion = (*((WORD*)BufferPack + 13) >> 12) & 1;

		for (int i = 0; i < MAX_PICK_EXTRA; ++i)
		{
			memset(this->Picker_work.Name[i], 0, 0x11u);
			memcpy(this->Picker_work.Name[i], &BufferPack[16 * i + 64], 0x10u);
		}

		if (this->AttackRange < 1 || this->AttackRange > 8)
			this->AttackRange = 8;
		if (m_PickDistance < 1 || m_PickDistance > 8)
			m_PickDistance = 8;

		RANGE_FIXED(&this->healPotion, 0, 100, 50);
		RANGE_FIXED(&this->healskillLife, 0, 100, 50);
		RANGE_FIXED(&this->healskilldrain, 0, 100, 50);
		RANGE_FIXED(&this->healParty, 0, 100, 50);
	}
	return true;
}

std::vector<std::string> CAIController::splitByPlus(const std::string& str)
{
	std::vector<std::string> tokens;
	size_t start = 0, end;

	while ((end = str.find('+', start)) != std::string::npos)
	{
		tokens.push_back(str.substr(start, end - start)); 
		start = end + 1;
	}
	tokens.push_back(str.substr(start));

	return tokens;
}

bool CAIController::matchesPattern(std::string text, std::string pattern)
{
	size_t pos = 0;
	std::vector<std::string> patternTokens = this->splitByPlus(pattern);

	for (const std::string& token : patternTokens)
	{
		pos = text.find(token, pos);
		if (pos == std::string::npos)
		{
			return false;
		}
		pos += token.length();
	}

	return true;
}

#include "stdafx.h"
#include "ItemMove.h"
#include "ItemManager.h"
#include "NewUICommon.h"
#include "NewUISystem.h"
#include "NewUIHotKey.h"
#include "wsclientinline.h"
#include "MixMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cItemMove::cItemMove() // OK
{
	ItemMove = false;
	SourceFlag = -1;
	SourceSlot = -1;
	TargetSlot = -1;
}

cItemMove::~cItemMove() // OK
{

}

void cItemMove::ResetMouseRButton()
{
	MouseRButton = false;
	MouseRButtonPop = false;
	MouseRButtonPush = false;
}

bool cItemMove::CheckCanMove()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MIXINVENTORY) || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADE))
	{
		return true;
	}

	return false;
}

int cItemMove::GetFlag()
{
	return g_MixRecipeMgr.GetMixInventoryEquipmentIndex();
}

void cItemMove::RightClickMoveItem()
{
	if (!SEASON3B::IsPress(VK_RBUTTON) || g_pPickedItem || ItemMove)
	{
		return;
	}

	if (!g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY))
	{
		return;
	}

	SEASON3B::CNewUIInventoryCtrl* pNewInventoryCtrl = g_pMixInventory->GetInventoryCtrl();

	SEASON3B::CNewUIInventoryCtrl* tNewInventoryCtrl = g_pTrade->GetMyInvenCtrl();

	SEASON3B::CNewUIInventoryCtrl* cNewInventoryCtrl = g_pMyInventory->GetInventoryCtrl();

	ItemMove = false;

	SourceFlag = -1;

	TargetSlot = -1;

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADE))
	{
		SourceSlot = tNewInventoryCtrl->FindItemptIndex(MouseX, MouseY);

		if (SourceSlot != -1)
		{
			ITEM* TradeItemStruct = tNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);

			if (TradeItemStruct)
			{
				ITEM_ATTRIBUTE* lpItemInfo = &ItemAttribute[TradeItemStruct->Type];

				TargetSlot = g_pMyInventory->FindEmptySlot(lpItemInfo->Width, lpItemInfo->Height);

				if (SourceSlot != -1 && TargetSlot != -1)
				{
					g_pTrade->SendRequestItemToMyInven(TradeItemStruct, SourceSlot, MAX_EQUIPMENT_INDEX + TargetSlot);

					ResetMouseRButton();

					ItemMove = true;

					SourceFlag = 2;
				}
			}
			return;
		}

		int InventoryMain = cNewInventoryCtrl->FindItemptIndex(MouseX, MouseY);

		if (this->CheckCanMove())
		{
			ITEM* ItemStruct = cNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);

			if (ItemStruct)
			{
				ITEM_ATTRIBUTE* lpItemInfo = &ItemAttribute[ItemStruct->Type];

				SourceSlot = MAX_EQUIPMENT + InventoryMain;

				TargetSlot = tNewInventoryCtrl->FindEmptySlot(lpItemInfo->Width, lpItemInfo->Height);

				if (SourceSlot != -1 && TargetSlot != -1)
				{
					g_pTrade->SendRequestItemToTrade(ItemStruct, SourceSlot, TargetSlot);

					ResetMouseRButton();

					ItemMove = true;

					SourceFlag = 0;
				}
			}
		}
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MIXINVENTORY))
	{
		SourceSlot = pNewInventoryCtrl->FindItemptIndex(MouseX, MouseY);

		if (SourceSlot != -1)
		{
			ITEM* MixItemStruct = pNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);

			if (MixItemStruct)
			{
				ITEM_ATTRIBUTE* lpItemInfo = &ItemAttribute[MixItemStruct->Type];

				TargetSlot = g_pMyInventory->FindEmptySlot(lpItemInfo->Width, lpItemInfo->Height);

				if (SourceSlot != -1 && TargetSlot != -1)
				{
					SendRequestEquipmentItem(this->GetFlag(), SourceSlot, MixItemStruct, 0, MAX_EQUIPMENT + TargetSlot);

					ResetMouseRButton();

					ItemMove = true;

					SourceFlag = 1;
				}
			}
			return;
		}

		int InventoryMain = cNewInventoryCtrl->FindItemptIndex(MouseX, MouseY);

		if (this->CheckCanMove())
		{
			ITEM* ItemStruct = cNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);

			if (ItemStruct)
			{
				if (g_pMixInventory->GetMixState() == g_pMixInventory->MIX_STATE::MIX_READY && g_MixRecipeMgr.IsMixSource(ItemStruct))
				{
					ITEM_ATTRIBUTE* lpItemInfo = &ItemAttribute[ItemStruct->Type];

					SourceSlot = MAX_EQUIPMENT + InventoryMain;

					TargetSlot = pNewInventoryCtrl->FindEmptySlot(lpItemInfo->Width, lpItemInfo->Height);

					if (SourceSlot != -1 && TargetSlot != -1)
					{
						SendRequestEquipmentItem(0, SourceSlot, ItemStruct, this->GetFlag(), TargetSlot);

						ResetMouseRButton();

						ItemMove = true;

						SourceFlag = 0;
					}
				}
			}
		}
	}
}

void cItemMove::RightClickDeleteItem(BYTE result) // OK
{
	if (ItemMove == true)
	{
		if (result != 0xFF)
		{
			if (SourceFlag == 0)
			{
				if (SourceSlot >= 0 && SourceSlot < MAX_EQUIPMENT)
				{
					g_pMyInventory->UnequipItem(SourceSlot);
				}
				else if (SourceSlot >= MAX_EQUIPMENT)
				{
					g_pMyInventory->DeleteItem(SourceSlot - MAX_EQUIPMENT);
				}
			}
			else if (SourceFlag == 2)
			{
				g_pTrade->DeleteMyTrade(SourceSlot);
			}
			else
			{
				g_pMixInventory->DeleteItem(SourceSlot);
			}
		}

		ItemMove = false;
	}
}

BYTE cItemMove::GetInventoryWearSlot(int ItemIndex)
{
	int slot = 0xFF;

	ITEM_ATTRIBUTE* lpItemInfo = &ItemAttribute[ItemIndex];

	BYTE Class = CharacterAttribute->Class;

	BYTE ChangeUp = GetChangeUp(Class);

	ITEM* ItemR = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT];
	ITEM* ItemL = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];
	ITEM* RingR = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];
	ITEM* RingL = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];

	if (lpItemInfo->m_byItemSlot >= 0 && lpItemInfo->m_byItemSlot < MAX_EQUIPMENT)
	{
		if (lpItemInfo->TwoHand == 0)
		{
			if (lpItemInfo->m_byItemSlot == 0 && ItemR->Type != 0xFFFF && ItemL->Type == 0xFFFF)
			{
				ITEM_ATTRIBUTE* lpItemRight = &ItemAttribute[ItemR->Type];

				if (lpItemRight->TwoHand == 0)
				{
					slot = 1;
				}
			}
			else if (lpItemInfo->m_byItemSlot == 1 && ItemL->Type != 0xFFFF && ItemR->Type == 0xFFFF)
			{
				ITEM_ATTRIBUTE* lpItemLeft = &ItemAttribute[ItemL->Type];

				if (lpItemLeft->TwoHand == 0)
				{
					slot = 1;
				}
			}
			else if (lpItemInfo->m_byItemSlot == 10 && RingR->Type != 0xFFFF && RingL->Type == 0xFFFF)
			{
				slot = 11;
			}
		}

		if (CharacterMachine->Equipment[lpItemInfo->m_byItemSlot].Type == 0xFFFF)
		{
			slot = lpItemInfo->m_byItemSlot;
		}
	}

	bool bEquipable = false;

	if (lpItemInfo->RequireClass[Class])
	{
		bEquipable = true;
	}
	else if (Class == 3 && lpItemInfo->RequireClass[0] && lpItemInfo->RequireClass[1])
	{
		bEquipable = true;
	}

	if (lpItemInfo->RequireClass[Class] > ChangeUp)
	{
		bEquipable = false;
	}

	if (bEquipable == true)
	{
		bEquipable = false;

		if (lpItemInfo->m_byItemSlot == slot)
		{
			bEquipable = true;
		}
		else if (lpItemInfo->m_byItemSlot == 0 && slot == 1)
		{
			if ((Class == 1 || Class == 3) && lpItemInfo->TwoHand == 0)
			{
				bEquipable = true;
			}
			else if (Class == 5 && !(ItemIndex >= gItemManager->GetItem(5, 0) && ItemIndex < gItemManager->GetItem(6, 0)))
			{
				bEquipable = true;
			}
		}
		else if (lpItemInfo->m_byItemSlot == 10 && slot == 11)
		{
			bEquipable = true;
		}

		if (Class == 2)
		{
			if (lpItemInfo->m_byItemSlot == 0 && ItemL->Type != gItemManager->GetItem(4, 7) && (ItemL->Type >= gItemManager->GetItem(4, 0) && ItemL->Type < gItemManager->GetItem(5, 0)) && ItemIndex != gItemManager->GetItem(4, 15))
			{
				bEquipable = false;
			}
		}

		if (Class == 3 && lpItemInfo->m_byItemSlot == 2)
		{
			bEquipable = false;
		}
	}

	return ((bEquipable) ? slot : 0xFF);
}
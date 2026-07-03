#include "stdafx.h"
#include "CustomRanking.h"
#include "CustomWing.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Item.h"
#include "MemScript.h"
#include "Path.h"
#include "Pets.h"
#include "SetItemOption.h"
#include "SetItemType.h"
#include "User.h"
#include "Util.h"

CCustomRanking gCustomRanking;

void CCustomRanking::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	memset(this->r_Data, 0, sizeof(this->r_Data));
	this->m_count = 0;

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (section == 0)
			{
				if (strcmp("end", lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				int index = lpMemScript->GetNumber();

				if (index < 0 || index >= MAX_RANK)
				{
					throw 1;
				}

				this->r_Data[index].index = index;
				strcpy_s(this->r_Data[index].Name, lpMemScript->GetAsString());
				strcpy_s(this->r_Data[index].Col1, lpMemScript->GetAsString());
				strcpy_s(this->r_Data[index].Col2, lpMemScript->GetAsString());
				this->m_count = max(this->m_count, index + 1);
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomRanking::SendCharacterInfo(int Index, int characterIndex, const char* name, BYTE dbClass,
	BYTE ctlCode, const BYTE* charSet, WORD petIndex, WORD wingIndex, BYTE result)
{
	if (gObjIsConnected(Index) == false)
	{
		return;
	}

	PMSG_RANKING_CHARACTER_INFO_SEND pMsg;
	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.header.set(0xF3, 0xEF, sizeof(pMsg));
	pMsg.characterIndex = characterIndex;
	pMsg.result = result;
	pMsg.PlayerClass = dbClass;
	pMsg.CtlCode = ctlCode;
	pMsg.PetIndex = petIndex;
	pMsg.WingIndex = wingIndex;

	if (charSet != 0)
	{
		memcpy(pMsg.Equipment, &charSet[1], sizeof(pMsg.Equipment));
	}

	if (name != 0)
	{
		memcpy(pMsg.Name, name, sizeof(pMsg.Name));
	}

	DataSend(Index, (LPBYTE)&pMsg, pMsg.header.size);
}

void CCustomRanking::GCReqRankingPlayer(int Index, PMSG_RANKING_CHARACTER_INFO_RECV* lpMsg)
{
	if (gObjIsConnected(Index) == false)
	{
		return;
	}

	char characterName[11] = { 0 };
	memcpy(characterName, lpMsg->name, sizeof(lpMsg->name));

	if (characterName[0] == 0)
	{
		return;
	}

	LPOBJ lpTarget = gObjFind(characterName);

	if (lpTarget != 0 && gObjIsConnected(lpTarget->Index) != false)
	{
		WORD petIndex = 0;
		WORD wingIndex = 0;
		BYTE ctlCode = 0;

		if (lpTarget->Authority == 32)
		{
			ctlCode = 32;
		}
		else if (lpTarget->Authority == 2)
		{
			ctlCode = 8;
		}

		if (lpTarget->Inventory[7].IsItem() != 0 &&
			gCustomWing.CheckCustomWingByItem(lpTarget->Inventory[7].m_Index) != 0)
		{
			wingIndex = (WORD)(lpTarget->Inventory[7].m_Index - GET_ITEM(12, 0));
		}

		if (lpTarget->Inventory[8].IsItem() != 0 &&
			gCustomPet.CheckCustomPetByItem(lpTarget->Inventory[8].m_Index) != 0)
		{
			petIndex = (WORD)(lpTarget->Inventory[8].m_Index - GET_ITEM(13, 0));
		}

		this->SendCharacterInfo(Index, lpMsg->characterIndex, characterName, lpTarget->DBClass,
			ctlCode, lpTarget->CharSet, petIndex, wingIndex, 1);
		return;
	}

	SDHP_RANKING_CHARACTER_INFO_SEND pMsg;
	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.header.set(0xF4, 0x01, sizeof(pMsg));
	pMsg.index = Index;
	pMsg.characterIndex = lpMsg->characterIndex;
	memcpy(pMsg.name, characterName, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCustomRanking::GDCustomRankingPlayerRecv(SDHP_RANKING_CHARACTER_INFO_RECV* lpMsg)
{
	if (gObjIsConnected(lpMsg->index) == false)
	{
		return;
	}

	if (lpMsg->result == 0)
	{
		this->SendCharacterInfo(lpMsg->index, lpMsg->characterIndex, lpMsg->name, 0, 0, 0, 0, 0, 0);
		return;
	}

	BYTE charSet[RANKING_CHARSET_SIZE];
	WORD petIndex = 0;
	WORD wingIndex = 0;

	this->BuildCharacterSet(lpMsg->DBClass, lpMsg->Inventory, charSet, &petIndex, &wingIndex);
	this->SendCharacterInfo(lpMsg->index, lpMsg->characterIndex, lpMsg->name, lpMsg->DBClass,
		lpMsg->CtlCode, charSet, petIndex, wingIndex, 1);
}

void CCustomRanking::GCReqRankingCount(int Index, PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg)
{
	if (gObjIsConnected(Index) == false)
	{
		return;
	}

	PMSG_CUSTOM_RANKING_COUNT_SEND pMsg;
	pMsg.header.set(0xF3, 0xE7, sizeof(pMsg));
	pMsg.count = (this->m_count > MAX_RANK) ? MAX_RANK : this->m_count;

	DataSend(Index, (LPBYTE)&pMsg, pMsg.header.size);
}

void CCustomRanking::GCReqRanking(int Index, PMSG_CUSTOM_RANKING_RECV* lpMsg)
{
	if (gObjIsConnected(Index) == false || this->m_count <= 0 || lpMsg->type >= this->m_count)
	{
		return;
	}

	SDHP_CUSTOM_RANKING_SEND pMsg;
	pMsg.header.set(0xF4, 0x00, sizeof(pMsg));
	pMsg.index = Index;
	pMsg.type = lpMsg->type;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCustomRanking::GDCustomRankingRecv(BYTE* ReceiveBuffer)
{
	SDHP_CUSTOM_RANKING_RECV* Data = (SDHP_CUSTOM_RANKING_RECV*)ReceiveBuffer;

	if (gObjIsConnected(Data->index) == false || Data->type < 0 || Data->type >= this->m_count)
	{
		return;
	}

	BYTE send[4096];
	PMSG_CUSTOM_RANKING_SEND pMsg;
	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.header.set(0xF3, 0xE6, 0);
	pMsg.RankIndex = Data->type;
	memcpy(pMsg.rankname, this->r_Data[Data->type].Name, sizeof(pMsg.rankname));
	memcpy(pMsg.col1, this->r_Data[Data->type].Col1, sizeof(pMsg.col1));
	memcpy(pMsg.col2, this->r_Data[Data->type].Col2, sizeof(pMsg.col2));

	int size = sizeof(pMsg);
	int offset = sizeof(SDHP_CUSTOM_RANKING_RECV);
	pMsg.count = 0;

	for (int n = 0; n < Data->count; n++)
	{
		CUSTOM_RANKING_DATA* Data2 = (CUSTOM_RANKING_DATA*)(ReceiveBuffer + offset);
		CUSTOM_RANKING_DATA info;
		memset(&info, 0, sizeof(info));

		memcpy(info.szName, Data2->szName, sizeof(info.szName));
		info.Score = Data2->Score;
		info.Vip = Data2->Vip;
		info.Class = Data2->Class;

		if ((size + sizeof(info)) >= sizeof(send))
		{
			break;
		}

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
		offset += sizeof(CUSTOM_RANKING_DATA);
		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);
	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(Data->index, send, size);
}

void CCustomRanking::BuildCharacterSet(BYTE dbClass, BYTE inventory[RANKING_INVENTORY_SLOTS][5],
	BYTE charSet[RANKING_CHARSET_SIZE], WORD* petIndex, WORD* wingIndex)
{
	memset(charSet, 0, RANKING_CHARSET_SIZE);
	*petIndex = 0;
	*wingIndex = 0;

	charSet[0] = (dbClass % 16) * 16;
	charSet[0] -= charSet[0] / 32;
	charSet[0] += (dbClass / 16) * 32;

	WORD itemIndex[RANKING_INVENTORY_SLOTS];
	bool itemExists[RANKING_INVENTORY_SLOTS];

	for (int i = 0; i < RANKING_INVENTORY_SLOTS; i++)
	{
		itemExists[i] = !(inventory[i][0] == 0xFF && (inventory[i][2] & 0x80) == 0x80 &&
			(inventory[i][4] & 0xF0) == 0xF0);

		if (itemExists[i] == false)
		{
			itemIndex[i] = ((i == 0 || i == 1) ? 0xFFFF : 0x1FF);
		}
		else
		{
			itemIndex[i] = inventory[i][0] + ((inventory[i][2] & 0x80) * 2) +
				((inventory[i][4] & 0xF0) * 32);

			if (i != 0 && i != 1)
			{
				itemIndex[i] %= MAX_ITEM_TYPE;
			}
		}
	}

	charSet[1] = itemIndex[0] % 256;
	charSet[12] |= (itemIndex[0] / 16) & 0xF0;
	charSet[2] = itemIndex[1] % 256;
	charSet[13] |= (itemIndex[1] / 16) & 0xF0;
	charSet[3] |= (itemIndex[2] & 0x0F) << 4;
	charSet[9] |= (itemIndex[2] & 0x10) << 3;
	charSet[13] |= (itemIndex[2] & 0x1E0) >> 5;
	charSet[3] |= itemIndex[3] & 0x0F;
	charSet[9] |= (itemIndex[3] & 0x10) << 2;
	charSet[14] |= (itemIndex[3] & 0x1E0) >> 1;
	charSet[4] |= (itemIndex[4] & 0x0F) << 4;
	charSet[9] |= (itemIndex[4] & 0x10) << 1;
	charSet[14] |= (itemIndex[4] & 0x1E0) >> 5;
	charSet[4] |= itemIndex[5] & 0x0F;
	charSet[9] |= itemIndex[5] & 0x10;
	charSet[15] |= (itemIndex[5] & 0x1E0) >> 1;
	charSet[5] |= (itemIndex[6] & 0x0F) << 4;
	charSet[9] |= (itemIndex[6] & 0x10) >> 1;
	charSet[15] |= (itemIndex[6] & 0x1E0) >> 5;

	int level = 0;
	BYTE table[7] = { 1, 0, 6, 5, 4, 3, 2 };
	int setCount[MAX_SET_ITEM_OPTION] = { 0 };
	int firstWeaponSet = -1;

	for (int i = 0; i < 7; i++)
	{
		if (itemExists[i] == false)
		{
			continue;
		}

		int itemLevel = (inventory[i][1] / 8) & 0x0F;
		level |= (((itemLevel > 0 ? itemLevel - 1 : 0) / 2) << (i * 3));
		charSet[10] |= ((inventory[i][2] & 0x3F) ? 2 : 0) << table[i];
		charSet[11] |= ((inventory[i][3] & 0x03) ? 2 : 0) << table[i];

		int setType = inventory[i][3] & 0x03;
		int fullItemIndex = ((i == 0 || i == 1) ? itemIndex[i] : GET_ITEM(i + 5, itemIndex[i]));

		if (setType != 0 && gSetItemType.CheckSetItemType(fullItemIndex) != 0)
		{
			int optionIndex = gSetItemType.GetSetItemOptionIndex(fullItemIndex, setType - 1);

			if (optionIndex > 0 && optionIndex < MAX_SET_ITEM_OPTION)
			{
				if (i == 0)
				{
					firstWeaponSet = optionIndex;
					setCount[optionIndex]++;
				}
				else if (i != 1 || optionIndex != firstWeaponSet || itemIndex[0] != itemIndex[1])
				{
					setCount[optionIndex]++;
				}
			}
		}
	}

	for (int i = 1; i < MAX_SET_ITEM_OPTION; i++)
	{
		if (setCount[i] > 0 && (setCount[i] - 1) == gSetItemOption.GetSetItemMaxOptionCount(i))
		{
			charSet[11] |= 1;
			break;
		}
	}

	charSet[6] = level >> 16;
	charSet[7] = level >> 8;
	charSet[8] = level;

	if (itemIndex[7] == 0x1FF)
		charSet[5] |= 12;
	else if (itemIndex[7] <= 2)
		charSet[5] |= itemIndex[7] << 2;
	else if (itemIndex[7] >= 3 && itemIndex[7] <= 6)
	{
		charSet[5] |= 12;
		charSet[9] |= itemIndex[7] - 2;
	}
	else if (itemIndex[7] == 30)
	{
		charSet[5] |= 12;
		charSet[9] |= 5;
	}
	else if (itemIndex[7] >= 36 && itemIndex[7] <= 40)
	{
		charSet[5] |= 12;
		charSet[16] |= (itemIndex[7] - 35) << 2;
	}
	else if (itemIndex[7] == 41)
	{
		charSet[5] |= 12;
		charSet[9] |= 6;
	}
	else if (itemIndex[7] == 42)
	{
		charSet[5] |= 12;
		charSet[16] |= 28;
	}
	else if (itemIndex[7] == 43)
	{
		charSet[5] |= 12;
		charSet[16] |= 24;
	}
	else if (itemIndex[7] >= 130 && itemIndex[7] <= 135)
	{
		charSet[5] |= 12;
		charSet[17] |= (itemIndex[7] - 129) << 5;
	}
	else if (gCustomWing.CheckCustomWingByItem(GET_ITEM(12, itemIndex[7])) != 0)
	{
		charSet[5] |= 12;
		charSet[17] |= (gCustomWing.GetCustomWingIndex(GET_ITEM(12, itemIndex[7])) + 1) << 1;
		*wingIndex = itemIndex[7];
	}

	if (itemIndex[8] == 0x1FF)
		charSet[5] |= 3;
	else if (itemIndex[8] <= 2)
		charSet[5] |= itemIndex[8];
	else if (itemIndex[8] == 3)
	{
		charSet[5] |= 3;
		charSet[10] |= 1;
	}
	else if (itemIndex[8] == 4)
	{
		charSet[5] |= 3;
		charSet[12] |= 1;
	}
	else if (itemIndex[8] == 37)
	{
		charSet[5] |= 3;
		charSet[10] &= 0xFE;
		charSet[12] &= 0xFE;
		charSet[12] |= 4;

		if ((inventory[8][2] & 1) != 0)
			charSet[16] |= 1;
		else if ((inventory[8][2] & 2) != 0)
			charSet[16] |= 2;
		else if ((inventory[8][2] & 4) != 0)
			charSet[17] |= 1;
	}
	else if (itemIndex[8] == 64 || itemIndex[8] == 65 || itemIndex[8] == 67)
		charSet[16] |= (itemIndex[8] - 63) << 5;
	else if (itemIndex[8] == 80)
		charSet[16] |= 0xE0;
	else if (itemIndex[8] == 106)
		charSet[16] |= 0xA0;
	else if (itemIndex[8] == 123)
		charSet[16] |= 0x60;
	else if (gCustomPet.CheckCustomPetByItem(GET_ITEM(13, itemIndex[8])) != 0)
		*petIndex = itemIndex[8];
}

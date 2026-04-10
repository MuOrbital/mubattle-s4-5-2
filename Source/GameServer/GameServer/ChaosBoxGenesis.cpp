#include "stdafx.h"
#include "ChaosBoxGenesis.h"
#include "ItemManager.h"
#include "GameMain.h"
#include "Util.h"
#include "Notice.h"
#include "LuaGameServer.h"
//#include "DeathSystem.h"

#ifdef CHAOS_MACHINE_GENESIS

CChaosBoxGenesis gChaosBoxGenesis;

CChaosBoxGenesis::CChaosBoxGenesis()
{

}

CChaosBoxGenesis::~CChaosBoxGenesis()
{

}

void CChaosBoxGenesis::Clear(LPOBJ lpObj)
{
	for (int n = 0; n < CHAOS_BOX_GENESIS_SIZE; n++)
	{
		lpObj->ChaosBoxGenesis[n].Clear();
	}
}

void CChaosBoxGenesis::ChaosBoxGenesisGetItemSaveSend(LPOBJ lpObj)
{
	if (gObjIsAccountValid(lpObj->Index, lpObj->Account) == 0)
	{
		return;
	}

	SDHP_CHAOS_BOX_GENESIS_GET_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x05, 0x60, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CChaosBoxGenesis::ChaosBoxGenesisGetItemSaveRecv(SDHP_CHAOS_BOX_GENESIS_GET_ITEM_SAVE_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[ChaosBoxGenesisGetItemSaveRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (_stricmp(lpMsg->name, lpObj->Name) != 0) {
		return;
	}

	if (lpMsg->Itens == 0) {
		return;
	}

	BYTE Clean = 0;

	for (int n = 0; n < CHAOS_BOX_GENESIS_SIZE; n++)
	{
		CItem item;

		lpObj->ChaosBoxGenesis[n].Clear();

		if (gItemManager.ConvertItemByte(&item, lpMsg->ChaosBoxItem[n]) != 0) {
			gItemManager.ChaosBoxGenesisAddItem(lpObj->Index, item, n);
			Clean++;
		}
	}

	if (Clean == 0) {
		return;
	}

	lpObj->ChaosBoxGenesisUsing = 1;

	lpObj->ChaosBoxGenesisState = lpMsg->State;

	lpObj->ChaosBoxGenesisSelectedKey = lpMsg->selectedKey;

	lpObj->ChaosBoxGenesisSelectedComb = lpMsg->selectedComb;

	//gDeathSystem.GDChaosBoxGenesisGetInformation(lpObj->Index);
}

void CChaosBoxGenesis::ChaosboxGenesisSaveItemSend(LPOBJ lpObj, int Check, bool SaveAnyway)
{
	if (lpObj->ChaosBoxGenesisUsing == 0 && !SaveAnyway)
	{
		return;
	}

	BYTE Clean = 0;

	SDHP_CHAOS_BOX_GENESS_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x05, 0x61, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	pMsg.State = lpObj->ChaosBoxGenesisState;

	pMsg.selectedKey = lpObj->ChaosBoxGenesisSelectedKey;

	pMsg.selectedComb = lpObj->ChaosBoxGenesisSelectedComb;

	for (int n = 0; n < CHAOS_BOX_GENESIS_SIZE; n++) {
		gItemManager.DBItemByteConvert(pMsg.ChaosBoxItem[n], &lpObj->ChaosBoxGenesis[n]);

		if (lpObj->ChaosBoxGenesis[n].IsItem()) {
			Clean++;
		}
	}

	if (Check == 0) {
		Clean = 1;
	}

	pMsg.Clean = Clean;

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CChaosBoxGenesis::ChaosBoxGenesisStateSend(int aIndex, int state)
{
	PMSG_CHAOS_BOX_GENESIS_STATE_SEND pMsg;

	pMsg.header.set(0xEF, 0x23, sizeof(pMsg));

	pMsg.state = state;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CChaosBoxGenesis::ChaosBoxGenesisSendOpenClient(int aIndex, int State, int SelectedKey, int SelectedComb)
{
	SDHP_CHAOS_BOX_GENESIS_OPEN_CLIENT_SEND pMsg;

	pMsg.header.set(0xEF, 0x20, sizeof(pMsg));

	pMsg.state = State;

	pMsg.selectedKey = SelectedKey;

	pMsg.selectedComb = SelectedComb;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CChaosBoxGenesis::CGChaosBoxGenesisChaosMix(SDHP_CHAOS_BOX_GENESIS_MIX_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOSGENESIS)
	{
		return;
	}

	if (!lpObj->ChaosBoxGenesisUsing) {
		return;
	}

	if (lpObj->ChaosBoxGenesisSelectedKey == 0 || lpObj->ChaosBoxGenesisSelectedComb == 0) {
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "Você precisa selecionar uma combinação primeiro!");
		return;
	}

	if (lpObj->ChaosBoxGenesisState != 0) {
		return;
	}

	if (lpObj->ChaosBoxGenesisLock != 0) {
		return;
	}

	lpObj->ChaosBoxGenesisLock = 1;

	lpObj->ChaosBoxGenesisUsed = 1;

	gLuaGameServer.ChaosMachineGenesisMix(aIndex);

	lpObj->ChaosBoxGenesisLock = 0;
}

void CChaosBoxGenesis::CGChaosBoxGenesisSetInfoRecv(SDHP_CHAOS_BOX_GENESIS_SET_INFO_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOSGENESIS)
	{
		return;
	}

	lpObj->ChaosBoxGenesisSelectedKey = lpMsg->selectedKey;

	lpObj->ChaosBoxGenesisSelectedComb = lpMsg->selectedComb;
}

void CChaosBoxGenesis::CGChaosBoxGenesisCloseRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOSGENESIS)
	{
		return;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;

	if (this->CheckChaosBoxGenesisEmpty(lpObj))
	{
		if (lpObj->ChaosBoxGenesisUsed)
		{
			this->ChaosboxGenesisSaveItemSend(lpObj, 1);
			lpObj->ChaosBoxGenesisUsed = 0;
		}

		lpObj->ChaosBoxGenesisLock = 0;
		lpObj->ChaosBoxGenesisSelectedComb = 0;
		lpObj->ChaosBoxGenesisSelectedKey = 0;
		lpObj->ChaosBoxGenesisState = 0;
		lpObj->ChaosBoxGenesisUsing = 0;
	}
}

void CChaosBoxGenesis::GCChaosBoxGenesisItemListSend(LPOBJ lpObj)
{
	BYTE send[4096];

	PMSG_CHAOS_BOX_GENESIS_LIST_SEND pMsg;

	pMsg.header.set(0xEF, 0x21, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_CHAOS_BOX_GENESIS_ITEM_INFO info;

	for (int n = 0; n < CHAOS_BOX_GENESIS_SIZE; n++)
	{
		if (lpObj->ChaosBoxGenesis[n].IsItem() == 0)
		{
			continue;
		}

		info.slot = n;

		gItemManager.ItemByteConvert(info.ItemInfo, lpObj->ChaosBoxGenesis[n]);

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);

	//gDeathSystem.SendChaosBoxGenesis(lpObj);
}

bool CChaosBoxGenesis::CheckChaosBoxGenesisEmpty(LPOBJ lpObj)
{
	for (int i = 0; i < CHAOS_BOX_GENESIS_SIZE; i++) {
		if (lpObj->ChaosBoxGenesis[i].IsItem()) {
			return 0;
		}
	}

	return 1;
}

#endif
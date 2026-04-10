#pragma once

#ifdef CHAOS_MACHINE_GENESIS

#include "User.h"
#include "Protocol.h"
#include "ItemManager.h"

//**********************************************//
//*********** GameServer -> Client  ************//
//**********************************************//
struct SDHP_CHAOS_BOX_GENESIS_OPEN_CLIENT_SEND
{
	PSBMSG_HEAD header; //
	int state;
	int selectedKey;
	int selectedComb;
};

struct PMSG_CHAOS_BOX_GENESIS_LIST_SEND
{
	PSWMSG_HEAD header; //
	BYTE count;
};

struct PMSG_CHAOS_BOX_GENESIS_ITEM_INFO
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_CHAOS_BOX_GENESIS_STATE_SEND
{
	PSBMSG_HEAD header; //
	int state;
};

//**********************************************//
//*********** Client -> GameServer  ************//
//**********************************************//
struct SDHP_CHAOS_BOX_GENESIS_SET_INFO_RECV
{
	PSBMSG_HEAD header; //
	int state;
	int selectedKey;
	int selectedComb;
};

struct SDHP_CHAOS_BOX_GENESIS_MIX_RECV
{
	PSBMSG_HEAD header; //
	int state;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_CHAOS_BOX_GENESIS_GET_ITEM_SAVE_SEND
{
	PSBMSG_HEAD header; // C2:05:60
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CHAOS_BOX_GENESS_ITEM_SAVE_SEND
{
	PSWMSG_HEAD header; // C2:05:61
	WORD index;
	char account[11];
	char name[11];
	BYTE ChaosBoxItem[CHAOS_BOX_GENESIS_SIZE][MAX_ITEM_BYTE];
	BYTE State;
	BYTE Clean;
	int selectedKey;
	int selectedComb;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CHAOS_BOX_GENESIS_GET_ITEM_SAVE_RECV
{
	PSWMSG_HEAD header; // C2:05:60
	WORD index;
	char account[11];
	char name[11];
	BYTE Itens;
	BYTE State;
	int selectedKey;
	int selectedComb;
	BYTE ChaosBoxItem[CHAOS_BOX_GENESIS_SIZE][MAX_ITEM_BYTE];
};

class CChaosBoxGenesis {
public:
	CChaosBoxGenesis();
	~CChaosBoxGenesis();

	void Clear(LPOBJ lpObj);
	void ChaosBoxGenesisGetItemSaveSend(LPOBJ lpObj);
	void ChaosBoxGenesisGetItemSaveRecv(SDHP_CHAOS_BOX_GENESIS_GET_ITEM_SAVE_RECV* lpMsg);
	void ChaosboxGenesisSaveItemSend(LPOBJ lpObj, int Check, bool SaveAnyway = false);
	void ChaosBoxGenesisStateSend(int aIndex, int state);
	void ChaosBoxGenesisSendOpenClient(int aIndex, int State, int SelectedKey, int SelectedComb);
	void CGChaosBoxGenesisClose(int aIndex);
	void CGChaosBoxGenesisChaosMix(SDHP_CHAOS_BOX_GENESIS_MIX_RECV* lpMsg, int aIndex);
	void CGChaosBoxGenesisSetInfoRecv(SDHP_CHAOS_BOX_GENESIS_SET_INFO_RECV* lpMsg, int aIndex);
	void CGChaosBoxGenesisCloseRecv(int aIndex);
	void GCChaosBoxGenesisItemListSend(LPOBJ lpObj);
	bool CheckChaosBoxGenesisEmpty(LPOBJ lpObj);
};

extern CChaosBoxGenesis gChaosBoxGenesis;
#endif
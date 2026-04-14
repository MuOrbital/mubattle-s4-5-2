#ifndef LUAGAMESERVER_H
#define LUAGAMESERVER_H

#pragma once

#define PATH_GAMESERVER_LUA "Scripts\\GameServer.lua"

#include "LuaState.h"
#include "CriticalSection.h"
#include "Util.h"
#include <functional>

class CLuaGameServer
{
public:
	CLuaGameServer();
	virtual ~CLuaGameServer();

public:
	void Init();
	void InitReload();
	void Reload();

public:
	void GameServer();
	void TimerSystem();
	int PlayerAttack(int aIndex, int TargetIndex, int IsSlap);
	int PlayerCheckDamage(int aIndex, int TargetIndex, int Damage, int ShieldDamage, int Effect);
	int LevelUpPointAdd(int aIndex, int Type);
	void PlayerMove(int aIndex, int Map, int ax, int ay, int sx, int sy);
	void CharacterSet(int aIndex);
	void EnterCharacter(int aIndex);
	void PlayerDie(int aIndex, int TargetIndex);
	void PlayerLogout(int aIndex, std::string name, std::string account);

	int PlayerDropItem(int aIndex, int px, int py, int pos);
	int PlayerPickItem(int aIndex, int index);

	int NpcTalk(int Npc, int Player);
	void MonsterDie(int aIndex, int TargetIndex);
	int MonsterDieGiveItem(int aIndex, int TargetIndex);
	int PlayerSendTrade(int aIndex, int TargetIndex);
	int PlayerSendParty(int aIndex, int TargetIndex);
	int BlockSendParty(int aIndex, int TargetIndex);
	int BlockRecParty(int aIndex, int TargetIndex);
	int PlayerTradeOk(int aIndex, int TargetIndex);
	int PlayerTradeXOk(int aIndex, int TargetIndex);
	int PlayerSendTradeX(int aIndex, int TargetIndex);
	void GuildWarProc(std::string Guild1, int GuildPoints1, std::string Guild2, int GuildPoints2);
	int PlayerSellItem(int aIndex, int Position);
	int PlayerBuyItem(int aIndex, int ShopIndex, int Slot);
	int CharacterMove(int aIndex, int map, int x, int y);
	int PlayerRingEquiped(int aIndex);
	int PlayerRepairItem(int aIndex, int Position);
	int ChatProc(int index, std::string text);
	void RespawnUser(int aIndex);
	int DeleteCharacter(int aIndex, std::string name);
	int PlayerMoveItem(int aIndex, int SlotSource, int SlotTarget, int Type);
	void PlayerVaultOpen(int aIndex);
	void PlayerVaultClose(int aIndex);
	void MonsterReload();
	void ReloadLuaMonster();
	void UpdateTopKill();
	void CloseLua();
	int DeclareWar(int aIndex);
	int PlayerUseItem(int aIndex, int SourceSlot, int TargetSlot);
	int PlayerCanEquipItem(int aIndex, int Slot, int SourceSlot);

	int PlayerAttackFenrirSkill(int aIndex, int SourceSlot);

	int CheckPersonalShopValue(int aIndex, int ItemIndex, int Coin1, int Coin2, int Coin3, int Coin4);
	void PersonalShopDecreaseValue(int aIndex, int Coin1, int Coin2, int Coin3, int Coin4);
	void PersonalShopAddValue(int aIndex, int Coin1, int Coin2, int Coin3, int Coin4);
	void MacAddressPlayer(int aIndex, std::string Mac);
	int CanEquipElementSlot(int aIndex);
	void CheckCustomChaosBox(int aIndex);
	void GameServerProtocol(int aIndex, BYTE head, std::string packet_name);
	void TradeXReceiveCoins(int aIndex, BYTE type, int value);

	void ChaosMachineGenesisMix(int aIndex);

	int PlayerOpenShop(int aIndex);

	int PlayerOpenPersonalShop(int aIndex);

	void PlayerCloseShop(int aIndex);

	void ReceiveEventReward(int aIndex, int EventType);

	void ItemBuyCheckCoins(int aIndex, int coin1, int coin2, int coin3, int* coin1Val, int* coin2Val, int* coin3Val);
	void ItemBuyBuyError(int aIndex, int coinVal, int type);
	void ItemBuyDecreaseCoins(int aIndex, int coin1, int coin2, int coin3);
	void ItemSellAddCoins(int aIndex, int coin1, int coin2, int coin3);

	void AutoResetPlayerProc(int aIndex);
	void QueryAsyncProcess(std::string name, std::string param, int aIndex);

	void ProcessErrorLog(lua_State * lua, std::string func, int nStatus);
	void ProcessErrorLog(std::string func, std::string error);

public:
	char m_IpAddress[17];
	char m_ServerName[30];

	bool m_Reloading;

private:
	CCriticalSection m_critical;
};

extern CLuaGameServer gLuaGameServer;

#endif
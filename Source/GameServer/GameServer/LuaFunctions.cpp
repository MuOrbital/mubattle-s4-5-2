#include "StdAfx.h"
#include "LuaFunctions.h"
#include "LuaStack.hpp"
#include "NpcTalk.h"
#include "ServerInfo.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "IllusionTemple.h"
#include "ItemStack.h"
#include "PersonalShop.h"
#include "Message.h"
#include "CommandManager.h"
#include "ScheduleManager.h"
#include "ItemBagManager.h"
#include "MasterSkillTree.h"
#include "CashShop.h"
#include "CustomStore.h"
#include "ChaosBoxGenesis.h"
//#include "DeathSystem.h"

static bool first = true;

void FuncLogAdd(std::string str) {
	LogAdd(LOG_BLACK, (char*)str.c_str());
}

void LuaLogAdd(std::string str) {
	LogAddLua(LOG_BLACK, (char*)str.c_str());
}

void LogAddC(int type, std::string str) {
	switch (type) {
	case 1:
		LogAdd(LOG_BLACK, (char*)str.c_str());
		break;
	case 2:
		LogAdd(LOG_RED, (char*)str.c_str());
		break;
	case 3:
		LogAdd(LOG_GREEN, (char*)str.c_str());
		break;
	case 4:
		LogAdd(LOG_BLUE, (char*)str.c_str());
		break;
	case 5:
		LogAdd(LOG_DARK_GREEN, (char*)str.c_str());
		break;
	case 6:
		LogAdd(LOG_DARK_RED, (char*)str.c_str());
		break;
	case 7:
		LogAdd(LOG_MAGENTA, (char*)str.c_str());
		break;
	case 8:
		LogAdd(LOG_PURPLE, (char*)str.c_str());
		break;
	case 9:
		LogAdd(LOG_DARK_BLUE, (char*)str.c_str());
		break;
	case 10:
		LogAdd(LOG_ORANGE, (char*)str.c_str());
		break;
	case 11:
		LogAdd(LOG_YELLOW, (char*)str.c_str());
		break;
	case 12:
		LogAdd(LOG_BROWN, (char*)str.c_str());
		break;
	case 13:
		LogAdd(LOG_CYAN, (char*)str.c_str());
		break;
	case 14:
		LogAdd(LOG_GRAY, (char*)str.c_str());
		break;
	default:
		LogAdd(LOG_BLACK, (char*)str.c_str());
		break;
	}
}

void LogAddTD(std::string str) {
	LogAdd(LOG_BLACK, (char*)str.c_str());
}

void SendMessageUser(std::string str, int aIndex, int type) 
{
	gNotice.GCNoticeLuaSend(aIndex, type, 0, 0, 0, 0, 0, (char*)str.c_str());
}

void SendMessageGlobal(std::string str, int type) 
{
	if (type == 2)
	{
		ChatYellowMessageAll((char*)str.c_str());
	}
	else if (type == 3)
	{
		ChatBlueMessageAll((char*)str.c_str());
	}
	else if (type == 4)
	{
		ChatGreenMessageAll((char*)str.c_str());
	}
	else
	{
		gNotice.GCNoticeLuaSendToAll(type, 0, 0, 0, 0, 0, (char*)str.c_str());
	}
}

void SendMessageAllServer(std::string str, int type) 
{
	GJServerMessageInfoSend((char*)str.c_str(), type);
}

void MoneySend(int aIndex) {
	GCMoneySend(aIndex, gObj[aIndex].Money);
}

void LevelUpSend(int aIndex) {
	GCLevelUpSend(&gObj[aIndex]);
	GCNewCharacterInfoSend(&gObj[aIndex]);
}

void PkLevelSend(int aIndex, int Level) {
	GCPKLevelSend(aIndex, Level);
}

int GetIndex(std::string str) {
	return gObjFindIndex((char*)str.c_str());
}

void Teleport(int aIndex, int map, int x, int y) {
	gObjTeleport(aIndex, map, x, y);
}

void TeleportMap(int aIndex, int range) {
	gMap[gObj[aIndex].Map].GetMapRandomPos(&gObj[aIndex].X, &gObj[aIndex].Y, range);
}

void TeleportMapRange(int aIndex, int map, int x, int y, int range) {
	if (MAP_RANGE(map) == 0) {
		return;
	}

	short GetX = x;
	short GetY = y;
	gMap[map].GetMapRandomPos(&GetX, &GetY, range);
	gObjTeleport(aIndex, map, GetX, GetY);
}

void CloseChar(int aIndex, int type) {
	gObjectManager.CharacterGameCloseSet(aIndex, type);
}

void SkillSend(int aIndex, int skill, int TargetIndex, int type) {
	gSkillManager.GCSkillAttackSend(&gObj[aIndex], skill, TargetIndex, type);
}

void ViewportCreate(int aIndex) {
	gObjViewportListProtocolCreate(&gObj[aIndex]);
}

void ViewportDestroy(int aIndex) {
	gObjViewportListProtocolDestroy(&gObj[aIndex]);
}

DWORD GetTick() {
	DWORD time = timeGetTime();
	return time;
}

int GetTickGS() {
	int time = timeGetTime();
	return time;
}

int AddMonster(int map) {
	return gObjAddMonster(map);
}

void SetPositionMonsterLua(int aIndex, int map) {
	SetPositionMonsterRand(aIndex, map);
}

void SetMonster(int aIndex, int MonsterClass) {
	gObjSetMonster(aIndex, MonsterClass);
}

void gObjDelLua(int aIndex) {
	gObjDel(aIndex);
}

int MonsterTopHitDamageUser(int aIndex) {
	return gObjMonsterGetTopHitDamageUser(&gObj[aIndex]);
}

void ItemSerialCreate(int aIndex, int map, int x, int y, int ItemIndex, int Level, int dur, int op1, int op2, int op3, int exc) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	int checkItemIndex = SafeGetItem(ItemIndex);

	if (checkItemIndex == -1) {
		return;
	}

	if (map == 236) {
		map = 235;
	}

	GDCreateItemSend(aIndex, map, (BYTE)x, (BYTE)y, ItemIndex, (BYTE)Level, (BYTE)dur, (BYTE)op1, (BYTE)op2, (BYTE)op3, -1, (BYTE)exc, 0, 0, 0, 0, 0, 0);
}

void ItemSerialCreatePeriodic(int aIndex, int map, int Value3, int Value4, int Value5, int Value6, int Value7, int Value8, int Value9, int Value10, int Value11, int Value12) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	int checkItemIndex = SafeGetItem(Value5);

	if (checkItemIndex == -1) {
		return;
	}

	if (map == 236) {
		map = 235;
	}

	DWORD Time = (DWORD)(time(0) + ((Value12 * 1440) * 60));

	GDCreateItemSend(aIndex, map, (BYTE)Value3, (BYTE)Value4, (int)Value5, (int)Value6, (BYTE)Value7, Value8, Value9, Value10, -1, Value11, 0, 0, 0, 0, 0, Time);
}

void ItemSerialCreateComplete(int aIndex, int map, int x, int y, int ItemIndex, int level, int dur, int op1, int op2, int op3, int ownerIndex, int exc, int ValueAncient, int ValueTimer) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	int checkItemIndex = SafeGetItem(ItemIndex);

	if (checkItemIndex == -1) {
		return;
	}

	if (map == 236)
	{
		map = 235;
	}

	DWORD Time = 0;

	if (ValueTimer > 0) {
		Time = (DWORD)(time(0) + ((ValueTimer * 1440) * 60));
	}

	BYTE ItemSetOption = 0;

	gItemOptionRate.MakeSetOption(ItemIndex, ValueAncient, &ItemSetOption);

	GDCreateItemSend(aIndex, map, (BYTE)x, (BYTE)y, ItemIndex, level, (BYTE)dur, op1, op2, op3, ownerIndex, exc, ItemSetOption, 0, 0, 0, 0, Time);
}

void CreateItemInventory(int aIndex, int ItemIndex, int level, int dur, int op1, int op2, int op3, int exc, int Ancient, int JoH, int SockBonus, int Sock1, int Sock2, int Sock3, int Sock4, int Sock5, int Timer) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	int checkItemIndex = SafeGetItem(ItemIndex);

	if (checkItemIndex == -1) {
		return;
	}

	DWORD ValueTimer = 0;

	if (Timer > 0) {
		ValueTimer = (DWORD)(time(0) + Timer);
	}

	BYTE ItemNewOption = Ancient;
	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { (BYTE)Sock1,(BYTE)Sock2,(BYTE)Sock3,(BYTE)Sock4,(BYTE)Sock5 };

	//ItemNewOption = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemNewOption);

	//gItemOptionRate.MakeSetOption(ItemIndex, ItemNewOption, &ItemNewOption);

	gItemOptionRate.MakeSocketOption(ItemIndex, ItemSocketOption[0], &ItemSocketOption[0]);

	GDCreateItemSend(aIndex, 235, (BYTE)0, (BYTE)0, ItemIndex, level, dur, op1, op2, op3, aIndex, exc, ItemNewOption, JoH, 0, ItemSocketOption, SockBonus, ValueTimer);
}

void CreateItemInventory2(int aIndex, int ItemIndex, int level, int op1, int op2, int op3, int exc, int Ancient, int JoH, int socket, int Timer) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	int checkItemIndex = SafeGetItem(ItemIndex);

	if (checkItemIndex == -1) {
		return;
	}

	DWORD ValueTimer = 0;

	if (Timer > 0) {
		ValueTimer = (DWORD)(time(0) + Timer);
	}

	BYTE ItemNewOption = Ancient;
	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
	{
		ItemSocketOption[n] = 0xFE;
	}

	ItemNewOption = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemNewOption);

	gItemOptionRate.MakeSetOption(ItemIndex, ItemNewOption, &ItemNewOption);

	GDCreateItemSend(aIndex, 235, (BYTE)0, (BYTE)0, ItemIndex, level, (BYTE)0, op1, op2, op3, aIndex, exc, ItemNewOption, JoH, 0, ItemSocketOption, 0xFF, ValueTimer);
}

void CreateItemMap(int aIndex, int Map, int X, int Y, int ItemIndex, int level, int op1, int op2, int op3, int exc, int Ancient, int JoH, int socket, int Timer) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	int checkItemIndex = SafeGetItem(ItemIndex);

	if (checkItemIndex == -1) {
		return;
	}

	DWORD ValueTimer = 0;

	if (Timer > 0) {
		ValueTimer = (DWORD)(time(0) + Timer);
	}

	BYTE ItemNewOption = Ancient;
	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
	{
		ItemSocketOption[n] = 0xFE;
	}

	ItemNewOption = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemNewOption);

	gItemOptionRate.MakeSetOption(ItemIndex, ItemNewOption, &ItemNewOption);

	GDCreateItemSend(aIndex, Map, (BYTE)X, (BYTE)Y, ItemIndex, level, (BYTE)0, op1, op2, op3, aIndex, exc, ItemNewOption, JoH, 0, ItemSocketOption, 0xFF, ValueTimer);
}

void InventoryDeleteItem(int aIndex, int slot) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gItemManager.InventoryDelItem(aIndex, slot);
}

void CreateInventoryItem(int aIndex, int ItemIndex, int level, int dur, int op1, int op2, int op3, int exc) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	int checkItemIndex = SafeGetItem(ItemIndex);

	if (checkItemIndex == -1) {
		return;
	}

	GDCreateItemSend(aIndex, 250, 0, 0, ItemIndex, level, (BYTE)dur, op1, op2, op3, -1, exc, 0, 0, 0, 0, 0, 0);
}

void SendInventoryDeleteItem(int aIndex, int slot) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gItemManager.GCItemDeleteSend(aIndex, slot, 1);
}

void ItemListSend(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gItemManager.GCItemListSend(aIndex);
}

void GetWarehouseList(int aIndex, std::string string) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gWarehouse.GDWarehouseItemSend(aIndex, (char*)string.c_str());
}

void ChatTargetSend(int aIndex, std::string string, int monster) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	GCChatTargetSend(&gObj[monster], aIndex, (char*)string.c_str());
}

void SetMapMonster(int aIndex, int map, int x, int y) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	SetPositionMonster(aIndex, map, x, y);
}

void CalCharacter(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gObjectManager.CharacterCalcAttribute(aIndex);
}

void LifeUpdate(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	GCLifeSend(aIndex, 0xFE, (int)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), (gObj[aIndex].MaxShield + gObj[aIndex].AddShield));
}

void ManaUpdate(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	GCManaSend(aIndex, 0xFE, (int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana), (gObj[aIndex].MaxBP + gObj[aIndex].AddBP));
}

void NextExpCalc(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gObjCalcExperience(&gObj[aIndex]);
}

void SetBP(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gObjectManager.CharacterCalcBP(&gObj[aIndex]);
}

void MakePreview(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gObjectManager.CharacterMakePreviewCharSet(aIndex);
}

void SendQuestPrize(int aIndex, int reward, int amount) {
	gQuest.GCQuestRewardSend(aIndex, reward, amount);
}

void LuaResetSkills(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	ResetSkills(&gObj[aIndex]);
}

void ClearViewPort(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gObjClearViewport(&gObj[aIndex]);
}

void SendPacketMain(int aIndex, unsigned char* packet, int size) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	DataSend(aIndex, packet, size);
}

void StateInfoSend(int aIndex, int state, int effect) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gEffectManager.GCEffectStateSend(&gObj[aIndex], (BYTE)state, (BYTE)effect);
}

int LevelExperience(int Level) {
	return gLevelExperience[Level - 1];
}

void RefreshCharacter(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	GCNewCharacterInfoSend(&gObj[aIndex]);
}

void FireWorks(int aIndex, int map, int x, int y) {
	if (aIndex == -1)
	{
		GCFireworksSendCustom(map, x, y);
	}
	else
	{
		GCFireworksSend(&gObj[aIndex], gObj[aIndex].X, gObj[aIndex].Y);
	}
}

int GetMapAttr(int map, int x, int y, int attr) {
	if (MAP_RANGE(map) == 0) {
		return 0;
	}

	return (gMap[map].GetAttr(x, y) & attr);
}

void LuagObjAdd(DWORD ssocket, std::string ipAddr, int aIndex) {
	SOCKET socket;

	if (ssocket == 0)
	{
		socket = INVALID_SOCKET;
	}
	else
	{
		socket = ssocket;
	}

	gObjAdd(socket, (char*)ipAddr.c_str(), aIndex);
}

void CharacterInfoSend(int aIndex, std::string name) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	GDCharacterInfoSend(aIndex, (char*)name.c_str());
}

void LuagObjSetPosition(int aIndex, int x, int y) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gObjSetPosition(aIndex, x, y);
}

void AccountInfoSend(int aIndex, std::string account, std::string pass) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	GJConnectAccountSend(aIndex, (char*)account.c_str(), (char*)pass.c_str(), "127.0.0.1");
}

void LuaMoveObject(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	MoveObject(aIndex);
}

int SearchIndex(DWORD ssocket, std::string ipaddr) {
	SOCKET socket;

	if (ssocket == 0)
	{
		socket = INVALID_SOCKET;
	}
	else
	{
		socket = ssocket;
	}

	return gObjAddSearch(socket, (char*)ipaddr.c_str());
}

int CheckQuestListState(int Value1, int Value2, int Value3) {
	if (!OBJECT_RANGE(Value1)) {
		return 0;
	}

	return gQuest.CheckQuestListState(&gObj[Value1], Value2, Value3);
}

void AddQuestList(int Value1, int Value2, int Value3) {
	if (!OBJECT_RANGE(Value1)) {
		return;
	}

	gQuest.AddQuestList(&gObj[Value1], Value2, Value3);
}

void InsertQuestReward(int Value1, int Value2) {
	if (!OBJECT_RANGE(Value1)) {
		return;
	}

	gQuestReward.InsertQuestReward(&gObj[Value1], Value2);
}

void LuaGCQuestInfoSend(int Value1) {
	if (!OBJECT_RANGE(Value1)) {
		return;
	}

	gQuest.GCQuestInfoSend(Value1);
}

void ChaosMixSend(int aIndex, int result) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gChaosBox.GCChaosMixSend(aIndex, (BYTE)result, 0);
}

void ChaosBoxInit(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gChaosBox.ChaosBoxInit(&gObj[aIndex]);
}

void ChaosBoxSend(int aIndex, int type) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gChaosBox.GCChaosBoxSend(&gObj[aIndex], (BYTE)type);
}

void ClearAllBuff(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gEffectManager.ClearAllEffect(&gObj[aIndex]);
}

int CheckBuffPlayer(int aIndex, int effect) {
	if (!OBJECT_RANGE(aIndex)) {
		return 0;
	}

	return gEffectManager.CheckEffect(&gObj[aIndex], effect);
}

int InventoryCheckSpaceByItem(int aIndex, int ItemIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return 0;
	}

	return gItemManager.CheckItemInventorySpace(&gObj[aIndex], ItemIndex);
}

int InventoryCheckSpaceBySize(int aIndex, int width, int height) {
	if (!OBJECT_RANGE(aIndex)) {
		return 0;
	}

	return gItemManager.CheckItemInventorySpace(&gObj[aIndex], width, height);
}

void ResetQuest(int aIndex) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	memset(gObj[aIndex].Quest, 0xFF, sizeof(gObj[aIndex].Quest));
}

int gObjIsConnectedGPLua(int aIndex) {
	return gObjIsConnectedGP(aIndex);
}

void RebuildMasterSkillTree(int aIndex) {
	gObjRebuildMasterSkillTree(&gObj[aIndex]);
}

int CheckUserIsMasterSkill(int aIndex) {
	return gMasterSkillTree.CheckMasterLevel(&gObj[aIndex]);
}

void SendMessagePlayer(int aIndex, int Type, std::string msg) {
	if (OBJECT_USER_RANGE(aIndex) == 0) {
		return;
	}

	if (Type == 2) {
		ChatYellowMessage(aIndex, (char*)msg.c_str());
	}
	else if (Type == 3) {
		ChatBlueMessage(aIndex, (char*)msg.c_str());
	}
	else if (Type == 4) {
		ChatGreenMessage(aIndex, (char*)msg.c_str());
	}
	else {
		gNotice.GCNoticeSend(aIndex, Type, 0, 0, 0, 0, 0, (char*)msg.c_str());
	}
}

int gObjCheckOffAttack(int aIndex) {
	if (OBJECT_USER_RANGE(aIndex) == 0) {
		return 0;
	}

	if (gObj[aIndex].AttackCustomOffline == 1) {
		return 1;
	}

#if (GAMESERVER_UPDATE == 603)
	if (gObj[aIndex].m_OfflineMode == 1) {
		return 1;
	}
#endif

	return 0;
}

int gObjCheckOffStore(int aIndex) {
	if (OBJECT_USER_RANGE(aIndex) == 0) {
		return 0;
	}

	if (gObj[aIndex].PShopCustomOffline == 1) {
		return 1;
	}

	return 0;
}

void DamageSend(int aIndex, int TargetIndex, int reflet, int damage, int type, int shieldDamage) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	GCDamageSend(aIndex, TargetIndex, reflet, damage, type, shieldDamage);
}

void KillPlayer(int aIndex) {
	if (!OBJECT_USER_RANGE(aIndex)) {
		return;
	}

	gObj[aIndex].KillerType = 0;
	gObj[aIndex].Live = 0;
	gObj[aIndex].State = OBJECT_DYING;
	gObj[aIndex].RegenTime = GetTickCount();
	gObj[aIndex].DieRegen = 1;
	gObj[aIndex].PathCount = 0;
	gObj[aIndex].Teleport = ((gObj[aIndex].Teleport == 1) ? 0 : gObj[aIndex].Teleport);

	GCUserDieSend(&gObj[aIndex], aIndex, 0, aIndex);
}

void AddEffect(int aIndex, int type, int index, int count, int value1, int value2, int value3, int value4) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gEffectManager.AddEffect(&gObj[aIndex], type, index, count, value1, value2, value3, value4);
}

void RemoveEffect(int aIndex, int index) {
	if (!OBJECT_RANGE(aIndex)) {
		return;
	}

	gEffectManager.DelEffect(&gObj[aIndex], index);
}

void MoveGate(int aIndex, int gate) {
	if (!OBJECT_USER_RANGE(aIndex)) {
		return;
	}

	gObjMoveGate(aIndex, gate);
}

void DestroyParty(int aIndex) {
	gParty.Destroy(aIndex);
}

int GetMemberCountParty(int index) {
	return gParty.GetMemberCount(index);
}

int CreateParty(int aIndex) {
	return gParty.Create(aIndex);
}

int AddMemberParty(int PartyNumber, int TargetIndex) {
	return gParty.AddMember(PartyNumber, TargetIndex);
}

void GCPartyResultSend(int aIndex, int result) {
	gParty.GCPartyResultSend(aIndex, result);
}

void GCPartyDelMemberSend(int aIndex) {
	gParty.GCPartyDelMemberSend(aIndex);
}

void GCPartyListSend(int aIndex) {
	gParty.GCPartyListSend(aIndex);
}

int IsLeaderParty(int index, int aIndex) {
	return gParty.IsLeader(index, aIndex);
}

int IsMemberParty(int index, int aIndex) {
	return gParty.IsMember(index, aIndex);
}

int IsParty(int index) {
	return gParty.IsParty(index);
}

int GetMemberIndexParty(int index, int number) {
	return gParty.GetMemberIndex(index, number);
}

int GetStackItem(int ItemIndex) {
	return gItemStack.GetItemMaxStack(ItemIndex);
}

int GetInventoryEmptySlotCount(int aIndex) {
	if (!OBJECT_USER_RANGE(aIndex)) {
		return 0;
	}

	return gItemManager.GetInventoryEmptySlotCount(&gObj[aIndex]);
}

int GetInventoryItemCount(int aIndex, int ItemIndex, int level) {
	if (!OBJECT_USER_RANGE(aIndex)) {
		return 0;
	}

	return gItemManager.GetInventoryItemCount(&gObj[aIndex], ItemIndex, level);
}

void DeleteItemCount(int aIndex, int ItemIndex, int level, int count) {
	if (!OBJECT_USER_RANGE(aIndex)) {
		return;
	}

	gItemManager.DeleteInventoryItemCount(&gObj[aIndex], ItemIndex, level, count);
}

int LuaGetGuildRelationShip(int aIndex, int TargetIndex) {
	if (!OBJECT_USER_RANGE(aIndex) || !OBJECT_USER_RANGE(TargetIndex)) {
		return 0;
	}

	return gObjGetRelationShip(&gObj[aIndex], &gObj[TargetIndex]);
}

void PersonalShopBuyItem(int aIndex, int TargetIndex, std::string name, int slot) {
	PMSG_PSHOP_BUY_ITEM_RECV pMsg;

	pMsg.index[0] = SET_NUMBERHB(TargetIndex);

	pMsg.index[1] = SET_NUMBERLB(TargetIndex);

	std::memcpy(pMsg.name, name.c_str(), sizeof(pMsg.name));

	pMsg.slot = slot;

	gPersonalShop.CGPShopBuyItemRecv((PMSG_PSHOP_BUY_ITEM_RECV*)&pMsg, aIndex);
}

int GetMapObject(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return 0;
	}

	return gObj[aIndex].Map;
}

std::string GetLanguageObject(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return 0;
	}

	return "Eng";
}

std::string GetNameObject(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return "NotFindUserWithThisName";
	}

	return gObj[aIndex].Name;
}

std::string GetAccountObject(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return "NotFindUserWithThisName";
	}

	return gObj[aIndex].Account;
}

int GetTimeDays(int dayOfWeek, int day, int hour, int minute, int second) {
	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	ScheduleManager.AddSchedule(-1, -1, day, hour, minute, second, dayOfWeek);

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	return (int)difftime(ScheduleTime.GetTime(), time(0));
}

#ifdef CHAOS_MACHINE_GENESIS
void OpenChaosMachineGenesis(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(901));
		return;
	}

	if (lpObj->Interface.use != 0 || lpObj->Interface.type != INTERFACE_NONE || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0) {
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(900));
		return;
	}

	lpObj->ChaosBoxGenesisUsing = 1;

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOSGENESIS;
	lpObj->Interface.state = 0;

	gChaosBoxGenesis.ChaosBoxGenesisSendOpenClient(aIndex, lpObj->ChaosBoxGenesisState, lpObj->ChaosBoxGenesisSelectedKey, lpObj->ChaosBoxGenesisSelectedComb);

	gChaosBoxGenesis.GCChaosBoxGenesisItemListSend(lpObj);
}

void ChaosBoxGenesisInit(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	gChaosBoxGenesis.Clear(lpObj);
}

void ChaosBoxGenesisMixSend(int aIndex, int state) {
	gChaosBoxGenesis.ChaosBoxGenesisStateSend(aIndex, state);
}

void ChaosBoxGenesisItemListSend(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	gChaosBoxGenesis.GCChaosBoxGenesisItemListSend(lpObj);
}

void ChaosBoxGenesisCreateItem(int aIndex, int type, int level, int dur, int op1, int op2, int op3, int exc, int op380, int ancient, int joh, int socket, int timer, DWORD serial)
{
	BYTE slot = gItemManager.ChaosBoxGenesisGetEmptySlot(aIndex);


	if (slot == 0xFF) 
	{
		return;
	}

	CItem item;

#if (GAMESERVER_UPDATE>=401)
	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
	{
		ItemSocketOption[n] = 0xFE;
	}

	DWORD ValueTimer = 0;
	BYTE Item380Option = 0;

	if (timer > 0) {
		ValueTimer = (DWORD)(time(0) + timer);
	}

	if (op380 != 0) {
		Item380Option |= 0x80;
	}

	item.Convert(type, op1, op2, op3, exc, ancient, joh, Item380Option, ItemSocketOption, 0xFF);
#else
	BYTE ItemSetOption = 0;

	item.Convert(type, op1, op2, op3, exc, 0, 0, 0, 0, 0xFF);
#endif

	item.m_Level = level;

	item.m_Serial = serial;

	item.m_Durability = (float)dur;// gItemManager.GetItemDurability(type, level, exc, ItemSetOption);

#if (GAMESERVER_UPDATE>=401)
	item.m_IsPeriodicItem = ((ValueTimer > 0) ? 1 : 0);

	item.m_LoadPeriodicItem = ((ValueTimer > 0) ? 1 : 0);

	item.m_PeriodicItemTime = ((ValueTimer > 0) ? ValueTimer : 0);
#endif

	gItemManager.ChaosBoxGenesisAddItem(aIndex, item, slot);
}

#endif

void DropEventItemBag(int aIndex, int SpecialValue) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return;
	}

	gItemBagManager.DropItemBySpecialValue(SpecialValue, &gObj[aIndex], gObj[aIndex].Map, gObj[aIndex].X, gObj[aIndex].Y);
}

void SaveChaosBoxGenesis(int aIndex) {
	if (OBJECT_RANGE(aIndex) == 0) {
		return;
	}

#ifdef CHAOS_MACHINE_GENESIS
	gChaosBoxGenesis.ChaosboxGenesisSaveItemSend(&gObj[aIndex], true, true);
#endif
}

int LGetRandomValue(int min, int max) {
	if (first)
	{
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);

}

void InsertDeath(int Value1, int Value2, int Value3) 
{
	//gDeathSystem.DeathIncrease(Value1, Value2, Value3);
}

void DecreaseDeath(int Value1, int Value2, int Value3) 
{
	//gDeathSystem.DeathDecrease(Value1, Value2, Value3);
}

void UpdateDeath(int aIndex) 
{
	if (!OBJECT_RANGE(aIndex)) 
	{
		return;
	}

	//gDeathSystem.SendInformation(&gObj[aIndex], 0);
}

using namespace luaaa;

CLuaFunctions gLuaFunctions;

CLuaFunctions::CLuaFunctions()
{
}

CLuaFunctions::~CLuaFunctions()
{
}

void CLuaFunctions::AddFunctions(lua_State* Lua)
{
	if (Lua == NULL)
	{
		return;
	}

	lua_pushcclosure(Lua, this->IncludeLuaOpenExtensionCrypted, 0);
	lua_setglobal(Lua, "OpenFolder");

	lua_pushcclosure(Lua, this->OpenFileLua, 0);
	lua_setglobal(Lua, "OpenFile");

	LuaModule(Lua).fun("LogAdd", &FuncLogAdd);
	LuaModule(Lua).fun("LogAddLua", &LuaLogAdd);
	LuaModule(Lua).fun("LogAddC", &LogAddC);
	LuaModule(Lua).fun("SendMessage", &SendMessageUser);
	LuaModule(Lua).fun("SendMessageGlobal", &SendMessageGlobal);
	LuaModule(Lua).fun("SendMessageAllServer", &SendMessageAllServer);
	LuaModule(Lua).fun("MoneySend", &MoneySend);
	LuaModule(Lua).fun("LevelUpSend", &LevelUpSend);
	LuaModule(Lua).fun("PkLevelSend", &PkLevelSend);
	LuaModule(Lua).fun("GetIndex", &GetIndex);
	LuaModule(Lua).fun("Teleport", &Teleport);
	LuaModule(Lua).fun("TeleportMap", &TeleportMap);
	LuaModule(Lua).fun("CloseChar", &CloseChar);
	LuaModule(Lua).fun("SkillSend", &SkillSend);
	LuaModule(Lua).fun("ViewportCreate", &ViewportCreate);
	LuaModule(Lua).fun("ViewportDestroy", &ViewportDestroy);
	LuaModule(Lua).fun("GetTick", &GetTick);
	LuaModule(Lua).fun("GetTickGS", &GetTickGS);
	LuaModule(Lua).fun("AddMonster", &AddMonster);
	LuaModule(Lua).fun("SetPositionMonster", &SetPositionMonsterLua);
	LuaModule(Lua).fun("SetMonster", &SetMonster);
	LuaModule(Lua).fun("gObjDel", &gObjDelLua);
	LuaModule(Lua).fun("gObjMonsterTopHitDamageUser", &MonsterTopHitDamageUser);
	LuaModule(Lua).fun("ItemSerialCreate", &ItemSerialCreate);
	LuaModule(Lua).fun("ItemSerialCreatePeriodic", &ItemSerialCreatePeriodic);
	LuaModule(Lua).fun("ItemSerialCreateComplete", &ItemSerialCreateComplete);
	LuaModule(Lua).fun("CreateItemInventory", &CreateItemInventory);
	LuaModule(Lua).fun("CreateItemInventory2", &CreateItemInventory2);
	LuaModule(Lua).fun("CreateItemMap", &CreateItemMap);
	LuaModule(Lua).fun("InventoryDeleteItem", &InventoryDeleteItem);
	LuaModule(Lua).fun("CreateInventoryItem", &CreateInventoryItem);
	LuaModule(Lua).fun("SendInventoryDeleteItem", &SendInventoryDeleteItem);
	LuaModule(Lua).fun("ItemListSend", &ItemListSend);
	LuaModule(Lua).fun("GetWarehouseList", &GetWarehouseList);
	LuaModule(Lua).fun("ChatTargetSend", &ChatTargetSend);
	LuaModule(Lua).fun("SetMapMonster", &SetMapMonster);
	LuaModule(Lua).fun("CalCharacter", &CalCharacter);
	LuaModule(Lua).fun("LifeUpdate", &LifeUpdate);
	LuaModule(Lua).fun("ManaUpdate", &ManaUpdate);
	LuaModule(Lua).fun("NextExpCalc", &NextExpCalc);
	LuaModule(Lua).fun("SetBP", &SetBP);
	LuaModule(Lua).fun("MakePreview", &MakePreview);
	LuaModule(Lua).fun("SendQuestPrize", &SendQuestPrize);
	LuaModule(Lua).fun("ResetSkills", &LuaResetSkills);
	LuaModule(Lua).fun("ClearViewPort", &ClearViewPort);
	LuaModule(Lua).fun("SendPacketMain", &SendPacketMain);
	LuaModule(Lua).fun("StateInfoSend", &StateInfoSend);
	LuaModule(Lua).fun("LevelExperience", &LevelExperience);
	LuaModule(Lua).fun("RefreshCharacter", &RefreshCharacter);
	LuaModule(Lua).fun("FireWorks", &FireWorks);
	LuaModule(Lua).fun("GetMapAttr", &GetMapAttr);
	LuaModule(Lua).fun("gObjAdd", &LuagObjAdd);
	LuaModule(Lua).fun("CharacterInfoSend", &CharacterInfoSend);
	LuaModule(Lua).fun("gObjSetPosition", &LuagObjSetPosition);
	LuaModule(Lua).fun("AccountInfoSend", &AccountInfoSend);
	LuaModule(Lua).fun("MoveObject", &LuaMoveObject);
	LuaModule(Lua).fun("SearchIndex", &SearchIndex);
	LuaModule(Lua).fun("GetRandomValue", &LGetRandomValue);

	LuaModule(Lua).fun("InsertCounter", &InsertDeath);
	LuaModule(Lua).fun("DecreaseCounter", &DecreaseDeath);
	LuaModule(Lua).fun("UpdateCounter", &UpdateDeath);

#ifdef CHAOS_MACHINE_GENESIS
	LuaModule(Lua).fun("OpenChaosMachineGenesis", &OpenChaosMachineGenesis);
	LuaModule(Lua).fun("ChaosBoxGenesisInit", &ChaosBoxGenesisInit);
	LuaModule(Lua).fun("ChaosBoxGenesisMixSend", &ChaosBoxGenesisMixSend);
	LuaModule(Lua).fun("ChaosBoxGenesisItemListSend", &ChaosBoxGenesisItemListSend);
	LuaModule(Lua).fun("ChaosBoxGenesisCreateItem", &ChaosBoxGenesisCreateItem);
#endif

	LuaModule(Lua).fun("CheckQuestListState", &CheckQuestListState);
	LuaModule(Lua).fun("AddQuestList", &AddQuestList);
	LuaModule(Lua).fun("InsertQuestReward", &InsertQuestReward);
	LuaModule(Lua).fun("GCQuestInfoSend", &LuaGCQuestInfoSend);
	LuaModule(Lua).fun("ChaosMixSend", &ChaosMixSend);
	LuaModule(Lua).fun("ChaosBoxInit", &ChaosBoxInit);
	LuaModule(Lua).fun("ChaosBoxSend", &ChaosBoxSend);
	LuaModule(Lua).fun("ClearAllBuff", &ClearAllBuff);
	LuaModule(Lua).fun("CheckBuffPlayer", &CheckBuffPlayer);
	LuaModule(Lua).fun("InventoryCheckSpaceByItem", &InventoryCheckSpaceByItem);
	LuaModule(Lua).fun("InventoryCheckSpaceBySize", &InventoryCheckSpaceBySize);
	LuaModule(Lua).fun("ResetQuest", &ResetQuest);
	LuaModule(Lua).fun("gObjIsConnectedGP", &gObjIsConnectedGPLua);
	LuaModule(Lua).fun("SendMessagePlayer", &SendMessagePlayer);
	LuaModule(Lua).fun("gObjCheckOffAttack", &gObjCheckOffAttack);
	LuaModule(Lua).fun("gObjCheckOffStore", &gObjCheckOffStore);


	LuaModule(Lua).fun("gObjRebuildMasterSkillTree", &RebuildMasterSkillTree);
	LuaModule(Lua).fun("CheckUserIsMasterSkill", &CheckUserIsMasterSkill);

	LuaModule(Lua).fun("DamageSend", &DamageSend);
	LuaModule(Lua).fun("KillPlayer", &KillPlayer);
	LuaModule(Lua).fun("AddEffect", &AddEffect);
	LuaModule(Lua).fun("RemoveEffect", &RemoveEffect);
	LuaModule(Lua).fun("TeleportMapRange", &TeleportMapRange);
	LuaModule(Lua).fun("MoveGate", &MoveGate);

	//Party
	LuaModule(Lua).fun("DestroyParty", &DestroyParty);
	LuaModule(Lua).fun("GetMemberCountParty", &GetMemberCountParty);
	LuaModule(Lua).fun("CreateParty", &CreateParty);
	LuaModule(Lua).fun("AddMemberParty", &AddMemberParty);
	LuaModule(Lua).fun("GCPartyResultSend", &GCPartyResultSend);
	LuaModule(Lua).fun("GCPartyDelMemberSend", &GCPartyDelMemberSend);
	LuaModule(Lua).fun("GCPartyListSend", &GCPartyListSend);
	LuaModule(Lua).fun("IsLeaderParty", &IsLeaderParty);
	LuaModule(Lua).fun("IsMemberParty", &IsMemberParty);
	LuaModule(Lua).fun("IsParty", &IsParty);
	LuaModule(Lua).fun("GetMemberIndexParty", &GetMemberIndexParty);

	//Item
	LuaModule(Lua).fun("GetStackItem", &GetStackItem);
	LuaModule(Lua).fun("GetInventoryEmptySlotCount", &GetInventoryEmptySlotCount);
	LuaModule(Lua).fun("GetInventoryItemCount", &GetInventoryItemCount);
	LuaModule(Lua).fun("DeleteItemCount", &DeleteItemCount);

	//Guild
	LuaModule(Lua).fun("GetGuildRelationShip", &LuaGetGuildRelationShip);

	//Personal shop
	LuaModule(Lua).fun("PersonalShopBuyItem", &PersonalShopBuyItem);

	LuaModule(Lua).fun("GetMapObject", &GetMapObject);
	LuaModule(Lua).fun("GetLanguageObject", &GetLanguageObject);
	LuaModule(Lua).fun("GetNameObject", &GetNameObject);
	LuaModule(Lua).fun("GetAccountObject", &GetAccountObject);

	LuaModule(Lua).fun("GetTimeDays", &GetTimeDays);

	LuaModule(Lua).fun("DropEventItemBag", &DropEventItemBag);

	LuaModule(Lua).fun("SaveChaosBoxGenesis", &SaveChaosBoxGenesis);
}

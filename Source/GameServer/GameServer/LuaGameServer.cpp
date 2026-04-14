#include "stdafx.h"
#include "StdAfx.h"
#include "LuaGameServer.h"
#include "GameMain.h"
#include "Util.h"
#include "Path.h"
#include "Protect.h"
#include "SocketManager.h"
#include "LuaSocket.h"
#include "LuaManager.h"
#include "QueryManager.h"
#include "Log.h"
#include "QueryAsync.h"

CLuaGameServer gLuaGameServer;

CLuaGameServer::CLuaGameServer() 
{
	this->m_Reloading = true;
}

CLuaGameServer::~CLuaGameServer()
{
}

void CLuaGameServer::Init()
{
	LuaManager.CreateLua();

	LuaManager.StartLua();

	gQueryAsync.start();

	this->m_Reloading = false;

	LogAdd(LOG_BLUE, "[ServerInfo] Lua loaded successfully");
}

void CLuaGameServer::InitReload()
{
	CLocker lock(gSocketManager.m_critical);

	LuaManager.StartLua();

	this->m_Reloading = false;
	LogAdd(LOG_BLUE, "[ServerInfo] Lua reloaded successfully");
}

int CLuaGameServer::PlayerCheckDamage(int aIndex, int TargetIndex, int Damage, int ShieldDamage, int Effect)
{
	if (this->m_Reloading)
	{
		return Damage;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return Damage;
	}

	lua_getglobal(L, "PlayerCheckDamage");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);
	lua_pushnumber(L, Damage);
	lua_pushnumber(L, ShieldDamage);
	lua_pushnumber(L, Effect);

	int nStatus = lua_pcall(L, 5, 1, 0);

	this->ProcessErrorLog(L, "PlayerCheckDamage", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}


void CLuaGameServer::Reload()
{
	this->CloseLua();

	CLocker lock(gSocketManager.m_critical);

	this->m_Reloading = true;

	gSocket.ClearPackets();

	LuaManager.ResetLua();

	LuaManager.CreateLua();

	this->InitReload();
}

void CLuaGameServer::GameServer()
{
	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "GameServer");

	int nStatus = lua_pcall(L, 0, 0, 0);

	this->ProcessErrorLog(L, "GameServer", nStatus);
}

void CLuaGameServer::UpdateTopKill()
{
	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "UpdateTopKill");

	lua_pushnumber(L,0);
	lua_pushnumber(L, 0);

	int nStatus = lua_pcall(L, 2, 0, 0);

	this->ProcessErrorLog(L, "UpdateTopKill", nStatus);
}

void CLuaGameServer::CloseLua()
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "CloseLua");

	int nStatus = lua_pcall(L, 0, 0, 0);

	this->ProcessErrorLog(L, "CloseLua", nStatus);
}

void CLuaGameServer::TimerSystem()
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	static long long lastTickTime = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now().time_since_epoch())
		.count();

	auto tick = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now().time_since_epoch())
		.count();

	if ((tick - lastTickTime) < 900) {
		return;
	}

	lastTickTime = tick;

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "TimerSystem");

	int nStatus = lua_pcall(L, 0, 0, 0);

	this->ProcessErrorLog(L, "TimerSystem", nStatus);
}

int CLuaGameServer::PlayerAttack(int aIndex, int TargetIndex, int IsSlap)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerAttack");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerAttack", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	//lua_gc(L, LUA_GCCOLLECT, 0);

	return result;
}

int CLuaGameServer::LevelUpPointAdd(int aIndex, int Type)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "LevelUpPointAdd");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, Type);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "LevelUpPointAdd", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

void CLuaGameServer::PlayerMove(int aIndex, int Map, int ax, int ay, int sx, int sy)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PlayerMove");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, Map);
	lua_pushnumber(L, ax);
	lua_pushnumber(L, ay);
	lua_pushnumber(L, sx);
	lua_pushnumber(L, sy);

	int nStatus = lua_pcall(L, 6, 0, 0);

	this->ProcessErrorLog(L, "PlayerMove", nStatus);
}

void CLuaGameServer::CharacterSet(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	

	lua_getglobal(L, "CharacterSet");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "CharacterSet", nStatus);
}

void CLuaGameServer::EnterCharacter(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "EnterCharacter");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "EnterCharacter", nStatus);
}

void CLuaGameServer::PlayerDie(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PlayerDie");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 0, 0);

	this->ProcessErrorLog(L, "PlayerDie", nStatus);
}

void CLuaGameServer::PlayerLogout(int aIndex, std::string name, std::string account)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PlayerLogout");

	lua_pushnumber(L, aIndex);
	lua_pushstring(L, name.c_str());
	lua_pushstring(L, account.c_str());

	int nStatus = lua_pcall(L, 3, 0, 0);

	this->ProcessErrorLog(L, "PlayerLogout", nStatus);
}

int CLuaGameServer::PlayerDropItem(int aIndex, int px, int py, int pos)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerDropItem");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, px);
	lua_pushnumber(L, py);
	lua_pushnumber(L, pos);

	int nStatus = lua_pcall(L, 4, 1, 0);

	this->ProcessErrorLog(L, "PlayerDropItem", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerPickItem(int aIndex, int index)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerPickItem");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, index);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerPickItem", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::NpcTalk(int Npc, int Player)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "NpcTalk");

	lua_pushnumber(L, Npc);
	lua_pushnumber(L, Player);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "NpcTalk", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

void CLuaGameServer::MonsterDie(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "MonsterDie");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 0, 0);

	this->ProcessErrorLog(L, "MonsterDie", nStatus);
}

int CLuaGameServer::MonsterDieGiveItem(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "MonsterDieGiveItem");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "MonsterDieGiveItem", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerSendTrade(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerSendTrade");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerSendTrade", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerSendParty(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerSendParty");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerSendParty", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	//lua_gc(L, LUA_GCCOLLECT, 0);

	return result;
}

int CLuaGameServer::BlockSendParty(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr)
	{
		return 0;
	}

	lua_getglobal(L, "BlockSendParty");

	if (lua_isfunction(L, -1) == 0)
	{
		lua_pop(L, 1);
		return 0;
	}

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	if (nStatus != 0)
	{
		this->ProcessErrorLog(L, "BlockSendParty", nStatus);
		return 0;
	}

	if (lua_isnumber(L, -1) == 0)
	{
		lua_pop(L, 1);
		return 0;
	}

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::BlockRecParty(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr)
	{
		return 0;
	}

	lua_getglobal(L, "BlockRecParty");

	if (lua_isfunction(L, -1) == 0)
	{
		lua_pop(L, 1);
		return 0;
	}

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	if (nStatus != 0)
	{
		this->ProcessErrorLog(L, "BlockRecParty", nStatus);
		return 0;
	}

	if (lua_isnumber(L, -1) == 0)
	{
		lua_pop(L, 1);
		return 0;
	}

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerTradeOk(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	

	lua_getglobal(L, "PlayerTradeOk");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerTradeOk", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	//lua_gc(L, LUA_GCCOLLECT, 0);

	return result;
}

int CLuaGameServer::PlayerTradeXOk(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerTradeXOk");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerTradeXOk", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	//lua_gc(L, LUA_GCCOLLECT, 0);

	return result;
}

int CLuaGameServer::PlayerSendTradeX(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	

	lua_getglobal(L, "PlayerSendTradeX");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerSendTradeX", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	//lua_gc(L, LUA_GCCOLLECT, 0);

	return result;
}

void CLuaGameServer::GuildWarProc(std::string Guild1, int GuildPoints1, std::string Guild2, int GuildPoints2)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	

	lua_getglobal(L, "GuildWarProc");

	lua_pushstring(L, Guild1.c_str());
	lua_pushnumber(L, GuildPoints1);

	lua_pushstring(L, Guild2.c_str());
	lua_pushnumber(L, GuildPoints2);

	int nStatus = lua_pcall(L, 4, 0, 0);

	this->ProcessErrorLog(L, "GuildWarProc", nStatus);
}

int CLuaGameServer::PlayerSellItem(int aIndex, int Position)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}
	
	lua_getglobal(L, "PlayerSellItem");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, Position);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerSellItem", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerBuyItem(int aIndex, int ShopIndex, int Slot)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();
	if (L == nullptr)
	{
		return 0;
	}

	lua_getglobal(L, "PlayerBuyItem");
	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, ShopIndex);
	lua_pushnumber(L, Slot);

	int nStatus = lua_pcall(L, 3, 1, 0);

	this->ProcessErrorLog(L, "PlayerBuyItem", nStatus);

	int result = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::CharacterMove(int aIndex, int map, int x, int y)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "CharacterMove");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, map);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);

	int nStatus = lua_pcall(L, 4, 1, 0);

	this->ProcessErrorLog(L, "CharacterMove", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerRingEquiped(int aIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	

	lua_getglobal(L, "PlayerRingEquiped");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 1, 0);

	this->ProcessErrorLog(L, "PlayerRingEquiped", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerRepairItem(int aIndex, int Position)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerRepairItem");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, Position);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerRepairItem", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::ChatProc(int index, std::string text)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "ChatProc");

	lua_pushnumber(L, index);
	lua_pushstring(L, text.c_str());

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "ChatProc", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

void CLuaGameServer::RespawnUser(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "RespawnUser");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "RespawnUser", nStatus);
}

int CLuaGameServer::DeleteCharacter(int aIndex, std::string name)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "DeleteCharacter");

	lua_pushnumber(L, aIndex);
	lua_pushstring(L, name.c_str());

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "DeleteCharacter", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerMoveItem(int aIndex, int SlotSource, int SlotTarget, int Type)
{
	if (this->m_Reloading)
	{
		return 0;   // permite o movimento
	}

	CLocker lock(gSocketManager.m_critical);
	auto L = LuaManager.getMyLua();
	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerMoveItem");
	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, SlotSource);
	lua_pushnumber(L, SlotTarget);
	lua_pushnumber(L, Type);

	int nStatus = lua_pcall(L, 4, 1, 0);        // MUDANÇA: agora espera 1 retorno
	this->ProcessErrorLog(L, "PlayerMoveItem", nStatus);

	int result = (int)lua_tointeger(L, -1);     // pega o retorno do Lua (1 = bloqueia)
	lua_pop(L, 1);

	return result;                              // RETORNA o valor para o servidor
}

void CLuaGameServer::PlayerVaultOpen(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PlayerVaultOpen");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "PlayerVaultOpen", nStatus);
}

void CLuaGameServer::PlayerVaultClose(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PlayerVaultClose");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "PlayerVaultClose", nStatus);
}

void CLuaGameServer::MonsterReload()
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "MonsterReload");

	int nStatus = lua_pcall(L, 0, 0, 0);

	this->ProcessErrorLog(L, "MonsterReload", nStatus);
}

void CLuaGameServer::ReloadLuaMonster()
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "ReloadLuaMonster");

	int nStatus = lua_pcall(L, 0, 0, 0);

	this->ProcessErrorLog(L, "ReloadLuaMonster", nStatus);
}

int CLuaGameServer::DeclareWar(int aIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "DeclareWar");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 1, 0);

	this->ProcessErrorLog(L, "DeclareWar", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerUseItem(int aIndex, int SourceSlot, int TargetSlot)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerUseItem");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, SourceSlot);
	lua_pushnumber(L, TargetSlot);

	int nStatus = lua_pcall(L, 3, 1, 0);

	this->ProcessErrorLog(L, "PlayerUseItem", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerCanEquipItem(int aIndex, int TargetSlot, int SourceSlot)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerCanEquipItem");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, SourceSlot);
	lua_pushnumber(L, TargetSlot);

	int nStatus = lua_pcall(L, 3, 1, 0);

	this->ProcessErrorLog(L, "PlayerCanEquipItem", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::PlayerAttackFenrirSkill(int aIndex, int TargetIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerAttackFenrirSkill");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, TargetIndex);

	int nStatus = lua_pcall(L, 2, 1, 0);

	this->ProcessErrorLog(L, "PlayerAttackFenrirSkill", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

int CLuaGameServer::CheckPersonalShopValue(int aIndex, int ItemIndex, int Coin1, int Coin2, int Coin3, int Coin4)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "CheckPersonalShopValue");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, ItemIndex);
	lua_pushnumber(L, Coin1);
	lua_pushnumber(L, Coin2);
	lua_pushnumber(L, Coin3);
	lua_pushnumber(L, Coin4);

	int nStatus = lua_pcall(L, 6, 1, 0);

	this->ProcessErrorLog(L, "CheckPersonalShopValue", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

void CLuaGameServer::PersonalShopDecreaseValue(int aIndex, int Coin1, int Coin2, int Coin3, int Coin4)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PersonalShopDecreaseValue");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, Coin1);
	lua_pushnumber(L, Coin2);
	lua_pushnumber(L, Coin3);
	lua_pushnumber(L, Coin4);

	int nStatus = lua_pcall(L, 5, 0, 0);

	this->ProcessErrorLog(L, "PersonalShopDecreaseValue", nStatus);
}

void CLuaGameServer::PersonalShopAddValue(int aIndex, int Coin1, int Coin2, int Coin3, int Coin4)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PersonalShopAddValue");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, Coin1);
	lua_pushnumber(L, Coin2);
	lua_pushnumber(L, Coin3);
	lua_pushnumber(L, Coin4);

	int nStatus = lua_pcall(L, 5, 0, 0);

	this->ProcessErrorLog(L, "PersonalShopAddValue", nStatus);
}

void CLuaGameServer::MacAddressPlayer(int aIndex, std::string Mac)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "MacAddressPlayer");

	lua_pushnumber(L, aIndex);
	lua_pushstring(L, Mac.c_str());

	int nStatus = lua_pcall(L, 2, 0, 0);

	this->ProcessErrorLog(L, "MacAddressPlayer", nStatus);
}

int CLuaGameServer::CanEquipElementSlot(int aIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "CanEquipElementSlot");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 1, 0);

	this->ProcessErrorLog(L, "CanEquipElementSlot", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

void CLuaGameServer::CheckCustomChaosBox(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "CheckCustomChaosBox");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "CheckCustomChaosBox", nStatus);
}

void CLuaGameServer::GameServerProtocol(int aIndex, BYTE head, std::string packet_name)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "GameServerProtocol");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, head);
	lua_pushstring(L, packet_name.c_str());

	int nStatus = lua_pcall(L, 3, 0, 0);

	this->ProcessErrorLog(L, "GameServerProtocol", nStatus);
}

void CLuaGameServer::TradeXReceiveCoins(int aIndex, BYTE type, int value)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "TradeXReceiveCoins");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, type);
	lua_pushnumber(L, value);

	int nStatus = lua_pcall(L, 3, 0, 0);

	this->ProcessErrorLog(L, "TradeXReceiveCoins", nStatus);
}

void CLuaGameServer::ChaosMachineGenesisMix(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "ChaosMachineGenesisMix");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "ChaosMachineGenesisMix", nStatus);
}

int CLuaGameServer::PlayerOpenShop(int aIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return 0;
	}

	lua_getglobal(L, "PlayerOpenShop");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 1, 0);

	this->ProcessErrorLog(L, "PlayerOpenShop", nStatus);

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}


int CLuaGameServer::PlayerOpenPersonalShop(int aIndex)
{
	if (this->m_Reloading)
	{
		return 0;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr)
	{
		return 0;
	}

	lua_getglobal(L, "PlayerOpenPersonalShop");

	if (lua_isfunction(L, -1) == 0)
	{
		lua_pop(L, 1);
		return 0;
	}

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 1, 0);

	if (nStatus != 0)
	{
		this->ProcessErrorLog(L, "PlayerOpenPersonalShop", nStatus);
		return 0;
	}

	if (lua_isnumber(L, -1) == 0)
	{
		lua_pop(L, 1);
		return 0;
	}

	int result = (int)lua_tointeger(L, -1);

	lua_pop(L, 1);

	return result;
}

void CLuaGameServer::PlayerCloseShop(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "PlayerCloseShop");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "PlayerCloseShop", nStatus);
}

void CLuaGameServer::ReceiveEventReward(int aIndex, int EventType)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "ReceiveEventReward");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, EventType);

	int nStatus = lua_pcall(L, 2, 0, 0);

	this->ProcessErrorLog(L, "ReceiveEventReward", nStatus);
}

void CLuaGameServer::ItemBuyCheckCoins(int aIndex, int coin1, int coin2, int coin3, int* coin1Val, int* coin2Val, int* coin3Val)
{
	if (this->m_Reloading)
	{
		return;
	}
	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "ItemBuyCheckCoins");

	lua_pushnumber(L, aIndex);

	lua_pushnumber(L, coin1);
	lua_pushnumber(L, coin2);
	lua_pushnumber(L, coin3);

	int nStatus = lua_pcall(L, 4, 3, 0);

	this->ProcessErrorLog(L, "ItemBuyCheckCoins", nStatus);

	(*coin1Val) = (int)lua_tointeger(L, -1);

	(*coin2Val) = (int)lua_tointeger(L, -2);

	(*coin3Val) = (int)lua_tointeger(L, -3);

	lua_pop(L, 3);
}

void CLuaGameServer::ItemBuyBuyError(int aIndex, int coinVal, int type)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "ItemBuyBuyError");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, coinVal);
	lua_pushnumber(L, type);

	int nStatus = lua_pcall(L, 3, 0, 0);

	this->ProcessErrorLog(L, "ItemBuyBuyError", nStatus);
}

void CLuaGameServer::ItemBuyDecreaseCoins(int aIndex, int coin1, int coin2, int coin3)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "ItemBuyDecreaseCoins");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, coin1);
	lua_pushnumber(L, coin2);
	lua_pushnumber(L, coin3);

	int nStatus = lua_pcall(L, 4, 0, 0);

	this->ProcessErrorLog(L, "ItemBuyDecreaseCoins", nStatus);
}

void CLuaGameServer::ItemSellAddCoins(int aIndex, int coin1, int coin2, int coin3)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "ItemSellAddCoins");

	lua_pushnumber(L, aIndex);
	lua_pushnumber(L, coin1);
	lua_pushnumber(L, coin2);
	lua_pushnumber(L, coin3);

	int nStatus = lua_pcall(L, 4, 0, 0);

	this->ProcessErrorLog(L, "ItemSellAddCoins", nStatus);
}

void CLuaGameServer::AutoResetPlayerProc(int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "AutoResetPlayerProc");

	lua_pushnumber(L, aIndex);

	int nStatus = lua_pcall(L, 1, 0, 0);

	this->ProcessErrorLog(L, "AutoResetPlayerProc", nStatus);
}

void CLuaGameServer::QueryAsyncProcess(std::string name, std::string param, int aIndex)
{
	if (this->m_Reloading)
	{
		return;
	}

	CLocker lock(gSocketManager.m_critical);

	auto L = LuaManager.getMyLua();

	if (L == nullptr) {
		return;
	}

	lua_getglobal(L, "QueryAsyncProcess");

	lua_pushstring(L, name.c_str());
	lua_pushstring(L, param.c_str());
	lua_pushinteger(L, aIndex);

	int nStatus = lua_pcall(L, 3, 0, 0);

	if (nStatus != 0) {
		std::map<std::string, QueryJobsResult>::iterator job = gQueryAsync.m_jobsResult.find(param);

		if (job != gQueryAsync.m_jobsResult.end()) {
			gQueryAsync.m_jobsResult.erase(job);
		}
	}

	this->ProcessErrorLog(L, "QueryAsyncProcess", nStatus);
}

void CLuaGameServer::ProcessErrorLog(lua_State * L, std::string func, int nStatus)
{
	if (nStatus != 0)
	{
		std::string szError = luaL_checklstring(L, -1, 0);
		szError = "[" + func + "] error: " + szError;
		LogAddLua(LOG_RED, (char*)szError.c_str());
		lua_pop(L, 1);
	}
}

void CLuaGameServer::ProcessErrorLog(std::string func, std::string error)
{
	if (error.empty()) {
		return;
	}

	std::string szError = "[" + func + "] error: ";
	szError = szError + error;
	LogAddLua(LOG_RED, (char*)szError.c_str());
}
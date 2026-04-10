// CommandManager.h: interface for the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

enum eCommandNumber
{
	COMMAND_MOVE = 0,
	COMMAND_POST,
	COMMAND_ADD_POINT1,
	COMMAND_ADD_POINT2,
	COMMAND_ADD_POINT3,
	COMMAND_ADD_POINT4,
	COMMAND_ADD_POINT5,
	COMMAND_PK_CLEAR,
	COMMAND_MONEY,
	COMMAND_CHANGE,
	COMMAND_WARE,
	COMMAND_RESET,
	COMMAND_GM_MOVE,
	COMMAND_GM_POST,
	COMMAND_TRACK,
	COMMAND_TRACE,
	COMMAND_DISCONNECT,
	COMMAND_FIREWORKS,
	COMMAND_MAKE,
	COMMAND_SKIN,
	COMMAND_SET_MONEY,
	COMMAND_NOTICE,
	COMMAND_MASTER_RESET,
	COMMAND_GUILD_WAR,
	COMMAND_BATTLE_SOCCER,
	COMMAND_REQUEST,
	COMMAND_CUSTOM_STORE ,
	COMMAND_CUSTOM_STORE_OFFLINE,
	COMMAND_HIDE,
	COMMAND_CUSTOM_ATTACK,
	COMMAND_CUSTOM_ATTACK_OFFLINE,
	COMMAND_READD,
	COMMAND_MAKESET,
	COMMAND_SETLEVEL,
	COMMAND_CLEAR_INVENTORY,
	COMMAND_OPEN_WAREHOUSE,
	COMMAND_CLASS,
	COMMAND_SETSTATS,
	COMMAND_SETVIP,
	COMMAND_STARTBC,
	COMMAND_STARTDS,
	COMMAND_STARTCC,
	COMMAND_STARTIT,
	COMMAND_SIEGE,
	COMMAND_CRYWOLF,
	COMMAND_CASTLEDEEP,
	COMMAND_RELOAD = 46,
	COMMAND_STARTINVASION = 47,
	COMMAND_PICK = 48,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_COMMAND_MASTER_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
	UINT MasterResetDay;
	UINT MasterResetWek;
	UINT MasterResetMon;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_MASTER_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct COMMAND_INFO
{
	int Index;
	int Switch[MAX_ACCOUNT_LEVEL];
	DWORD Delay;
	int GMLevel;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	DWORD Money[MAX_ACCOUNT_LEVEL];
	DWORD Coin[MAX_ACCOUNT_LEVEL];
	char Command[32];
};

class CCommandManager
{
public:
	CCommandManager();
	virtual ~CCommandManager();
	void Load(char* path);
	void MainProc();
	long GetNumber(char* arg,int pos);
	void GetString(char* arg,char* out,int size,int pos);
	bool GetCommandCode(int Index, COMMAND_INFO* lpInfo);
	bool GetCommandCode(char* label, COMMAND_INFO* lpInfo, LPOBJ lpObj);
	void ManagementCore(LPOBJ lpObj,char* message);
	void CommandMove(LPOBJ lpObj,char* arg);
	void CommandPost(LPOBJ lpObj,char* arg);
	void CommandAddPoint(LPOBJ lpObj,char* arg,int type);
	void CommandAddPointAuto(LPOBJ lpObj,char* arg,int type);
	void CommandPKClear(LPOBJ lpObj,char* arg);
	void CommandMoney(LPOBJ lpObj,char* arg);
	void CommandChange(LPOBJ lpObj,char* arg);
	void CommandWare(LPOBJ lpObj,char* arg);
	void CommandReset(LPOBJ lpObj,char* arg);
	void CommandResetAuto(LPOBJ lpObj,char* arg);
	void CommandGMMove(LPOBJ lpObj,char* arg);
	void CommandGMPost(LPOBJ lpObj,char* arg);
	void CommandTrack(LPOBJ lpObj,char* arg);
	void CommandTrace(LPOBJ lpObj,char* arg);
	void CommandDisconnect(LPOBJ lpObj,char* arg);
	void CommandFireworks(LPOBJ lpObj,char* arg);
	void CommandMake(LPOBJ lpObj,char* arg);
	void CommandSkin(LPOBJ lpObj,char* arg);
	void CommandSetMoney(LPOBJ lpObj,char* arg);
	void CommandNotice(LPOBJ lpObj,char* arg);
	void CommandMasterReset(LPOBJ lpObj,char* arg);
	void CommandGuildWar(LPOBJ lpObj,char* arg);
	void CommandBattleSoccer(LPOBJ lpObj,char* arg);
	void CommandRequest(LPOBJ lpObj,char* arg);
	void CommandHide(LPOBJ lpObj,char* arg);
	void CommandAddPointAutoProc(LPOBJ lpObj);
	void CommandResetAutoProc(LPOBJ lpObj);
	void DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg);
	void DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg);
	void CommandReAdd(LPOBJ lpObj, char* arg);
	void CommandMakeSet(LPOBJ lpObj, char* arg);
	void CommandSetLevel(LPOBJ lpObj, char* arg);
	void CommandClearInventory(LPOBJ lpObj, char* arg);
	void CommandOpenWarehouse(LPOBJ lpObj);
	void CommandChangeClass(LPOBJ lpObj, char* arg);
	void CommandSetStats(LPOBJ lpObj, char* arg);
	void CommandSetVIP(LPOBJ lpObj, char* arg);
	void CommandStartBC(LPOBJ lpObj);
	void CommandStartDS(LPOBJ lpObj);
	void CommandStartCC(LPOBJ lpObj);
	void CommandStartIT(LPOBJ lpObj);
	void CommandStartCS(LPOBJ lpObj, char* arg);
	void CommandStartCW(LPOBJ lpObj, char* arg);
	void CommandStartCD(LPOBJ lpObj);
	bool CommandReload(LPOBJ lpObj, char* arg);
	bool CommandStartInvasion(LPOBJ lpObj, char* arg);
private:
	std::map<int, COMMAND_INFO> m_CommandListInfo;
};

extern CCommandManager gCommandManager;

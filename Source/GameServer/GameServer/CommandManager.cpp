// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "BonusManager.h"
#include "CustomAttack.h"
#include "CustomStore.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Guild.h"
#include "InvasionManager.h"
#include "ItemManager.h"
#include "Log.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestReward.h"
#include "ResetTable.h"
#include "ServerInfo.h"
#include "Util.h"
#include "MemScript.h"
#include "Warehouse.h"
#include "JSProtocol.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "IllusionTemple.h"
#include "CastleSiege.h"
#include "Crywolf.h"
#include "CastleDeep.h"
#include "CustomPick.h"
#include "ItemBagManager.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() // OK
{
	this->m_CommandListInfo.clear();
}

CCommandManager::~CCommandManager() // OK
{

}

void CCommandManager::Load(char* path) // OK
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

	this->m_CommandListInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			COMMAND_INFO info;

			memset(&info, 0, sizeof(info));

			info.Index = lpMemScript->GetNumber();

			info.Switch[0] = lpMemScript->GetAsNumber();

			info.Switch[1] = lpMemScript->GetAsNumber();

			info.Switch[2] = lpMemScript->GetAsNumber();

			info.Switch[3] = lpMemScript->GetAsNumber();

			strcpy_s(info.Command, lpMemScript->GetAsString());

			info.Delay = lpMemScript->GetAsNumber();

			info.GMLevel = lpMemScript->GetAsNumber();

			info.MinLevel = lpMemScript->GetAsNumber();

			info.MaxLevel = lpMemScript->GetAsNumber();

			info.MinReset = lpMemScript->GetAsNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.Money[0] = lpMemScript->GetAsNumber();

			info.Money[1] = lpMemScript->GetAsNumber();

			info.Money[2] = lpMemScript->GetAsNumber();

			info.Money[3] = lpMemScript->GetAsNumber();

			info.Coin[0] = lpMemScript->GetAsNumber();

			info.Coin[1] = lpMemScript->GetAsNumber();

			info.Coin[2] = lpMemScript->GetAsNumber();

			info.Coin[3] = lpMemScript->GetAsNumber();

			this->m_CommandListInfo.insert(std::pair<int, COMMAND_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCommandManager::MainProc() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if(gObj[n].AutoAddPointCount > 0)
		{
			this->CommandAddPointAutoProc(&gObj[n]);
		}

		if(gObj[n].AutoResetEnable != 0)
		{
			this->CommandResetAutoProc(&gObj[n]);
		}
	}
}

long CCommandManager::GetNumber(char* arg,int pos) // OK
{
	int count=0,p=0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len>=sizeof(buffer))?(sizeof(buffer)-1):len);

	for(int n=0;n < len;n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg,char* out,int size,int pos) // OK
{
	int count=0,p=0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len>=sizeof(buffer))?(sizeof(buffer)-1):len);

	for(int n=0;n < len;n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	memcpy(out,buffer,(size-1));
}


bool CCommandManager::GetCommandCode(int Index, COMMAND_INFO* lpInfo) // OK
{
	std::map<int, COMMAND_INFO>::iterator it = this->m_CommandListInfo.find(Index);

	if (it != this->m_CommandListInfo.end())
	{
		(*lpInfo) = it->second;
		return 1;
	}

	return 0;
}

bool CCommandManager::GetCommandCode(char* label, COMMAND_INFO* lpInfo, LPOBJ lpObj) // OK
{
	for (std::map<int, COMMAND_INFO>::iterator it = this->m_CommandListInfo.begin(); it != this->m_CommandListInfo.end(); it++)
	{
		bool exactMatch = (strcmp(label, it->second.Command) == 0);
		bool ignoreCaseMatch = (_stricmp(label, it->second.Command) == 0);

		if (gServerInfo.m_CommandCaseSensitive == 1)
		{
			if (exactMatch)
			{
				(*lpInfo) = it->second;
				return 1;
			}

			if (ignoreCaseMatch && !exactMatch && lpObj != nullptr)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(666));
				return 0;
			}
		}
		else
		{
			if (ignoreCaseMatch)
			{
				(*lpInfo) = it->second;
				return 1;
			}
		}
	}
	return 0;
}
void CCommandManager::ManagementCore(LPOBJ lpObj,char* message) // OK
{
	char command[32] = { 0 };

	memset(command, 0, sizeof(command));

	this->GetString(message, command, sizeof(command), 0);

	char* argument = &message[strlen(command)];

	if (argument[0] == 0x20)
	{
		argument++;
	}

	COMMAND_INFO lpInfo;

	if (this->GetCommandCode(command, &lpInfo, lpObj) == 0)
	{
		return;
	}

	if (lpInfo.Switch[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if (lpObj->TradeMoney != 0)
	{
		return;
	}

	DWORD TickCount = (GetTickCount() - lpObj->CommandCheckTime[lpInfo.Index]) / 1000;

	if (TickCount < lpInfo.Delay)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(650), (lpInfo.Delay - TickCount), lpInfo.Command);
		return;
	}

	if (lpInfo.GMLevel != -1 && lpInfo.GMLevel > gGameMaster.CheckGameMasterLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(301), lpInfo.Command);
		return;
	}

	if (lpInfo.MinLevel != -1 && lpInfo.MinLevel > lpObj->Level)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(651), lpInfo.MinLevel, lpInfo.Command);
		return;
	}

	if (lpInfo.MaxLevel != -1 && lpInfo.MaxLevel < lpObj->Level)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(652), lpInfo.MaxLevel, lpInfo.Command);
		return;
	}

	if (lpInfo.MinReset != -1 && lpInfo.MinReset > lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(653), lpInfo.MinReset, lpInfo.Command);
		return;
	}

	if (lpInfo.MaxReset != -1 && lpInfo.MaxReset < lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(654), lpInfo.MaxReset, lpInfo.Command);
		return;
	}

	if (lpInfo.Money[lpObj->AccountLevel] > lpObj->Money)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(655), lpInfo.Money[lpObj->AccountLevel], lpInfo.Command);
		return;
	}

	int Result = 0;

	switch(lpInfo.Index)
	{
		case COMMAND_MOVE:
			this->CommandMove(lpObj,argument);
			break;
		case COMMAND_POST:
			this->CommandPost(lpObj,argument);
			break;
		case COMMAND_ADD_POINT1:
			this->CommandAddPoint(lpObj,argument,0);
			break;
		case COMMAND_ADD_POINT2:
			this->CommandAddPoint(lpObj,argument,1);
			break;
		case COMMAND_ADD_POINT3:
			this->CommandAddPoint(lpObj,argument,2);
			break;
		case COMMAND_ADD_POINT4:
			this->CommandAddPoint(lpObj,argument,3);
			break;
		case COMMAND_ADD_POINT5:
			this->CommandAddPoint(lpObj,argument,4);
			break;
		case COMMAND_PK_CLEAR:
			this->CommandPKClear(lpObj,argument);
			break;
		case COMMAND_MONEY:
			this->CommandMoney(lpObj,argument);
			break;
		case COMMAND_CHANGE:
			this->CommandChange(lpObj,argument);
			break;
		case COMMAND_WARE:
			this->CommandWare(lpObj,argument);
			break;
		case COMMAND_RESET:
			this->CommandReset(lpObj,argument);
			break;
		case COMMAND_GM_MOVE:
			this->CommandGMMove(lpObj,argument);
			break;
		case COMMAND_GM_POST:
			this->CommandGMPost(lpObj,argument);
			break;
		case COMMAND_TRACK:
			this->CommandTrack(lpObj,argument);
			break;
		case COMMAND_TRACE:
			this->CommandTrace(lpObj,argument);
			break;
		case COMMAND_DISCONNECT:
			this->CommandDisconnect(lpObj,argument);
			break;
		case COMMAND_FIREWORKS:
			this->CommandFireworks(lpObj,argument);
			break;
		case COMMAND_MAKE:
			this->CommandMake(lpObj,argument);
			break;
		case COMMAND_SKIN:
			this->CommandSkin(lpObj,argument);
			break;
		case COMMAND_SET_MONEY:
			this->CommandSetMoney(lpObj,argument);
			break;
		case COMMAND_NOTICE:
			this->CommandNotice(lpObj,argument);
			break;
		case COMMAND_MASTER_RESET:
			this->CommandMasterReset(lpObj,argument);
			break;
		case COMMAND_GUILD_WAR:
			this->CommandGuildWar(lpObj,argument);
			break;
		case COMMAND_BATTLE_SOCCER:
			this->CommandBattleSoccer(lpObj,argument);
			break;
		case COMMAND_REQUEST:
			this->CommandRequest(lpObj,argument);
			break;
		case COMMAND_CUSTOM_STORE:
			gCustomStore.CommandCustomStore(lpObj,argument);
			break;
		case COMMAND_CUSTOM_STORE_OFFLINE:
			gCustomStore.CommandCustomStoreOffline(lpObj,argument);
			break;
		case COMMAND_HIDE:
			this->CommandHide(lpObj,argument);
			break;
		case COMMAND_CUSTOM_ATTACK:
			gCustomAttack.CommandCustomAttack(lpObj,argument);
			break;
		case COMMAND_CUSTOM_ATTACK_OFFLINE:
			gCustomAttack.CommandCustomAttackOffline(lpObj,argument);
			break;
		case COMMAND_READD:
			this->CommandReAdd(lpObj, argument);
			break;
		case COMMAND_MAKESET:
			this->CommandMakeSet(lpObj, argument);
			break;
		case COMMAND_SETLEVEL:
			this->CommandSetLevel(lpObj, argument);
			break; 
		case COMMAND_CLEAR_INVENTORY:
			this->CommandClearInventory(lpObj, argument);
			break;
		case COMMAND_OPEN_WAREHOUSE:
			this->CommandOpenWarehouse(lpObj);
			break;
		case COMMAND_CLASS:
			this->CommandChangeClass(lpObj, argument);
			break;
		case COMMAND_SETSTATS:
			this->CommandSetStats(lpObj, argument);
			break;
		case COMMAND_SETVIP:
			this->CommandSetVIP(lpObj, argument);
			break;
		case COMMAND_STARTBC:
			this->CommandStartBC(lpObj);
			break;
		case COMMAND_STARTDS:
			this->CommandStartDS(lpObj);
			break;
		case COMMAND_STARTCC:
			this->CommandStartCC(lpObj);
			break;
		case COMMAND_STARTIT:
			this->CommandStartIT(lpObj);
			break;
		case COMMAND_SIEGE:
			this->CommandStartCS(lpObj, argument);
			break;
		case COMMAND_CRYWOLF:
			this->CommandStartCW(lpObj, argument);
			break;
		case COMMAND_CASTLEDEEP:
			this->CommandStartCD(lpObj);
			break;
		default:
			return;
		case COMMAND_RELOAD:
			this->CommandReload(lpObj, argument);
			break;
		case COMMAND_STARTINVASION:
			this->CommandStartInvasion(lpObj, argument);
			break;
		case COMMAND_PICK:
			Result = gCustomPick.CommandPick(lpObj, argument);
			break;

	}

	lpObj->CommandCheckTime[lpInfo.Index] = GetTickCount();
}

void CCommandManager::CommandMove(LPOBJ lpObj,char* arg) // OK
{
	MOVE_INFO MoveInfo;

	if(gMove.GetInfoByName(arg,&MoveInfo) != 0)
	{
		gMove.Move(lpObj,MoveInfo.Index);
		return;
	}
}

void CCommandManager::CommandPost(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Level < gServerInfo.m_CommandPostLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(65),gServerInfo.m_CommandPostLevel[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Reset < gServerInfo.m_CommandPostReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(66),gServerInfo.m_CommandPostReset[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandPostMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(67),gServerInfo.m_CommandPostMoney[lpObj->AccountLevel]);
		return;
	}

	DWORD tick = (GetTickCount()-lpObj->PostTime)/1000;

	if(tick < ((DWORD)gServerInfo.m_CommandPostDelay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(68),(gServerInfo.m_CommandPostDelay[lpObj->AccountLevel]-tick));
		return;
	}

	lpObj->PostTime = GetTickCount();

	lpObj->Money -= gServerInfo.m_CommandPostMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	if(gServerInfo.m_CommandPostType == 0)
	{
		PostMessage1(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 1)
	{
		PostMessage2(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 2)
	{
		PostMessage3(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 3)
	{
		PostMessage4(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 4)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),0,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 5)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),1,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 6)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),2,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 7)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),3,lpObj->Name,arg);
	}

	gLog.Output(LOG_COMMAND,"[CommandPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandAddPoint(LPOBJ lpObj,char* arg,int type) // OK
{
	if(lpObj->Class != CLASS_DL && type == 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(71));
		return;
	}

	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandAddPointAuto(lpObj,arg,type);
		return;
	}

	int amount = this->GetNumber(arg,0);

	if(amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(72));
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(73),gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]);
		return;
	}

	if(gObjectManager.CharacterLevelUpPointAdd(lpObj,type,amount) == 0)
	{
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	GCNewCharacterInfoSend(lpObj);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(74),amount,lpObj->LevelUpPoint);

	gLog.Output(LOG_COMMAND,"[CommandAddPoint][%s][%s] - (Type: %d, Amount: %d)",lpObj->Account,lpObj->Name,type,amount);
}

void CCommandManager::CommandAddPointAuto(LPOBJ lpObj,char* arg,int type) // OK
{
	if(gServerInfo.m_CommandAddPointAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(70));
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(73),gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel]);
		return;
	}

	int amount = this->GetNumber(arg,1);

	if(amount > 0)
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = amount;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->Money -= gServerInfo.m_CommandAddPointMoney[lpObj->AccountLevel];

		GCMoneySend(lpObj->Index,lpObj->Money);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(114),amount);

		gLog.Output(LOG_COMMAND,"[CommandAddPointAuto][%s][%s] - (Type: %d, Amount: %d)",lpObj->Account,lpObj->Name,type,amount);
	}
	else
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = 0;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(115));
	}
}

void CCommandManager::CommandPKClear(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->PKLevel <= 3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(76));
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandPKClearMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(77),gServerInfo.m_CommandPKClearMoney[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandPKClearMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->PKLevel = 3;

	GCPKLevelSend(lpObj->Index,lpObj->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(78));

	gLog.Output(LOG_COMMAND,"[CommandPKClear][%s][%s] - (PKLevel: %d)",lpObj->Account,lpObj->Name,lpObj->PKLevel);
}

void CCommandManager::CommandMoney(LPOBJ lpObj,char* arg) // OK
{
	int money = this->GetNumber(arg,0);

	if(money < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(80));
		return;
	}

	if(money == 0)
	{
		lpObj->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpObj->Index,money) == 0)
	{
		lpObj->Money = MAX_MONEY;
	}
	else
	{
		lpObj->Money += money;
	}

	GCMoneySend(lpObj->Index,lpObj->Money);

	gLog.Output(LOG_COMMAND,"[CommandMoney][%s][%s] - (Money: %d)",lpObj->Account,lpObj->Name,money);
}

void CCommandManager::CommandChange(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandChangeMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(82),gServerInfo.m_CommandChangeMoney[lpObj->AccountLevel]);
		return;
	}

	int ChangeUp = lpObj->ChangeUp;

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		ChangeUp++;
	}

	if(ChangeUp >= gServerInfo.m_CommandChangeLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(83));
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandChangeMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	for(int n=0;n <= ((ChangeUp==0)?3:6);n++)
	{
		if(gQuest.CheckQuestListState(lpObj,n,QUEST_FINISH) == 0)
		{
			if(n != 3 || lpObj->Class == CLASS_DK)
			{
				gQuest.AddQuestList(lpObj,n,QUEST_ACCEPT);
				gQuestReward.InsertQuestReward(lpObj,n);
				gQuest.AddQuestList(lpObj,n,QUEST_FINISH);
			}
		}
	}

	lpObj->SendQuestInfo = 0;

	gQuest.GCQuestInfoSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(84));

	gLog.Output(LOG_COMMAND,"[CommandChange][%s][%s] - (ChangeUp: %d)",lpObj->Account,lpObj->Name,ChangeUp);
}

void CCommandManager::CommandWare(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(86));
		return;
	}

	int number = this->GetNumber(arg,0);

	if(number < 0 || number >= gServerInfo.m_CommandWareNumber[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(87));
		return;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap,0xFF,WAREHOUSE_SIZE);

	for(int n=0;n < WAREHOUSE_SIZE;n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(88),number);

	gLog.Output(LOG_COMMAND,"[CommandWare][%s][%s] - (Number: %d)",lpObj->Account,lpObj->Name,number);
}

void CCommandManager::CommandReset(LPOBJ lpObj,char* arg) // OK
{
	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandResetAuto(lpObj,arg);
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(90));
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(91),gResetTable.GetResetLevel(lpObj));
		return;
	}

	if(lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(92),gResetTable.GetResetMoney(lpObj));
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(93),gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(94));
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::CommandResetAuto(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(89));
		return;
	}

	if(lpObj->AutoResetEnable == 0)
	{
		lpObj->AutoResetEnable = 1;

		lpObj->AutoResetStats[0] = this->GetNumber(arg,1);

		lpObj->AutoResetStats[1] = this->GetNumber(arg,2);

		lpObj->AutoResetStats[2] = this->GetNumber(arg,3);

		lpObj->AutoResetStats[3] = this->GetNumber(arg,4);

		lpObj->AutoResetStats[4] = ((lpObj->Class==CLASS_DL)?this->GetNumber(arg,5):0);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(116));
	}
	else
	{
		lpObj->AutoResetEnable = 0;

		lpObj->AutoResetStats[0] = 0;

		lpObj->AutoResetStats[1] = 0;

		lpObj->AutoResetStats[2] = 0;

		lpObj->AutoResetStats[3] = 0;

		lpObj->AutoResetStats[4] = 0;

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(117));
	}
}

void CCommandManager::CommandGMMove(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	int map = this->GetNumber(arg,1);

	int x = this->GetNumber(arg,2);

	int y = this->GetNumber(arg,3);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(97));
		return;
	}

	gObjTeleport(lpTarget->Index,map,x,y);

	gLog.Output(LOG_COMMAND,"[CommandGMMove][%s][%s] - (Name: %s, Map: %d, X: %d, Y: %d)",lpObj->Account,lpObj->Name,name,map,x,y);
}

void CCommandManager::CommandGMPost(LPOBJ lpObj,char* arg) // OK
{
	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[%s] %s",lpObj->Name,arg);

	gLog.Output(LOG_COMMAND,"[CommandGMPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandTrack(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return;
	}

	gObjTeleport(lpObj->Index,lpTarget->Map,lpTarget->X,lpTarget->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrack][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandTrace(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(102));
		return;
	}

	gObjTeleport(lpTarget->Index,lpObj->Map,lpObj->X,lpObj->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrace][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandDisconnect(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(104));
		return;
	}

	gObjUserKill(lpTarget->Index);

	gLog.Output(LOG_COMMAND,"[CommandDisconnect][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandFireworks(LPOBJ lpObj,char* arg) // OK
{
	for(int n=0;n < 15;n++)
	{
		GCFireworksSend(lpObj,(lpObj->X+(((GetLargeRand()%5)*2)-4)),(lpObj->Y+(((GetLargeRand()%5)*2)-4)));
	}

	gLog.Output(LOG_COMMAND,"[CommandFireworks][%s][%s] - (Map: %d, X: %d, Y: %d)",lpObj->Account,lpObj->Name,lpObj->Map,lpObj->X,lpObj->Y);
}

void CCommandManager::CommandMake(LPOBJ lpObj,char* arg) // OK
{
	int qtd = 0, section, type, level, skill, luck, option, exc, set, joh, o380, socket;

	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	section = this->GetNumber(arg, 0);

	if (section < 0)
	{
		qtd = section;
		section = this->GetNumber(arg, 1);
		type = this->GetNumber(arg, 2);
		level = this->GetNumber(arg, 3);
		skill = this->GetNumber(arg, 4);
		luck = this->GetNumber(arg, 5);
		option = this->GetNumber(arg, 6);
		exc = this->GetNumber(arg, 7);
		set = this->GetNumber(arg, 8);
		joh = this->GetNumber(arg, 9);
		o380 = this->GetNumber(arg, 10);
		socket = this->GetNumber(arg, 11);
	}
	else
	{
		type = this->GetNumber(arg, 1);
		level = this->GetNumber(arg, 2);
		skill = this->GetNumber(arg, 3);
		luck = this->GetNumber(arg, 4);
		option = this->GetNumber(arg, 5);
		exc = this->GetNumber(arg, 6);
		set = this->GetNumber(arg, 7);
		joh = this->GetNumber(arg, 8);
		o380 = this->GetNumber(arg, 9);
		socket = this->GetNumber(arg, 10);
	}

	if (IsValidItem(GET_ITEM(section, type)) == 0)
	{
		return;
	}

	for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
	{
		ItemSocketOption[n] = 0xFE;
	}

	(qtd < 0) ? qtd -= qtd * 2 : qtd;

	if (qtd > 0)
	{
		for (int n = 0; n < qtd; n++)
		{
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, joh, (o380 > 0) ? 255 : 0, ItemSocketOption, 0xFF, 0);
		}
	}
	else
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, joh, (o380 > 0) ? 255 : 0, ItemSocketOption, 0xFF, 0);
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(107));

	gLog.Output(LOG_COMMAND, "[CommandMake][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, JoH: %d, 380: %d, Sockets: %d, [Amount: %d])", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc, set, joh, (o380 > 0) ? 255 : 0, socket, (qtd > 0) ? qtd : 1);
}

void CCommandManager::CommandSkin(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(109));
		return;
	}
	
	lpTarget->Change = this->GetNumber(arg,1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND,"[CommandSkin][%s][%s] - (Name: %s, Change: %d)",lpObj->Account,lpObj->Name,name,lpTarget->Change);
}

void CCommandManager::CommandSetMoney(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(111));
		return;
	}

	int money = this->GetNumber(arg,1);

	if(money < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(112));
		return;
	}

	if(money == 0)
	{
		lpTarget->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpTarget->Index,money) == 0)
	{
		lpTarget->Money = MAX_MONEY;
	}
	else
	{
		lpTarget->Money += money;
	}

	GCMoneySend(lpTarget->Index,lpTarget->Money);

	gLog.Output(LOG_COMMAND,"[CommandSetMoney][%s][%s] - (Name: %s, Money: %d)",lpObj->Account,lpObj->Name,name,money);
}

void CCommandManager::CommandNotice(LPOBJ lpObj,char* arg) // OK
{
	GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,arg);

	gLog.Output(LOG_COMMAND,"[CommandNotice][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandMasterReset(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(119));
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(120),gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel]);
		return;
	}

	if (lpObj->Strength < gServerInfo.m_CommandMasterResetStrengthReq)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(674), gServerInfo.m_CommandMasterResetStrengthReq);
		return;
	}

	if (lpObj->Dexterity < gServerInfo.m_CommandMasterResetAgilityReq)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(675), gServerInfo.m_CommandMasterResetAgilityReq);
		return;
	}

	if (lpObj->Vitality < gServerInfo.m_CommandMasterResetVitalityhReq)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(676), gServerInfo.m_CommandMasterResetVitalityhReq);
		return;
	}

	if (lpObj->Energy < gServerInfo.m_CommandMasterResetEnergyReq)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(677), gServerInfo.m_CommandMasterResetEnergyReq);
		return;
	}

	if ((lpObj->DBClass == DB_CLASS_DL) || (lpObj->DBClass == DB_CLASS_LE))
	{
		if (lpObj->Leadership < gServerInfo.m_CommandMasterResetCommandReq)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(678), gServerInfo.m_CommandMasterResetCommandReq);
			return;
		}
	}

	if(lpObj->Reset < gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(121),gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(122),gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->MasterReset >= gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(123),gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel]);
		return;
	}

	if(gServerInfo.m_CommandMasterResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(124));
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_MASTER_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x01,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::CommandGuildWar(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName,lpObj->Index,0);
	}
}

void CCommandManager::CommandBattleSoccer(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName,lpObj->Index,1);
	}
}

void CCommandManager::CommandRequest(LPOBJ lpObj,char* arg) // OK
{
	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"on") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(126));
	}
	else if(strcmp(mode,"off") == 0)
	{
		lpObj->Option &= ~1;
		lpObj->Option &= ~2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(127));
	}
	else if(strcmp(mode,"auto") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option |= 4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		this->GetString(arg,lpObj->AutoPartyPassword,sizeof(lpObj->AutoPartyPassword),1);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(128));
	}
}

void CCommandManager::CommandHide(LPOBJ lpObj,char* arg) // OK
{
	if(gEffectManager.CheckEffect(lpObj,EFFECT_INVISIBILITY) == 0)
	{
		gEffectManager.AddEffect(lpObj,0,EFFECT_INVISIBILITY,0,0,0,0,0);

		gObjViewportListProtocolDestroy(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,1);
	}
	else
	{
		gEffectManager.DelEffect(lpObj,EFFECT_INVISIBILITY);

		gObjViewportListProtocolCreate(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,0);
	}
}

void CCommandManager::CommandAddPointAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->LevelUpPoint == 0 || lpObj->AutoAddPointCount == 0)
	{
		return;
	}

	int AddStatCount = lpObj->AutoAddPointCount;

	int* stat[5] = {&lpObj->Strength,&lpObj->Dexterity,&lpObj->Vitality,&lpObj->Energy,&lpObj->Leadership};

	for(int n=0;n < 5;n++)
	{
		if(lpObj->AutoAddPointStats[n] > 0)
		{
			if(AddStatCount == 0)
			{
				break;
			}

			int AddStat = lpObj->LevelUpPoint/AddStatCount;

			AddStat = (((lpObj->LevelUpPoint%AddStatCount)>0)?(AddStat+1):AddStat);

			AddStat = ((AddStat>lpObj->AutoAddPointStats[n])?lpObj->AutoAddPointStats[n]:AddStat);

			AddStat = ((((*stat[n])+AddStat)>gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])?(gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]-(*stat[n])):AddStat);

			AddStatCount--;

			(*stat[n]) += AddStat;

			lpObj->LevelUpPoint -= AddStat;

			lpObj->AutoAddPointStats[n] -= (((*stat[n])>=gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])?lpObj->AutoAddPointStats[n]:AddStat);

			lpObj->AutoAddPointCount = 0;

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);
}

void CCommandManager::CommandResetAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		return;
	}

	if(lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if(lpMsg->ResetDay >= ((DWORD)gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(129),gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetWek >= ((DWORD)gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(130),gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetMon >= ((DWORD)gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(131),gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]==-1)?(lpObj->Level-gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]):gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level-1];

	lpObj->Reset += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetDay += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetWek += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetMon += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoAddPointStats[0] = 0;

	lpObj->AutoAddPointStats[1] = 0;

	lpObj->AutoAddPointStats[2] = 0;

	lpObj->AutoAddPointStats[3] = 0;

	lpObj->AutoAddPointStats[4] = 0;

	if(gServerInfo.m_CommandResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass%16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandResetSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if(gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level-1)*gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:((lpObj->Level>220)?((lpObj->Level-220)*gServerInfo.m_PlusStatPoint):0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	if(gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;

		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;

		lpObj->LevelUpPoint += point;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDResetInfoSaveSend(lpObj->Index,lpMsg->ResetDay,lpMsg->ResetWek,lpMsg->ResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(95),lpObj->Reset);

	if(gServerInfo.m_CommandResetMove[lpObj->AccountLevel] != 0)
	{
		switch(lpObj->Class)
		{
			case CLASS_DW:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DK:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_FE:
				gObjMoveGate(lpObj->Index,27);
				break;
			case CLASS_MG:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DL:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_SU:
				gObjMoveGate(lpObj->Index,267);
				break;
			case CLASS_RF:
				gObjMoveGate(lpObj->Index,17);
				break;
		}
	}

	lpObj->AutoAddPointStats[0] = lpObj->AutoResetStats[0];

	lpObj->AutoAddPointStats[1] = lpObj->AutoResetStats[1];

	lpObj->AutoAddPointStats[2] = lpObj->AutoResetStats[2];

	lpObj->AutoAddPointStats[3] = lpObj->AutoResetStats[3];

	lpObj->AutoAddPointStats[4] = lpObj->AutoResetStats[4];

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	this->CommandAddPointAutoProc(lpObj);

	gLog.Output(LOG_COMMAND,"[CommandReset][%s][%s] - (Reset: %d)",lpObj->Account,lpObj->Name,lpObj->Reset);
}

void CCommandManager::DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg) // OK
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCommandMasterResetRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];
	lpObj->CommandManagerTransaction[0] = 0;

	if (lpMsg->MasterResetDay >= ((DWORD)gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(132), gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]);
		return;
	}
	if (lpMsg->MasterResetWek >= ((DWORD)gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(133), gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]);
		return;
	}
	if (lpMsg->MasterResetMon >= ((DWORD)gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(134), gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel];
	GCMoneySend(lpObj->Index, lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel] == -1) ? (lpObj->Level - gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]) : gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]);
	lpObj->Experience = gLevelExperience[lpObj->Level - 1];

	lpObj->Reset = ((gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel] == -1) ? (lpObj->Reset - gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]) : gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]);

	lpObj->MasterReset += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->MasterResetDay += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);
	lpMsg->MasterResetWek += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);
	lpMsg->MasterResetMon += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_MASTER_RESET_AMOUNT, gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	if (gServerInfo.m_CommandMasterResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass / 16) * 16);
		lpObj->ChangeUp = lpObj->DBClass % 16;
		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);
		BYTE Class = (lpObj->ChangeUp * 16);
		Class -= (Class / 32);
		Class += (lpObj->Class * 32);
		gQuest.GCQuestRewardSend(lpObj->Index, 201, Class);
		memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));
		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if (gServerInfo.m_CommandMasterResetSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;
		lpObj->MasterPoint = 0;
		for (int n = 0; n < MAX_SKILL_LIST; n++)
		{
			lpObj->Skill[n].Clear();
		}
		for (int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
		{
			lpObj->MasterSkill[n].Clear();
		}
		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);
		gSkillManager.GCSkillListSend(lpObj, 0);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if (gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel] * lpObj->MasterReset;
		point = (point * gServerInfo.m_CommandMasterResetPointRate[lpObj->Class]) / 100;
		point += (lpObj->Level - 1) * gServerInfo.m_LevelUpPoint[lpObj->Class];
		point += ((gQuest.CheckQuestListState(lpObj, 2, QUEST_FINISH) == 0) ? 0 : ((lpObj->Level > 220) ? ((lpObj->Level - 220) * gServerInfo.m_PlusStatPoint) : 0));
		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);
		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	/*
	if (gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);
		point = (point * gServerInfo.m_CommandResetPointRate[lpObj->Class]) / 100;
		lpObj->LevelUpPoint += point;
	}
	*/

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);
	GDMasterResetInfoSaveSend(lpObj->Index, lpMsg->MasterResetDay, lpMsg->MasterResetWek, lpMsg->MasterResetMon);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(125), lpObj->MasterReset);

	int iBagType = ITEM_BAG_MRESET_REWARD;

	if (gServerInfo.m_CommandMasterResetMove[lpObj->AccountLevel] != 0)
	{
		switch (lpObj->Class)
		{
		case CLASS_DW: gObjMoveGate(lpObj->Index, 17); break;
		case CLASS_DK: gObjMoveGate(lpObj->Index, 17); break;
		case CLASS_FE: gObjMoveGate(lpObj->Index, 27); break;
		case CLASS_MG: gObjMoveGate(lpObj->Index, 17); break;
		case CLASS_DL: gObjMoveGate(lpObj->Index, 17); break;
		case CLASS_SU: gObjMoveGate(lpObj->Index, 267); break;
		case CLASS_RF: gObjMoveGate(lpObj->Index, 17); break;
		}

		gItemBagManager.DropItemBySpecialValue(iBagType, lpObj, lpObj->Map, lpObj->X, lpObj->Y);
	}
	else
	{
		gItemBagManager.DropItemBySpecialValue(iBagType, lpObj, lpObj->Map, lpObj->X, lpObj->Y);
		GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
	}

	gLog.Output(LOG_COMMAND, "[CommandMasterReset][%s][%s] - (MasterReset: %d)", lpObj->Account, lpObj->Name, lpObj->MasterReset);
}

void CCommandManager::CommandReAdd(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.use != 0 || lpObj->Teleport != 0 || lpObj->DieRegen != 0 || lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(86));
		return;
	}

	int Resto = 0;

	for (int n = 0; n < 5; n++) 
	{
		Resto += gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, n);
	}

	lpObj->LevelUpPoint += (lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership - Resto);

	lpObj->Strength = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
	lpObj->Dexterity = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
	lpObj->Vitality = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
	lpObj->Energy = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
	lpObj->Leadership = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 4);

	lpObj->Strength = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
	lpObj->Dexterity = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
	lpObj->Vitality = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
	lpObj->Energy = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
	lpObj->Leadership = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 4);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	gLog.Output(LOG_COMMAND, "[CommandReAdd][%s][%s] - OK", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandMakeSet(LPOBJ lpObj, char* arg) // OK
{
	int type = this->GetNumber(arg, 0);
	int level = this->GetNumber(arg, 1);
	int skill = this->GetNumber(arg, 2);
	int luck = this->GetNumber(arg, 3);
	int option = this->GetNumber(arg, 4);
	int exc = this->GetNumber(arg, 5);
	int set = this->GetNumber(arg, 6);
	int joh = this->GetNumber(arg, 7);
	int o380 = this->GetNumber(arg, 8);
	int socket = this->GetNumber(arg, 9);

	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++)
	{
		ItemSocketOption[n] = 0xFE;
	}

	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(7, type), level, 0, skill, luck, option, -1, exc, set, joh, (o380 > 0) ? 255 : 0, ItemSocketOption, 0xFF, 0);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(8, type), level, 0, skill, luck, option, -1, exc, set, joh, (o380 > 0) ? 255 : 0, ItemSocketOption, 0xFF, 0);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(9, type), level, 0, skill, luck, option, -1, exc, set, joh, (o380 > 0) ? 255 : 0, ItemSocketOption, 0xFF, 0);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(10, type), level, 0, skill, luck, option, -1, exc, set, joh, (o380 > 0) ? 255 : 0, ItemSocketOption, 0xFF, 0);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(11, type), level, 0, skill, luck, option, -1, exc, set, joh, (o380 > 0) ? 255 : 0, ItemSocketOption, 0xFF, 0);

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(107));

	gLog.Output(LOG_COMMAND, "[CommandMakeSet][%s][%s] - (Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, Sockets: %d)", lpObj->Account, lpObj->Name, type, level, skill, luck, option, exc, set, socket);
}

void CCommandManager::CommandSetLevel(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(100));
		return;
	}

	int Level = this->GetNumber(arg, 1);

	if (Level > MAX_CHARACTER_LEVEL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(266));
		return;
	}

	lpTarget->Level = Level;

	lpTarget->Experience = gLevelExperience[lpTarget->Level - 1];

	GCLevelUpSend(lpTarget);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	gLog.Output(LOG_COMMAND, "[CommandSetLevel][%s][%s] - (Name: %s, Level: %d)", lpObj->Account, lpObj->Name, name, Level);
}

void CCommandManager::CommandClearInventory(LPOBJ lpObj, char* arg) // OK
{
	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		gItemManager.InventoryDelItem(lpObj->Index, i);
		gItemManager.GCItemDeleteSend(lpObj->Index, i, 1);
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(12));

	gLog.Output(LOG_COMMAND, "[Command Clear Inventory] Use for:[%s][%s]", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandOpenWarehouse(LPOBJ lpObj) // OK
{
	if (lpObj->Interface.use != 0)
	{
		return;
	}

	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX || lpObj->Interface.type == INTERFACE_TRADE || lpObj->Interface.type == INTERFACE_PERSONAL_SHOP)
	{
		return;
	}

	if (gServerInfo.m_CommandOpenWareOnlySafeZone == 1 && gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(13));
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_WAREHOUSE;
	lpObj->Interface.state = 0;

	gWarehouse.GDWarehouseItemSend(lpObj->Index, lpObj->Account);

	gLog.Output(LOG_COMMAND, "[Command Open Warehouse] Use for:[%s][%s]", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandChangeClass(LPOBJ lpObj, char* arg)
{
	char classe[4] = { 0 };

	this->GetString(arg, classe, sizeof(classe), 0);

	if (lpObj->Interface.use != 0 || lpObj->Teleport != 0 || lpObj->DieRegen != 0 || lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(21));
		return;
	}

	if (classe[0] == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(22));
		return;
	}

	int newclasse = -1;
	int newDBclass = -1;

	if (strcmp(classe, "dw") == 0)
	{
		if (lpObj->Class == CLASS_DW)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(23));
			return;
		}

		if (gServerInfo.m_CommandChangeClassToDW == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(24));
			return;
		}

		newclasse = CLASS_DW;

		newDBclass = DB_CLASS_DW;
	}

	if (strcmp(classe, "dk") == 0)
	{
		if (lpObj->Class == CLASS_DK)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(23));
			return;
		}

		if (gServerInfo.m_CommandChangeClassToDK == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(24));
			return;
		}

		newclasse = CLASS_DK;

		newDBclass = DB_CLASS_DK;
	}

	if (strcmp(classe, "elf") == 0)
	{
		if (lpObj->Class == CLASS_FE)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(23));
			return;
		}
		if (gServerInfo.m_CommandChangeClassToELF == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(24));
			return;
		}

		newclasse = CLASS_FE;

		newDBclass = DB_CLASS_FE;
	}

	if (strcmp(classe, "mg") == 0)
	{
		if (lpObj->Class == CLASS_MG)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(23));
			return;
		}
		if (gServerInfo.m_CommandChangeClassToMG == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(24));
			return;
		}

		newclasse = CLASS_MG;

		newDBclass = DB_CLASS_MG;
	}

	if (strcmp(classe, "dl") == 0)
	{
		if (lpObj->Class == CLASS_DL)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(23));
			return;
		}
		if (gServerInfo.m_CommandChangeClassToDL == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(24));
			return;
		}

		newclasse = CLASS_DL;

		newDBclass = DB_CLASS_DL;
	}

	if (strcmp(classe, "su") == 0)
	{
		if (lpObj->Class == CLASS_SU)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(23));
			return;
		}
		if (gServerInfo.m_CommandChangeClassToSU == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(24));
			return;
		}

		newclasse = CLASS_SU;

		newDBclass = DB_CLASS_SU;
	}

	if (newclasse < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(22));
		return;
	}

	for (int i = 0; i < 11; i++)
	{
		if (lpObj->Inventory[i].IsItem() != 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(25));
			return;
		}
	}

	if (lpObj->Interface.use != 0)
	{
		return;
	}

	lpObj->Class = newclasse;
	lpObj->DBClass = newDBclass;
	lpObj->ChangeUp = lpObj->DBClass % 16;

	gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

	BYTE Class = (lpObj->ChangeUp * 16);
	Class -= (Class / 32);
	Class += (lpObj->Class * 32);

	gQuest.GCQuestRewardSend(lpObj->Index, 201, Class);
	memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));
	gQuest.GCQuestInfoSend(lpObj->Index);

	lpObj->MasterLevel = 0;
	lpObj->MasterPoint = 0;
	lpObj->MasterExperience = 0;

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		lpObj->Skill[n].Clear();
	}

	for (int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
	{
		lpObj->MasterSkill[n].Clear();
	}

	gEffectManager.ClearAllEffect(lpObj);

	lpObj->CloseCount = 1;
	lpObj->CloseType = 1;

	gLog.Output(LOG_COMMAND, "[CommandChangeClass][%s][%s] - (ClassNum: %d)", lpObj->Account, lpObj->Name, newDBclass);
}

void CCommandManager::CommandSetStats(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(100));
		return;
	}

	int points = this->GetNumber(arg, 1);

	lpTarget->LevelUpPoint = (points < 0) ? 0 : lpTarget->LevelUpPoint + points;

	GCLevelUpSend(lpTarget);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	gLog.Output(LOG_COMMAND, "[CommandSetStats][%s][%s] - (Name: %s, LevelUpPoints: %d)", lpObj->Account, lpObj->Name, name, points);
}

void CCommandManager::CommandSetVIP(LPOBJ lpObj, char* arg) // OK
{
	char account[11] = { 0 };

	this->GetString(arg, account, sizeof(account), 0);

	int tipo = this->GetNumber(arg, 1);

	int dias = this->GetNumber(arg, 2);

	if (dias < 0 || tipo < 0)
	{
		return;
	}

	GJAccountLevelSaveSend(lpObj->Index, tipo, (dias * 86400), account);

	GJAccountLevelSend(lpObj->Index, account);

	gLog.Output(LOG_COMMAND, "[CommandSetVIP][%s][%s] - (Account: %s, Tipo: %d, Dias: %d)", lpObj->Account, lpObj->Name, account, tipo, dias);
}

void CCommandManager::CommandStartBC(LPOBJ lpObj)
{
	gBloodCastle.StartBC();

	gLog.Output(LOG_COMMAND, "[CommandStartBC][%s][%s] - Used", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandStartDS(LPOBJ lpObj) // OK
{
	gDevilSquare.StartDS();
	gLog.Output(LOG_COMMAND, "[CommandStartDS][%s][%s] - Used", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandStartCC(LPOBJ lpObj) // OK
{
	gChaosCastle.StartCC();
	gLog.Output(LOG_COMMAND, "[CommandStartCC][%s][%s] - Used", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandStartIT(LPOBJ lpObj) // OK
{
	gIllusionTemple.StartIT();
	gLog.Output(LOG_COMMAND, "[CommandStartIT][%s][%s] - Used", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandStartCS(LPOBJ lpObj, char* arg) // OK
{
	int Type = this->GetNumber(arg, 0);

	if (Type < 0 || Type > 7)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Os tipos são de 0 a 7.");
		return;
	}

	#if(GAMESERVER_TYPE == 1)
	switch (Type)
	{
	case 0:
		gCastleSiege.SetState(CS_STATE_BLANK, true);
		break;
	case 1:
		gCastleSiege.SetState(CS_STATE_SIGN1, true);
		break;
	case 2:
		gCastleSiege.SetState(CS_STATE_SIGN2, true);
		break;
	case 3:
		gCastleSiege.SetState(CS_STATE_NOTIFY, true);
		break;
	case 4:
		gCastleSiege.SetState(CS_STATE_READY, true);
		break;
	case 5:
		gCastleSiege.SetState(CS_STATE_START, true);
		break;
	case 6:
		gCastleSiege.SetState(CS_STATE_END, true);
		break;
	case 7:
		gCastleSiege.SetState(CS_STATE_END_CYCLE, true);
		break;
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "CS Iniciado com sucesso, Tipo usado: %d!", Type);

	#else

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Esse comando só funciona no GameServer Siege");

	#endif

	gLog.Output(LOG_COMMAND, "[CommandStartCS][%s][%s] - Used", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandStartCW(LPOBJ lpObj, char* arg) // OK
{
	int Type = this->GetNumber(arg, 0);

	if (Type < 0 || Type > 6)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Os tipos são de 0 a 6.");
		return;
	}

	#if(GAMESERVER_TYPE == 1)
	switch (Type)
	{
	case 0:
		gCrywolf.SetState(CRYWOLF_STATE_NONE);
		break;
	case 1:
		gCrywolf.SetState(CRYWOLF_STATE_NOTIFY1);
		break;
	case 2:
		gCrywolf.SetState(CRYWOLF_STATE_NOTIFY2);
		break;
	case 3:
		gCrywolf.SetState(CRYWOLF_STATE_READY);
		break;
	case 4:
		gCrywolf.SetState(CRYWOLF_STATE_START);
		break;
	case 5:
		gCrywolf.SetState(CRYWOLF_STATE_END);
		break;
	case 6:
		gCrywolf.SetState(CRYWOLF_STATE_ENDCYCLE);
		break;
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "CW Iniciado com sucesso, Tipo usado: %d!", Type);

	#else

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Esse comando só funciona no GameServer Siege");

	#endif


	gLog.Output(LOG_COMMAND, "[CommandStartCW][%s][%s] - Used", lpObj->Account, lpObj->Name);
}


void CCommandManager::CommandStartCD(LPOBJ lpObj) // OK
{
	gCastleDeep.StartCD();
	gLog.Output(LOG_COMMAND, "[CommandStartCC][%s][%s] - Used", lpObj->Account, lpObj->Name);
}

bool CCommandManager::CommandReload(LPOBJ lpObj, char* arg) // OK
{

	char name[20] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	if (strcmp(name, "cashshop") == 0)
	{
		gServerInfo.ReadCashShopInfo();
	}
	else if (strcmp(name, "chaosmix") == 0)
	{
		gServerInfo.ReadChaosMixInfo();
	}
	else if (strcmp(name, "character") == 0)
	{
		gServerInfo.ReadCharacterInfo();
	}
	else if (strcmp(name, "command") == 0)
	{
		gServerInfo.ReadCommandInfo();
	}
	else if (strcmp(name, "common") == 0)
	{
		gServerInfo.ReadCommonInfo();
	}
	else if (strcmp(name, "custom") == 0)
	{
		gServerInfo.ReadCustomInfo();
	}
	else if (strcmp(name, "event") == 0)
	{
		gServerInfo.ReadEventInfo();
	}
	else if (strcmp(name, "eventitembag") == 0)
	{
		gServerInfo.ReadEventItemBagInfo();
	}
	else if (strcmp(name, "hack") == 0)
	{
		gServerInfo.ReadHackInfo();
	}
	else if (strcmp(name, "item") == 0)
	{
		gServerInfo.ReadItemInfo();
	}
	else if (strcmp(name, "monster") == 0)
	{
		gServerInfo.ReloadMonsterInfo();
	}
	else if (strcmp(name, "move") == 0)
	{
		gServerInfo.ReadMoveInfo();
	}
	else if (strcmp(name, "quest") == 0)
	{
		gServerInfo.ReadQuestInfo();
	}
	else if (strcmp(name, "shop") == 0)
	{
		gServerInfo.ReadShopInfo();
	}
	else if (strcmp(name, "skill") == 0)
	{
		gServerInfo.ReadSkillInfo();
	}
	else if (strcmp(name, "util") == 0)
	{
		gServerInfo.ReadUtilInfo();
	}
	else if (strcmp(name, "all") == 0)
	{
		gServerInfo.ReloadAll();
	}
	else if (strcmp(name, "lua") == 0)
	{
		gLuaGameServer.Reload();
	}
	else if (strcmp(name, "terrain") == 0)
	{
		gServerInfo.ReadMapInfo();
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(648));
		return 0;
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(649));
	return 1;
}

bool CCommandManager::CommandStartInvasion(LPOBJ lpObj, char* arg) // OK
{

	int index = this->GetNumber(arg, 0);

	if (index < 0)
	{
		return 0;
	}

	gInvasionManager.StartInvasion(index);
	gLog.Output(LOG_COMMAND, "[CommandStartInvasion][%s][%s] - Used", lpObj->Account, lpObj->Name);
	return 1;
}
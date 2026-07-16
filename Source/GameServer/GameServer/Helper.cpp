// Helper.cpp: implementation of the CHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Helper.h"
#include "CastleSiege.h"
#include "GameMain.h"
#include "Map.h"
#include "MapManager.h"
#include "ServerInfo.h"
#include "Util.h"

CHelper gHelper;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelper::CHelper() // OK
{

}

CHelper::~CHelper() // OK
{

}

void CHelper::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		if(lpObj->HelperDelayTime != 0)
		{
			int HelperStage = 0;

			if(gMapManager.GetMapHelperEnable(lpObj->Map) == 0)
			{
				this->DisableHelper(lpObj);
				continue;
			}

			if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				this->DisableHelper(lpObj);
				continue;
			}

			if((HelperStage=((GetTickCount()-lpObj->HelperTotalTime)/12000000)) >= MAX_HELPER_STAGE)
			{
				this->DisableHelper(lpObj);
				continue;
			}

			if((GetTickCount()-lpObj->HelperDelayTime) >= ((DWORD)(gServerInfo.m_HelperActiveDelay*60000)))
			{
				if(lpObj->Money < ((DWORD)((lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[HelperStage])))
				{
					this->DisableHelper(lpObj);
					continue;
				}

				lpObj->HelperDelayTime = GetTickCount();

				lpObj->Money -= (lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[HelperStage];

				GCMoneySend(lpObj->Index,lpObj->Money);

				this->GCHelperStartSend(lpObj->Index,((GetTickCount()-lpObj->HelperTotalTime)/60000),((lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[HelperStage]),0);
			}
		}
	}

	#endif
}

void CHelper::DisableHelper(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	if(lpObj->HelperDelayTime == 0)
	{
		return;
	}

	lpObj->HelperDelayTime = 0;

	lpObj->HelperTotalTime = 0;

	this->GCHelperStartSend(lpObj->Index,0,0,1);

	#endif
}

void CHelper::CGHelperDataRecv(PMSG_HELPER_DATA_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	this->GDHelperDataSaveSend(aIndex,lpMsg->data);

	#endif
}

void CHelper::CGHelperStartRecv(PMSG_HELPER_START_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	#ifdef _DEBUG
	LogAdd(LOG_BLUE,"[MuHelper] Start request index=%d type=%d switch=%d",aIndex,lpMsg->type,gServerInfo.m_HelperSwitch);
	#endif

	if(gServerInfo.m_HelperSwitch == 0)
	{
		#ifdef _DEBUG
		LogAdd(LOG_RED,"[MuHelper] Start rejected: HelperSwitch is disabled (index=%d)",aIndex);
		#endif
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Level < gServerInfo.m_HelperActiveLevel)
	{
		#ifdef _DEBUG
		LogAdd(LOG_RED,"[MuHelper] Start rejected: level %d is below %d [%s][%s]",lpObj->Level,gServerInfo.m_HelperActiveLevel,lpObj->Account,lpObj->Name);
		#endif
		return;
	}

	if(gMapManager.GetMapHelperEnable(lpObj->Map) == 0)
	{
		#ifdef _DEBUG
		LogAdd(LOG_RED,"[MuHelper] Start rejected: HelperEnable=0 for map %d [%s][%s]",lpObj->Map,lpObj->Account,lpObj->Name);
		#endif
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		#ifdef _DEBUG
		LogAdd(LOG_RED,"[MuHelper] Start rejected: character is in safe zone map=%d x=%d y=%d [%s][%s]",lpObj->Map,lpObj->X,lpObj->Y,lpObj->Account,lpObj->Name);
		#endif
		return;
	}

	if(lpMsg->type == 0)
	{
		lpObj->HelperDelayTime = GetTickCount();

		lpObj->HelperTotalTime = GetTickCount();

		this->GCHelperStartSend(aIndex,0,0,0);

		#ifdef _DEBUG
		LogAdd(LOG_BLUE,"[MuHelper] Started successfully [%s][%s] map=%d",lpObj->Account,lpObj->Name,lpObj->Map);
		#endif
	}
	else
	{
		lpObj->HelperDelayTime = 0;

		lpObj->HelperTotalTime = 0;

		this->GCHelperStartSend(aIndex,0,0,1);
	}

	#endif
}

void CHelper::GCHelperStartSend(int aIndex,int time,int money,int result) // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	PMSG_HELPER_START_SEND pMsg;

	pMsg.header.set(0xBF,0x51,sizeof(pMsg));

	pMsg.time = time;

	pMsg.money = money;

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CHelper::DGHelperDataRecv(SDHP_HELPER_DATA_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGHelperDataRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_HELPER_DATA_SEND pMsg;

	pMsg.header.set(0xAE,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memcpy(pMsg.data,lpMsg->data,sizeof(pMsg.data));

	DataSend(lpMsg->index,(BYTE*)&pMsg,sizeof(pMsg));

	#endif
}

void CHelper::GDHelperDataSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_HELPER_DATA_SEND pMsg;

	pMsg.header.set(0x17,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CHelper::GDHelperDataSaveSend(int aIndex,BYTE* data) // OK
{
	#if(GAMESERVER_UPDATE>=603 || GAMESERVER_UPDATE==505)

	SDHP_HELPER_DATA_SAVE_SEND pMsg;

	pMsg.header.set(0x17,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	memcpy(pMsg.data,data,sizeof(pMsg.data));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));

	#endif
}

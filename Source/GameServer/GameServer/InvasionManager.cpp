// InvasionManager.cpp: implementation of the CInvasionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvasionManager.h"
#include "Map.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CInvasionManager gInvasionManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvasionManager::CInvasionManager() // OK
{
	this->m_ClientSyncTick = GetTickCount();

	for (int n = 0; n < MAX_INVASION; n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];
		lpInfo->Index = n;
		lpInfo->State = INVASION_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();
		lpInfo->AlarmMinLeft = -1;
		memset(lpInfo->AlertMessage, 0, sizeof(lpInfo->AlertMessage));
		lpInfo->CountNotify = 1;
		this->CleanMonster(lpInfo);
	}
}

CInvasionManager::~CInvasionManager() // OK
{

}

void CInvasionManager::Init() // OK
{
	for (int n = 0; n < MAX_INVASION; n++)
	{
		if (gServerInfo.m_InvasionManagerSwitch == 0)
		{
			this->SetState(&this->m_InvasionInfo[n], INVASION_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_InvasionInfo[n], INVASION_STATE_EMPTY);
		}
	}
}

void CInvasionManager::Load(char* path) // OK
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

	for (int n = 0; n < MAX_INVASION; n++)
	{
		this->m_InvasionInfo[n].RespawnMessage = -1;
		this->m_InvasionInfo[n].DespawnMessage = -1;
		this->m_InvasionInfo[n].BossIndex = -1;
		this->m_InvasionInfo[n].BossMessage = -1;
		this->m_InvasionInfo[n].InvasionTime = 0;
		this->m_InvasionInfo[n].StartTime.clear();
		this->m_InvasionInfo[n].RespawnInfo[0].clear();
		this->m_InvasionInfo[n].RespawnInfo[1].clear();
		this->m_InvasionInfo[n].RespawnInfo[2].clear();
		this->m_InvasionInfo[n].RespawnInfo[3].clear();
		this->m_InvasionInfo[n].RespawnInfo[4].clear();
		this->m_InvasionInfo[n].RespawnInfo[5].clear();
		this->m_InvasionInfo[n].RespawnInfo[6].clear();
		this->m_InvasionInfo[n].RespawnInfo[7].clear();
		this->m_InvasionInfo[n].RespawnInfo[8].clear();
		this->m_InvasionInfo[n].RespawnInfo[9].clear();
		this->m_InvasionInfo[n].RespawnInfo[10].clear();
		this->m_InvasionInfo[n].RespawnInfo[11].clear();
		this->m_InvasionInfo[n].RespawnInfo[12].clear();
		this->m_InvasionInfo[n].RespawnInfo[13].clear();
		this->m_InvasionInfo[n].RespawnInfo[14].clear();
		this->m_InvasionInfo[n].RespawnInfo[15].clear();
		this->m_InvasionInfo[n].RespawnInfo[16].clear();
		this->m_InvasionInfo[n].RespawnInfo[17].clear();
		this->m_InvasionInfo[n].RespawnInfo[18].clear();
		this->m_InvasionInfo[n].RespawnInfo[19].clear();
		this->m_InvasionInfo[n].MonsterInfo.clear();
		this->m_InvasionInfo[n].total_data_monster.clear();
	}

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					INVASION_START_TIME info;

					int index = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].StartTime.push_back(info);
				}
				else if (section == 1)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					this->m_InvasionInfo[index].RespawnMessage = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].DespawnMessage = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].BossIndex = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].BossMessage = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].InvasionTime = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].AlarmTime = lpMemScript->GetAsNumber();

					strcpy_s(this->m_InvasionInfo[index].AlertMessage, sizeof(this->m_InvasionInfo[index].AlertMessage), lpMemScript->GetAsString());

					this->m_InvasionInfo[index].CountNotify = lpMemScript->GetAsNumber();
				}
				else if (section == 2)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					INVASION_RESPWAN_INFO info;

					int index = lpMemScript->GetNumber();

					info.Group = lpMemScript->GetAsNumber();

					info.Map = lpMemScript->GetAsNumber();

					info.Value = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].RespawnInfo[info.Group].push_back(info);
				}
				else if (section == 3)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					INVASION_MONSTER_INFO info;

					int index = lpMemScript->GetNumber();

					info.Group = lpMemScript->GetAsNumber();

					info.MonsterClass = lpMemScript->GetAsNumber();

					info.RegenType = lpMemScript->GetAsNumber();

					info.RegenTime = lpMemScript->GetAsNumber();

					this->m_InvasionInfo[index].MonsterInfo.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CInvasionManager::MainProc() // OK
{
	for (int n = 0; n < MAX_INVASION; n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		DWORD elapsed = GetTickCount() - lpInfo->TickCount;

		if (elapsed < 1000)
		{
			continue;
		}


		if (lpInfo->State == INVASION_STATE_BLANK)
		{
			if (gServerDisplayer.EventInvasion[n] != -1)
			{
				gServerDisplayer.EventInvasion[n] = -1;
			}
		}
		else
		{
			if (lpInfo->State == INVASION_STATE_EMPTY)
			{
				gServerDisplayer.EventInvasion[n] = lpInfo->RemainTime;
			}
			else
			{
				if (gServerDisplayer.EventInvasion[n] != 0)
				{
					gServerDisplayer.EventInvasion[n] = 0;
				}
			}
		}

		if (gServerDisplayer.EventInvasion[n] > 0)
		{
			if (lpInfo->State != INVASION_STATE_EMPTY && lpInfo->State != INVASION_STATE_BLANK)
			{
				gServerDisplayer.EventInvasion[n] = 0;
			}
		}

		lpInfo->TickCount = GetTickCount();

		lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime, time(0));

		switch (lpInfo->State)
		{
		case INVASION_STATE_BLANK:
			this->ProcState_BLANK(lpInfo);
			break;
		case INVASION_STATE_EMPTY:
			this->ProcState_EMPTY(lpInfo);
			break;
		case INVASION_STATE_START:
			this->ProcState_START(lpInfo);
			break;
		}
	}

	DWORD currentTick = GetTickCount();

	if ((currentTick - this->m_ClientSyncTick) >= 5000)
	{
		this->m_ClientSyncTick = currentTick;

		for (int n = 0; n < MAX_INVASION; n++)
		{
			INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

			if (lpInfo->CountNotify == 0)
			{
				continue;
			}

			this->EnsureNotifyMonsterInfo(lpInfo);

			if (lpInfo->State == INVASION_STATE_START && lpInfo->total_data_monster.empty() == 0)
			{
				this->SendInfoInvasion(lpInfo, 1, -1);
			}
			else
			{
				this->SendInfoInvasion(lpInfo, 0, -1);
			}
		}
	}
}

void CInvasionManager::ProcState_BLANK(INVASION_INFO* lpInfo) // OK
{

}

void CInvasionManager::ProcState_EMPTY(INVASION_INFO* lpInfo) // OK
{
	if (lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->AlarmTime * 60))
	{
		if ((lpInfo->AlarmMinSave = (((lpInfo->RemainTime % 60) == 0) ? ((lpInfo->RemainTime / 60) - 1) : (lpInfo->RemainTime / 60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(534), lpInfo->AlertMessage, (lpInfo->AlarmMinLeft + 1));
		}
	}

	if (lpInfo->RemainTime <= 0)
	{
		if (lpInfo->RespawnMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(lpInfo->RespawnMessage));
		}

		this->SetState(lpInfo, INVASION_STATE_START);
	}
}

void CInvasionManager::ProcState_START(INVASION_INFO* lpInfo) // OK
{
	if (lpInfo->RemainTime <= 0)
	{
		if (lpInfo->DespawnMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(lpInfo->DespawnMessage));
		}

		this->SetState(lpInfo, INVASION_STATE_EMPTY);
	}
}

void CInvasionManager::SetState(INVASION_INFO* lpInfo, int state) // OK
{
	lpInfo->State = state;

	switch (lpInfo->State)
	{
	case INVASION_STATE_BLANK:
		this->SetState_BLANK(lpInfo);
		break;
	case INVASION_STATE_EMPTY:
		this->SetState_EMPTY(lpInfo);
		break;
	case INVASION_STATE_START:
		this->SetState_START(lpInfo);
		break;
	}
}

void CInvasionManager::SetState_BLANK(INVASION_INFO* lpInfo) // OK
{

}

void CInvasionManager::SetState_EMPTY(INVASION_INFO* lpInfo) // OK
{
	this->ClearMonster(lpInfo);

	this->CheckSync(lpInfo);

	lpInfo->total_data_monster.clear();

	if(lpInfo->CountNotify == 1)
	{
		this->SendInfoInvasion(lpInfo, 0, -1);
	}
}

void CInvasionManager::SetState_START(INVASION_INFO* lpInfo) // OK
{
	lpInfo->total_data_monster.clear();

	for (int n = 0; n < MAX_INVASION_RESPAWN_GROUP; n++)
	{
		if (lpInfo->RespawnInfo[n].empty() == 0)
		{
			INVASION_RESPWAN_INFO* lpRespawnInfo = &lpInfo->RespawnInfo[n][(GetLargeRand() % lpInfo->RespawnInfo[n].size())];

			for (std::vector<INVASION_MONSTER_INFO>::iterator it = lpInfo->MonsterInfo.begin(); it != lpInfo->MonsterInfo.end(); it++)
			{
				if (it->Group == lpRespawnInfo->Group)
				{
					this->SetMonster(lpInfo, lpRespawnInfo, &(*it));
				}
			}
		}
	}

	lpInfo->RemainTime = lpInfo->InvasionTime;

	lpInfo->TargetTime = (int)(time(0) + lpInfo->RemainTime);

	this->RebuildNotifyMonsterInfo(lpInfo);

	if(lpInfo->CountNotify == 1)
	{
		this->SendInfoInvasion(lpInfo, 1, -1);
	}
}

void CInvasionManager::CheckSync(INVASION_INFO* lpInfo) // OK
{
	if (lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo, INVASION_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<INVASION_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo, INVASION_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

int CInvasionManager::GetState(int index) // OK
{
	if (index < 0 || index >= MAX_INVASION)
	{
		return INVASION_STATE_BLANK;
	}

	return this->m_InvasionInfo[index].State;
}

int CInvasionManager::GetRemainTime(int index) // OK
{
	if (index < 0 || index >= MAX_INVASION)
	{
		return 0;
	}

	INVASION_INFO* lpInfo = &this->m_InvasionInfo[index];

	if (lpInfo->StartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<INVASION_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	return (((RemainTime % 60) == 0) ? (RemainTime / 60) : ((RemainTime / 60) + 1));
}

bool CInvasionManager::AddMonster(INVASION_INFO* lpInfo, int aIndex) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if (this->GetMonster(lpInfo, aIndex) != 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpInfo->MonsterIndex[n]) == 0)
		{
			lpInfo->MonsterIndex[n] = aIndex;
			return 1;
		}
	}

	return 0;
}

bool CInvasionManager::DelMonster(INVASION_INFO* lpInfo, int aIndex) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int* index = this->GetMonster(lpInfo, aIndex);

	if (index == 0)
	{
		return 0;
	}

	(*index) = -1;
	return 1;
}

int* CInvasionManager::GetMonster(INVASION_INFO* lpInfo, int aIndex) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (lpInfo->MonsterIndex[n] == aIndex)
		{
			return &lpInfo->MonsterIndex[n];
		}
	}

	return 0;
}

void CInvasionManager::CleanMonster(INVASION_INFO* lpInfo) // OK
{
	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		lpInfo->MonsterIndex[n] = -1;
	}
}

void CInvasionManager::ClearMonster(INVASION_INFO* lpInfo) // OK
{
	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpInfo->MonsterIndex[n]) != 0)
		{
			gObjDel(lpInfo->MonsterIndex[n]);
			lpInfo->MonsterIndex[n] = -1;
		}
	}
}

int CInvasionManager::GetMonsterCount(INVASION_INFO* lpInfo) // OK
{
	int count = 0;

	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpInfo->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;
}

void CInvasionManager::SetMonster(INVASION_INFO* lpInfo, INVASION_RESPWAN_INFO* lpRespawnInfo, INVASION_MONSTER_INFO* lpMonsterInfo) // OK
{
	for (int n = 0; n < gMonsterSetBase.m_count; n++)
	{
		MONSTER_SET_BASE_INFO* lpMsbInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if (lpMsbInfo->Type != 3 || lpMsbInfo->MonsterClass != lpMonsterInfo->MonsterClass || lpMsbInfo->Map != lpRespawnInfo->Map || lpMsbInfo->Value != lpRespawnInfo->Value)
		{
			continue;
		}

		int index = gObjAddMonster(lpMsbInfo->Map);

		if (OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[index];

		if (gObjSetPosMonster(index, n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if (gObjSetMonster(index, lpMsbInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

		lpObj->MaxRegenTime = ((lpMonsterInfo->RegenType == 0) ? (lpInfo->InvasionTime * 1000) : lpMonsterInfo->RegenTime);
		LogAdd(LOG_BLACK, "[Invasion Manager] Index(%d) Boss Position (Map: %d X: %d Y: %d)", lpInfo->Index, lpObj->Map, lpObj->X, lpObj->Y);

		if (this->AddMonster(lpInfo, index) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(lpInfo->CountNotify == 1)
		{
			this->AddNotifyMonsterInfo(lpInfo, lpMsbInfo->MonsterClass, 1);
		}
	}

}

void CInvasionManager::MonsterDieProc(LPOBJ lpObj, LPOBJ lpTarget) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);
	if (OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	for (int n = 0; n < MAX_INVASION; n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		if (this->GetState(lpInfo->Index) != INVASION_STATE_START)
			continue;

		if (this->GetMonster(lpInfo, lpObj->Index) == 0)
			continue;

		int liveCount = 0;

		for (int i = 0; i < MAX_INVASION_MONSTER; i++)
		{
			int monsterIndex = lpInfo->MonsterIndex[i];

			if (monsterIndex == lpObj->Index)
			{
				continue;
			}

			if (OBJECT_RANGE(monsterIndex) && gObj[monsterIndex].Live != 0)
			{
				liveCount++;
			}
		}

		if (lpInfo->CountNotify == 1)
		{
			this->RebuildNotifyMonsterInfo(lpInfo);

			type_count_monster::iterator it = lpInfo->total_data_monster.find(lpObj->Class);

			if(it != lpInfo->total_data_monster.end())
			{
				it->second.Monster_Kill++;
				this->SendInfoKill(&it->second, lpInfo->Index);
			}
		}

		if (lpObj->Class == lpInfo->BossIndex)
		{
			if (lpInfo->BossMessage != -1)
			{
				gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0,
					gMessage.GetMessage(lpInfo->BossMessage), lpTarget->Name);
			}
		}

		if (liveCount <= 0)
		{
			this->SetState(lpInfo, INVASION_STATE_EMPTY);
		}

		break;
	}
}
void CInvasionManager::StartInvasion(int index)
{
	CTime CurrentTime = CTime::GetTickCount();

	int hour = (int)CurrentTime.GetHour();
	int minute = (int)CurrentTime.GetMinute() + 1;

	if (minute >= 60)
	{
		hour++;
		minute = minute - 60;
	}

	INVASION_START_TIME info;

	info.Year = (int)CurrentTime.GetYear();

	info.Month = (int)CurrentTime.GetMonth();

	info.Day = (int)CurrentTime.GetDay();

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_InvasionInfo[index].StartTime.push_back(info);

	LogAdd(LOG_EVENT, "[Set Invasion Start] Index %d At %2d:%2d:00", index, hour, minute);

	this->SetState(&this->m_InvasionInfo[index], INVASION_STATE_EMPTY);
}

const char* CInvasionManager::GetInvasionName(int index)
{
	if (index < 0 || index >= MAX_INVASION || strlen(this->m_InvasionInfo[index].AlertMessage) == 0)
	{
		return "Custom";
	}
	return this->m_InvasionInfo[index].AlertMessage;
}

void CInvasionManager::SendPlayerInfo(int index)
{
	for(int n=0;n < MAX_INVASION;n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		if(lpInfo->CountNotify == 0)
		{
			continue;
		}

		this->EnsureNotifyMonsterInfo(lpInfo);

		if(lpInfo->State == INVASION_STATE_START && lpInfo->total_data_monster.empty() == 0)
		{
			this->SendInfoInvasion(lpInfo, 1, index);
		}
		else
		{
			this->SendInfoInvasion(lpInfo, 0, index);
		}
	}
}

void CInvasionManager::SendInfoKill(INVASION_INFO_MONSTER* lpInfo,int index)
{
	PMSG_INVASION_KILL_INFO pMsg;

	pMsg.Head.set(0x4E, 0x3E, sizeof(pMsg));
	pMsg.Index = index;
	pMsg.MonsterIndex = lpInfo->MonsterIndex;
	pMsg.MonsterCount = lpInfo->MonsterCount;
	pMsg.Monster_Kill = lpInfo->Monster_Kill;

	DataSendAll((BYTE*)&pMsg, sizeof(pMsg));
}

void CInvasionManager::AddNotifyMonsterInfo(INVASION_INFO* lpInfo,int MonsterClass,int MonsterCount)
{
	if(lpInfo == 0 || lpInfo->CountNotify == 0 || MonsterClass < 0 || MonsterCount <= 0)
	{
		return;
	}

	type_count_monster::iterator it = lpInfo->total_data_monster.find(MonsterClass);

	if(it == lpInfo->total_data_monster.end())
	{
		INVASION_INFO_MONSTER info;
		info.MonsterIndex = MonsterClass;
		info.Monster_Kill = 0;
		info.MonsterCount = MonsterCount;
		lpInfo->total_data_monster.insert(type_count_monster::value_type(MonsterClass, info));
	}
	else
	{
		it->second.MonsterCount += MonsterCount;
	}
}

void CInvasionManager::EnsureNotifyMonsterInfo(INVASION_INFO* lpInfo)
{
	if(lpInfo == 0 || lpInfo->CountNotify == 0 || lpInfo->State != INVASION_STATE_START || lpInfo->total_data_monster.empty() == 0)
	{
		return;
	}

	this->RebuildNotifyMonsterInfo(lpInfo);
}

void CInvasionManager::RebuildNotifyMonsterInfo(INVASION_INFO* lpInfo)
{
	if(lpInfo == 0 || lpInfo->CountNotify == 0 || lpInfo->State != INVASION_STATE_START)
	{
		return;
	}

	type_count_monster killInfo;

	for(type_count_monster::iterator it = lpInfo->total_data_monster.begin(); it != lpInfo->total_data_monster.end(); it++)
	{
		killInfo.insert(type_count_monster::value_type(it->first, it->second));
	}

	lpInfo->total_data_monster.clear();

	for(std::vector<INVASION_MONSTER_INFO>::iterator it = lpInfo->MonsterInfo.begin(); it != lpInfo->MonsterInfo.end(); it++)
	{
		this->AddNotifyMonsterInfo(lpInfo, it->MonsterClass, this->GetNotifyMonsterCount(lpInfo, &(*it)));
	}

	for(int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpInfo->MonsterIndex[n]) == 0)
		{
			continue;
		}

		int MonsterClass = gObj[lpInfo->MonsterIndex[n]].Class;

		if(lpInfo->total_data_monster.find(MonsterClass) == lpInfo->total_data_monster.end())
		{
			this->AddNotifyMonsterInfo(lpInfo, MonsterClass, 1);
		}
	}

	for(type_count_monster::iterator it = lpInfo->total_data_monster.begin(); it != lpInfo->total_data_monster.end(); it++)
	{
		type_count_monster::iterator kill = killInfo.find(it->first);

		if(kill != killInfo.end())
		{
			it->second.Monster_Kill = ((kill->second.Monster_Kill > it->second.MonsterCount) ? it->second.MonsterCount : kill->second.Monster_Kill);
		}
	}
}

int CInvasionManager::GetNotifyMonsterCount(INVASION_INFO* lpInfo,INVASION_MONSTER_INFO* lpMonsterInfo)
{
	if(lpInfo == 0 || lpMonsterInfo == 0)
	{
		return 0;
	}

	int MonsterCount = 0;

	for(int n = 0; n < MAX_INVASION_RESPAWN_GROUP; n++)
	{
		if(lpInfo->RespawnInfo[n].empty() != 0)
		{
			continue;
		}

		int GroupCount = 0;

		for(std::vector<INVASION_RESPWAN_INFO>::iterator ri = lpInfo->RespawnInfo[n].begin(); ri != lpInfo->RespawnInfo[n].end(); ri++)
		{
			if(lpMonsterInfo->Group != ri->Group)
			{
				continue;
			}

			int CandidateCount = 0;

			for(int m = 0; m < gMonsterSetBase.m_count; m++)
			{
				MONSTER_SET_BASE_INFO* lpMsbInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[m];

				if(lpMsbInfo->Type == 3 && lpMsbInfo->MonsterClass == lpMonsterInfo->MonsterClass && lpMsbInfo->Map == ri->Map && lpMsbInfo->Value == ri->Value)
				{
					CandidateCount++;
				}
			}

			if(CandidateCount > GroupCount)
			{
				GroupCount = CandidateCount;
			}
		}

		MonsterCount += GroupCount;
	}

	return ((MonsterCount > 0) ? MonsterCount : 1);
}

void CInvasionManager::SendInfoInvasion(INVASION_INFO* lpInfo,int status,int index)
{
	PMSG_INVASION_INFO pMsg;
	memset(&pMsg, 0, sizeof(pMsg));

	if(status == 1)
	{
		this->EnsureNotifyMonsterInfo(lpInfo);

		BYTE send[2048];
		int size = sizeof(pMsg);

		pMsg.Head.set(0x4E, 0x3F, 0);
		pMsg.Index = lpInfo->Index;
		pMsg.RemainTime = lpInfo->RemainTime;
		strncpy_s(pMsg.Name, this->GetInvasionName(lpInfo->Index), sizeof(pMsg.Name)-1);
		pMsg.count = 0;

		for(type_count_monster::iterator it=lpInfo->total_data_monster.begin();it != lpInfo->total_data_monster.end();it++)
		{
			INVASION_INFO_MONSTER info;
			info.MonsterIndex = it->second.MonsterIndex;
			info.MonsterCount = it->second.MonsterCount;
			info.Monster_Kill = it->second.Monster_Kill;

			memcpy(&send[size], &info, sizeof(info));
			size += sizeof(info);
			pMsg.count++;
		}

		pMsg.Head.size[0] = SET_NUMBERHB(size);
		pMsg.Head.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		if(index != -1)
		{
			DataSend(index, send, size);
		}
		else
		{
			DataSendAll(send, size);
		}
	}
	else
	{
		int size = sizeof(pMsg);

		pMsg.Head.set(0x4E, 0x3F, size);
		pMsg.Index = lpInfo->Index;
		pMsg.RemainTime = 0;
		pMsg.count = 0;
		strncpy_s(pMsg.Name, this->GetInvasionName(lpInfo->Index), sizeof(pMsg.Name)-1);

		if(index != -1)
		{
			DataSend(index, (BYTE*)&pMsg, size);
		}
		else
		{
			DataSendAll((BYTE*)&pMsg, size);
		}
	}
}

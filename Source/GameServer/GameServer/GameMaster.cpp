// GameMaster.cpp: implementation of the CGameMaster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameMaster.h"
#include "MemScript.h"
#include "Util.h"

CGameMaster gGameMaster;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameMaster::CGameMaster() // OK
{
	this->m_GameMasterInfo.clear();
}

CGameMaster::~CGameMaster() // OK
{

}

void CGameMaster::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_GameMasterInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			GAME_MASTER_INFO info;

			memset(&info,0,sizeof(info));

			strcpy_s(info.Account,lpMemScript->GetString());

			strcpy_s(info.Name,lpMemScript->GetAsString());

			info.Level = lpMemScript->GetAsNumber();

			this->m_GameMasterInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CGameMaster::CheckGameMasterLevel(LPOBJ lpObj) // OK
{
	for (std::vector<GAME_MASTER_INFO>::iterator it = this->m_GameMasterInfo.begin(); it != this->m_GameMasterInfo.end(); it++)
	{
		if (strcmp(it->Account, lpObj->Account) != 0)
		{
			continue;
		}

		if (strcmp(it->Name, lpObj->Name) == 0 || strcmp(it->Name, "*") == 0)
		{
			return it->Level;
		}
	}

	return -1;
}
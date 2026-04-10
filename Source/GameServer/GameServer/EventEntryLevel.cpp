// EventEntryLevel.cpp: interface for the CEventEntryLevel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventEntryLevel.h"
#include "DefaultClassInfo.h"
#include "MemScript.h"
#include "Util.h"
#include "ServerInfo.h"

CEventEntryLevel gEventEntryLevel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CEventEntryLevel::Load(char* path) // OK
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

	memset(this->m_BloodCastleEntryLevelCommon,0,sizeof(this->m_BloodCastleEntryLevelCommon));

	memset(this->m_BloodCastleEntryLevelSpecial,0,sizeof(this->m_BloodCastleEntryLevelSpecial));
	
	memset(this->m_DevilSquareEntryLevelCommon,0,sizeof(this->m_DevilSquareEntryLevelCommon));
	
	memset(this->m_DevilSquareEntryLevelSpecial,0,sizeof(this->m_DevilSquareEntryLevelSpecial));

	memset(this->m_ChaosCastleEntryLevelCommon,0,sizeof(this->m_ChaosCastleEntryLevelCommon));
	
	memset(this->m_ChaosCastleEntryLevelSpecial,0,sizeof(this->m_ChaosCastleEntryLevelSpecial));
	
	memset(this->m_IllusionTempleEntryLevelMin,0,sizeof(this->m_IllusionTempleEntryLevelMin));
	
	memset(this->m_IllusionTempleEntryLevelMax,0,sizeof(this->m_IllusionTempleEntryLevelMax));

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int level = lpMemScript->GetNumber();

					this->m_BloodCastleEntryLevelCommon[level][0] = lpMemScript->GetAsNumber();

					this->m_BloodCastleEntryLevelCommon[level][1] = lpMemScript->GetAsNumber();

					this->m_BloodCastleEntryLevelSpecial[level][0] = lpMemScript->GetAsNumber();

					this->m_BloodCastleEntryLevelSpecial[level][1] = lpMemScript->GetAsNumber();
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int level = lpMemScript->GetNumber();

					this->m_DevilSquareEntryLevelCommon[level][0] = lpMemScript->GetAsNumber();

					this->m_DevilSquareEntryLevelCommon[level][1] = lpMemScript->GetAsNumber();

					this->m_DevilSquareEntryLevelSpecial[level][0] = lpMemScript->GetAsNumber();

					this->m_DevilSquareEntryLevelSpecial[level][1] = lpMemScript->GetAsNumber();
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int level = lpMemScript->GetNumber();

					this->m_ChaosCastleEntryLevelCommon[level][0] = lpMemScript->GetAsNumber();

					this->m_ChaosCastleEntryLevelCommon[level][1] = lpMemScript->GetAsNumber();

					this->m_ChaosCastleEntryLevelSpecial[level][0] = lpMemScript->GetAsNumber();

					this->m_ChaosCastleEntryLevelSpecial[level][1] = lpMemScript->GetAsNumber();
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int level = lpMemScript->GetNumber();

					this->m_IllusionTempleEntryLevelMin[level] = lpMemScript->GetAsNumber();

					this->m_IllusionTempleEntryLevelMax[level] = lpMemScript->GetAsNumber();
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CEventEntryLevel::GetBCLevel(LPOBJ lpObj) // OK
{
	int level = -1;

	#if(GAMESERVER_UPDATE>=401)

	if(lpObj->ChangeUp >= 2)
	{
		return 7;
	}

	#endif

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		for(int n=0;n < 7;n++)
		{
			if(this->m_BloodCastleEntryLevelSpecial[n][0] != -1 && this->m_BloodCastleEntryLevelSpecial[n][0] > lpObj->Reset)
			{
				continue;
			}

			if(this->m_BloodCastleEntryLevelSpecial[n][1] != -1 && this->m_BloodCastleEntryLevelSpecial[n][1] < lpObj->Reset)
			{
				continue;
			}

			level = n;
		}
	}
	else
	{
		for(int n=0;n < 7;n++)
		{
			if(this->m_BloodCastleEntryLevelCommon[n][0] != -1 && this->m_BloodCastleEntryLevelCommon[n][0] > lpObj->Reset)
			{
				continue;
			}

			if(this->m_BloodCastleEntryLevelCommon[n][1] != -1 && this->m_BloodCastleEntryLevelCommon[n][1] < lpObj->Reset)
			{
				continue;
			}

			level = n;
		}
	}

	return level;
}

int CEventEntryLevel::GetDSLevel(LPOBJ lpObj) // OK
{
	int level = -1;

	#if(GAMESERVER_UPDATE>=401)

	if(lpObj->ChangeUp >= 2)
	{
		return 6;
	}

	#endif

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		for(int n=0;n < 6;n++)
		{
			if(this->m_DevilSquareEntryLevelSpecial[n][0] != -1 && this->m_DevilSquareEntryLevelSpecial[n][0] > lpObj->Reset)
			{
				continue;
			}

			if(this->m_DevilSquareEntryLevelSpecial[n][1] != -1 && this->m_DevilSquareEntryLevelSpecial[n][1] < lpObj->Reset)
			{
				continue;
			}

			level = n;
		}
	}
	else
	{
		for(int n=0;n < 6;n++)
		{
			if(this->m_DevilSquareEntryLevelCommon[n][0] != -1 && this->m_DevilSquareEntryLevelCommon[n][0] > lpObj->Reset)
			{
				continue;
			}

			if(this->m_DevilSquareEntryLevelCommon[n][1] != -1 && this->m_DevilSquareEntryLevelCommon[n][1] < lpObj->Reset)
			{
				continue;
			}

			level = n;
		}
	}

	return level;
}

int CEventEntryLevel::GetCCLevel(LPOBJ lpObj) // OK
{
	int level = -1;

	#if(GAMESERVER_UPDATE>=401)

	if(lpObj->ChangeUp >= 2)
	{
		return 6;
	}

	#endif

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		for(int n=0;n < 6;n++)
		{
			if(this->m_DevilSquareEntryLevelSpecial[n][0] != -1 && this->m_DevilSquareEntryLevelSpecial[n][0] > lpObj->Reset)
			{
				continue;
			}

			if(this->m_DevilSquareEntryLevelSpecial[n][1] != -1 && this->m_DevilSquareEntryLevelSpecial[n][1] < lpObj->Reset)
			{
				continue;
			}

			level = n;
		}
	}
	else
	{
		for(int n=0;n < 6;n++)
		{
			if(this->m_ChaosCastleEntryLevelCommon[n][0] != -1 && this->m_ChaosCastleEntryLevelCommon[n][0] > lpObj->Reset)
			{
				continue;
			}

			if(this->m_ChaosCastleEntryLevelCommon[n][1] != -1 && this->m_ChaosCastleEntryLevelCommon[n][1] < lpObj->Reset)
			{
				continue;
			}

			level = n;
		}
	}

	return level;
}

int CEventEntryLevel::GetITLevel(LPOBJ lpObj) // OK
{
	int level = -1;

	#if(GAMESERVER_UPDATE>=301)

	#if(GAMESERVER_UPDATE>=401)

	if(lpObj->ChangeUp >= 2)
	{
		return 5;
	}

	#endif

	for(int n=0;n < 5;n++)
	{
		if(this->m_IllusionTempleEntryLevelMin[n] != -1 && this->m_IllusionTempleEntryLevelMin[n] > lpObj->Reset)
		{
			continue;
		}

		if(this->m_IllusionTempleEntryLevelMax[n] != -1 && this->m_IllusionTempleEntryLevelMax[n] < lpObj->Reset)
		{
			continue;
		}

		level = n;
	}

	#endif

	return level;
}
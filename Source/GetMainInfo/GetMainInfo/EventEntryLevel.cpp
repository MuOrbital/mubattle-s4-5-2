// EventEntryLevel.cpp: implementation of the CEventEntryLevel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventEntryLevel.h"
#include "MemScript.h"

CEventEntryLevel gEventEntryLevel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CEventEntryLevel::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR,path);
		return 0;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return 0;
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
				static int ItemStackCount = 0;

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
		printf(lpMemScript->GetLastError());
		return 0;
	}

	delete lpMemScript;

	return 1;
}
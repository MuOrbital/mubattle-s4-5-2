// EventEntryLevel.h: interface for the CEventEntryLevel class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

class CEventEntryLevel
{
public:
	void Load(char* path);
	int GetBCLevel(LPOBJ lpObj);
	int GetDSLevel(LPOBJ lpObj);
	int GetCCLevel(LPOBJ lpObj);
	int GetITLevel(LPOBJ lpObj);
private:
	int m_BloodCastleEntryLevelCommon[7][2];
	int m_BloodCastleEntryLevelSpecial[7][2];
	int m_DevilSquareEntryLevelCommon[6][2];
	int m_DevilSquareEntryLevelSpecial[6][2];
	int m_ChaosCastleEntryLevelCommon[6][2];
	int m_ChaosCastleEntryLevelSpecial[6][2];
	int m_IllusionTempleEntryLevelMin[5];
	int m_IllusionTempleEntryLevelMax[5];
};

extern CEventEntryLevel gEventEntryLevel;
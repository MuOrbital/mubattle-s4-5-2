#pragma once

#include "Lua.h"

class CEventEntryLevel
{
public:
	CEventEntryLevel();
	~CEventEntryLevel() = default;

	void Init();


	int m_BloodCastleEntryLevelCommon[7][2];
	int m_BloodCastleEntryLevelSpecial[7][2];
	int m_DevilSquareEntryLevelCommon[6][2];
	int m_DevilSquareEntryLevelSpecial[6][2];
	int m_ChaosCastleEntryLevelCommon[6][2];
	int m_ChaosCastleEntryLevelSpecial[6][2];
	int m_IllusionTempleEntryLevelMin[5];
	int m_IllusionTempleEntryLevelMax[5];
private:
	Lua m_Lua;

};

extern CEventEntryLevel gEventEntryLevel;
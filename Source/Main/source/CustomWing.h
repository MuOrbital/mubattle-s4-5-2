// CustomWing.h: interface for the CCustomWing class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "Lua.h"
struct CUSTOM_WING_INFO
{
	int ItemIndex;
	int DefenseConstA;
	int IncDamageConstA;
	int IncDamageConstB;
	int DecDamageConstA;
	int DecDamageConstB;
	int OptionIndex1;
	int OptionValue1;
	int OptionIndex2;
	int OptionValue2;
	int OptionIndex3;
	int OptionValue3;
	int NewOptionIndex1;
	int NewOptionValue1;
	int NewOptionIndex2;
	int NewOptionValue2;
	int NewOptionIndex3;
	int NewOptionValue3;
	int NewOptionIndex4;
	int NewOptionValue4;
	int ModelType;
	char ModelName[32];
	//CapeInfos
	int IsCape;
};
class CCustomWing
{
public:
	CCustomWing();
	virtual ~CCustomWing();
	void Init();
	void RestartLua();
	CUSTOM_WING_INFO* GetInfoByItem(int ItemIndex);
	BOOL CheckCustomWingByItem(int ItemIndex);
	BOOL CheckCustomWingByModelType(int ItemIndex, int ModelType);
	int GetCustomWingDefense(int ItemIndex, int ItemLevel);
	int GetCustomWingIncDamage(int ItemIndex, int ItemLevel);
	int GetCustomWingDecDamage(int ItemIndex, int ItemLevel);
	int GetCustomWingOptionIndex(int ItemIndex, int OptionNumber);
	int GetCustomWingOptionValue(int ItemIndex, int OptionNumber);
	int GetCustomWingNewOptionIndex(int ItemIndex, int OptionNumber);
	int GetCustomWingNewOptionValue(int ItemIndex, int OptionNumber);
	int CheckCustomWingIsCape(int ItemIndex);
public:
	std::map<int, CUSTOM_WING_INFO> m_CustomWingInfo;
private:
	Lua m_Lua;
};
extern CCustomWing gCustomWing;
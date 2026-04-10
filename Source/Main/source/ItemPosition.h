#pragma once

#include "Lua.h"
#include <map>

struct CUSTOM_ITEM_POSITION_INFO
{
	int ItemIndex;
	float PosX;
	float PosY;
	float AngleX;
	float AngleY;
	float AngleZ;
};

class CCustomItemPosition
{
public:
	CCustomItemPosition();
	~CCustomItemPosition();

	bool CheckCustomItemPosition(int ItemIndex);
	void GetPositionItem(int ItemIndex, float* PosX, float* PosY, float* AngleX, float* AngleY, float* AngleZ);
	CUSTOM_ITEM_POSITION_INFO* getItemPosition(int ItemIndex);
	void Init();
	void RestartLua();

private:
	Lua m_Lua;

public:
	std::map<int, CUSTOM_ITEM_POSITION_INFO> m_CustomItemPositionInfo;
};

extern CCustomItemPosition gCustomItemPosition;
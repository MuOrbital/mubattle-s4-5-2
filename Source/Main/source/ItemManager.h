// ItemManager.h: interface for the CItemManager class.
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NewSingleton.h"
#include "Lua.h"

struct CUSTOM_ITEM_INFO
{
	int Index;
	int ItemIndex;
	int ColorR;
	int ColorG;
	int ColorB;
	char ModelName[32];
};

class CItemManager
{
	CItemManager();

	virtual ~CItemManager();

	SingletonInstance(CItemManager);
public:

	void Init();

	void RestartLua();

	BOOL GetCustomItemColor(int Type, float* ItemColor);

	void GetItemColor(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon = false);

	int GET_ITEM_MODEL(int section, int index);

	int GetItem(int section, int index);

	int GetItemModel(int section, int index);

public:

	std::map<int, CUSTOM_ITEM_INFO> m_CustomItemInfo;

private:

	Lua m_Lua;

};

#define gItemManager SingNull(CItemManager)
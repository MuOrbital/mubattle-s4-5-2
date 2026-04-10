#pragma once

#include "Lua.h"

struct CUSTOM_ITEM_SIZE_INFO {
	int ItemIndex;
	float SizeInventory;
};

class CCustomItemSize {
public:
	CCustomItemSize();
	~CCustomItemSize();

	bool CheckCustomItemSize(int ItemIndex);
	void GetSizeItem(int ItemIndex, float& Size);
	CUSTOM_ITEM_SIZE_INFO* getItemSize(int ItemIndex);
	void Init();
	void RestartLua();

private:
	Lua m_Lua;

public:
	std::map<int, CUSTOM_ITEM_SIZE_INFO> m_CustomItemSizeInfo;
};

extern CCustomItemSize gCustomItemSize;
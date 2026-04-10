#pragma once

#include "Lua.h"

struct DISABLE_EXCELLENT_INFO {
	int ItemIndex;
};

class DisableExcellent {
public:
	DisableExcellent();
	~DisableExcellent();

	void Init();
	bool CanDisableExcellent(int ItemIndex);
	int CheckDisableExcellent(int ItemIndex);

public:
	std::map<int, DISABLE_EXCELLENT_INFO> m_DisableExcellent;
	Lua m_Lua;
};

extern DisableExcellent gDisableExcellent;
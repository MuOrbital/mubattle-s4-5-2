#pragma once

#include "Lua.h"
#include "LuaBMD.h"
#include <map>

class HELPER_INFO {
public:
	HELPER_INFO() {
		this->ItemIndex = -1;
		this->ModelID = -1;
	};

	~HELPER_INFO() {

	};

	int ItemIndex;
	int Type;
	int Movement;
	float HeightFloor;
	float Size;
	float SizeCharList;
	char Model[32];
	char ObjectModel[32];
	int ModelID;
	int Miniature;
	float SizeMiniature;
	float VelocityMiniature;
};

class CHelperSystem
{
public:
	CHelperSystem();
	~CHelperSystem();

	void Init();
	int GetHelperModel(int ItemIndex);
	bool CheckIsHelper(int ItemIndex);
	bool CheckHelperType(int ItemIndex, int value);
	HELPER_INFO* GetHelper(int ItemIndex);

	int CheckIsFenrirOrDino(int ItemIndex);
public:
	std::map<int, HELPER_INFO> m_HelperInfo;

public:
	Lua m_Lua;
	LuaBMD m_LuaBMD;
};

extern CHelperSystem gHelperSystem;

#pragma once

#include "Lua.h"
#include "LuaInterface.h"
#include "LuaGlobal.h"
#include "LuaCharacter.h"
#include "LuaUser.h"
#include "LuaLoadImage.h"

class CInterface
{
public:
	CInterface();
	~CInterface();
	void Init();
	void RestartLua();

public:
	Lua m_Lua;
	bool interfaceLock;
	HDC hdc;

private:
	LuaInterface m_LuaInterface;
	LuaGlobal m_LuaGlobal;
	CLuaCharacter m_LuaCharacter;
	CLuaUser m_LuaUser;
	LuaLoadImage m_LoadImage;
};

extern CInterface gInterface;
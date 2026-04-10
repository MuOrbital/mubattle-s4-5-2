#ifndef LUAFUNCTIONS_H
#define LUAFUNCTIONS_H

#pragma once

#include "Util.h"
#include "user.h"
#include "DSProtocol.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "SocketManager.h"
#include "GameMain.h"
#include "LuaOpenExtension.h"
#include "JSProtocol.h"
#include "GameMain.h"
#include "Path.h"
#include "Map.h"
#include "Notice.h"
#include "EffectManager.h"
#include "Warehouse.h"
#include "Quest.h"
#include "Protect.h"
#include "ChaosBox.h"
#include "QuestReward.h"
#include "ItemOptionRate.h"
#include "LuaGameServer.h"
#include "LuaState.h"

class CLuaFunctions
{
public:
	CLuaFunctions();
	virtual ~CLuaFunctions();

	void AddFunctions(lua_State* Lua);

private:
	static int IncludeLuaOpenExtension(lua_State * Lua)
	{
		gSocketManager.m_critical.lock();

		try
		{
			if (Lua == NULL)
			{
				gSocketManager.m_critical.unlock();
				return false;
			}

			char name[MAX_PATH] = { 0 };
			sprintf_s(name, gPath.GetFullPath("Scripts\\%s\\"), (LPCSTR)luaL_checklstring(Lua, 1, 0));
			lua_settop(Lua, 1);

			gLuaOpenExtension.LoadFilesOfFolder(Lua, name);

			gSocketManager.m_critical.unlock();
			return true;
		}
		catch (...)
		{
			LogAddLua(LOG_RED, "Error IncludeLuaOpenExtension");
			gSocketManager.m_critical.unlock();
			return false;
		}
	};

	static int IncludeLuaOpenExtensionCrypted(lua_State * Lua)
	{
		gSocketManager.m_critical.lock();

		try
		{
			if (Lua == NULL)
			{
				gSocketManager.m_critical.unlock();
				return false;
			}

			char name[256] = { 0 };
			sprintf_s(name, gPath.GetFullPath("Scripts\\%s\\"), (LPCSTR)luaL_checklstring(Lua, 1, 0));
			lua_settop(Lua, 1);

			gLuaOpenExtension.LoadCryptExtension(Lua, name);

			gSocketManager.m_critical.unlock();
			return true;
		}
		catch (...)
		{
			LogAddLua(LOG_RED, "Error IncludeLuaOpenExtensionCrypted");
			gSocketManager.m_critical.unlock();
			return false;
		}
	};

	static int OpenFileLua(lua_State * Lua)
	{
		gSocketManager.m_critical.lock();

		try
		{
			if (Lua == NULL)
			{
				gSocketManager.m_critical.unlock();
				return false;
			}

			char name[256] = { 0 };
			sprintf_s(name, gPath.GetFullPath("Scripts\\%s.lua"), (LPCSTR)luaL_checklstring(Lua, 1, 0));
			lua_settop(Lua, 1);

			gLuaOpenExtension.LoadFile(Lua, name);

			gSocketManager.m_critical.unlock();
			return true;
		}
		catch (...)
		{
			LogAddLua(LOG_RED, "Error IncludeLuaOpenExtensionCrypted");
			gSocketManager.m_critical.unlock();
			return false;
		}
	};
};

extern CLuaFunctions gLuaFunctions;


#endif
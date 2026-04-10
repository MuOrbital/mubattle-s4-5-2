#include "stdafx.h"
#include "LuaOpenExtension.h"
#include "Util.h"
#include "GameMain.h"
#include "MuLuaFileProtect.h"

CLuaOpenExtension gLuaOpenExtension;
static const int sentinel_ = 0;
#define sentinel ((void *)&sentinel_)

CLuaOpenExtension::CLuaOpenExtension()
{
}

CLuaOpenExtension::~CLuaOpenExtension()
{
}

void CLuaOpenExtension::LoadFile(lua_State * L, char * file)
{
	lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(L, 2, file);

	if (lua_toboolean(L, -1))
	{
		if (lua_touserdata(L, -1) == sentinel)
		{
			luaL_error(L, "loop or previous error loading module %s", file);
		}
		return;
	}

	if (luaL_loadfile(L, file) != 0)
	{
		luaL_error(L, "error loading module %s from file %s:\n\t%s", lua_tostring(L, 1), file, lua_tostring(L, -1));
	}

	lua_pushlightuserdata(L, sentinel);
	lua_setfield(L, 2, file);
	lua_pushstring(L, file);

	lua_call(L, 1, 1);

	if (!lua_isnil(L, -1))
	{
		lua_setfield(L, 2, file);
	}

	lua_getfield(L, 2, file);

	if (lua_touserdata(L, -1) == sentinel)
	{
		lua_pushboolean(L, 1);
		lua_pushvalue(L, -1);
		lua_setfield(L, 2, file);
	}
}

void CLuaOpenExtension::LoadFilesOfFolder(lua_State * L, char * folder)
{
	char name[MAX_PATH] = { 0 };
	char wildcard_path[MAX_PATH];

	wsprintf(wildcard_path, "%s*.lua", folder);

	WIN32_FIND_DATA data;

	HANDLE file = FindFirstFile(wildcard_path, &data);

	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			if (_strcmpi("CharacterStatus.lua", data.cFileName) == 0)
			{
				continue;
			}

			wsprintf(name, "%s%s", folder, data.cFileName);

			lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
			lua_getfield(L, 2, name);

			if (lua_toboolean(L, -1))
			{
				if (lua_touserdata(L, -1) == sentinel)
				{
					luaL_error(L, "loop or previous error loading module %s", name);
				}
				return;
			}

			if (luaL_loadfile(L, name) != 0)
			{
				luaL_error(L, "error loading module %s from file %s:\n\t%s", lua_tostring(L, 1), name, lua_tostring(L, -1));
			}

			lua_pushlightuserdata(L, sentinel);
			lua_setfield(L, 2, name);
			lua_pushstring(L, name);

			lua_call(L, 1, 1);

			if (!lua_isnil(L, -1))
			{
				lua_setfield(L, 2, name);
			}

			lua_getfield(L, 2, name);

			if (lua_touserdata(L, -1) == sentinel)
			{
				lua_pushboolean(L, 1);
				lua_pushvalue(L, -1);
				lua_setfield(L, 2, name);
			}
		}
	} while (FindNextFile(file, &data) != 0);
}

void CLuaOpenExtension::LoadCryptExtension(lua_State * L, char * folder)
{
	char name[MAX_PATH] = { 0 };
	char wildcard_path[MAX_PATH];

	wsprintf(wildcard_path, "%s*.lua", folder);

	WIN32_FIND_DATA data;

	HANDLE file = FindFirstFile(wildcard_path, &data);

	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	static CCriticalSection m_critical;

	do
	{
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			if (_strcmpi("CharacterStatus.lua", data.cFileName) == 0)
			{
				continue;
			}

			m_critical.lock();

			wsprintf(name, "%s\\%s", folder, data.cFileName);

			gFileProtect.ConvertMainFilePath((char*)name);

			lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
			lua_getfield(L, 2, name);

			if (lua_toboolean(L, -1))
			{
				if (lua_touserdata(L, -1) == sentinel)
				{
					luaL_error(L, "loop or previous error loading module %s", name);
				}

				gFileProtect.DeleteTemporaryFile();
				m_critical.unlock();
				return;
			}

			if (luaL_loadfile(L, name) != 0)
			{
				luaL_error(L, "error loading module %s from file %s:\n\t%s", lua_tostring(L, 1), name, lua_tostring(L, -1));
				gFileProtect.DeleteTemporaryFile();
				m_critical.unlock();
				return;
			}

			lua_pushlightuserdata(L, sentinel);
			lua_setfield(L, 2, name);
			lua_pushstring(L, name);

			lua_call(L, 1, 1);

			if (!lua_isnil(L, -1))
			{
				lua_setfield(L, 2, name);
			}

			lua_getfield(L, 2, name);

			if (lua_touserdata(L, -1) == sentinel)
			{
				lua_pushboolean(L, 1);
				lua_pushvalue(L, -1);
				lua_setfield(L, 2, name);
			}

			gFileProtect.DeleteTemporaryFile();

			m_critical.unlock();
		}
	} while (FindNextFile(file, &data) != 0);
}
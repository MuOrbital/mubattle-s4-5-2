#include "StdAfx.h"
#include "Lua.h"
#include "LuaDecrypt.h"
#include "LuaOpenFolder.h"
#include "LuaReg.h"
#include "Console.h"

Lua::Lua() : m_luaState(NULL)
{
	this->RegisterLua();
}

Lua::~Lua()
{
	this->CloseLua();
}

static BOOL LuaOpenFolder(lua_State* Lua)
{
	gLuaOpenFolder.m_critical.lock();

	if (Lua == NULL)
	{
		gLuaOpenFolder.m_critical.unlock();
		return false;
	}

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "Data\\Lua\\Manager\\%s\\", (LPCSTR)luaL_checklstring(Lua, 1, 0));
	lua_settop(Lua, 1);

	gLuaOpenFolder.LoadFolder(Lua, name);

	gLuaOpenFolder.m_critical.unlock();
	return 1;
};

static BOOL LuaLogDebug(lua_State* Lua)
{
	//PMSG_NOTICE_RECV pMsg;
	//
	//pMsg.type = 1;
	//
	//strcpy_s(pMsg.message, luaL_checklstring(Lua, 1, 0));
	//
	//((void(*)(PMSG_NOTICE_RECV*))0x6403B0)(&pMsg);
	return 1;
};

static BOOL LuaOpenFile(lua_State* L)
{
	char name[MAX_PATH] = { 0 };

	sprintf_s(name, "Data//Lua//%s", (LPCSTR)luaL_checklstring(L, 1, 0));

	gLuaOpenFolder.LoadFile(L, name);

	return true;
};

static BOOL LuaConsole(lua_State* Lua)
{
	Console.Write(false, "%s", luaL_checklstring(Lua, 1, 0));
	return 1;
};

void Lua::RegisterLua()
{
	Console.Write(1, "Lua Carregado com sucesso!");

	this->m_luaState = luaL_newstate();

	luaL_openlibs(this->m_luaState);

	lua_gc(this->m_luaState, LUA_GCCOLLECT, 0);

	lua_register(this->m_luaState, "OpenFolder", LuaOpenFolder);

	lua_register(this->m_luaState, "LogDebug", LuaLogDebug);

	lua_register(this->m_luaState, "OpenFile", LuaOpenFile);

#if USE_CONSOLE == FALSE
	lua_register(this->m_luaState, "Console", LuaConsole);
#endif

	//m_LuaMemory.RegisterLua(this->m_luaState);

	RegisterLuaReg(this->m_luaState);
}

void Lua::CloseLua()
{
	if (this->m_luaState)
	{
		lua_close(this->m_luaState);
		this->m_luaState = NULL;
	}
}

lua_State* Lua::GetState()
{
	return this->m_luaState;
}

void Lua::DoFile(char* file_name)
{
	if (this->m_luaState == NULL)
	{
		return;
	}

	char buffer[256];
	std::memset(buffer, 0, sizeof(buffer));
	std::memcpy(buffer, file_name, strlen(file_name));

	std::string result = gFileProtectLua.ConvertMainFilePath(buffer);

	if (luaL_loadbuffer(this->m_luaState, result.c_str(), result.length(), file_name))
	{
		gFileProtectLua.DeleteTemporaryFile();
		if (!gProtect->m_MainInfo.LuaCrypt)
		{
			sprintf_s(buffer, "Error!!, : %s\n", lua_tostring(this->m_luaState, -1));
			MessageBox(NULL, buffer, "Initialize Error", MB_OK);
			return;
		}
		else
		{
			exit(0);
			return;
		}
	}

	if (lua_pcall(this->m_luaState, 0, LUA_MULTRET, 0)) {
		gFileProtectLua.DeleteTemporaryFile();
		if (!gProtect->m_MainInfo.LuaCrypt)
		{
			sprintf_s(buffer, "Error!!, : %s\n", lua_tostring(this->m_luaState, -1));
			MessageBox(NULL, buffer, "Initialize Error", MB_OK);
			return;
		}
		else
		{
			exit(0);
			return;
		}
	}

	gFileProtectLua.DeleteTemporaryFile();
}

bool Lua::Generic_Call(const char* func, const char* sig, ...)
{
	if (this->m_luaState == NULL)
	{
		return false;
	}

	this->m_critical.lock();

	va_list	VA_LIST;
	int nArg = 0;

	va_start(VA_LIST, sig);
	lua_getglobal(this->m_luaState, func);

	char buffer[256];

	while (*sig)
	{
		switch (*sig++)
		{
		case 'd':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, double));
		} break;

		case 'i':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, int));
		} break;

		case 's':
		{
			char* pszString = va_arg(VA_LIST, char*);
			lua_pushlstring(this->m_luaState, pszString, strlen(pszString));
		} break;

		case 'w':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, unsigned int));
		} break;

		case 'l':
		{
			lua_pushinteger(this->m_luaState, va_arg(VA_LIST, unsigned long long));
		} break;

		case 'j':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, unsigned long));
		} break;

		case 'f':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, float));
		} break;

		case '>':
		{
			goto endwhile;
		}

		default:
		{
			sprintf_s(buffer, "luacall_Generic_Call invalid option (%c)", *(sig - 1));
			MessageBox(NULL, buffer, "Lua Error", MB_OK);
			this->m_critical.unlock();
			return false;
		}
		}

		nArg++;
		luaL_checkstack(this->m_luaState, 1, "too many arguments");

	} endwhile:

	int nRes = strlen(sig);

	if (lua_pcall(this->m_luaState, nArg, nRes, 0) != 0)
	{
		sprintf_s(buffer, "luacall_Generic_Call error running function '%s': '%s'", func, lua_tolstring(this->m_luaState, -1, 0));
		MessageBox(NULL, buffer, "Lua Error", MB_OK);
		this->m_critical.unlock();
		return false;
	}

	int nRetValCnt = nRes;
	nRes = -nRes;

	while (*sig)
	{
		switch (*sig++)
		{
		case 'd':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				this->m_critical.unlock();
				return false;
			}
			*va_arg(VA_LIST, double*) = lua_tonumber(this->m_luaState, nRes);

		} break;

		case 'i':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				this->m_critical.unlock();
				return false;
			}

			*va_arg(VA_LIST, int*) = (int)lua_tointeger(this->m_luaState, nRes);
		} break;

		case 's':
		{
			if (!lua_isstring(this->m_luaState, nRes))
			{
				this->m_critical.unlock();
				return false;
			}

			*va_arg(VA_LIST, const char**) = lua_tostring(this->m_luaState, nRes);
		} break;

		case 'w':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				this->m_critical.unlock();
				return false;
			}

			*va_arg(VA_LIST, unsigned int*) = (unsigned int)lua_tointeger(this->m_luaState, nRes);
		} break;

		case 'l':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				this->m_critical.unlock();
				return false;
			}

			*va_arg(VA_LIST, unsigned long long*) = (unsigned long long)lua_tointeger(this->m_luaState, nRes);
		} break;

		case 'f':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				this->m_critical.unlock();
				return false;
			}
			*va_arg(VA_LIST, float*) = (float)lua_tonumber(this->m_luaState, nRes);

		} break;

		case 'j':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				this->m_critical.unlock();
				return false;
			}
			*va_arg(VA_LIST, unsigned long*) = (unsigned long)lua_tointeger(this->m_luaState, nRes);

		} break;

		default:
		{
			this->m_critical.unlock();
			return false;
		} break;

		}

		nRes++;
	}

	va_end(VA_LIST);

	if (nRetValCnt)
	{
		lua_pop(this->m_luaState, nRetValCnt);
	}

	lua_gc(this->m_luaState, LUA_GCCOLLECT, 0);

	this->m_critical.unlock();

	return true;
}
#include "stdafx.h"
#include "LuaState.h"
#include "LuaManager.h"
#include "Util.h"
#include "LuaCommands.h"
#include "LuaDatabase.h"
#include "LuaUser.h"
#include "LuaFunctions.h"
#include "LuaInventory.h"
#include "LuaWarehouse.h"
#include "LuaTrade.h"
#include "LuaUserGuild.h"
#include "MuLuaFileProtect.h"
#include "LuaSocket.h"
#include "LuaDatabaseAsync.h"
#include "Log.h"
#include "LuaChaosBoxGenesis.h"

LuaState::LuaState() : Loaded(false), m_luaState(nullptr)
{

}

LuaState::~LuaState()
{
	this->CloseLua();
}

void LuaState::CreateLua()
{
	this->m_luaState = luaL_newstate();

	if (this->m_luaState == nullptr) {
		return;
	}

	luaL_openlibs(this->m_luaState);

	gLuaFunctions.AddFunctions(this->m_luaState);
	gLuaUserGuild.AddGuild(this->m_luaState);
	gSocket.SetGlobalFunctions(this->m_luaState);
	RegisterDatabase(this->m_luaState);
	RegisterInventoryClass(this->m_luaState);
	RegisterTradeClass(this->m_luaState);
	RegisterWarehouseClass(this->m_luaState);
	RegisterUserClass(this->m_luaState);
	RegisterCommandsClas(this->m_luaState);
	#ifdef CHAOS_MACHINE_GENESIS
	RegisterChaosBoxGenesisClass(this->m_luaState);
	#endif
	gLuaDatabaseAsync.RegisterFunction(this->m_luaState);

	lua_gc(this->m_luaState, LUA_GCCOLLECT, 0);
}

void LuaState::CloseLua()
{
	if (this->m_luaState)
	{
		lua_close(this->m_luaState);
		this->m_luaState = nullptr;
	}
}

void LuaState::do_file(std::string string)
{
	if (this->m_luaState == nullptr)
	{
		return;
	}

	if (luaL_loadfile(this->m_luaState, string.c_str()))
	{
		ErrorMessageBox("Error!!, : %s\n", lua_tostring(this->m_luaState, -1));
		return;
	}

	INT iState = lua_pcall(this->m_luaState, 0, -1, 0);

	if (iState)
	{
		ErrorMessageBox("Error!!, : %s : State = %d\n", lua_tostring(this->m_luaState, -1), iState);
		return;
	}
}

bool LuaState::Generic_Call(char* func, char* sig, ...)
{
	if (this->m_luaState == nullptr)
	{
		return false;
	}

	va_list	VA_LIST;
	int nArg = 0;

	va_start(VA_LIST, sig);
	lua_getglobal(this->m_luaState, func);

	char buffer[512];
	memset(buffer, 0, sizeof(buffer));

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
			sprintf_s(buffer, "GameServerCALL error running function '%s': '%s'", func, lua_tolstring(this->m_luaState, -1, 0));
			gLog.Output(LOG_LUA, buffer);
			return false;
		}
		}

		nArg++;
		luaL_checkstack(this->m_luaState, 1, "too many arguments");

	} endwhile:

	int nRes = strlen(sig);

	int nStatus = lua_pcall(this->m_luaState, nArg, nRes, 0);

	if (nStatus != 0)
	{
		gLuaGameServer.ProcessErrorLog(this->m_luaState, func, nStatus);
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
				return false;
			}
			*va_arg(VA_LIST, double*) = lua_tonumber(this->m_luaState, nRes);

		} break;

		case 'i':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, int*) = (int)lua_tointeger(this->m_luaState, nRes);
		} break;

		case 's':
		{
			if (!lua_isstring(this->m_luaState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, const char**) = lua_tostring(this->m_luaState, nRes);
		} break;

		case 'w':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, unsigned int*) = (unsigned int)lua_tointeger(this->m_luaState, nRes);
		} break;

		case 'l':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, unsigned long long*) = (unsigned long long)lua_tointeger(this->m_luaState, nRes);
		} break;

		case 'f':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				return false;
			}
			*va_arg(VA_LIST, float*) = (float)lua_tonumber(this->m_luaState, nRes);

		} break;

		case 'j':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				return false;
			}
			*va_arg(VA_LIST, unsigned long*) = (unsigned long)lua_tointeger(this->m_luaState, nRes);

		} break;

		default:
		{
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

	return true;
}

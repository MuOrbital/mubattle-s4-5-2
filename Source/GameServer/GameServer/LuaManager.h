#pragma once

#include "LuaState.h"
#include "Path.h"
#include "Util.h"
#include "LuaGameServer.h"
#include "CriticalSection.h"

class CLuaManager {
public:
	CLuaManager() : MyLua(nullptr) {
		
	};

	~CLuaManager() {
		this->ResetLua();
	};

	void ResetLua() {
		if (this->MyLua) {
			this->MyLua->CloseLua();
			delete MyLua;
			this->MyLua = nullptr;
		}
	}

	void CreateLua() {
		this->MyLua = new LuaState();

		this->MyLua->Loaded = false;

		this->MyLua->CreateLua();
	};

	lua_State* getMyLua() {
		if (this->MyLua == nullptr) {
			return nullptr;
		}

		return this->MyLua->m_luaState;
	};

	void StartLua() {
		this->MyLua->do_file(gPath.GetFullPath("Scripts\\GameServer.lua"));

		gLuaGameServer.GameServer();
	}

public:
	inline std::size_t getMemoryUsed() {
		if (this->MyLua == nullptr) {
			return 0;
		}

		std::size_t kb = static_cast<std::size_t>(lua_gc(this->MyLua->m_luaState, LUA_GCCOUNT, 0));
		kb *= 1024;
		kb += static_cast<std::size_t>(lua_gc(this->MyLua->m_luaState, LUA_GCCOUNTB, 0));
		return kb;
	}

public:
	LuaState* MyLua;
};

extern CLuaManager LuaManager;
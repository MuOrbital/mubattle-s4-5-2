#pragma once
class CLuaReload 
{
public:
	CLuaReload() = default;
	~CLuaReload() = default;

	void RestartLua();

};

extern CLuaReload gLuaReload;
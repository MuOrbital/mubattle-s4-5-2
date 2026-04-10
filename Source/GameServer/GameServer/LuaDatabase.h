#pragma once

#include "LuaManager.h"
#include "LuaGameServer.h"
#include "QueryManager.h"

class SQLServer {
public:
	SQLServer() {

	};

	~SQLServer() {

	};

	int connect(std::string ConStr, std::string UID, std::string PWD) {
		return gQueryManager.Connect((char*)ConStr.c_str(), (char*)UID.c_str(), (char*)PWD.c_str());
	};

	int exec(std::string str) {
		return gQueryManager.ExecQuery(str);
	};

	int execGetInt(std::string str) {
		return gQueryManager.GetAsInteger((char*)str.c_str());
	};

	std::string execGetStr(std::string str) {
		static char buff[256];
		memset(buff, 0, sizeof(buff));

		gQueryManager.GetAsString((char*)str.c_str(), buff, sizeof(buff));

		return &buff[0];
	};

	int fetch() {
		return gQueryManager.Fetch();
	};

	void clear() {
		return gQueryManager.Close();
	};

	int getInt(std::string string) {
		return gQueryManager.GetAsInteger((char*)string.c_str());
	};

	int getResult(int value) {
		return gQueryManager.GetResult(value);
	};

	float getFloat(std::string string) {
		return gQueryManager.GetAsFloat((char*)string.c_str());
	};

	std::string getStr(std::string string) {
		static char buff[256];
		memset(buff, 0, sizeof(buff));

		gQueryManager.GetAsString((char*)string.c_str(), buff, sizeof(buff));

		return &buff[0];
	};

	void ExecuteQuery(std::string query) {
		auto execResult = gQueryManager.ExecQuery(query);


	}
};

void RegisterDatabase(lua_State* lua);
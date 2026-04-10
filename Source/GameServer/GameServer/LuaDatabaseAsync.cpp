#include "stdafx.h"
#include "LuaDatabaseAsync.h"
#include "QueryAsyncManager.h"
#include "QueryAsync.h"
#include "LuaStack.hpp"

CLuaDatabaseAsync gLuaDatabaseAsync;

CLuaDatabaseAsync::CLuaDatabaseAsync()
{
}

CLuaDatabaseAsync::~CLuaDatabaseAsync()
{
}

int ConnectQueryAsync(std::string ConStr, std::string UID, std::string PWD) {
	return gQueryAsync.manager.Connect((char*)ConStr.c_str(), (char*)UID.c_str(), (char*)PWD.c_str());
}

void CreateAsyncQuery(const std::string& name, const std::string& query, int aIndex, int getResult) {
	gQueryAsync.Query(name, query, aIndex, getResult);
}

std::string QueryAsyncGetValue(const std::string& name, const std::string& column) {
	std::map<std::string, QueryJobsResult>::iterator job = gQueryAsync.m_jobsResult.find(name);

	if (job == gQueryAsync.m_jobsResult.end()) {
		return "nil";
	}

	int count = 0;

	for (auto element : job->second.result) {
		if (count == job->second.result_count) {
			if (_stricmp(column.c_str(), element.name.c_str()) == 0) {
				job->second.result_count++;
				return element.result.c_str();
			}
		}

		count++;
	}

	return "nil";
}

void QueryAsyncDelete(const std::string& name) {
	std::map<std::string, QueryJobsResult>::iterator job = gQueryAsync.m_jobsResult.find(name);

	if (job != gQueryAsync.m_jobsResult.end()) {
		gQueryAsync.m_jobsResult.erase(job);
	}
}

void CLuaDatabaseAsync::RegisterFunction(lua_State* lua)
{
	luaaa::LuaModule(lua).fun("ConnectQueryAsync", &ConnectQueryAsync);
	luaaa::LuaModule(lua).fun("CreateAsyncQuery", &CreateAsyncQuery);

	luaaa::LuaModule(lua).fun("QueryAsyncGetValue", &QueryAsyncGetValue);
	luaaa::LuaModule(lua).fun("QueryAsyncDelete", &QueryAsyncDelete);
}

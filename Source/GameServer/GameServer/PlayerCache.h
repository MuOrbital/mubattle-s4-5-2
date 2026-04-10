#pragma once

#include "User.h"
#include <unordered_map>

class CPlayerCache {
public:
	CPlayerCache();
	~CPlayerCache();

	std::unordered_map<std::string, int> m_cacheInt;
	std::unordered_map<std::string, std::string> m_cacheStr;
};

extern CPlayerCache gPlayerCache[MAX_OBJECT];
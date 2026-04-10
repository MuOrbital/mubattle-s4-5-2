#include "stdafx.h"
#include "PlayerCache.h"

CPlayerCache gPlayerCache[MAX_OBJECT];

CPlayerCache::CPlayerCache()
{
	this->m_cacheInt.clear();
}

CPlayerCache::~CPlayerCache()
{
	this->m_cacheInt.clear();
}

// GameMaster.h: interface for the CGameMaster class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

#define MAX_GAME_MASTER 100

struct GAME_MASTER_INFO
{
	char Account[11];
	char Name[11];
	BYTE Level;
};

class CGameMaster
{
public:
	CGameMaster();
	virtual ~CGameMaster();
	void Load(char* path);
	int CheckGameMasterLevel(LPOBJ lpObj);
private:
	std::vector<GAME_MASTER_INFO> m_GameMasterInfo;
};

extern CGameMaster gGameMaster;

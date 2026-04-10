#pragma once
#include "Packets.h"

#define MAX_MAIN_VIEWPORT 400

struct PMSG_NEW_HEALTH_BAR_RECV
{
	PSWMSG_HEAD header;// C2:F3:E2
	BYTE count;
};

struct PMSG_NEW_HEALTH_RECV
{
	WORD index;
	BYTE type;
	BYTE rate;
};

struct NEW_HEALTH_BAR
{
	WORD index;
	BYTE type;
	BYTE rate;
};

class CMonsterBar
{
public:
	CMonsterBar();
	virtual ~CMonsterBar();
	SingletonInstance(CMonsterBar);

public:
	void GCNewHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg);
	void ClearNewHealthBar();
	void InsertNewHealthBar(WORD index, BYTE type, BYTE rate);
	NEW_HEALTH_BAR* GetNewHealthBar(WORD index, BYTE type);
	void RenderHPBar();

private:
	NEW_HEALTH_BAR gNewHealthBar[MAX_MAIN_VIEWPORT];
};

#define gMonsterBar SingNull(CMonsterBar)

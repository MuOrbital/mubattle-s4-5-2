#pragma once

#include "Protocol.h"

#define MAX_RANK 10
#define RANKING_CHARSET_SIZE 18
#define RANKING_EQUIPMENT_SIZE 17
#define RANKING_INVENTORY_SLOTS 9

struct CUSTOM_RANKING
{
	int index;
	char Name[20];
	char Col1[20];
	char Col2[20];
};

struct CUSTOM_RANKING_DATA
{
	char szName[20];
	BYTE Class;
	BYTE Vip;
	int Score;
};

#pragma pack(push, 1)

struct SDHP_CUSTOM_RANKING_SEND
{
	PSBMSG_HEAD header;
	WORD index;
	WORD type;
};

struct SDHP_RANKING_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header;
	int characterIndex;
	WORD index;
	char name[11];
};

struct SDHP_RANKING_CHARACTER_INFO_RECV
{
	PWMSG_HEAD header;
	int characterIndex;
	WORD index;
	BYTE result;
	char name[11];
	BYTE DBClass;
	BYTE CtlCode;
	BYTE Inventory[RANKING_INVENTORY_SLOTS][5];
};

struct PMSG_RANKING_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header;
	int characterIndex;
	BYTE result;
	BYTE PlayerClass;
	BYTE CtlCode;
	BYTE Equipment[RANKING_EQUIPMENT_SIZE];
	WORD PetIndex;
	WORD WingIndex;
	char Name[10];
};

struct PMSG_CUSTOM_RANKING_COUNT_RECV
{
	PSBMSG_HEAD header;
};

struct PMSG_CUSTOM_RANKING_RECV
{
	PSBMSG_HEAD header;
	BYTE type;
};

struct PMSG_RANKING_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header;
	int characterIndex;
	char name[10];
};

#pragma pack(pop)

struct SDHP_CUSTOM_RANKING_RECV
{
	PWMSG_HEAD header;
	int index;
	int type;
	int count;
};

struct PMSG_CUSTOM_RANKING_SEND
{
	PSWMSG_HEAD header;
	int RankIndex;
	char rankname[20];
	char col1[20];
	char col2[20];
	int count;
};

struct PMSG_CUSTOM_RANKING_COUNT_SEND
{
	PSBMSG_HEAD header;
	int count;
};

class CCustomRanking
{
public:
	void Load(char* path);
	void GCReqRankingPlayer(int Index, PMSG_RANKING_CHARACTER_INFO_RECV* lpMsg);
	void GDCustomRankingPlayerRecv(SDHP_RANKING_CHARACTER_INFO_RECV* lpMsg);
	void GCReqRanking(int Index, PMSG_CUSTOM_RANKING_RECV* lpMsg);
	void GCReqRankingCount(int Index, PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg);
	void GDCustomRankingRecv(BYTE* ReceiveBuffer);

private:
	void SendCharacterInfo(int Index, int characterIndex, const char* name, BYTE dbClass,
		BYTE ctlCode, const BYTE* charSet, WORD petIndex, WORD wingIndex, BYTE result);
	void BuildCharacterSet(BYTE dbClass, BYTE inventory[RANKING_INVENTORY_SLOTS][5],
		BYTE charSet[RANKING_CHARSET_SIZE], WORD* petIndex, WORD* wingIndex);

	int m_count;
	CUSTOM_RANKING r_Data[MAX_RANK];
};

extern CCustomRanking gCustomRanking;

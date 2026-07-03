// CustomAttack.h: interface for the CCustomAttack class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include "User.h"
#include "Protocol.h"

struct SDHP_CARESUME_SEND
{
	PBMSG_HEAD header; // C1:F5
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CARESUME_SAVE_SEND
{
	PBMSG_HEAD header; // C1:F6
	WORD index;
	char account[11];
	char name[11];
	WORD active;
	WORD skill;
	WORD map;
	WORD posx;
	WORD posy;
	WORD autobuff;
};

struct SDHP_CARESUME_RECV
{
	PBMSG_HEAD header; // C1:F5
	WORD index;
	char account[11];
	char name[11];
	WORD active;
	WORD skill;
	WORD map;
	WORD posx;
	WORD posy;
};

struct SDHP_OFFATTACK_SAVE_SEND
{
	PBMSG_HEAD header; // C1:F7
	char account[11];
	char name[11];
	WORD active;
	WORD skill;
	WORD map;
	WORD posx;
	WORD posy;
	WORD autobuff;
	WORD accountLevel;
	char accountExpireDate[20];
};

struct SDHP_OFFATTACK_LIST_SEND
{
	PBMSG_HEAD header; // C1:F8
};

struct SDHP_OFFATTACK_RESUME_RECV
{
	PBMSG_HEAD header; // C1:F8
	char account[11];
	char name[11];
	WORD skill;
	WORD map;
	WORD posx;
	WORD posy;
	WORD autobuff;
	WORD accountLevel;
	char accountExpireDate[20];
};

struct CUSTOM_OFFATTACK_RESUME_INFO
{
	char account[11];
	char name[11];
	WORD skill;
	WORD map;
	WORD posx;
	WORD posy;
	WORD autobuff;
	WORD accountLevel;
	char accountExpireDate[20];
};

class CCustomAttack
{
public:
	CCustomAttack();
	virtual ~CCustomAttack();
	void ReadCustomAttackInfo(char* section,char* path);
	void CommandCustomAttack(LPOBJ lpObj,char* arg);
	bool CommandCustomAttackOffline(LPOBJ lpObj, char* arg);
	bool GetAttackSkill(LPOBJ lpObj, int* SkillNumber, int SetSkill);
	bool GetTargetMonster(LPOBJ lpObj,int SkillNumber,int* MonsterIndex);
	bool CheckRequireMoney(LPOBJ lpObj);
	void OnAttackClose(LPOBJ lpObj);
	void OnAttackSecondProc(LPOBJ lpObj);
	void OnAttackAlreadyConnected(LPOBJ lpObj);
	void OnAccountLevelChange(LPOBJ lpObj);
	void OnAttackMonsterAndMsgProc(LPOBJ lpObj);
	void SendSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber);
	void SendMultiSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber);
	void SendDurationSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber);

	void SendRFSkillAttack(LPOBJ lpObj, int aIndex, int SkillNumber);
	void GetTargetParty(LPOBJ lpObj, int SkillNumber);
	void DGCustomAttackResumeSend(int aIndex);
	void DGCustomAttackResumeRecv(SDHP_CARESUME_RECV* lpMsg);
	void DGCustomAttackResumeSaveSend(int Index);
	void DGCustomOffAttackResumeListSend();
	void DGCustomOffAttackResumeRecv(SDHP_OFFATTACK_RESUME_RECV* lpMsg);
	void DGCustomOffAttackResumeSaveSend(int Index,int active);
	bool ApplyCustomOffAttackResume(LPOBJ lpObj);
public:
	//CustomAttack
	int m_CustomAttackTime[4];
	int m_CustomAttackDelay;
	int m_CustomAttackPotionDelay;
	int m_CustomAttackAutoBuff[4];
	int m_CustomAttackAutoBuffDelay;

	int m_CustomAttackAutoResume[4];
	int m_CustomAttackRangeDisable[4];

	int m_CustomAttackOfflineGPGain;
	int m_CustomOffAttackAutoResume[4];
	std::map<int,CUSTOM_OFFATTACK_RESUME_INFO> m_CustomOffAttackResumeInfo;
};

extern CCustomAttack gCustomAttack;

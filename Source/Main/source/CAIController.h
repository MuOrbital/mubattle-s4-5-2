#pragma once
#include "MuHelperProtocol.h"
#include "Singleton.h"

#define INVENTORY_HELPER_ACTIVE
//#define MOUSE_WHELL_HELPER_ON

#define MAX_PICK_EXTRA				12

typedef struct
{
	bool Shutdown;
	BYTE bRate;
}MUAction_info;

typedef struct
{
	int Index;
	bool ShutdownDelay;
	DWORD TimeDelay;
	bool Counter;
	int CounterPre;
	int CounterSub;
}MUSkill_info;

typedef struct
{
	bool Near;
	bool Select;
	bool Jewel;
	bool Ancient;
	bool Excellent;
	bool Zen;
	bool Extra;
	unicode::t_char Name[MAX_PICK_EXTRA][28];
}MUPick_info;

class CAIController : public Singleton<CAIController>
{
private:
	CHARACTER* sCharacter;
	bool m_IsStart;
	CHARACTER* sMonsterATK;
	CHARACTER* sMonsterPre;
	ITEM_t* prev_item;
	ITEM_t* cur_item;
	DWORD m_CurTimeATK1;
	DWORD m_TimeCondition[2];
	DWORD m_WaitTimeHealAT;
	DWORD m_WaitTimeConsume;
	DWORD m_WaitTimeBuffAT;
	CheckerTime buffParty_Work;//m_TimeBuffParty;
	CheckerTime healParty_Work;// m_TimeHealParty;
	CheckerTime WorkAI;
	int StartPositionX;
	int StartPositionY;
	std::chrono::steady_clock::time_point m_WaitTimeReturn;
	int m_CounATK;
	bool m_IsAutoAttack;
	//DWORD m_DelayThird[3];
	DWORD buff_member_work[3][MAX_PARTYS];
	bool isSendPetCommand;
	DWORD m_DelayFindTarget;
	bool ShutdownReturnOn;
public:
	int AttackRange;
	MUAction_info healPotion;
	MUAction_info healskillLife;
	MUAction_info healskilldrain;
	bool m_DistanceLong;
	bool DistanceReturn;
	int m_DistanceOnSec;
	int SkillIndex;
	MUSkill_info SkillGroup[2];
	bool ShutdownCombo;
	bool ShutdownParty;
	MUAction_info healParty;
	bool buff_duration_all_work;
	DWORD space_of_casting_buff;
	bool ShutdownPetCmd;
	int PetCmdType;
	bool ShutdownBuff;
	int m_SkillBuff[3];
	int m_PartySkillBuff[3];
	int m_PickDistance;
	bool ShutdownRepair;
	MUPick_info Picker_work;

	bool m_AutoFriend;
	bool m_AutoGuildMember;
	bool m_EliteManaPotion;
public:
	CAIController(CHARACTER* Character);
	virtual ~CAIController();

	bool IsRunning();
	bool CanUseAIController();
	void Start();
	void Stop();
	void SetActiveCharacter(CHARACTER* pCha);
	void ResetConfig();
	void LoadDefaultConfg();
	void SaveConfig();
	float GetDistance(CHARACTER* pCha);
	float GetDistance(OBJECT* o);
	void WhatToDoNext();
	bool ConsumePotion();
	int runtime_choosetarget();
	int GetHostileEnemyNum();
	CHARACTER* RankEnemy(CHARACTER* Rank1, CHARACTER* Rank2);
	bool IsEnemyAttaced();
	bool IsEnemyAttacking(CHARACTER* Target);
	bool CheckPickup();
	int RateItem(ITEM_t* item);
	bool PickupItem(ITEM_t* item);
	void RepairItem();
	int GetSkillIndex(int SkillKey);
	bool SkillAt(int SkillKey, CHARACTER* Target, bool bActive);
	bool UseConditionalSkill(MUSkill_info* Skill, DWORD* cTime);
	bool IsBuffActive(CHARACTER* pCha, int SkillIndex);
	bool SelfBuffAt();
	bool runtime_healAt(CHARACTER* Target);
	bool runtime_party_healing();
	bool runtime_party_buff();
	void MoveTo(int x, int y);
	bool PlayerReached(int x, int y);
	bool RecievePartyRequest(int a2);
	bool CanUsingEliteManaPotion();
	void SetPartyBuff();
	bool Pack(BYTE* BufferUnpack);
	bool Unpack(BYTE* BufferPack);
private:
	std::vector<std::string> splitByPlus(const std::string& str);
	bool matchesPattern(std::string text, std::string pattern);
};

CAIController* GetMuHelperController();
#define gmAIController			(GetMuHelperController())

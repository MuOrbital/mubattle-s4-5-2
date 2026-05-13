#pragma once
#include "Packets.h"
#define MAX_MAIN_VIEWPORT 400
struct PMSG_NEW_HEALTH_BAR_RECV
{
    PSWMSG_HEAD header;
    BYTE count;
};
struct PMSG_NEW_HEALTH_RECV
{
    WORD index;
    BYTE type;
    BYTE rate;
    DWORD life;
    DWORD maxLife;
    WORD topIndex[10];
    DWORD topDamage[10];
};
struct NEW_HEALTH_BAR
{
    WORD index;
    BYTE type;
    BYTE rate;
    DWORD life;
    DWORD maxLife;
    WORD topIndex[10];
    DWORD topDamage[10];
};
class CMonsterBar
{
public:
    CMonsterBar();
    virtual ~CMonsterBar();
    SingletonInstance(CMonsterBar);
    bool  m_ShowTopHit;
    int   m_ExpandedBossCharIndex; // indice em CharactersClient do boss expandido (-1 = nenhum)
    DWORD m_ClickDelay;
public:
    void GCNewHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg);
    void ClearNewHealthBar();
    void InsertNewHealthBar(WORD index, BYTE type, BYTE rate, DWORD life, DWORD maxLife, WORD* topIndex, DWORD* topDamage);
    NEW_HEALTH_BAR* GetNewHealthBar(WORD index, BYTE type);
    void RenderHPBar();
private:
    NEW_HEALTH_BAR gNewHealthBar[MAX_MAIN_VIEWPORT];
    enum IMAGE_LIST
    {
        IMAGE_TOP_HIT_BOSS = BITMAP_TOP_HIT_BOSS
    };
    void LoadImages();
    void UnloadImages();
};
#define gMonsterBar SingNull(CMonsterBar)
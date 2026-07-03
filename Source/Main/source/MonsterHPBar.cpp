#include "stdafx.h"
#include "MonsterHPBar.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "NewUISystem.h"
#include <CharacterManager.h>

CMonsterBar::CMonsterBar()
{
    this->ClearNewHealthBar();
    this->m_ShowTopHit            = false;
    this->m_ExpandedBossCharIndex = -1;
    this->m_ClickDelay            = 0;

    this->LoadImages();
}

CMonsterBar::~CMonsterBar()
{
    this->UnloadImages();
}

static void ResetNewHealthBarSlot(NEW_HEALTH_BAR& bar)
{
    bar.index      = 0xFFFF;
    bar.type       = 0;
    bar.rate       = 0;
    bar.life       = 0;
    bar.maxLife    = 0;
    bar.updateTick = 0;

    for (int i = 0; i < 10; i++)
    {
        bar.topIndex[i]  = (WORD)-1;
        bar.topDamage[i] = 0;
    }
}

static bool IsHealthBarOwnerLive(WORD index, BYTE type)
{
    for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
    {
        CHARACTER* c = &CharactersClient[i];
        if (c->Key == index && c->Object.Kind == type)
        {
            return c->Object.Live && !c->SafeZone;
        }
    }

    return false;
}
void CMonsterBar::GCNewHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg)
{
    for (int n = 0; n < lpMsg->count; n++)
    {
        PMSG_NEW_HEALTH_RECV* lpInfo = (PMSG_NEW_HEALTH_RECV*)(
            ((BYTE*)lpMsg) + sizeof(PMSG_NEW_HEALTH_BAR_RECV) + (sizeof(PMSG_NEW_HEALTH_RECV) * n));
        InsertNewHealthBar(lpInfo->index, lpInfo->type, lpInfo->rate,
            lpInfo->life, lpInfo->maxLife, lpInfo->topIndex, lpInfo->topDamage);
    }
}

void CMonsterBar::ClearNewHealthBar()
{
    for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
    {
        ResetNewHealthBarSlot(gNewHealthBar[n]);
    }
}
void CMonsterBar::InsertNewHealthBar(WORD index, BYTE type, BYTE rate, DWORD life, DWORD maxLife, WORD* topIndex, DWORD* topDamage)
{
    int slot = -1;

    for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
    {
        if (gNewHealthBar[n].index == index && gNewHealthBar[n].type == type)
        {
            slot = n;
            break;
        }

        if (slot == -1 && gNewHealthBar[n].index == 0xFFFF)
        {
            slot = n;
        }
    }

    if (slot == -1)
    {
        return;
    }

    gNewHealthBar[slot].index      = index;
    gNewHealthBar[slot].type       = type;
    gNewHealthBar[slot].rate       = rate;
    gNewHealthBar[slot].life       = life;
    gNewHealthBar[slot].maxLife    = maxLife;
    gNewHealthBar[slot].updateTick = GetTickCount();

    for (int i = 0; i < 10; i++)
    {
        gNewHealthBar[slot].topIndex[i]  = topIndex[i];
        gNewHealthBar[slot].topDamage[i] = topDamage[i];
    }
}
NEW_HEALTH_BAR* CMonsterBar::GetNewHealthBar(WORD index, BYTE type)
{
    for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
    {
        if (gNewHealthBar[n].index != 0xFFFF)
        {
            if (gNewHealthBar[n].index == index && gNewHealthBar[n].type == type)
                return &gNewHealthBar[n];
        }
    }
    return 0;
}

void FormatNumber(QWORD value, char* output)
{
    char temp[50];
    sprintf(temp, "%I64u", value);

    int len    = (int)strlen(temp);
    int commas = (len - 1) / 3;
    int newLen = len + commas;

    output[newLen] = '\0';

    int i     = len - 1;
    int j     = newLen - 1;
    int count = 0;

    while (i >= 0)
    {
        if (count == 3)
        {
            output[j--] = ',';
            count = 0;
        }
        output[j--] = temp[i--];
        count++;
    }
}

static bool IsSpecialMonster(int monsterIndex)
{
    switch (monsterIndex)
    {
    case 77: case 900: case 295: case 309:
    case 349: case 361: case 459: case 561:
        return true;
    default:
        return false;
    }
}

void CMonsterBar::RenderHPBar()
{
    NEW_HEALTH_BAR* lpHealthBar;
    char Text[100];

    DWORD currentTick = GetTickCount();

    for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
    {
        if (gNewHealthBar[n].index == 0xFFFF)
        {
            continue;
        }

        if (!IsHealthBarOwnerLive(gNewHealthBar[n].index, gNewHealthBar[n].type))
        {
            ResetNewHealthBarSlot(gNewHealthBar[n]);
            continue;
        }

        if (gNewHealthBar[n].updateTick != 0 && (currentTick - gNewHealthBar[n].updateTick) > 15000)
        {
            gNewHealthBar[n].updateTick = currentTick;
        }
    }
    const int   FixedPosX            = (int)((float)GetWindowsX / 1.95f);
    const int   FixedPosY            = 5;
    const float FixedWidth           = 400.0f;
    const float FixedHeight          = 30.0f;
    const float SpecialBarSlotHeight  = 35.0f;

    float Angle[3];
    int   PosX, PosY;

    struct SpecialBossEntry
    {
        int charIndex;
        int slot;
    };

    SpecialBossEntry specialBosses[MAX_CHARACTERS_CLIENT];
    int specialBossCount = 0;

    for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
    {
        CHARACTER* c = &CharactersClient[i];
        if (!c->Object.Live)                    continue;
        if (c->Object.Kind != KIND_MONSTER)     continue;
        if (c->SafeZone)                        continue;
        if (!IsSpecialMonster(c->MonsterIndex)) continue;
        if (this->GetNewHealthBar(c->Key, c->Object.Kind) == 0) continue;

        specialBosses[specialBossCount].charIndex = i;
        specialBosses[specialBossCount].slot      = specialBossCount;
        specialBossCount++;
    }

    if (m_ExpandedBossCharIndex != -1)
    {
        bool stillAlive = false;
        for (int s = 0; s < specialBossCount; s++)
        {
            if (specialBosses[s].charIndex == m_ExpandedBossCharIndex)
            {
                stillAlive = true;
                break;
            }
        }
        if (!stillAlive)
        {
            m_ShowTopHit            = false;
            m_ExpandedBossCharIndex = -1;
        }
    }

    static bool s_LastBtnDown = false;
    bool curDown = ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);

    for (int s = 0; s < specialBossCount; s++)
    {
        int displayPosX = FixedPosX;
        int displayPosY = FixedPosY + (int)(specialBosses[s].slot * SpecialBarSlotHeight);

        float barLeft = (float)displayPosX - (FixedWidth / 2.0f) - 5.2f;
        float barTop  = (float)displayPosY - 0.2f;
        float barW    = FixedWidth + 19.0f + 40.0f;
        float barH    = FixedHeight + 10.0f;

        if (CheckMouseIn((int)barLeft, (int)barTop, (int)barW, (int)barH))
        {
            MouseLButton      = 0;
            MouseLButtonPush  = 0;
            MouseLButtonPop   = 0;
            MouseUpdateTime   = 0;

            int btnX = displayPosX + 190;
            int btnY = displayPosY + 5;

            if (CheckMouseIn(btnX - 20, btnY - 5, 40, 55))
            {
                if (curDown && !s_LastBtnDown)
                {
                    if (GetTickCount() - this->m_ClickDelay > 300)
                    {
                        int clickedCharIndex = specialBosses[s].charIndex;

                        if (m_ShowTopHit && m_ExpandedBossCharIndex == clickedCharIndex)
                        {
                            m_ShowTopHit            = false;
                            m_ExpandedBossCharIndex = -1;
                        }
                        else
                        {
                            m_ShowTopHit            = true;
                            m_ExpandedBossCharIndex = clickedCharIndex;
                        }

                        this->m_ClickDelay = GetTickCount();
                    }
                }
            }

            break;
        }
    }

    s_LastBtnDown = curDown;

    for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
    {
        CHARACTER* c = &CharactersClient[i];
        OBJECT*    o = &c->Object;

        if (!o->Live) continue;
        if (c->Object.Kind != KIND_PLAYER || c == Hero) continue;
        if (c->SafeZone) continue;

        lpHealthBar = this->GetNewHealthBar(c->Key, c->Object.Kind);
        if (lpHealthBar == 0) continue;

        float PlayerWidth     = 55.0f;
        float PlayerHeight    = 13.0f;
        float PlayerYOffset3D = 160.0f;

        Vector(o->Position[0], o->Position[1],
               o->Position[2] + o->BoundingBoxMax[2] + PlayerYOffset3D, Angle);
        Projection(Angle, &PosX, &PosY);

        float x = (float)PosX - (PlayerWidth / 2.0f);
        float y = (float)PosY;

        EnableAlphaTest(true);
        RenderBitmap(BITMAP_HP_BAR_BG, x + 3, y + 1, PlayerWidth + 2, PlayerHeight - 8,
                     0.f, 0.f, 1.0f, 1.0f);

        float currentFillWidth = (PlayerWidth * lpHealthBar->rate) / 100.0f;
        if (currentFillWidth > 0)
        {
            float fillU = currentFillWidth / PlayerWidth;
            RenderBitmap(BITMAP_HP_BAR_FILL, x + 3.f, y + 1.2f,
                         currentFillWidth + 2.5f, PlayerHeight - 7.98f,
                         0.f, 0.f, fillU, 1.0f);
        }

        DisableAlphaBlend();
    }

    for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
    {
        CHARACTER* c = &CharactersClient[i];
        OBJECT*    o = &c->Object;

        if (!o->Live)                          continue;
        if (c->Object.Kind != KIND_MONSTER)    continue;
        if (c->SafeZone)                       continue;
        if (IsSpecialMonster(c->MonsterIndex)) continue;

        if (g_pOption->m_MonsterHPBar == 0 &&
            (SelectedCharacter == -1 || &CharactersClient[SelectedCharacter] != c))
            continue;

        lpHealthBar = this->GetNewHealthBar(c->Key, c->Object.Kind);
        if (lpHealthBar == 0) continue;

        Vector(o->Position[0], o->Position[1],
               o->Position[2] + o->BoundingBoxMax[2] + 100.0f, Angle);
        Projection(Angle, &PosX, &PosY);

        float w = 55.0f;
        float h = 14.0f;
        float x = (float)PosX - (w / 2.0f);
        float y = (float)PosY;

        EnableAlphaTest(true);
        RenderBitmap(BITMAP_HP_BAR_BG, x + 3, y + 1, w + 2, h - 8, 0.f, 0.f, 1.0f, 1.0f);

        float currentFillWidth = (w * lpHealthBar->rate) / 100.0f;
        if (currentFillWidth > 0)
        {
            float fillU = currentFillWidth / w;
            RenderBitmap(BITMAP_HP_BAR_FILL, x + 3.f, y + 1.2f,
                         currentFillWidth + 2.5f, h - 7.98f,
                         0.f, 0.f, fillU, 1.0f);
        }

        g_pRenderText->SetFont(g_hFont);
        g_pRenderText->SetBgColor(0, 0, 0, 0);
        g_pRenderText->SetTextColor(255, 255, 255, 255);
        sprintf(Text, "%s", c->ID);
        g_pRenderText->RenderText(PosX - 24, PosY - 6, Text, 0, 0, RT3_SORT_CENTER, 0);

        DisableAlphaBlend();
    }

    for (int s = 0; s < specialBossCount; s++)
    {
        CHARACTER*  c   = &CharactersClient[specialBosses[s].charIndex];
        lpHealthBar     = this->GetNewHealthBar(c->Key, c->Object.Kind);

        int   displayPosX = FixedPosX;
        int   displayPosY = FixedPosY + (int)(specialBosses[s].slot * SpecialBarSlotHeight);
        float w           = FixedWidth;
        float h           = FixedHeight;
        float x           = (float)displayPosX - (w / 2.0f);
        float y           = (float)displayPosY;

        EnableAlphaTest(true);
        RenderBitmap(BITMAP_HP_BAR_BG, x + 8, y, w - 10, h - 5.5f, 0.f, 0.f, 1.0f, 1.0f);

        float maxFillWidth     = w - 21.8f;
        float currentFillWidth = (maxFillWidth * lpHealthBar->rate) / 100.0f;
        if (currentFillWidth > 0)
        {
            float fillU = (float)lpHealthBar->rate / 100.0f;
            RenderBitmap(BITMAP_HP_BAR_FILL, x + 15.8f, y + 1.2f,
                         currentFillWidth, h - 7.8f,
                         0.f, 0.f, fillU, 1.0f);
        }

        RenderBitmap(BITMAP_HP_BAR_FRAME, x - 5.2f, y - 0.2f, w + 19, h + 10, 0.f, 0.f, 1.0f, 1.0f);

        g_pRenderText->SetFont(g_hFontBold);
        g_pRenderText->SetBgColor(0, 0, 0, 0);
        g_pRenderText->SetTextColor(255, 255, 0, 255);

        char lifeText[50];
        char maxLifeText[50];
        FormatNumber(lpHealthBar->life,    lifeText);
        FormatNumber(lpHealthBar->maxLife, maxLifeText);

        sprintf(Text, "%s / %s", lifeText, maxLifeText);
        g_pRenderText->RenderText(displayPosX - 50, displayPosY + 8,
                                  Text, 0, 0, RT3_SORT_CENTER, 0);

        g_pRenderText->SetTextColor(0, 255, 255, 255);
        g_pRenderText->RenderText(displayPosX - 190, displayPosY + 25,
                                  c->ID, 70, 0, RT3_SORT_CENTER, 0);

        int  btnX        = displayPosX + 190;
        int  btnY        = displayPosY + 5;
        bool bThisExpanded = (m_ShowTopHit && m_ExpandedBossCharIndex == specialBosses[s].charIndex);

        EnableAlphaTest();
        glColor4f(1.f, 1.f, 1.f, 1.f);

        if (bThisExpanded)
            RenderImage(SEASON3B::CNewUIBuffWindow::IMAGE_BUFF_COLLAPSE,
                        btnX, btnY, 15, 15, 0, 0, 0.65f, 0.85f);
        else
            RenderImage(SEASON3B::CNewUIBuffWindow::IMAGE_BUFF_EXPAND,
                        btnX, btnY, 15, 15, 0, 0, 0.65f, 0.85f);

        DisableAlphaBlend();

        if (bThisExpanded)
        {
            float bgW = 130.0f;
            float bgH = 247.0f;
            float bgX = (float)GetWindowsX - 450.0f;
            float bgY = 80.0f;

            EnableAlphaTest();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

            RenderBitmap(IMAGE_TOP_HIT_BOSS, bgX + 320, bgY - 10, bgW, bgH,
                         0.0f, 0.0f, 200.0f / 256.0f, 200.0f / 256.0f);
            glDisable(GL_BLEND);

            g_pRenderText->SetFont(g_hFont);
            g_pRenderText->SetBgColor(0, 0, 0, 0);

            for (int pos = 0; pos < 10; pos++)
            {
                if (lpHealthBar->topIndex[pos] == (WORD)-1)
                    continue;

                char name[11] = { 0 };
                bool found    = false;

                for (int k = 0; k < MAX_CHARACTERS_CLIENT; k++)
                {
                    if (CharactersClient[k].Key == lpHealthBar->topIndex[pos])
                    {
                        strcpy(name, CharactersClient[k].ID);
                        found = true;
                        break;
                    }
                }

                if (!found || name[0] == '\0')
                    continue;

                char dmgText[50];
                FormatNumber(lpHealthBar->topDamage[pos], dmgText);

                char line[128];
                sprintf(line, "%s || %s", name, dmgText);

                if      (pos == 0) g_pRenderText->SetTextColor(255, 215,  0, 255);
                else if (pos == 1) g_pRenderText->SetTextColor(200, 200, 200, 255);
                else if (pos == 2) g_pRenderText->SetTextColor(205, 127,  50, 255);
                else               g_pRenderText->SetTextColor(120, 120, 120, 255);

                g_pRenderText->RenderText(bgX + 348, bgY + 32 + (pos * 20.5f),
                                          line, 0, 0, RT3_SORT_CENTER, 0);
            }

            DisableAlphaBlend();
        }
    }

    DisableAlphaBlend();
    glColor3f(1.f, 1.f, 1.f);
}

void CMonsterBar::LoadImages()
{
    LoadBitmap("Interface\\newi_top_hit_boss.tga", IMAGE_TOP_HIT_BOSS, GL_LINEAR);
}

void CMonsterBar::UnloadImages()
{
    DeleteBitmap(IMAGE_TOP_HIT_BOSS);
}

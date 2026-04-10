#include "stdafx.h"
#include "MonsterHPBar.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "NewUISystem.h"
#include <CharacterManager.h>
CMonsterBar::CMonsterBar()
{
    this->ClearNewHealthBar();
}

CMonsterBar::~CMonsterBar()
{
}

void CMonsterBar::GCNewHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg)
{
    ClearNewHealthBar();
    for (int n = 0; n < lpMsg->count; n++)
    {
        PMSG_NEW_HEALTH_RECV* lpInfo = (PMSG_NEW_HEALTH_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_NEW_HEALTH_BAR_RECV) + (sizeof(PMSG_NEW_HEALTH_RECV) * n));
        InsertNewHealthBar(lpInfo->index, lpInfo->type, lpInfo->rate);
    }
}

void CMonsterBar::ClearNewHealthBar()
{
    for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
    {
        gNewHealthBar[n].index = 0xFFFF;
        gNewHealthBar[n].type = 0;
        gNewHealthBar[n].rate = 0;
    }
}

void CMonsterBar::InsertNewHealthBar(WORD index, BYTE type, BYTE rate)
{
    for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
    {
        if (gNewHealthBar[n].index == 0xFFFF)
        {
            gNewHealthBar[n].index = index;
            gNewHealthBar[n].type = type;
            gNewHealthBar[n].rate = rate;
            return;
        }
    }
}

NEW_HEALTH_BAR* CMonsterBar::GetNewHealthBar(WORD index, BYTE type)
{
    for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
    {
        if (gNewHealthBar[n].index != 0xFFFF)
        {
            if (gNewHealthBar[n].index == index && gNewHealthBar[n].type == type)
            {
                return &gNewHealthBar[n];
            }
        }
    }
    return 0;
}

void CMonsterBar::RenderHPBar()
{
    NEW_HEALTH_BAR* lpHealthBar;
    float Angle[3];
    int PosX, PosY;
    char Text[100];

    // ====================== MONSTROS ESPECIAIS ======================
    int FixedPosX = (int)((float)GetWindowsX / 1.95f);
    int FixedPosY = 5;
    float FixedWidth = 400.0f;
    float FixedHeight = 30.0f;
    int FixedTextPercentX = -10;
    int FixedTextPercentY = 8;
    int FixedTextNameX = -190;
    int FixedTextNameY = 25;

    // ====================== MONSTROS NORMAIS ======================
    float NormalWidth = 55.0f;
    float NormalHeight = 14.0f;
    float NormalYOffset3D = 100.0f;
    int NormalTextPercentX = 13;
    int NormalTextPercentY = -0.1f;
    int NormalTextNameX = -24;
    int NormalTextNameY = -6;

    // ====================== JOGADORES ======================
    float PlayerWidth = 55.0f;
    float PlayerHeight = 13.0f;
    float PlayerYOffset3D = 160.0f;
    int PlayerTextPercentX = -6;
    int PlayerTextPercentY = 0;
    int PlayerTextNameX = -20;
    int PlayerTextNameY = -10;

    for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
    {
        CHARACTER* c = &CharactersClient[i];
        OBJECT* o = &c->Object;

        if (!o->Live) continue;

        // ====================== MONSTROS ESPECIAIS ======================
        if (c->Object.Kind == KIND_MONSTER)
        {
            bool bIsSpecialMonster = false;
            switch (c->MonsterIndex)
            {
            case 275: case 295: case 349: case 361: case 459: case 561:
                bIsSpecialMonster = true;
                break;
            }

            if (bIsSpecialMonster)
            {
                if (g_pOption->m_MonsterHPBar == 0 && (SelectedCharacter == -1 || &CharactersClient[SelectedCharacter] != c))
                    continue;

                lpHealthBar = this->GetNewHealthBar(c->Key, c->Object.Kind);
                if (lpHealthBar == 0) continue;

                int displayPosX = FixedPosX;
                int displayPosY = FixedPosY;
                float w = FixedWidth;
                float h = FixedHeight;

                float x = (float)displayPosX - (w / 2.0f);
                float y = (float)displayPosY;

                EnableAlphaTest(true);

                RenderBitmap(BITMAP_HP_BAR_BG, x + 8, y, w - 10, h - 5.5f, 0.f, 0.f, 1.0f, 1.0f);

                float maxFillWidth = w - 21.8f;
                float currentFillWidth = (maxFillWidth * lpHealthBar->rate) / 100.0f;
                if (currentFillWidth > 0)
                {
                    float fillU = (float)lpHealthBar->rate / 100.0f;
                    RenderBitmap(BITMAP_HP_BAR_FILL, x + 15.8f, y + 1.2f, currentFillWidth, h - 7.8f, 0.f, 0.f, fillU, 1.0f);
                }

                RenderBitmap(BITMAP_HP_BAR_FRAME, x - 5.2f, y - 0.2f, w + 19, h + 10, 0.f, 0.f, 1.0f, 1.0f);

                g_pRenderText->SetFont(g_hFontBold);
                g_pRenderText->SetTextColor(255, 255, 0, 255);
                g_pRenderText->SetBgColor(0, 0, 0, 0);
                sprintf(Text, "%d%%", lpHealthBar->rate);
                g_pRenderText->RenderText(displayPosX + FixedTextPercentX, displayPosY + FixedTextPercentY, Text, 0, 0, RT3_SORT_CENTER, 0);

                g_pRenderText->SetFont(g_hFontBold);
                g_pRenderText->SetTextColor(0, 255, 255, 255);
                g_pRenderText->SetBgColor(0, 0, 0, 0);
                sprintf(Text, "%s", c->ID);
                g_pRenderText->RenderText(displayPosX + FixedTextNameX, displayPosY + FixedTextNameY, Text, 70, 0, RT3_SORT_CENTER, 0);

                DisableAlphaBlend();
                continue;
            }
        }

        // ====================== MONSTROS NORMAIS ======================
        if (c->Object.Kind == KIND_MONSTER)
        {
            if (g_pOption->m_MonsterHPBar == 0 && (SelectedCharacter == -1 || &CharactersClient[SelectedCharacter] != c))
                continue;

            lpHealthBar = this->GetNewHealthBar(c->Key, c->Object.Kind);
            if (lpHealthBar == 0) continue;

            Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + NormalYOffset3D, Angle);
            Projection(Angle, &PosX, &PosY);

            float x = (float)PosX - (NormalWidth / 2.0f);
            float y = (float)PosY;
            float w = NormalWidth;
            float h = NormalHeight;

            EnableAlphaTest(true);

            RenderBitmap(BITMAP_HP_BAR_BG, x + 3, y + 1, w + 2, h - 8, 0.f, 0.f, 1.0f, 1.0f);

            float currentFillWidth = (w * lpHealthBar->rate) / 100.0f;
            if (currentFillWidth > 0)
            {
                float fillU = currentFillWidth / w;
                RenderBitmap(BITMAP_HP_BAR_FILL, x + 3.f, y + 1.2f, currentFillWidth + 2.5f, h - 7.98f, 0.f, 0.f, fillU, 1.0f);
            }

            //RenderBitmap(BITMAP_HP_BAR_FRAME, x, y, w + 8, h, 0.f, 0.f, 1.0f, 1.0f);

            g_pRenderText->SetFont(g_hFont);
            g_pRenderText->SetTextColor(0, 255, 0, 255);
            g_pRenderText->SetBgColor(0, 0, 0, 0);
            sprintf(Text, "%d%%", lpHealthBar->rate);
            g_pRenderText->RenderText(PosX + NormalTextPercentX, PosY + NormalTextPercentY, Text, 0, 0, RT3_SORT_CENTER, 0);

            g_pRenderText->SetFont(g_hFont);
            g_pRenderText->SetTextColor(255, 255, 255, 255);
            g_pRenderText->SetBgColor(0, 0, 0, 0);
            sprintf(Text, "%s", c->ID);
            g_pRenderText->RenderText(PosX + NormalTextNameX, PosY + NormalTextNameY, Text, 0, 0, RT3_SORT_CENTER, 0);

            DisableAlphaBlend();
            continue;
        }

        // ====================== JOGADOR (Hero) ======================
        if (c->Object.Kind == KIND_PLAYER)
        {
            DWORD wLife = CharacterAttribute->Life;
            DWORD wLifeMax = CharacterAttribute->LifeMax;

            if (gCharacterManager.IsMasterLevelExpCheck(Hero->Class) == true)
            {
                wLifeMax = Master_Level_Data.wMaxLife;
                wLife = min(max(0, CharacterAttribute->Life), wLifeMax);
            }

            BYTE hpRate = 0;
            if (wLifeMax > 0)
            {
                hpRate = (BYTE)((wLife * 100) / wLifeMax);
            }

            // ====================== OUTROS JOGADORES ======================
            if (c->Object.Kind == KIND_PLAYER && c != Hero)
            {
                lpHealthBar = this->GetNewHealthBar(c->Key, c->Object.Kind);
                if (lpHealthBar == 0) continue;

                BYTE hpRate = lpHealthBar->rate;

                Vector(o->Position[0], o->Position[1], o->Position[2] + o->BoundingBoxMax[2] + PlayerYOffset3D, Angle);
                Projection(Angle, &PosX, &PosY);

                float x = (float)PosX - (PlayerWidth / 2.0f);
                float y = (float)PosY;
                float w = PlayerWidth;
                float h = PlayerHeight;

                EnableAlphaTest(true);

                RenderBitmap(BITMAP_HP_BAR_BG, x + 3, y + 1, w + 2, h - 8, 0.f, 0.f, 1.0f, 1.0f);

                float currentFillWidth = (w * hpRate) / 100.0f;
                if (currentFillWidth > 0)
                {
                    float fillU = currentFillWidth / w;
                    RenderBitmap(BITMAP_HP_BAR_FILL, x + 3.f, y + 1.2f, currentFillWidth+2.5f, h - 7.98f, 0.f, 0.f, fillU, 1.0f);
                }

                //RenderBitmap(BITMAP_HP_BAR_FRAME, x, y, w + 8, h, 0.f, 0.f, 1.0f, 1.0f);

                //g_pRenderText->SetFont(g_hFont);
                //g_pRenderText->SetTextColor(0, 255, 0, 255);
                //g_pRenderText->SetBgColor(0, 0, 0, 0);
                //sprintf(Text, "%d%%", hpRate);
                //g_pRenderText->RenderText(PosX + PlayerTextPercentX, PosY + PlayerTextPercentY, Text, 0, 0, RT3_SORT_CENTER, 0);
                //
                //g_pRenderText->SetFont(g_hFont);
                //g_pRenderText->SetTextColor(255, 255, 255, 255);
                //g_pRenderText->SetBgColor(0, 0, 0, 0);
                //sprintf(Text, "%s", c->ID);
                //g_pRenderText->RenderText(PosX + PlayerTextNameX, PosY + PlayerTextNameY, Text, 0, 0, RT3_SORT_CENTER, 0);

                DisableAlphaBlend();
            }
        }
    }

    DisableAlphaBlend();
    glColor3f(1.f, 1.f, 1.f);
}
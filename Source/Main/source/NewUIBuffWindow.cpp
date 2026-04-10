#include "stdafx.h"
#include "NewUIBuffWindow.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "ZzzTexture.h"
#include "ZzzInventory.h"
#include "UIControls.h"
#include "NewUICommonMessageBox.h"
#include "BuffIcon.h"

using namespace SEASON3B;

bool gBuffCollapsed = false;
DWORD LastToggleTime = 0;

namespace
{
    const float BUFF_IMG_WIDTH = 15.0f;
    const float BUFF_IMG_HEIGHT = 20.0f;
    const int BUFF_MAX_LINE_COUNT = 8;
    const int BUFF_IMG_SPACE = 1;
    const float EXPAND_ICON_WIDTH = 15.0f;
    const float EXPAND_ICON_HEIGHT = 15.0f;
    const float COLLAPSE_ICON_WIDTH = 15.0f;
    const float COLLAPSE_ICON_HEIGHT = 15.0f;
    const float COLLAPSE_OVERLAP_OFFSET = 5.0f;
    const float ARROW_HIT_WIDTH = 25.0f;
    const float ARROW_HIT_HEIGHT = 25.0f;
    const float ARROW_HIT_OFFSET_X = -5.0f;
    const float ARROW_HIT_OFFSET_Y = -5.0f;
};

bool IsMousePressed()
{
    static bool previousLButtonState = false;
    bool currentLButtonState = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    bool result = currentLButtonState && !previousLButtonState;
    previousLButtonState = currentLButtonState;
    return result;
}

SEASON3B::CNewUIBuffWindow::CNewUIBuffWindow()
{
    m_pNewUIMng = NULL;
    m_Pos.x = m_Pos.y = 0;
}

SEASON3B::CNewUIBuffWindow::~CNewUIBuffWindow()
{
    Release();
}

bool SEASON3B::CNewUIBuffWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
    if (NULL == pNewUIMng)
        return false;

    m_pNewUIMng = pNewUIMng;
    m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_BUFF_WINDOW, this);

    LoadPositionFromIni();

    m_MoveWindow.Create(m_Pos.x + 63, m_Pos.y + 455, 300, 30);

    LoadImages();
    Show(true);
    return true;
}

void SEASON3B::CNewUIBuffWindow::Release()
{
    UnloadImages();

    if (m_pNewUIMng)
    {
        m_pNewUIMng->RemoveUIObj(this);
        m_pNewUIMng = NULL;
    }
}

void SEASON3B::CNewUIBuffWindow::SetPos(int x, int y)
{
    m_Pos.x = x;
    m_Pos.y = y - 80;
    m_MoveWindow.Create(m_Pos.x + 63, m_Pos.y + 455, 300, 30);
}

void SEASON3B::CNewUIBuffWindow::SetPos(int iScreenWidth)
{
    if (iScreenWidth == 640) {
        SetPos(220, 15);
    }
    else if (iScreenWidth == 450) {
        SetPos(125, 15);
    }
    else if (iScreenWidth == 373) {
        SetPos(86, 15);
    }
    else if (iScreenWidth == 260) {
        SetPos(30, 15);
    }
    else {
        SetPos(220, 15);
    }
}

void SEASON3B::CNewUIBuffWindow::BuffSort(std::list<eBuffState>& buffstate)
{
    OBJECT* pHeroObject = &Hero->Object;
    int iBuffSize = g_CharacterBuffSize(pHeroObject);

    for (int i = 0; i < iBuffSize; ++i)
    {
        eBuffState eBuffType = g_CharacterBuff(pHeroObject, i);
        if (SetDisableRenderBuff(eBuffType)) continue;
        if (eBuffType != eBuffNone) {
            eBuffClass eBuffClassType = g_IsBuffClass(eBuffType);
            if (eBuffClassType == eBuffClass_Buff) {
                buffstate.push_front(eBuffType);
            }
            else if (eBuffClassType == eBuffClass_DeBuff) {
                buffstate.push_back(eBuffType);
            }
            else {
                assert(!"SetDisableRenderBuff");
            }
        }
    }
}

bool SEASON3B::CNewUIBuffWindow::SetDisableRenderBuff(const eBuffState& _BuffState)
{
    switch (_BuffState)
    {
#ifdef PBG_ADD_PKSYSTEM_INGAMESHOP
    case eDeBuff_MoveCommandWin:
#endif
    case eDeBuff_FlameStrikeDamage:
    case eDeBuff_GiganticStormDamage:
    case eDeBuff_LightningShockDamage:
    case eDeBuff_Discharge_Stamina:
        return true;
    default:
        return false;
    }
}

bool SEASON3B::CNewUIBuffWindow::UpdateMouseEvent()
{
    bool wasMoving = m_MoveWindow.IsMove;
    
    if (m_MoveWindow.UpdatePosition(m_Pos.x + 63, m_Pos.y + 455, 0))
    {
        m_Pos.x = m_MoveWindow.m_PosTemp.x - 63;
        m_Pos.y = m_MoveWindow.m_PosTemp.y - 455;
    
        SavePositionToIni();
    
        return false;
    }
    
    if (wasMoving && !m_MoveWindow.IsMove)
    {
        SavePositionToIni();
    }

    float frameX = 63.0f;
    float frameY = 470.0f;
    std::list<eBuffState> buffstate;
    BuffSort(buffstate);
    int totalBuffs = buffstate.size();
    float spacingX = gBuffCollapsed ? COLLAPSE_OVERLAP_OFFSET : (BUFF_IMG_WIDTH + BUFF_IMG_SPACE);
    float overlapOffset = gBuffCollapsed ? -COLLAPSE_OVERLAP_OFFSET : 0.0f;
    float lastBuffX = 0.0f, lastBuffY = 0.0f;
    int IndexTime = 0;
    for (auto iter = buffstate.begin(); iter != buffstate.end(); ++iter, ++IndexTime)
    {
        eBuffState buff = *iter;
        float x = m_Pos.x + frameX + (IndexTime * spacingX) + (IndexTime * overlapOffset);
        float y = m_Pos.y + frameY;
        if (SEASON3B::CheckMouseIn(x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT))
        {
            if (buff == eBuff_InfinityArrow && SEASON3B::IsRelease(VK_RBUTTON))
                SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CInfinityArrowCancelMsgBoxLayout));
            else if (buff == eBuff_SwellOfMagicPower && SEASON3B::IsRelease(VK_RBUTTON))
                SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CBuffSwellOfMPCancelMsgBoxLayOut));
            return false;
        }
        lastBuffX = x;
        lastBuffY = y;
    }
    if (totalBuffs > 1)
    {
        float arrowX, arrowY;

        if (gBuffCollapsed)
        {
            arrowX = m_Pos.x + frameX + BUFF_IMG_WIDTH + 7.0f;
            arrowY = m_Pos.y + frameY - 5.0f;
        }
        else
        {
            arrowX = m_Pos.x + frameX + ((totalBuffs - 1) * (BUFF_IMG_WIDTH + BUFF_IMG_SPACE)) + BUFF_IMG_WIDTH + 1.0f;
            arrowY = m_Pos.y + frameY - 5.0f;
        }

        if (SEASON3B::CheckMouseIn(arrowX + ARROW_HIT_OFFSET_X, arrowY + ARROW_HIT_OFFSET_Y, ARROW_HIT_WIDTH, ARROW_HIT_HEIGHT))
        {
            if (SEASON3B::IsRelease(VK_LBUTTON) && GetTickCount() - LastToggleTime > 500)
            {
                gBuffCollapsed = !gBuffCollapsed;
                LastToggleTime = GetTickCount();
            }
            return false;
        }
    }
    return true;
}

bool SEASON3B::CNewUIBuffWindow::UpdateKeyEvent()
{
    return true;
}

bool SEASON3B::CNewUIBuffWindow::Update()
{
    if (!gBuffCollapsed)
    {
        if (GetTickCount() - LastToggleTime > 15000)
        {
            gBuffCollapsed = true;
            LastToggleTime = GetTickCount();
        }
    }

    return true;
}

bool SEASON3B::CNewUIBuffWindow::Render()
{
    EnableAlphaTest();
    glColor4f(1.f, 1.f, 1.f, 1.f);

    RenderBuffStatus(BUFF_RENDER_ICON);
    RenderBuffStatus(BUFF_RENDER_TOOLTIP);

    DisableAlphaBlend();
    return true;
}

void SEASON3B::CNewUIBuffWindow::RenderBuffStatus(BUFF_RENDER renderstate)
{
    OBJECT* pHeroObject = &Hero->Object;
    float frameX = 63;
    float frameY = 470;
    float x = 0.0f, y = 0.0f;
    int buffwidthcount = 0;
    std::list<eBuffState> buffstate;
    BuffSort(buffstate);

    int totalBuffs = buffstate.size();
    int visibleBuffs = totalBuffs;
    std::list<eBuffState>::iterator iter;
    int IndexTime = 0;
    float spacingX = gBuffCollapsed ? COLLAPSE_OVERLAP_OFFSET : (BUFF_IMG_WIDTH + BUFF_IMG_SPACE);
    float lastBuffX = 0.0f, lastBuffY = 0.0f;
    float overlapOffset = gBuffCollapsed ? -COLLAPSE_OVERLAP_OFFSET : 0.0f;

    for (iter = buffstate.begin(); iter != buffstate.end(); )
    {
        if (IndexTime >= visibleBuffs)
            break;

        std::list<eBuffState>::iterator tempiter = iter;
        ++iter;
        eBuffState buff = (*tempiter);

        if (gBuffCollapsed)
        {
            x = m_Pos.x + frameX + (IndexTime * COLLAPSE_OVERLAP_OFFSET);
            y = m_Pos.y + frameY;
        }
        else
        {
            x = m_Pos.x + (buffwidthcount * (BUFF_IMG_WIDTH + BUFF_IMG_SPACE)) + frameX;
            y = m_Pos.y + frameY;
        }


        if (renderstate == BUFF_RENDER_ICON)
        {
            float customWidth = BUFF_IMG_WIDTH;
            float customHeight = BUFF_IMG_HEIGHT;
            RenderBuffIcon(buff, x, y, customWidth, customHeight);

            if (IndexTime < 100 && gIconBuff.UserBuff[IndexTime].Index != 0)
            {
                if (gIconBuff.UserBuff[IndexTime].Index != 28)
                {
                    int pBuffHp = 100;
                    if (gIconBuff.UserBuff[IndexTime].MaxTime > 0 && gIconBuff.UserBuff[IndexTime].Time > 0)
                    {
                        pBuffHp = (gIconBuff.UserBuff[IndexTime].Time * 100) / gIconBuff.UserBuff[IndexTime].MaxTime;
                    }
                    EnableAlphaTest();
                    glColor4f(0.0, 1.0, 0.0, 1.0);
                    RenderColor(x, (y + customHeight), (customWidth * pBuffHp / 100.0f), 2.0f, 0.0f, 0);
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                    EnableAlphaTest();
                }
            }
        }
        else if (renderstate == BUFF_RENDER_TOOLTIP)
        {
            if (!gBuffCollapsed && SEASON3B::CheckMouseIn(x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT))
            {
                float fTooltip_x = x + (BUFF_IMG_WIDTH / 2);
                float fTooltip_y = y + (BUFF_IMG_HEIGHT / 2) - 50;
                eBuffClass buffclass = g_IsBuffClass(buff);
                RenderBuffTooltip(buffclass, buff, fTooltip_x, fTooltip_y, IndexTime);
            }
        }

        lastBuffX = x;
        lastBuffY = y;
        IndexTime++;
        ++buffwidthcount;
    }

    if (totalBuffs > 1)
    {
        // Usa exatamente a última posição calculada no loop principal (lastBuffX / lastBuffY)
        // Isso mantém a posição original do jogo e funciona nos dois modos
        float arrowX = lastBuffX + 7.0f;
        float arrowY = lastBuffY - 5.0f;

        if (renderstate == BUFF_RENDER_ICON)
        {
            EnableAlphaTest();
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

            if (gBuffCollapsed)
            {
                RenderImage(IMAGE_BUFF_EXPAND, arrowX, arrowY, EXPAND_ICON_WIDTH, EXPAND_ICON_HEIGHT, 0.0f, 0.0f, 0.65f, 0.85f);
            }
            else
            {
                RenderImage(IMAGE_BUFF_COLLAPSE, arrowX, arrowY, COLLAPSE_ICON_WIDTH, COLLAPSE_ICON_HEIGHT, 0.0f, 0.0f, 0.65f, 0.85f);
            }

            DisableAlphaBlend();
        }
    }
}

void SEASON3B::CNewUIBuffWindow::RenderBuffIcon(eBuffState& eBuffType, float x, float y, float width, float height)
{
    int iWidthIndex, iHeightIndex;
    float u, v, uWidth, vHeight;
    const float textureWidth = 256.0f;
    const float textureHeight = 256.0f;
    const float iconWidth = 20.0f;
    const float iconHeight = 28.0f;

    if (eBuffType < 81)
    {
        iWidthIndex = (eBuffType - 1) % 10;
        iHeightIndex = (eBuffType - 1) / 10;
        u = iWidthIndex * iconWidth / textureWidth;
        v = iHeightIndex * iconHeight / textureHeight;
        uWidth = (width / textureWidth) * (iconWidth / width);
        vHeight = (height / textureHeight) * (iconHeight / height);
        RenderBitmap(IMAGE_BUFF_STATUS, x, y, width, height, u, v, uWidth, vHeight);
    }
    else
    {
        iWidthIndex = (eBuffType - 81) % 10;
        iHeightIndex = (eBuffType - 81) / 10;
        u = iWidthIndex * iconWidth / textureWidth;
        v = iHeightIndex * iconHeight / textureHeight;
        uWidth = (width / textureWidth) * (iconWidth / width);
        vHeight = (height / textureHeight) * (iconHeight / height);
        RenderBitmap(IMAGE_BUFF_STATUS2, x, y, width, height, u, v, uWidth, vHeight);
    }
}

void SEASON3B::CNewUIBuffWindow::RenderBuffTooltip(eBuffClass& eBuffClassType, eBuffState& eBuffType, float x, float y)
{
    int TextNum = 0;
    ::memset(TextList[0], 0, sizeof(char) * 30 * 100);
    ::memset(TextListColor, 0, sizeof(int) * 30);
    ::memset(TextBold, 0, sizeof(int) * 30);

    std::list<std::string> tooltipinfo;
    g_BuffToolTipString(tooltipinfo, eBuffType);

    for (std::list<std::string>::iterator iter = tooltipinfo.begin(); iter != tooltipinfo.end(); ++iter)
    {
        std::string& temp = *iter;
        unicode::_sprintf(TextList[TextNum], temp.c_str());
        if (TextNum == 0)
        {
            TextListColor[TextNum] = TEXT_COLOR_BLUE;
            TextBold[TextNum] = true;
        }
        else
        {
            TextListColor[TextNum] = TEXT_COLOR_WHITE;
            TextBold[TextNum] = false;
        }
        TextNum += 1;
    }

    std::string bufftime;
    g_BuffStringTime(eBuffType, bufftime);
    if (bufftime.size() != 0)
    {
        unicode::_sprintf(TextList[TextNum], GlobalText[2533], bufftime.c_str());
        TextListColor[TextNum] = TEXT_COLOR_PURPLE;
        TextBold[TextNum] = false;
        TextNum += 1;
    }

    SIZE TextSize = { 0, 0 };
    g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), TextList[0], 1, &TextSize);
    RenderTipTextList(x, y, TextNum, 0);
}

void SEASON3B::CNewUIBuffWindow::RenderBuffTooltip(eBuffClass& eBuffClassType, eBuffState& eBuffType, float x, float y, int IndexTime)
{
    RenderBuffTooltip(eBuffClassType, eBuffType, x, y);
}

float SEASON3B::CNewUIBuffWindow::GetLayerDepth()
{
    return 0.95f;
}

void SEASON3B::CNewUIBuffWindow::OpenningProcess()
{
}

void SEASON3B::CNewUIBuffWindow::ClosingProcess()
{
}

void SEASON3B::CNewUIBuffWindow::LoadImages()
{
    LoadBitmap("Interface\\newui_statusicon.jpg", IMAGE_BUFF_STATUS, GL_LINEAR);
    LoadBitmap("Interface\\newui_statusicon2.jpg", IMAGE_BUFF_STATUS2, GL_LINEAR);
    LoadBitmap("Interface\\Pegasus_BUFFICON_01.tga", IMAGE_BUFF_EXPAND, GL_LINEAR);
    LoadBitmap("Interface\\Pegasus_BUFFICON.tga", IMAGE_BUFF_COLLAPSE, GL_LINEAR);
}

void SEASON3B::CNewUIBuffWindow::UnloadImages()
{
    DeleteBitmap(IMAGE_BUFF_STATUS);
    DeleteBitmap(IMAGE_BUFF_STATUS2);
    DeleteBitmap(IMAGE_BUFF_EXPAND);
    DeleteBitmap(IMAGE_BUFF_COLLAPSE);
}

void SEASON3B::CNewUIBuffWindow::SavePositionToIni()
{
    char szIniFilePath[256 + 20] = "";
    char szCurrentDir[256];
    GetCurrentDirectory(256, szCurrentDir);
    strcpy(szIniFilePath, szCurrentDir);

    if (szCurrentDir[strlen(szCurrentDir) - 1] == '\\')
        strcat(szIniFilePath, "config.ini");
    else
        strcat(szIniFilePath, "\\Data\\Custom\\config.ini");

    char szPosX[32], szPosY[32];
    sprintf(szPosX, "%d", (int)m_Pos.x);
    sprintf(szPosY, "%d", (int)m_Pos.y);

    WritePrivateProfileString("BuffIcon", "PositionX", szPosX, szIniFilePath);
    WritePrivateProfileString("BuffIcon", "PositionY", szPosY, szIniFilePath);
}

void SEASON3B::CNewUIBuffWindow::LoadPositionFromIni()
{
    char szIniFilePath[256 + 20] = "";
    char szCurrentDir[256];
    GetCurrentDirectory(256, szCurrentDir);
    strcpy(szIniFilePath, szCurrentDir);

    if (szCurrentDir[strlen(szCurrentDir) - 1] == '\\')
        strcat(szIniFilePath, "config.ini");
    else
        strcat(szIniFilePath, "\\Data\\Custom\\config.ini");

    int defaultX = 220;
    int defaultY = 15 - 80;

    int savedX = GetPrivateProfileInt("BuffIcon", "PositionX", defaultX, szIniFilePath);
    int savedY = GetPrivateProfileInt("BuffIcon", "PositionY", defaultY, szIniFilePath);

    m_Pos.x = (float)savedX;
    m_Pos.y = (float)savedY;
}
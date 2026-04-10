// NewUIEnterDevilSquare.cpp: implementation of the CNewUIPartyInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIEnterDevilSquare.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "NewUICommonMessageBox.h"
#include "CharacterManager.h"
#include "EventEntryLevel.h"

using namespace SEASON3B;

CNewUIEnterDevilSquare::CNewUIEnterDevilSquare()
{
	m_pNewUIMng = NULL;
	memset(&m_Pos, 0, sizeof(POINT));
	memset(&m_EnterUITextPos, 0, sizeof(POINT));

	m_iNumActiveBtn = 1;
	m_BtnEnterStartPos.x = m_BtnEnterStartPos.y = 0;
	m_dwBtnTextColor[0] = RGBA(150, 150, 150, 255);
	m_dwBtnTextColor[1] = RGBA(255, 255, 255, 255);

	m_iDevilSquareLimitLevel[0][0] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[0][0]; m_iDevilSquareLimitLevel[0][1] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[0][1];
	m_iDevilSquareLimitLevel[1][0] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[1][0]; m_iDevilSquareLimitLevel[1][1] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[1][1];
	m_iDevilSquareLimitLevel[2][0] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[2][0]; m_iDevilSquareLimitLevel[2][1] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[2][1];
	m_iDevilSquareLimitLevel[3][0] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[3][0]; m_iDevilSquareLimitLevel[3][1] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[3][1];
	m_iDevilSquareLimitLevel[4][0] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[4][0]; m_iDevilSquareLimitLevel[4][1] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[4][1];
	m_iDevilSquareLimitLevel[5][0] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[5][0]; m_iDevilSquareLimitLevel[5][1] = gEventEntryLevel.m_DevilSquareEntryLevelCommon[5][1];
	m_iDevilSquareLimitLevel[6][0] = 0; m_iDevilSquareLimitLevel[6][1] = 0;

	m_iDevilSquareLimitLevel[7][0] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[0][0]; m_iDevilSquareLimitLevel[7][1] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[0][1];
	m_iDevilSquareLimitLevel[8][0] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[1][0]; m_iDevilSquareLimitLevel[8][1] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[1][1];
	m_iDevilSquareLimitLevel[9][0] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[2][0]; m_iDevilSquareLimitLevel[9][1] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[2][1];
	m_iDevilSquareLimitLevel[10][0] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[3][0]; m_iDevilSquareLimitLevel[10][1] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[3][1];
	m_iDevilSquareLimitLevel[11][0] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[4][0]; m_iDevilSquareLimitLevel[11][1] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[4][1];
	m_iDevilSquareLimitLevel[12][0] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[5][0]; m_iDevilSquareLimitLevel[12][1] = gEventEntryLevel.m_DevilSquareEntryLevelSpecial[5][1];
	m_iDevilSquareLimitLevel[13][0] = 0; m_iDevilSquareLimitLevel[13][1] = 0;
}

CNewUIEnterDevilSquare::~CNewUIEnterDevilSquare()
{
	Release();
}

bool CNewUIEnterDevilSquare::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_DEVILSQUARE, this);

	SetPos(x, y);

	LoadImages();

	// Exit Button
	m_BtnExit.ChangeButtonImgState(true, IMAGE_ENTERDS_BASE_WINDOW_BTN_EXIT, false);
	m_BtnExit.ChangeButtonInfo(m_Pos.x + 13, m_Pos.y + 398, 36, 29);
	m_BtnExit.ChangeToolTipText(GlobalText[1002], true);

	// Enter Button
	int iVal = 0;
	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		iVal = ENTER_BTN_VAL * i;
		m_BtnEnter[i].ChangeButtonImgState(true, IMAGE_ENTERDS_BASE_WINDOW_BTN_ENTER, true);
		m_BtnEnter[i].ChangeButtonInfo(m_BtnEnterStartPos.x, m_BtnEnterStartPos.y + iVal, 180, 29);
	}

	Show(false);

	return true;
}

void CNewUIEnterDevilSquare::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CNewUIEnterDevilSquare::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_EnterUITextPos.x = m_Pos.x + 3;
	m_EnterUITextPos.y = m_Pos.y + 45;

	SetBtnPos(m_Pos.x + 6, m_Pos.y + 155);

	m_BtnExit.ChangeButtonInfo(m_Pos.x + 13, m_Pos.y + 398, 36, 29);

	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		int iVal = ENTER_BTN_VAL * i;
		m_BtnEnter[i].ChangeButtonInfo(m_BtnEnterStartPos.x, m_BtnEnterStartPos.y + iVal, 180, 29);
	}
}

void CNewUIEnterDevilSquare::SetBtnPos(int x, int y)
{
	m_BtnEnterStartPos.x = x;
	m_BtnEnterStartPos.y = y;
}

bool CNewUIEnterDevilSquare::UpdateMouseEvent()
{
	if (true == BtnProcess())
		return false;

	if (CheckMouseIn(m_Pos.x, m_Pos.y, ENTERDS_BASE_WINDOW_WIDTH, ENTERDS_BASE_WINDOW_HEIGHT))
		return false;

	return true;
}

bool CNewUIEnterDevilSquare::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DEVILSQUARE) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_DEVILSQUARE);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

int CNewUIEnterDevilSquare::CheckLimitLV(int iIndex)
{
	int	iVal = 0;
	int iRet = 0;

	if (iIndex == 1)
	{
		iVal = 7;
	}

	int iLevel = CharacterAttribute->ViewResets;

	if (gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == false)
	{
		for (int iSquareLV = 0; iSquareLV < MAX_ENTER_GRADE - 1; ++iSquareLV)
		{
			if (iLevel >= m_iDevilSquareLimitLevel[iVal + iSquareLV][0]
				&& iLevel <= m_iDevilSquareLimitLevel[iVal + iSquareLV][1])
			{
				iRet = iSquareLV;
				break;
			}
		}
	}
	else
		iRet = MAX_ENTER_GRADE - 1;


	return iRet;
}

bool CNewUIEnterDevilSquare::Update()
{
	if (!IsVisible())
		return true;

	return true;
}

bool CNewUIEnterDevilSquare::Render()
{
	EnableAlphaTest();

	RenderImage(IMAGE_ENTERDS_BASE_WINDOW_BACK, m_Pos.x, m_Pos.y, float(ENTERDS_BASE_WINDOW_WIDTH), float(ENTERDS_BASE_WINDOW_HEIGHT));
	RenderImage(IMAGE_ENTERDS_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, float(ENTERDS_BASE_WINDOW_WIDTH), 64.f);
	RenderImage(IMAGE_ENTERDS_BASE_WINDOW_LEFT, m_Pos.x, m_Pos.y + 64.f, 21.f, float(ENTERDS_BASE_WINDOW_HEIGHT) - 64.f - 45.f);
	RenderImage(IMAGE_ENTERDS_BASE_WINDOW_RIGHT, m_Pos.x + float(ENTERDS_BASE_WINDOW_WIDTH) - 21.f, m_Pos.y + 64.f, 21.f, float(ENTERDS_BASE_WINDOW_HEIGHT) - 64.f - 45.f);
	RenderImage(IMAGE_ENTERDS_BASE_WINDOW_BOTTOM, m_Pos.x, m_Pos.y + float(ENTERDS_BASE_WINDOW_HEIGHT) - 45.f, float(ENTERDS_BASE_WINDOW_WIDTH), 45.f);


	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(0xFFFFFFFF);
	g_pRenderText->SetBgColor(0x00000000);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 12, GlobalText[39], 72, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFont);

	char txtline[NUM_LINE_CMB][MAX_LENGTH_CMB];
	int tl = SeparateTextIntoLines(GlobalText[832], txtline[0], NUM_LINE_CMB, MAX_LENGTH_CMB);
	for (int j = 0; j < tl; ++j)
	{
		g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y + j * 20, txtline[j], 190, 0, RT3_SORT_CENTER);
	}

	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		m_BtnEnter[i].Render();
	}

	// Exit Button
	m_BtnExit.Render();

	DisableAlphaBlend();

	return true;
}

//---------------------------------------------------------------------------------------------
// BtnProcess
bool CNewUIEnterDevilSquare::BtnProcess()
{
	POINT ptExitBtn1 = { m_Pos.x + 169, m_Pos.y + 7 };

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_DEVILSQUARE);
		return true;
	}

	if (m_BtnExit.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_DEVILSQUARE);
		return true;
	}

	if ((m_iNumActiveBtn != -1) && (m_BtnEnter[m_iNumActiveBtn].UpdateMouseEvent() == true))
	{
		int iItemIndex = -1;

		iItemIndex = g_pMyInventory->FindItemIndex(ITEM_POTION + 19, 0);

		if (iItemIndex == -1)
		{
			iItemIndex = g_pMyInventory->FindItemIndex(ITEM_POTION + 19, m_iNumActiveBtn + 1);
		}

		if (iItemIndex == -1)
		{
			iItemIndex = g_pMyInventory->FindItemIndex(ITEM_HELPER + 46, -1);
		}

		if (iItemIndex > -1)
		{
			SendRequestMoveToDevilSquare(m_iNumActiveBtn, MAX_EQUIPMENT + iItemIndex);
		}
		else
		{
			SEASON3B::CreateOkMessageBox(GlobalText[677]);
		}
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_DEVILSQUARE);

	}

	return false;
}

float CNewUIEnterDevilSquare::GetLayerDepth()
{
	return 4.0f;
}

void CNewUIEnterDevilSquare::OpenningProcess()
{
	SendExitInventory();

	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		m_BtnEnter[i].ChangeTextColor(m_dwBtnTextColor[ENTERBTN_DISABLE]);
		m_BtnEnter[i].Lock();
	}

	int iLimitLVIndex = 0;
	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK || gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD
#ifdef PBG_ADD_NEWCHAR_MONK
		|| GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER
#endif //PBG_ADD_NEWCHAR_MONK
		)
	{
		iLimitLVIndex = 1;
	}

	m_iNumActiveBtn = CheckLimitLV(iLimitLVIndex);

	m_BtnEnter[m_iNumActiveBtn].UnLock();
	m_BtnEnter[m_iNumActiveBtn].ChangeTextColor(m_dwBtnTextColor[ENTERBTN_ENABLE]);

	unicode::t_char sztext[255] = { 0, };

	for (int i = 0; i < MAX_ENTER_GRADE - 1; i++)
	{
		unicode::_sprintf(sztext, GlobalText[645], i + 1
			, m_iDevilSquareLimitLevel[(iLimitLVIndex * (MAX_ENTER_GRADE)) + i][0]
			, m_iDevilSquareLimitLevel[(iLimitLVIndex * (MAX_ENTER_GRADE)) + i][1]);
		m_BtnEnter[i].SetFont(g_hFontBold);
		m_BtnEnter[i].ChangeText(sztext);
	}

	unicode::_sprintf(sztext, GlobalText[1778], 7);
	m_BtnEnter[MAX_ENTER_GRADE - 1].SetFont(g_hFontBold);
	m_BtnEnter[MAX_ENTER_GRADE - 1].ChangeText(sztext);
}

void CNewUIEnterDevilSquare::ClosingProcess()
{
	SendExitInventory();
}

void CNewUIEnterDevilSquare::LoadImages()
{
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_ENTERDS_BASE_WINDOW_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back01.tga", IMAGE_ENTERDS_BASE_WINDOW_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_ENTERDS_BASE_WINDOW_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_ENTERDS_BASE_WINDOW_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_ENTERDS_BASE_WINDOW_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\newui_exit_00.tga", IMAGE_ENTERDS_BASE_WINDOW_BTN_EXIT, GL_LINEAR);				// Exit Button
	LoadBitmap("Interface\\newui_btn_empty_big.tga", IMAGE_ENTERDS_BASE_WINDOW_BTN_ENTER, GL_LINEAR);		// Enter Button
}

//---------------------------------------------------------------------------------------------
// UnloadImages
void CNewUIEnterDevilSquare::UnloadImages()
{
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_BACK);
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_TOP);
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_LEFT);
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_RIGHT);
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_BOTTOM);
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_BTN_EXIT);		// Exit Button
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_BTN_ENTER);	// Enter Button
}
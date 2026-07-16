// NewUIHeroPositionInfo.cpp: implementation of the CNewUIHeroPositionInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIHeroPositionInfo.h"
#include "NewUISystem.h"
#include "CAIController.h"
#include "wsclientinline.h"
#include "MapManager.h"
#include <Winmain.h>
#include <ServerListManager.h>

using namespace SEASON3B;

CNewUIHeroPositionInfo::CNewUIHeroPositionInfo()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_CurHeroPosition.x = m_CurHeroPosition.y = 0;
	m_LastHelperAlertLevel = 0;
}

CNewUIHeroPositionInfo::~CNewUIHeroPositionInfo()
{
	Release();
}

//---------------------------------------------------------------------------------------------
// Create
bool CNewUIHeroPositionInfo::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj( SEASON3B::INTERFACE_HERO_POSITION_INFO, this );

	#ifdef GENS_ACTIVATE
	WidenX = HERO_POSITION_INFO_BASEB_WINDOW_WIDTH;
	if(WindowWidth == 800)
	{
		WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.4f));
	}
	else
	if(WindowWidth == 1024)
	{
		WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.2f));
	}
	#endif

	SetPos(x, y);
	LoadImages();
	if (IsMuHelperEnabled())
	{
		SetHelperButtonInfo();
	}
	Show( true );
	return true;
}

void CNewUIHeroPositionInfo::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void CNewUIHeroPositionInfo::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool CNewUIHeroPositionInfo::BtnProcess()
{
	if (!IsMuHelperEnabled())
	{
		return false;
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADE) ||
		g_pNewUISystem->IsImpossibleTradeInterface())
	{
		return false;
	}

	if (m_BtnSetup.UpdateMouseEvent())
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MACRO_OFICIAL);
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MACRO_OFICIAL))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_MACRO_OFICIAL_SUB);
		}
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (!gmAIController->IsRunning())
	{
		if (m_BtnStart.UpdateMouseEvent())
		{
			if (gmAIController->CanUseAIController())
			{
				SendRequestStartHelper(false);
			}
			return true;
		}
	}
	else if (m_BtnPause.UpdateMouseEvent())
	{
		SendRequestStartHelper(true);
		return true;
	}

	return false;
}

bool CNewUIHeroPositionInfo::UpdateMouseEvent()
{
	if( true == BtnProcess() )
		return false;

	if (IsMuHelperEnabled())
	{
		const int width = MUHELPER_POSITION_A_WIDTH + MUHELPER_POSITION_B_WIDTH + MUHELPER_POSITION_C_WIDTH;
		if (CheckMouseIn(m_Pos.x, m_Pos.y, width, MUHELPER_POSITION_HEIGHT))
			return false;
	}
	else if (CheckMouseIn(m_Pos.x, m_Pos.y, HERO_POSITION_INFO_BASE_WINDOW_WIDTH, HERO_POSITION_INFO_BASE_WINDOW_HEIGHT))
	{
		return false;
	}
	
	return true;
}

bool CNewUIHeroPositionInfo::UpdateKeyEvent()
{
	if (!IsMuHelperEnabled())
	{
		return true;
	}

	if (SEASON3B::IsPress(VK_HOME) && GetFocus() == g_hWnd)
	{
		if (gmAIController->IsRunning())
			SendRequestStartHelper(true);
		else if (gmAIController->CanUseAIController())
			SendRequestStartHelper(false);
		return false;
	}

	if (SEASON3B::IsPress('Z'))
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MACRO_OFICIAL);
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MACRO_OFICIAL_SUB))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_MACRO_OFICIAL_SUB);
		}
		return false;
	}

	return true;
}

bool CNewUIHeroPositionInfo::Update()
{
	if( (IsVisible() == true) && (Hero != NULL) )
	{
		m_CurHeroPosition.x = ( Hero->PositionX );
		m_CurHeroPosition.y = ( Hero->PositionY );

		// Reproduce the reference level alert without altering the legacy
		// ANSI level-up packet parser. The first observed level is cached so
		// reconnecting at the activation level does not show a false alert.
		if (IsMuHelperEnabled() && CharacterAttribute != NULL)
		{
			const int currentLevel = CharacterAttribute->Level;
			if (m_LastHelperAlertLevel > 0 &&
				currentLevel != m_LastHelperAlertLevel &&
				currentLevel == gProtect->m_MainInfo.m_HelperActiveLevel &&
				gProtect->m_MainInfo.m_HelperActiveAlert != 0)
			{
				SEASON3B::CreateOkMessageBox(GlobalText[3606], -1);
			}
			m_LastHelperAlertLevel = currentLevel;
		}
	}

	return true;
}

void ShowInfoTitleWindow()
{
	bool condition = true;

	int ServerCode = CServerListManager::GetInstance()->GetSelectServerIndex();

	std::string windowName = gProtect->m_MainInfo.WindowName;

	//if (ServerCode >= 0 && ServerCode <= 19)
	//{
	//	windowName += " [Easy Server]";
	//}
	//else if (ServerCode >= 20 && ServerCode <= 39)
	//{
	//	windowName += " [Hard Server]";
	//}

	if (Hero)
	{
		windowName += " || Personagem: " + std::string(CharacterAttribute->Name) + " ||";
		windowName += " Level: " + std::to_string(CharacterAttribute->Level) + " ||";
		windowName += " Resets: " + std::to_string(CharacterAttribute->ViewResets) + " ";

		if (gProtect->m_MainInfo.ShowFPSWindow != 0)
		{
			windowName += "|| FPS: " + std::to_string((int)FPS_AVG) + " ";
		}

		if (gProtect->m_MainInfo.m_GetHWID != 0 && gProtect->m_MainInfo.m_ShowHWID != 0)
		{
			char LocalHardwareID[36] = { 0 };

			if (GetLocalComputerHardwareId(LocalHardwareID, sizeof(LocalHardwareID)))
			{
				windowName += "|| HWID: " + std::string(LocalHardwareID);
			}
		}
	}

	SetWindowText(g_hWnd, windowName.c_str());
}

bool CNewUIHeroPositionInfo::Render()
{
	unicode::t_char szText[256] = { 0 };
	ShowInfoTitleWindow();

	if (IsMuHelperEnabled())
	{
		EnableAlphaTest(true);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		RenderHelperFrame();
		RenderHelperButtons();
		DisableAlphaBlend();
		return true;
	}

	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float img_x = (float)m_Pos.x + 1.0f;
	float img_y = (float)m_Pos.y - 17.0f;
	float img_w = (float)HERO_POSITION_INFO_BASE_WINDOW_WIDTH + 135.0f;
	float img_h = (float)HERO_POSITION_INFO_BASE_WINDOW_HEIGHT + 65.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.85f);

	RenderBitmap(IMAGE_HERO_POSITION_INFO_CORD, img_x, img_y, img_w, img_h, 0.0f, 0.0f, 1.0f, 1.0f);

	glDisable(GL_BLEND);

	unicode::_sprintf(szText, "%d - %d | FPS: %.f",
		m_CurHeroPosition.x, m_CurHeroPosition.y, FPS_AVG);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 0, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->RenderText(m_Pos.x + 19, m_Pos.y + 7, szText, 75, 12, RT3_SORT_CENTER);

	DisableAlphaBlend();
	return true;
}

float CNewUIHeroPositionInfo::GetLayerDepth()
{
	return 4.3f;
}

void CNewUIHeroPositionInfo::OpenningProcess()
{
	
}

void CNewUIHeroPositionInfo::ClosingProcess()
{
	
}

void CNewUIHeroPositionInfo::SetCurHeroPosition( int x, int y )
{
	m_CurHeroPosition.x = x;
	m_CurHeroPosition.y = y;
}

void CNewUIHeroPositionInfo::LoadImages()
{
	// The legacy coordinate skin remains the fallback when MuHelperEnabled=0.
	LoadBitmap("Interface\\newui_new_btn_cord.tga", IMAGE_HERO_POSITION_INFO_CORD, GL_LINEAR);

	if (IsMuHelperEnabled())
	{
		LoadBitmap("Interface\\Minimap_positionA.tga", IMAGE_MUHELPER_POSITION_A, GL_LINEAR);
		LoadBitmap("Interface\\Minimap_positionB.tga", IMAGE_MUHELPER_POSITION_B, GL_LINEAR);
		LoadBitmap("Interface\\MacroUI\\Minimap_positionC.tga", IMAGE_MUHELPER_POSITION_C, GL_LINEAR);
		LoadBitmap("Interface\\MacroUI\\MacroUI_Setup.tga", IMAGE_MUHELPER_SETUP, GL_LINEAR);
		LoadBitmap("Interface\\MacroUI\\MacroUI_Start.tga", IMAGE_MUHELPER_START, GL_LINEAR);
		LoadBitmap("Interface\\MacroUI\\MacroUI_Stop.tga", IMAGE_MUHELPER_STOP, GL_LINEAR);
	}
}

void CNewUIHeroPositionInfo::UnloadImages()
{
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_CORD);
	if (IsMuHelperEnabled())
	{
		DeleteBitmap(IMAGE_MUHELPER_POSITION_A);
		DeleteBitmap(IMAGE_MUHELPER_POSITION_B);
		DeleteBitmap(IMAGE_MUHELPER_POSITION_C);
		DeleteBitmap(IMAGE_MUHELPER_SETUP);
		DeleteBitmap(IMAGE_MUHELPER_START);
		DeleteBitmap(IMAGE_MUHELPER_STOP);
	}
}

bool CNewUIHeroPositionInfo::IsMuHelperEnabled() const
{
	return (gProtect->m_MainInfo.m_MuHelperEnabled != 0);
}

void CNewUIHeroPositionInfo::SetHelperButtonInfo()
{
	// Match the Season 5.2 Helper button geometry and tooltips.
	m_BtnSetup.ChangeButtonImgState(1, IMAGE_MUHELPER_SETUP, true, false, true);
	m_BtnSetup.ChangeButtonInfo(m_Pos.x + 121, m_Pos.y, 18, 13);
	m_BtnSetup.ChangeToolTipText(GlobalText[3561], false);

	m_BtnStart.ChangeButtonImgState(1, IMAGE_MUHELPER_START, true, false, true);
	m_BtnStart.ChangeButtonInfo(m_Pos.x + 139, m_Pos.y, 18, 13);
	m_BtnStart.ChangeToolTipText(GlobalText[3562], false);

	m_BtnPause.ChangeButtonImgState(1, IMAGE_MUHELPER_STOP, true, false, true);
	m_BtnPause.ChangeButtonInfo(m_Pos.x + 139, m_Pos.y, 18, 13);
	m_BtnPause.ChangeToolTipText(GlobalText[3563], false);
}

void CNewUIHeroPositionInfo::RenderHelperFrame()
{
	unicode::t_char text[256] = { 0 };
	RenderImage(IMAGE_MUHELPER_POSITION_A, m_Pos.x, m_Pos.y,
		(float)MUHELPER_POSITION_A_WIDTH, (float)MUHELPER_POSITION_HEIGHT);
	RenderImage(IMAGE_MUHELPER_POSITION_B, m_Pos.x + MUHELPER_POSITION_A_WIDTH, m_Pos.y,
		(float)MUHELPER_POSITION_B_WIDTH, (float)MUHELPER_POSITION_HEIGHT,
		0.1f, 0.f, 22.4f / 32.f, 25.f / 32.f);
	RenderImage(IMAGE_MUHELPER_POSITION_C,
		m_Pos.x + MUHELPER_POSITION_A_WIDTH + MUHELPER_POSITION_B_WIDTH,
		m_Pos.y, (float)MUHELPER_POSITION_C_WIDTH, 20.f);

	unicode::_sprintf(text, "%s (%d , %d)",
		gMapManager.GetMapName(gMapManager.WorldActive),
		m_CurHeroPosition.x, m_CurHeroPosition.y);
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->RenderText(m_Pos.x + 10, m_Pos.y + 5, text, 100, 9, RT3_SORT_CENTER);
}

void CNewUIHeroPositionInfo::RenderHelperButtons()
{
	m_BtnSetup.Render();
	if (gmAIController->IsRunning())
		m_BtnPause.Render();
	else
		m_BtnStart.Render();
}



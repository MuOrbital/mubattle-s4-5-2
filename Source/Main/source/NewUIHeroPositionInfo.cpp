// NewUIHeroPositionInfo.cpp: implementation of the CNewUIHeroPositionInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIHeroPositionInfo.h"
#include "NewUISystem.h"
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
	
	return false;
}

bool CNewUIHeroPositionInfo::UpdateMouseEvent()
{
	if( true == BtnProcess() )
		return false;

	#ifdef HELPER_ACTIVATE
	int Width = HERO_POSITION_INFO_BASEA_WINDOW_WIDTH + WidenX + HERO_POSITION_INFO_BASEC_WINDOW_WIDTH;
	if (CheckMouseIn(m_Pos.x, m_Pos.y, Width, HERO_POSITION_INFO_BASE_WINDOW_HEIGHT))
		return false;
	#else
	if (CheckMouseIn(m_Pos.x, m_Pos.y, HERO_POSITION_INFO_BASE_WINDOW_WIDTH, HERO_POSITION_INFO_BASE_WINDOW_HEIGHT))
		return false;
	#endif
	
	return true;
}

bool CNewUIHeroPositionInfo::UpdateKeyEvent()
{
	return true;
}

bool CNewUIHeroPositionInfo::Update()
{
	if( (IsVisible() == true) && (Hero != NULL) )
	{
		m_CurHeroPosition.x = ( Hero->PositionX );
		m_CurHeroPosition.y = ( Hero->PositionY );
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
	#ifdef HELPER_ACTIVATE
	return 8.2f;
	#else
	return 4.3f;
	#endif
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
	// Carrega apenas a nova imagem com suporte a alpha
	LoadBitmap("Interface\\newui_new_btn_cord.tga", IMAGE_HERO_POSITION_INFO_CORD, GL_LINEAR);
}

void CNewUIHeroPositionInfo::UnloadImages()
{
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_CORD);
}



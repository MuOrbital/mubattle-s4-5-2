// NewUIOptionWindow.cpp: implementation of the CNewUIOptionWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "DSPlaySound.h"
#include "OptionWin.h"
#include "AutoClick.h"

using namespace SEASON3B;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SEASON3B::CNewUIOptionWindow::CNewUIOptionWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_NewY = 30;
	m_bAutoAttack = false;
	m_bWhisperSound = false;
	m_bSlideHelp = false;
	m_iVolumeLevel = 0;
	m_iRenderLevel = 4;
	m_EffectSprite = false;
	m_EffectParticle = false;
	m_Terrain = false;
	m_Wings = false;
	m_Swords = false;
	m_Equipments = false;
	m_Music = true;
	m_Custom = false;
	m_Window = false;
	m_Object = false;
	m_Shadows = true;
	m_WingsLv2 = false;
	m_Monsters = false;
	m_Players = false;
	m_Rotation = false;
	m_NSkillCollor = true;
	if (gProtect && gProtect->m_MainInfo.m_MountMiniSafeZone != 0)
	{
		m_MiniSafeZone = true;
	}
	else
	{
		m_MiniSafeZone = false;
	}
	m_FogSystem = false;
	m_DisableSkill = false;
	m_NShadows = false;
	m_MonsterHPBar = false;
	m_Fps30 = false;
	m_Fps60 = true;
	m_Fps120 = false;
	m_HideInterface = false;
	m_highestFps = 0.f;
	m_ActiveTab = TAB_GAME;
}

SEASON3B::CNewUIOptionWindow::~CNewUIOptionWindow()
{
	Release();
}

void SEASON3B::CNewUIOptionWindow::UpdateFpsLimitInIni(int limit)
{
	char path[MAX_PATH] = { 0 };
	strcpy(path, ".\\Data\\Custom\\config.ini");

	char strLimit[16];
	sprintf(strLimit, "%d", limit);

	WritePrivateProfileString("FPSSystem", "FpsLimit", strLimit, path);
}

int SEASON3B::CNewUIOptionWindow::ReadFpsLimitFromIni()
{
	char path[MAX_PATH] = { 0 };
	strcpy(path, ".\\Data\\Custom\\config.ini");

	return GetPrivateProfileInt("FPSSystem", "FpsLimit", 64, path);
}

void SEASON3B::CNewUIOptionWindow::SaveOptionToIni(const char* key, int value)
{
	char path[MAX_PATH] = { 0 };
	strcpy(path, ".\\Data\\Custom\\config.ini");
	char strValue[16];
	sprintf(strValue, "%d", value);
	WritePrivateProfileString("AntiLagOptions", key, strValue, path);
}

int SEASON3B::CNewUIOptionWindow::ReadOptionFromIni(const char* key, int defaultValue)
{
	char path[MAX_PATH] = { 0 };
	strcpy(path, ".\\Data\\Custom\\config.ini");
	return GetPrivateProfileInt("AntiLagOptions", key, defaultValue, path);
}

bool SEASON3B::CNewUIOptionWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_OPTION, this);

	m_MoveWindow.Create(x, y, 190, 292);
	SetPos(x + 50, y);

	LoadImages();
	SetButtonInfo();
	Show(false);

	int currentFps = ReadFpsLimitFromIni();
	m_Fps30 = (currentFps == 32);
	m_Fps60 = (currentFps == 64);
	m_Fps120 = (currentFps == 128);

	if (!m_Fps30 && !m_Fps60 && !m_Fps120)
	{
		m_Fps60 = true;
		UpdateFpsLimitInIni(64);
	}

	m_bAutoAttack = ReadOptionFromIni("AutoAttack", 1) != 0;
	m_bWhisperSound = ReadOptionFromIni("WhisperSound", 0) != 0;
	m_bSlideHelp = ReadOptionFromIni("SlideHelp", 0) != 0;
	m_Music = ReadOptionFromIni("Music", 1) != 0;
	m_EffectSprite = ReadOptionFromIni("EffectSprite", 0) != 0;
	m_EffectParticle = ReadOptionFromIni("EffectParticle", 0) != 0;
	m_Terrain = ReadOptionFromIni("Terrain", 0) != 0;
	m_Wings = ReadOptionFromIni("Wings", 0) != 0;
	m_Swords = ReadOptionFromIni("Swords", 0) != 0;
	m_Equipments = ReadOptionFromIni("Equipments", 0) != 0;
	m_Object = ReadOptionFromIni("Object", 0) != 0;
	m_Shadows = ReadOptionFromIni("Shadows", 1) != 0;
	m_WingsLv2 = ReadOptionFromIni("WingsLv2", 0) != 0;
	m_Monsters = ReadOptionFromIni("Monsters", 0) != 0;
	m_Players = ReadOptionFromIni("Players", 0) != 0;
	m_NSkillCollor = ReadOptionFromIni("NewSkill", 1) != 0;

	if (gProtect && gProtect->m_MainInfo.m_MountMiniSafeZone != 0)
	{
		m_MiniSafeZone = ReadOptionFromIni("MiniMounts", 1) != 0;
	}
	else
	{
		m_MiniSafeZone = false;
	}

	m_HideInterface = ReadOptionFromIni("HideInterface", 0) == 1;
	m_FogSystem = ReadOptionFromIni("FogSystem", 1) != 0;
	m_DisableSkill = ReadOptionFromIni("DisableSkill", 0) == 1;
	m_NShadows = ReadOptionFromIni("NShadows", 0) == 1;

	m_MonsterHPBar = ReadOptionFromIni("MonsterHPBar", 1) != 0;

	SaveOptionToIni("AutoAttack", m_bAutoAttack ? 1 : 0);
	SaveOptionToIni("WhisperSound", m_bWhisperSound ? 1 : 0);
	SaveOptionToIni("SlideHelp", m_bSlideHelp ? 1 : 0);
	SaveOptionToIni("Music", m_Music ? 1 : 0);
	SaveOptionToIni("EffectSprite", m_EffectSprite ? 1 : 0);
	SaveOptionToIni("EffectParticle", m_EffectParticle ? 1 : 0);
	SaveOptionToIni("Terrain", m_Terrain ? 1 : 0);
	SaveOptionToIni("Wings", m_Wings ? 1 : 0);
	SaveOptionToIni("Swords", m_Swords ? 1 : 0);
	SaveOptionToIni("Equipments", m_Equipments ? 1 : 0);
	SaveOptionToIni("Object", m_Object ? 1 : 0);
	SaveOptionToIni("Shadows", m_Shadows ? 1 : 0);
	SaveOptionToIni("WingsLv2", m_WingsLv2 ? 1 : 0);
	SaveOptionToIni("Monsters", m_Monsters ? 1 : 0);
	SaveOptionToIni("Players", m_Players ? 1 : 0);
	SaveOptionToIni("NewSkill", m_NSkillCollor ? 1 : 0);

	if (gProtect && gProtect->m_MainInfo.m_MountMiniSafeZone != 0)
		SaveOptionToIni("MiniMounts", m_MiniSafeZone ? 1 : 0);

	SaveOptionToIni("HideInterface", m_HideInterface ? 1 : 0);
	SaveOptionToIni("FogSystem", m_FogSystem ? 1 : 0);
	SaveOptionToIni("DisableSkill", m_DisableSkill ? 1 : 0);
	SaveOptionToIni("NShadows", m_DisableSkill ? 1 : 0);
	SaveOptionToIni("MonsterHPBar", m_MonsterHPBar ? 1 : 0);

	return true;
}

void SEASON3B::CNewUIOptionWindow::SetButtonInfo()
{
}

void SEASON3B::CNewUIOptionWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}


void SEASON3B::CNewUIOptionWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIOptionWindow::UpdateMouseEvent()
{
	if (m_MoveWindow.UpdatePosition(m_Pos.x, m_Pos.y))
	{
		SetPos(m_MoveWindow.m_PosTemp.x, m_MoveWindow.m_PosTemp.y);
		SetButtonInfo();
		return false;
	}

	if (m_BtnClose.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
		return false;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 10, m_Pos.y + 25, 50, 25))
	{
		m_ActiveTab = TAB_GAME;
		PlayBuffer(SOUND_CLICK01);
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 65, m_Pos.y + 25, 50, 25))
	{
		m_ActiveTab = TAB_GRAPHICS;
		PlayBuffer(SOUND_CLICK01);
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 135, m_Pos.y + 25, 50, 25))
	{
		m_ActiveTab = TAB_PERFORMANCE;
		PlayBuffer(SOUND_CLICK01);
	}

	if (m_ActiveTab == TAB_GAME)
	{
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 23 + m_NewY, 15, 15))
		{
			SwitchKeyRight();
			m_bAutoAttack = !m_bAutoAttack;
			SaveOptionToIni("AutoAttack", m_bAutoAttack ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 41 + m_NewY, 15, 15))
		{
			m_bWhisperSound = !m_bWhisperSound;
			SaveOptionToIni("WhisperSound", m_bWhisperSound ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 59 + m_NewY, 15, 15))
		{
			m_bSlideHelp = !m_bSlideHelp;
			SaveOptionToIni("SlideHelp", m_bSlideHelp ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 77 + m_NewY, 15, 15))
		{
			m_Music = !m_Music;
			SaveOptionToIni("Music", m_Music ? 1 : 0);
		}
		if (CheckMouseIn(m_Pos.x + 34, m_Pos.y + 120 + m_NewY, 132, 16))
		{
			int iOldValue = m_iVolumeLevel;

			if (MouseWheel > 0)
			{
				MouseWheel = 0;
				m_iVolumeLevel++;
				if (m_iVolumeLevel > 10)
					m_iVolumeLevel = 10;
			}
			else if (MouseWheel < 0)
			{
				MouseWheel = 0;
				m_iVolumeLevel--;
				if (m_iVolumeLevel < 0)
					m_iVolumeLevel = 0;
			}

			if (SEASON3B::IsRepeat(VK_LBUTTON))
			{
				int x = MouseX - (m_Pos.x + 34);

				if (x < 0)
					m_iVolumeLevel = 0;
				else if (x > 124)
					m_iVolumeLevel = 10;
				else
				{
					float fValue = (10.f * x) / 124.f;
					m_iVolumeLevel = (int)(fValue + 0.5f);
				}
			}

			if (iOldValue != m_iVolumeLevel)
			{
				SetEffectVolumeLevel(m_iVolumeLevel);
			}
		}
		if (CheckMouseIn(m_Pos.x + 25, m_Pos.y + 160 + m_NewY, 141, 29))
		{
			if (SEASON3B::IsRepeat(VK_LBUTTON))
			{
				int x = MouseX - (m_Pos.x + 25);
				float fValue = (5.f * x) / 141.f;
				m_iRenderLevel = (int)fValue;
			}
		}
	}

	if (m_ActiveTab == TAB_GRAPHICS)
	{
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 21 + m_NewY, 15, 15))
		{
			m_EffectSprite = !m_EffectSprite;
			SaveOptionToIni("EffectSprite", m_EffectSprite ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 37 + m_NewY, 15, 15))
		{
			m_EffectParticle = !m_EffectParticle;
			SaveOptionToIni("EffectParticle", m_EffectParticle ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 53 + m_NewY, 15, 15))
		{
			m_Terrain = !m_Terrain;
			SaveOptionToIni("Terrain", m_Terrain ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 69 + m_NewY, 15, 15))
		{
			m_Wings = !m_Wings;
			SaveOptionToIni("Wings", m_Wings ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 85 + m_NewY, 15, 15))
		{
			m_Swords = !m_Swords;
			SaveOptionToIni("Swords", m_Swords ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 101 + m_NewY, 15, 15))
		{
			m_Equipments = !m_Equipments;
			SaveOptionToIni("Equipments", m_Equipments ? 1 : 0);
			if (m_Equipments == 1)
			{
				m_iRenderLevel = 0;
			}
			else
			{
				m_iRenderLevel = 4;
			}
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 117 + m_NewY, 15, 15))
		{
			m_Object = !m_Object;
			SaveOptionToIni("Object", m_Object ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 133 + m_NewY, 15, 15))
		{
			m_Shadows = !m_Shadows;
			SaveOptionToIni("Shadows", m_Shadows ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 149 + m_NewY, 15, 15))
		{
			m_WingsLv2 = !m_WingsLv2;
			SaveOptionToIni("WingsLv2", m_WingsLv2 ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 165 + m_NewY, 15, 15))
		{
			m_Monsters = !m_Monsters;
			SaveOptionToIni("Monsters", m_Monsters ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 181 + m_NewY, 15, 15))
		{
			m_Players = !m_Players;
			SaveOptionToIni("Players", m_Players ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 110, m_Pos.y + 21 + m_NewY, 15, 15))
		{
			m_FogSystem = !m_FogSystem;
			SaveOptionToIni("FogSystem", m_FogSystem ? 1 : 0);
			if (g_pOption->m_FogSystem == 1)
			{
				PlayBuffer(SOUND_PETEFFECT01);
			}
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 110, m_Pos.y + 37 + m_NewY, 15, 15))
		{
			m_DisableSkill = !m_DisableSkill;
			SaveOptionToIni("DisableSkill", m_DisableSkill ? 1 : 0);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 110, m_Pos.y + 53 + m_NewY, 15, 15))
		{
			m_NShadows = !m_NShadows;
			SaveOptionToIni("NShadows", m_NShadows ? 1 : 0);
		}
	}

	if (m_ActiveTab == TAB_PERFORMANCE)
	{
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 21 + m_NewY, 15, 15))
		{

			m_Window = !m_Window;

			LONG windowStyle = GetWindowLong(g_hWnd, GWL_STYLE);

			RECT rcWindow;
			GetWindowRect(g_hWnd, &rcWindow);

			RECT rcClient;
			GetClientRect(g_hWnd, &rcClient);

			if (m_Window)
			{
				windowStyle &= ~WS_CAPTION;
				windowStyle &= ~WS_THICKFRAME;

				AdjustWindowRect(&rcClient, WS_POPUP | WS_CLIPCHILDREN, FALSE);

				SetWindowPos(g_hWnd, NULL, rcWindow.left, rcWindow.top,
					rcClient.right - rcClient.left,
					rcClient.bottom - rcClient.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				windowStyle |= WS_CAPTION;
				windowStyle &= ~WS_THICKFRAME;

				AdjustWindowRect(&rcClient, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, FALSE);

				SetWindowPos(g_hWnd, NULL, rcWindow.left, rcWindow.top,
					rcClient.right - rcClient.left,
					rcClient.bottom - rcClient.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}

			SetWindowLong(g_hWnd, GWL_STYLE, windowStyle);

			SetWindowPos(g_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 130, m_Pos.y + 21 + m_NewY, 15, 15))
		{
			m_NSkillCollor = !m_NSkillCollor;
			SaveOptionToIni("NewSkill", m_NSkillCollor ? 1 : 0);
		}
		if (gProtect && gProtect->m_MainInfo.m_MountMiniSafeZone != 0)
		{
			if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 37 + m_NewY, 15, 15))
			{
				m_MiniSafeZone = !m_MiniSafeZone;
				SaveOptionToIni("MiniMounts", m_MiniSafeZone ? 1 : 0);
				if (g_pOption->m_MiniSafeZone == 1)
				{
					PlayBuffer(SOUND_PETEFFECT01);
				}
			}
		}

		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 70, m_Pos.y + 21 + m_NewY, 15, 15))
		{
			m_HideInterface = !m_HideInterface;
			SaveOptionToIni("HideInterface", m_HideInterface ? 1 : 0);
			if (g_pOption->m_HideInterface == 1)
			{
				PlayBuffer(SOUND_PETEFFECT01);
			}
		}

		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 70, m_Pos.y + 37 + m_NewY, 15, 15))
		{
			m_MonsterHPBar = !m_MonsterHPBar;
			SaveOptionToIni("MonsterHPBar", m_MonsterHPBar ? 1 : 0);
		}

		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 12, m_Pos.y + 177 + m_NewY, 15, 15))
		{
			if (!m_Fps30) {
				m_Fps30 = true;
				m_Fps60 = false;
				m_Fps120 = false;
				UpdateFpsLimitInIni(32);
			}
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 70, m_Pos.y + 177 + m_NewY, 15, 15))
		{
			if (!m_Fps60) {
				m_Fps30 = false;
				m_Fps60 = true;
				m_Fps120 = false;
				UpdateFpsLimitInIni(64);
			}
		}
		if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 130, m_Pos.y + 177 + m_NewY, 15, 15))
		{
			if (!m_Fps120) {
				m_Fps30 = false;
				m_Fps60 = false;
				m_Fps120 = true;
				UpdateFpsLimitInIni(128);
			}
		}
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, 190, 250) == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::UpdateKeyEvent()
{
	if (SEASON3B::IsPress('O') == true)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
		}
		return false;
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::Update()
{
	if (FPS > m_highestFps)
	{
		m_highestFps = FPS_AVG;
	}
	return true;
}

bool SEASON3B::CNewUIOptionWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderTabs();
	RenderContents();
	RenderButtons();
	DisableAlphaBlend();
	return true;
}

float SEASON3B::CNewUIOptionWindow::GetLayerDepth()
{
	return 10.5f;
}

float SEASON3B::CNewUIOptionWindow::GetKeyEventOrder()
{
	return 10.0f;
}

void SEASON3B::CNewUIOptionWindow::OpenningProcess()
{

}

void SEASON3B::CNewUIOptionWindow::ClosingProcess()
{

}

void SEASON3B::CNewUIOptionWindow::RenderTabs()
{
	glColor4ub(100, 10, 10, 200);
	RenderColor(m_Pos.x + 3, m_Pos.y + 33, 203, 15);
	EndRenderColor();

	glColor4ub(200, 200, 200, 255);

	if (m_ActiveTab == TAB_GAME)
		RenderColor(m_Pos.x + 20, m_Pos.y + 50, 80, 20);
	else if (m_ActiveTab == TAB_GRAPHICS)

		RenderColor(m_Pos.x + 100, m_Pos.y + 50, 80, 20);
	else if (m_ActiveTab == TAB_PERFORMANCE)

		RenderColor(m_Pos.x + 180, m_Pos.y + 50, 80, 20);
	EndRenderColor();

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0);

	if (m_ActiveTab == TAB_GAME)
		g_pRenderText->SetTextColor(255, 255, 0, 255);
	else
		g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x - 5, m_Pos.y + 35, "Game", 80, 20, RT3_SORT_CENTER);

	if (m_ActiveTab == TAB_GRAPHICS)
		g_pRenderText->SetTextColor(255, 255, 0, 255);
	else
		g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 35, "Graphics", 80, 20, RT3_SORT_CENTER);

	if (m_ActiveTab == TAB_PERFORMANCE)
		g_pRenderText->SetTextColor(255, 255, 0, 255);
	else
		g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 127, m_Pos.y + 35, "Interface", 80, 20, RT3_SORT_CENTER);
}

void SEASON3B::CNewUIOptionWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_OPTION_FRAME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_OPTION_FRAME_DOWN, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_top.tga", IMAGE_OPTION_FRAME_UP, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_back06(L).tga", IMAGE_OPTION_FRAME_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_back06(R).tga", IMAGE_OPTION_FRAME_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_line.jpg", IMAGE_OPTION_LINE, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_point.tga", IMAGE_OPTION_POINT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_check.tga", IMAGE_OPTION_BTN_CHECK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_effect03.tga", IMAGE_OPTION_EFFECT_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_effect04.tga", IMAGE_OPTION_EFFECT_COLOR, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_volume01.tga", IMAGE_OPTION_VOLUME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_volume02.tga", IMAGE_OPTION_VOLUME_COLOR, GL_LINEAR);
	LoadBitmap("Custom\\ui_dialog_e.tga", IMAGE_OPTION_FOOTER, GL_LINEAR);
}

void SEASON3B::CNewUIOptionWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_OPTION_FRAME_BACK);
	DeleteBitmap(IMAGE_OPTION_FRAME_DOWN);
	DeleteBitmap(IMAGE_OPTION_FRAME_UP);
	DeleteBitmap(IMAGE_OPTION_FRAME_LEFT);
	DeleteBitmap(IMAGE_OPTION_FRAME_RIGHT);
	DeleteBitmap(IMAGE_OPTION_LINE);
	DeleteBitmap(IMAGE_OPTION_POINT);
	DeleteBitmap(IMAGE_OPTION_BTN_CHECK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_BACK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_COLOR);
	DeleteBitmap(IMAGE_OPTION_VOLUME_BACK);
	DeleteBitmap(IMAGE_OPTION_VOLUME_COLOR);
	DeleteBitmap(IMAGE_OPTION_FOOTER);
}

void SEASON3B::CNewUIOptionWindow::RenderFrame()
{
	RenderImageScaleAuto(IMAGE_OPTION_FOOTER, m_Pos.x, m_Pos.y, 210.f, 250.f);
}

void SEASON3B::CNewUIOptionWindow::RenderContents()
{
	glColor4ub(21, 21, 21, 120);
	RenderColor(m_Pos.x + 15, m_Pos.y + 62 + m_NewY, 280, 200);
	EndRenderColor();

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);

	if (m_ActiveTab == TAB_GAME)
	{
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 26 + m_NewY, m_bAutoAttack ? "Attack Disable" : "Attack Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 44 + m_NewY, m_bWhisperSound ? "WisperSound Disable" : "WisperSound Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 62 + m_NewY, m_bSlideHelp ? "Slide Disable" : "Slide");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 80 + m_NewY, m_Music ? "Music Disable" : "Music Enable");
		g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 145 + m_NewY, GlobalText[1840], 120, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 40, m_Pos.y + 105 + m_NewY, GlobalText[389], 120, 0, RT3_SORT_CENTER);
	}

	if (m_ActiveTab == TAB_GRAPHICS)
	{
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 23 + m_NewY, m_EffectSprite ? "Static Disable" : "Static Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 39 + m_NewY, m_EffectParticle ? "Dynamic Disable" : "Dynamic Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 55 + m_NewY, m_Terrain ? "Terrain Disable" : "Terrain Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 71 + m_NewY, m_Wings ? "Wings Disable" : "Wings Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 87 + m_NewY, m_Swords ? "Armas Disable" : "Armas Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 103 + m_NewY, m_Equipments ? "Sets Disable" : "Sets Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 119 + m_NewY, m_Object ? "Objects Disable" : "Objects Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 135 + m_NewY, m_Shadows ? "Sombras Disable" : "Sombras Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 151 + m_NewY, m_WingsLv2 ? "Wings Lvl1 Enable" : "Wings Lvl1 Disable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 167 + m_NewY, m_Monsters ? "Monsters Disable" : "Monsters Enable");
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 183 + m_NewY, m_Players ? "Players Disable" : "Players Enable");
		g_pRenderText->RenderText(m_Pos.x + 130, m_Pos.y + 23 + m_NewY, m_FogSystem ? "Fog Enable" : "Fog Disable");
		g_pRenderText->RenderText(m_Pos.x + 130, m_Pos.y + 39 + m_NewY, m_DisableSkill ? "Skill Enable" : "Skill Disable");
		g_pRenderText->RenderText(m_Pos.x + 130, m_Pos.y + 55 + m_NewY, m_NShadows ? "NewShadow" : "NewShadow");
	}

	if (m_ActiveTab == TAB_PERFORMANCE)
	{
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 23 + m_NewY, "Borderlles");
		g_pRenderText->RenderText(m_Pos.x + 89, m_Pos.y + 23 + m_NewY, "Interface");
		g_pRenderText->RenderText(m_Pos.x + 148, m_Pos.y + 23 + m_NewY, "Remake Skill");

		g_pRenderText->RenderText(m_Pos.x + 89, m_Pos.y + 39 + m_NewY, "MonsterHPBar");

		if (gProtect && gProtect->m_MainInfo.m_MountMiniSafeZone != 0)
		{
			g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 39 + m_NewY, "MiniMounts");
		}

		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 179 + m_NewY, "30 FPS");
		g_pRenderText->RenderText(m_Pos.x + 88, m_Pos.y + 179 + m_NewY, "60 FPS");
		g_pRenderText->RenderText(m_Pos.x + 148, m_Pos.y + 179 + m_NewY, "120 FPS");
	}

	char nameWindow[128];
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(10, 10, 10, 0);
	snprintf(nameWindow, sizeof(nameWindow), "%s Options", gProtect->m_MainInfo.WindowName);
	g_pRenderText->SetTextColor(200, 200, 0, 255);
	g_pRenderText->RenderText(m_Pos.x - 45, m_Pos.y + m_NewY - 13, nameWindow, 299, 0, RT3_SORT_CENTER);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
}

void SEASON3B::CNewUIOptionWindow::RenderButtons()
{
	m_BtnClose.Render();

	if (m_ActiveTab == TAB_GAME)
	{
		if (m_bAutoAttack)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 23 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 23 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_bWhisperSound)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 41 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 41 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_bSlideHelp)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 59 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 59 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Music)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 77 + m_NewY, 15, 15, 0, 0);
			StopMusic();
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 77 + m_NewY, 15, 15, 0, 15.f);
		}

		RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x + 34, m_Pos.y + 120 + m_NewY, 124.f, 16.f);
		if (m_iVolumeLevel > 0)
		{
			RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x + 34, m_Pos.y + 120 + m_NewY, 124.f * 0.1f * (m_iVolumeLevel), 16.f);
		}
		else
		{
			AllStopSound();
		}

		RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x + 25, m_Pos.y + 160 + m_NewY, 141.f, 29.f);
		if (m_iRenderLevel >= 0)
		{
			RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x + 25, m_Pos.y + 160 + m_NewY, 141.f * 0.2f * (m_iRenderLevel + 1), 29.f);
		}
	}

	if (m_ActiveTab == TAB_GRAPHICS)
	{
		if (m_EffectSprite)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 21 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 21 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_EffectParticle)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 37 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 37 + m_NewY, 15, 15, 0, 15.f);
		}

		if (m_Terrain)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 53 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 53 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Wings)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 69 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 69 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Swords)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 85 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 85 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Equipments)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 101 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 101 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Object)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 117 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 117 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Shadows)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 133 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 133 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_WingsLv2)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 149 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 149 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Monsters)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 165 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 165 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Players)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 181 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 181 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_FogSystem)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 110, m_Pos.y + 21 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 110, m_Pos.y + 21 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_DisableSkill)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 110, m_Pos.y + 37 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 110, m_Pos.y + 37 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_NShadows)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 110, m_Pos.y + 53 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 110, m_Pos.y + 53 + m_NewY, 15, 15, 0, 15.f);
		}
	}

	if (m_ActiveTab == TAB_PERFORMANCE)
	{
		if (m_Window)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 21 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 21 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_NSkillCollor)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 130, m_Pos.y + 21 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 130, m_Pos.y + 21 + m_NewY, 15, 15, 0, 15.f);
		}
		if (gProtect && gProtect->m_MainInfo.m_MountMiniSafeZone != 0)
		{
			if (m_MiniSafeZone)
			{
				RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 37 + m_NewY, 15, 15, 0, 0);
			}
			else
			{
				RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 37 + m_NewY, 15, 15, 0, 15.f);
			}
		}


		if (m_MonsterHPBar)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 70, m_Pos.y + 37 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 70, m_Pos.y + 37 + m_NewY, 15, 15, 0, 15.f);
		}

		if (m_HideInterface)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 70, m_Pos.y + 21 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 70, m_Pos.y + 21 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Fps30)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 177 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 12, m_Pos.y + 177 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Fps60)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 70, m_Pos.y + 177 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 70, m_Pos.y + 177 + m_NewY, 15, 15, 0, 15.f);
		}
		if (m_Fps120)
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 130, m_Pos.y + 177 + m_NewY, 15, 15, 0, 0);
		}
		else
		{
			RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 130, m_Pos.y + 177 + m_NewY, 15, 15, 0, 15.f);
		}
	}
}


void SEASON3B::CNewUIOptionWindow::SetAutoAttack(bool bAuto)
{
	m_bAutoAttack = bAuto;
}

bool SEASON3B::CNewUIOptionWindow::IsAutoAttack()
{
	return m_bAutoAttack;
}

void SEASON3B::CNewUIOptionWindow::SetWhisperSound(bool bSound)
{
	m_bWhisperSound = bSound;
}

bool SEASON3B::CNewUIOptionWindow::IsWhisperSound()
{
	return m_bWhisperSound;
}

void SEASON3B::CNewUIOptionWindow::SetSlideHelp(bool bHelp)
{
	m_bSlideHelp = bHelp;
}

bool SEASON3B::CNewUIOptionWindow::IsSlideHelp()
{
	return m_bSlideHelp;
}

void SEASON3B::CNewUIOptionWindow::SetVolumeLevel(int iVolume)
{
	m_iVolumeLevel = iVolume;
}

int SEASON3B::CNewUIOptionWindow::GetVolumeLevel()
{
	return m_iVolumeLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderLevel(int iRender)
{
	m_iRenderLevel = iRender;
}

int SEASON3B::CNewUIOptionWindow::GetRenderLevel()
{
	return m_iRenderLevel;
}
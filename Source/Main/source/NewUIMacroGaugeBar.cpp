#include "stdafx.h"
#include "NewUISystem.h"
#include "ZzzInterface.h"
#include "CAIController.h"
#include "NewUIMacroGaugeBar.h"

SEASON3B::CNewUIMacroGaugeBar::CNewUIMacroGaugeBar()
{
	m_pNewUIMng = NULL;
	Initialize();
}

SEASON3B::CNewUIMacroGaugeBar::~CNewUIMacroGaugeBar()
{
	Release();
}

void SEASON3B::CNewUIMacroGaugeBar::LoadImages()
{
	LoadBitmap("Interface\\MacroUI\\MacroUI_Gauge_Frame.tga", BITMAP_GAUGE_FRAME, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Gauge_Bar_(B).jpg", BITMAP_GAUGE_FRAME_B, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Gauge_Bar_(G).jpg", BITMAP_GAUGE_FRAME_G, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Gauge_Bar_(Y).jpg", BITMAP_GAUGE_FRAME_Y, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Gauge_Bar_(O).jpg", BITMAP_GAUGE_FRAME_O, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Gauge_Bar_(R).jpg", BITMAP_GAUGE_FRAME_R, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
}

void SEASON3B::CNewUIMacroGaugeBar::UnloadImages()
{
	DeleteBitmap(BITMAP_GAUGE_FRAME, false);
	DeleteBitmap(BITMAP_GAUGE_FRAME_B, false);
	DeleteBitmap(BITMAP_GAUGE_FRAME_G, false);
	DeleteBitmap(BITMAP_GAUGE_FRAME_Y, false);
	DeleteBitmap(BITMAP_GAUGE_FRAME_O, false);
	DeleteBitmap(BITMAP_GAUGE_FRAME_R, false);
}

bool SEASON3B::CNewUIMacroGaugeBar::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	bool success;

	if (pNewUIMng)
	{
		m_pNewUIMng = pNewUIMng;
		m_pNewUIMng->AddUIObj(INTERFACE_MACRO_PROGRESS, this);
		this->SetPos(x, y);
		this->LoadImages();
		success = true;
	}
	else
	{
		success = false;
	}
	return success;
}

void SEASON3B::CNewUIMacroGaugeBar::Initialize()
{
	m_Start = false;
	m_initialTime = 0.0;
	m_stage = 0;
	m_Progress = 0.0;
	m_cost = 0;
	m_RenderToolip = false;
}

void SEASON3B::CNewUIMacroGaugeBar::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIMacroGaugeBar::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_tooltipX = m_Pos.x + 20;
	m_tooltipY = m_Pos.y + 10;
}

bool SEASON3B::CNewUIMacroGaugeBar::Render()
{
	if (m_Start == true)
	{
		float x = m_Pos.x;
		float y = m_Pos.y;

		EnableAlphaTest(true);
		glColor4f(1.f, 1.f, 1.f, 1.f);

		RenderImage(BITMAP_GAUGE_FRAME, x, y, 154.f, 8.f);
		if (m_stage == 1)
		{
			float width = (m_Progress * 152.0);
			RenderImage(BITMAP_GAUGE_FRAME_B, x + 2.0, y + 2.0, width, 4.0);
		}
		else if (m_stage > 1)
		{
			float width = (m_Progress * 152.0);
			RenderImage(m_stage + (BITMAP_GAUGE_FRAME - 1), x + 2.0, y + 2.0, 152.f, 4.f);
			RenderImage(m_stage + BITMAP_GAUGE_FRAME, x + 2.0, y + 2.0, width, 4.0);
		}
		if (m_RenderToolip == true)
		{
			RenderTipText(m_tooltipX, m_tooltipY, m_tooltip.c_str());
		}

		DisableAlphaBlend();
	}
	return true;
}

bool SEASON3B::CNewUIMacroGaugeBar::Update()
{
	if (m_Start == true)
	{
		//if (IsVisible() == false)
		//{
		//	Show(this);
		//}
		double work_time = m_Timer.GetTimeElapsed() + m_initialTime;
		unicode::t_char pszText[256];
		memset(pszText, 0, sizeof(pszText));

		if (work_time <= 60000000.0)
		{
			m_stage = (int)((work_time - 60000.0) / 12000000.0 + 1.0);
			if (m_stage >= 6)
				m_stage = 5;
			m_Progress = (double)((double)((int)(work_time - 60000.0) % 12000000) + 60000) / 12000000.0;
		}
		int mins = (int)(work_time / 60000.0);
		if (m_cost)
		{
			if (mins >= 60)
			{
				unicode::_sprintf(pszText, GlobalText[3585], mins / 60, mins % 60, m_stage, m_cost);
			}
			else
			{
				unicode::_sprintf(pszText, GlobalText[3584], mins % 60, m_stage, m_cost);
			}
		}
		else
		{
			unicode::_sprintf(pszText, GlobalText[3583], mins % 60);
		}
		m_tooltip = pszText;
	}
	return true;
}

bool SEASON3B::CNewUIMacroGaugeBar::UpdateKeyEvent()
{
	return true;
}

bool SEASON3B::CNewUIMacroGaugeBar::UpdateMouseEvent()
{
	if (m_Start == true)
	{
		m_RenderToolip = CheckMouseIn(m_Pos.x, m_Pos.y, m_Pos.x + 152, m_Pos.y + 4);
	}
	return true;
}

float SEASON3B::CNewUIMacroGaugeBar::GetLayerDepth()
{
	return 4.4f;
}

void SEASON3B::CNewUIMacroGaugeBar::StartAccumulate()
{
	this->Initialize();
	m_Timer.ResetTimer();
	m_Start = true;
	Show(true);
}

void SEASON3B::CNewUIMacroGaugeBar::StopAccumulate()
{
	m_Start = false;
	Show(false);
}

void SEASON3B::CNewUIMacroGaugeBar::SynceGaugeBar(time_t time)
{
	double start_time = (double)(60000 * (double)time);
	if (start_time <= 60000000.0)
	{
		m_Timer.ResetTimer();
		m_initialTime = start_time;
	}
}

void SEASON3B::CNewUIMacroGaugeBar::SetMacroExpense(int Cost)
{
	m_cost = Cost;
}

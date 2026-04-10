#include "stdafx.h"
#include "NewUISystem.h"
#include "NewUIRankingTop.h"
#include "ZzzTexture.h"
#include "wsclientinline.h"
#include "CharacterManager.h"

SEASON3B::CNewUIRankingTop::CNewUIRankingTop()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	is_request = false;
	m_RankListView = 10;
	m_RankMaxTop = 0;
	m_RankIndexCur = 0;
	m_RankSelectIndex = -1;
	memset(this->m_RankColum, 0, sizeof(this->m_RankColum));
	memset(this->m_RankName, 0, sizeof(this->m_RankName));
}

SEASON3B::CNewUIRankingTop::~CNewUIRankingTop()
{
	Release();
}

bool SEASON3B::CNewUIRankingTop::Create(CNewUIManager* pNewUIMng, float x, float y)
{
	bool Success = false;

	if (pNewUIMng)
	{
		m_pNewUIMng = pNewUIMng;

		m_pNewUIMng->AddUIObj(INTERFACE_RANKING_TOP, this);

		this->LoadImages();

		this->SetPos(x, y);

		this->SetInfo();

		this->Show(false);

		Success = true;
	}
	return Success;
}

void SEASON3B::CNewUIRankingTop::Release()
{
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);

		this->UnloadImages();

		m_RankList.clear();
	}
}

void SEASON3B::CNewUIRankingTop::SetInfo()
{
	m_RenderCharacter.Init(0);
	m_RenderCharacter.SetArrangeType(1, 119, 30);
	m_RenderCharacter.SetSize(141, 184);
	m_RenderCharacter.CopyPlayer();
	m_RenderCharacter.SetAutoupdatePlayer(TRUE);
	m_RenderCharacter.SetAnimation(AT_STAND1);
	m_RenderCharacter.SetAngle(90.f);
	m_RenderCharacter.SetZoom(0.80f);


	//m_pScrollBar.Create(m_Pos.x + 350, m_Pos.y + 51, 169);
	//m_pScrollBar.SetPercent(0.0);
}

void SEASON3B::CNewUIRankingTop::SetPos(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_RenderCharacter.SetPosition(x + 12, y + 34);
	//m_pScrollBar.SetPos(m_Pos.x + 350, m_Pos.y + 51);
}

void SEASON3B::CNewUIRankingTop::LoadImages()
{
	LoadBitmap("Interface\\HUD\\top_back_1.tga", IMAGE_TOP_BACK1, GL_LINEAR);
	LoadBitmap("Interface\\HUD\\top_back_2.tga", IMAGE_TOP_BACK2, GL_LINEAR);
	LoadBitmap("Interface\\HUD\\VipLevel1.tga", IMAGE_TOP_LEVEL1, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\HUD\\VipLevel2.tga", IMAGE_TOP_LEVEL2, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
	LoadBitmap("Interface\\HUD\\VipLevel3.tga", IMAGE_TOP_LEVEL3, GL_LINEAR, GL_CLAMP_TO_EDGE, true, false);
}

void SEASON3B::CNewUIRankingTop::UnloadImages()
{
	DeleteBitmap(IMAGE_TOP_BACK1);
	DeleteBitmap(IMAGE_TOP_BACK2);
	DeleteBitmap(IMAGE_TOP_LEVEL1);
	DeleteBitmap(IMAGE_TOP_LEVEL2);
	DeleteBitmap(IMAGE_TOP_LEVEL3);
}

bool SEASON3B::CNewUIRankingTop::UpdateKeyEvent()
{
	if (IsVisible() == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE))
		{
			g_pNewUISystem->Hide(INTERFACE_RANKING_TOP);
			return false;
		}

		if (SEASON3B::IsRelease(VK_LEFT))
		{
			this->RequestServerRankingInfo((BYTE)(m_RankIndexCur - 1));
			return false;
		}
		if (SEASON3B::IsRelease(VK_RIGHT))
		{
			this->RequestServerRankingInfo((BYTE)(m_RankIndexCur + 1));
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIRankingTop::UpdateMouseEvent()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x, m_Pos.y, 366, 230))
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + 344.f, m_Pos.y + 3.5f, 19.f, 19.f) && SEASON3B::IsRelease(VK_LBUTTON))
		{
			g_pNewUISystem->Hide(INTERFACE_RANKING_TOP);
			return false;
		}

		size_t secure = 0;
		size_t current = 0;
		size_t good_count = m_RankList.size();

		if (good_count > m_RankListView)
		{
			double prev = 1;

			if (SEASON3B::CheckMouseIn(m_Pos.x + 162, m_Pos.y + 50, 195, 176))
			{
				if (MouseWheel <= 0)
				{
					if (MouseWheel < 0)
					{
						MouseWheel = 0;
						//prev += 0.1;
						//if (prev > 1.0)
						//	prev = 1.0;
						//m_pScrollBar.SetPercent(prev);
					}
				}
				else
				{
					MouseWheel = 0;
					//prev -= 0.1;
					//if (prev < 0.0)
					//	prev = 0.0;
					//m_pScrollBar.SetPercent(prev);
				}
			}
			//m_pScrollBar.UpdateMouseEvent();

			current = (int)((double)(unsigned int)(good_count - m_RankListView) * prev);
		}

		for (size_t i = current; i < good_count && secure < m_RankListView; i++, secure++)
		{
			if (SEASON3B::CheckMouseIn(m_Pos.x + 162, m_Pos.y + i * 16 + 50, 195, 16) && SEASON3B::IsRelease(VK_LBUTTON))
			{
				m_RankSelectIndex = i;
			}
		}

		m_RenderCharacter.DoMouseAction();
	}

	return !SEASON3B::CheckMouseIn(m_Pos.x, m_Pos.y, 366, 230);
}

bool SEASON3B::CNewUIRankingTop::Render()
{
	EnableAlphaTest(true);

	glColor4f(1.f, 1.f, 1.f, 1.f);

	this->RenderFrame();

	this->RenderTexte();

	DisableAlphaBlend();

	m_RenderCharacter.Render();

	return true;
}

bool SEASON3B::CNewUIRankingTop::Update()
{
	if (IsVisible())
	{
		//m_pScrollBar.Update();
	}
	return true;
}

float SEASON3B::CNewUIRankingTop::GetLayerDepth()
{
	return 10.0f;
}

void SEASON3B::CNewUIRankingTop::OpenningProcess()
{
	//is_request = false;
	m_RankIndexCur = 0;

	SendRequestMaxRanking();

	RequestServerRankingInfo(m_RankIndexCur);

	//m_pScrollBar.SetPercent(0.0);

	m_RenderCharacter.CopyPlayer();
}

void SEASON3B::CNewUIRankingTop::ClosingProcess()
{
}

void SEASON3B::CNewUIRankingTop::RenderFrame()
{
	float x = m_Pos.x;
	float y = m_Pos.y;

	RenderImage(IMAGE_TOP_BACK1, x, y, 183.f, 230.f, 0.0, 0.0, 732.0, 917.0); //-- background
	x += 183.f;
	RenderImage(IMAGE_TOP_BACK2, x, y, 183.f, 230.f, 0.0, 0.0, 732.0, 917.0); //-- background
}

void SEASON3B::CNewUIRankingTop::RenderTexte()
{
	int x = m_Pos.x;
	int y = m_Pos.y;

	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(CLRDW_GOLD);
	g_pRenderText->SetFont(g_hFontBold);

	glColor4f(0.0, 0.0, 0.0, 0.8f);
	RenderColor(x, y - 30, 366, 25, 0.0, 0);
	EndRenderColor();

	g_pRenderText->RenderText(x + 183, y - 30, GlobalText[3236], 0, 25, RT3_WRITE_CENTER);

	g_pRenderText->SetTextColor(-1);

	char pszText[50];

	sprintf_s(pszText, "%s - %s", GlobalText[27], this->m_RankName);
	g_pRenderText->RenderText(x + 12, y + 10, pszText, 0, 0, RT3_SORT_LEFT);

	x = m_Pos.x + 162;
	g_pRenderText->SetBgColor(52, 57, 67, 255);
	g_pRenderText->RenderText(x, y + 32, "#", 25, 18, RT3_SORT_CENTER);

	x += 25;
	g_pRenderText->RenderText(x, y + 32, GlobalText[1389], 50, 18, RT3_SORT_CENTER); //-- name

	x += 50;
	g_pRenderText->RenderText(x, y + 32, GlobalText[1973], 60, 18, RT3_SORT_CENTER); //-- class name

	x += 60;
	g_pRenderText->RenderText(x, y + 32, this->m_RankColum, 60, 18, RT3_SORT_CENTER); //-- score
	g_pRenderText->SetBgColor(0);
	y += 50;


	size_t secure = 0;
	size_t current = 0;
	size_t good_count = m_RankList.size();

	if (good_count > m_RankListView)
	{
		double prev = 1;

		current = (int)((double)(unsigned int)(good_count - m_RankListView) * prev);
	}


	int HEIGH_SPACING = 16;

	for (size_t i = current; i < good_count && secure < m_RankListView; i++, secure++)
	{
		x = m_Pos.x + 162;

		if (SEASON3B::CheckMouseIn(x, y, 195, HEIGH_SPACING) || m_RankSelectIndex == i)
		{
			EnableAlphaTest(true);
			glColor4ub(79, 86, 100, 255);
			RenderColor(x, y, 195, 15);
			EndRenderColor();
		}

		sprintf_s(pszText, "%02d", i + 1);
		g_pRenderText->RenderText(x, y, pszText, 25, HEIGH_SPACING, RT3_SORT_CENTER);

		x += 25;

		BYTE LevelVip = m_RankList[i].GetVip();

		g_pRenderText->RenderText(x + 14, y, m_RankList[i].GetName(), 50, HEIGH_SPACING, RT3_SORT_LEFT);

		if (LevelVip > 0)
		{
			SEASON3B::RenderImage(IMAGE_TOP_BACK3 + LevelVip, x, y + 3, 10.f, 10.f, 0.0, 0.0, 1.f, 1.f, -1); //-- Render VIP
		}

		x += 50;
		g_pRenderText->RenderText(x, y, m_RankList[i].GetClass(), 60, HEIGH_SPACING, RT3_SORT_CENTER);

		x += 60;
		sprintf_s(pszText, "%02d", m_RankList[i].GetScore());
		g_pRenderText->RenderText(x, y, pszText, 60, HEIGH_SPACING, RT3_SORT_CENTER);

		y += HEIGH_SPACING;
	}

	if (good_count > m_RankListView)
	{
		//m_pScrollBar.Render();
	}
}

void SEASON3B::CNewUIRankingTop::ReceiveRankingInfo(BYTE* ReceiveBuffer)
{
	LPPWHEADER_DEFAULT_WORD Data = (LPPWHEADER_DEFAULT_WORD)ReceiveBuffer;

	m_RankSelectIndex = -1;
	if (m_RankMaxTop == 0 && Data->Value != 0)
	{
		m_RankMaxTop = Data->Value;
		RequestServerRankingInfo(m_RankIndexCur);
	}
	else
	{
		m_RankMaxTop = Data->Value;
	}
}

void SEASON3B::CNewUIRankingTop::ReceiveRankingListInfo(BYTE* ReceiveBuffer)
{
	LPPHEADER_RANKING_LIST Data = (LPPHEADER_RANKING_LIST)ReceiveBuffer;

	m_RankList.clear();
	//m_pScrollBar.SetPercent(0.0);

	this->m_RankIndexCur = Data->index;

	m_RankSelectIndex = -1;

	memset(this->m_RankName, 0, sizeof(this->m_RankName));

	strcpy_s(this->m_RankName, Data->rankname);

	strcpy_s(this->m_RankColum, Data->col2);

	int offset = sizeof(PHEADER_RANKING_LIST);

	for (int n = 0; n < Data->count; n++)
	{
		LPPCREATE_RANKING_INFO Data2 = (LPPCREATE_RANKING_INFO)(ReceiveBuffer + offset);

		BYTE Class = gCharacterManager.ChangeServerClassTypeToClientClassType(Data2->PlayerClass);

		m_RankList.push_back(TEMPLATE_RANKING(Data2->Name, gCharacterManager.GetCharacterClassText(Class), Data2->LevelVip, Data2->TotalScore));

		offset += (sizeof(PCREATE_RANKING_INFO));
	}

	std::sort(m_RankList.begin(), m_RankList.end(), [](const TEMPLATE_RANKING& a, const TEMPLATE_RANKING& b) {
		return a.Score > b.Score; // Orden descendente
		});

	is_request = false;
}

void SEASON3B::CNewUIRankingTop::RequestServerRankingInfo(BYTE Index)
{
	if (is_request == false && (Index >= 0 && Index < m_RankMaxTop))
	{
		is_request = true;
		SendRequestRankingInfo(Index);
	}
}

#include "stdafx.h"
#include "NewUISystem.h"
#include "NewUIRankingTop.h"
#include "ZzzTexture.h"
#include "wsclientinline.h"
#include "CharacterManager.h"
#include "Preview.h"

static const WORD RANKING_PREVIEW_KEY = 0xFFFE;

static BYTE RankingDBClassToClientClass(BYTE dbClass)
{
	// Ranking packets carry the database encoding (0,16,32,48,64,80), not
	// the compact network encoding used by ChangeServerClassTypeToClientClassType.
	BYTE baseClass = dbClass / 16;
	BYTE evolution = dbClass % 16;

	if (baseClass >= 6)
	{
		return 0;
	}

	BYTE clientClass = baseClass;

	if (evolution == 1)
	{
		clientClass |= 0x08;
	}
	else if (evolution >= 2)
	{
		clientClass |= 0x10;
	}

	return clientClass;
}

static void BuildEmptyRankingEquipment(BYTE* equipment)
{
	static const BYTE emptyEquipment[EQUIPMENT_LENGTH] =
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x00, 0x00, 0x00, 0xF8, 0x00,
		0x00, 0xF0, 0xFF, 0xFF, 0xFF,
		0x00, 0x00
	};

	memcpy(equipment, emptyEquipment, sizeof(emptyEquipment));
}

SEASON3B::CNewUIRankingTop::CNewUIRankingTop()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	this->is_request = false;
	this->is_loading = false;
	this->CharacterIndex = -1;
	this->TotalView = 10;
	this->TotalSize = 0;
	this->SelectIndex = 0;
	this->ScoreName = "";
	this->RankingName = "";
	this->m_pScrollBar = NULL;
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

		this->SetInfo();

		this->SetPos(x, y);

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

		this->rankingList.clear();
		SAFE_DELETE(this->m_pScrollBar);
		this->m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIRankingTop::SetInfo()
{
	m_RenderCharacter.Init(0);
	m_RenderCharacter.SetOption(UIPHOTOVIEWER_CANCONTROL);
	m_RenderCharacter.SetArrangeType(1, 119, 30);
	m_RenderCharacter.SetSize(141, 184);
	m_RenderCharacter.CopyPlayer();
	m_RenderCharacter.SetAutoupdatePlayer(TRUE);
	m_RenderCharacter.SetAnimation(AT_STAND1);
	m_RenderCharacter.SetAngle(90.f);
	m_RenderCharacter.SetZoom(0.80f);


	m_pScrollBar = new CNewUIScrollBar;
	m_pScrollBar->Create(m_Pos.x + 350, m_Pos.y + 51, 169);
	m_pScrollBar->SetPercent(0.0f);
}

void SEASON3B::CNewUIRankingTop::SetPos(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_RenderCharacter.SetPosition(x + 12, y + 34);
	if (m_pScrollBar)
	{
		m_pScrollBar->SetPos(m_Pos.x + 350, m_Pos.y + 51);
	}
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
			this->RequestServerRankingInfo((BYTE)(this->SelectIndex - 1));
			return false;
		}
		if (SEASON3B::IsRelease(VK_RIGHT))
		{
			this->RequestServerRankingInfo((BYTE)(this->SelectIndex + 1));
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

		size_t secure = 0, current = 0;
		size_t good_count = this->GetTotalCharacter();

		if (good_count > this->TotalView && m_pScrollBar)
		{
			float prev = m_pScrollBar->GetPercent();

			if (SEASON3B::CheckMouseIn(m_Pos.x + 162, m_Pos.y + 50, 195, 176))
			{
				if (MouseWheel < 0)
				{
					MouseWheel = 0;
					prev += 0.1f;
				}
				else if (MouseWheel > 0)
				{
					MouseWheel = 0;
					prev -= 0.1f;
				}
				if (prev < 0.0f) prev = 0.0f;
				if (prev > 1.0f) prev = 1.0f;
				m_pScrollBar->SetPercent(prev);
			}

			m_pScrollBar->UpdateMouseEvent();
			current = (size_t)((good_count - this->TotalView) * m_pScrollBar->GetPercent());
		}

		int RenderPosY = this->m_Pos.y + 50;
		for (size_t i = current; i < good_count && secure < this->TotalView; i++, secure++)
		{
			if (SEASON3B::CheckMouseIn(m_Pos.x + 162, RenderPosY, 195, 16) && SEASON3B::IsRelease(VK_LBUTTON))
			{
				this->RequestCharacterInfo((int)i);
			}
			RenderPosY += 16;
		}

		m_RenderCharacter.DoMouseAction();
	}

	return !SEASON3B::CheckMouseIn(m_Pos.x, m_Pos.y, 366, 230);
}

bool SEASON3B::CNewUIRankingTop::Render()
{
	EnableAlphaTest();

	glColor4f(1.f, 1.f, 1.f, 1.f);

	this->RenderFrame();

	this->RenderText();

	DisableAlphaBlend();

	CHARACTER* preview = m_RenderCharacter.GetPhotoChar();
	Vector(1.0f, 1.0f, 1.0f, preview->Light);
	Vector(1.0f, 1.0f, 1.0f, preview->Object.Light);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	m_RenderCharacter.Render();

	return true;
}

bool SEASON3B::CNewUIRankingTop::Update()
{
	if (IsVisible())
	{
		if (m_pScrollBar)
		{
			m_pScrollBar->Update();
		}
	}
	return true;
}

float SEASON3B::CNewUIRankingTop::GetLayerDepth()
{
	return 10.0f;
}

void SEASON3B::CNewUIRankingTop::OpenningProcess()
{
	this->SelectIndex = 0;
	this->is_request = false;
	this->is_loading = false;
	this->SetCharacterIndex(-1);

	SendRequestMaxRanking();

	RequestServerRankingInfo(0);

	if (m_pScrollBar)
	{
		m_pScrollBar->SetPercent(0.0f);
	}

	gCustomPreview.DeleteCustomPreview(RANKING_PREVIEW_KEY);
	m_RenderCharacter.SetAutoupdatePlayer(TRUE);
	m_RenderCharacter.CopyPlayer();
	m_RenderCharacter.GetPhotoChar()->Object.m_BuffMap.ClearBuff();
	m_RenderCharacter.GetPhotoChar()->CtlCode = 0;
}

void SEASON3B::CNewUIRankingTop::ClosingProcess()
{
	gCustomPreview.DeleteCustomPreview(RANKING_PREVIEW_KEY);
	this->is_loading = false;
}

void SEASON3B::CNewUIRankingTop::SetCharacterIndex(int characterIndex)
{
	this->CharacterIndex = characterIndex;
}

int SEASON3B::CNewUIRankingTop::GetCharacterIndex()
{
	return this->CharacterIndex;
}

int SEASON3B::CNewUIRankingTop::GetTotalCharacter()
{
	return (int)this->rankingList.size();
}

void SEASON3B::CNewUIRankingTop::RenderFrame()
{
	float x = m_Pos.x;
	float y = m_Pos.y;

	RenderImageScale(IMAGE_TOP_BACK1, x, y, 183.f, 230.f, 0.0f, 0.0f, 732.0f, 917.0f, 0.0f);
	x += 183.f;
	RenderImageScale(IMAGE_TOP_BACK2, x, y, 183.f, 230.f, 0.0f, 0.0f, 732.0f, 917.0f, 0.0f);
}

void SEASON3B::CNewUIRankingTop::RenderText()
{
	int x = m_Pos.x;
	int y = m_Pos.y;

	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(CLRDW_GOLD);
	g_pRenderText->SetFont(g_hFontBold);

	glColor4f(0.0, 0.0, 0.0, 0.8f);
	RenderColor(x, y - 30, 366, 25, 0.0, 0);
	EndRenderColor();

	g_pRenderText->RenderText(x + 183, y - 30, "Ranking de Jogadores", 0, 25, RT3_WRITE_CENTER);

	g_pRenderText->SetTextColor(-1);

	char pszText[50];

	sprintf_s(pszText, "Ranking - %s", this->RankingName.c_str());
	g_pRenderText->RenderText(x + 12, y + 10, pszText, 0, 0, RT3_SORT_LEFT);

	x = m_Pos.x + 162;
	g_pRenderText->SetBgColor(52, 57, 67, 255);
	g_pRenderText->RenderText(x, y + 32, "#", 25, 18, RT3_SORT_CENTER);

	x += 25;
	g_pRenderText->RenderText(x, y + 32, "Nome", 50, 18, RT3_SORT_CENTER);

	x += 50;
	g_pRenderText->RenderText(x, y + 32, "Classe", 60, 18, RT3_SORT_CENTER);

	x += 60;
	g_pRenderText->RenderText(x, y + 32, this->ScoreName.c_str(), 60, 18, RT3_SORT_CENTER); //-- score
	g_pRenderText->SetBgColor(0);
	y += 50;


	size_t secure = 0;
	size_t current = 0;
	size_t good_count = this->rankingList.size();

	if (good_count > this->TotalView && m_pScrollBar)
	{
		current = (size_t)((good_count - this->TotalView) * m_pScrollBar->GetPercent());
	}


	int HEIGH_SPACING = 16;

	for (size_t i = current; i < good_count && secure < this->TotalView; i++, secure++)
	{
		x = m_Pos.x + 162;

		if (SEASON3B::CheckMouseIn(x, y, 195, HEIGH_SPACING) || this->GetCharacterIndex() == (int)i)
		{
			EnableAlphaTest(true);
			glColor4ub(79, 86, 100, 255);
			RenderColor(x, y, 195, 15);
			EndRenderColor();
		}

		sprintf_s(pszText, "%02d", i + 1);
		g_pRenderText->RenderText(x, y, pszText, 25, HEIGH_SPACING, RT3_SORT_CENTER);

		x += 25;

		BYTE LevelVip = this->rankingList[i].GetVip();

		g_pRenderText->RenderText(x + 14, y, this->rankingList[i].GetName(), 50, HEIGH_SPACING, RT3_SORT_LEFT);

		if (LevelVip > 0)
		{
			SEASON3B::RenderImage(IMAGE_TOP_BACK3 + LevelVip, x, y + 3, 10.f, 10.f, 0.0, 0.0, 1.f, 1.f, -1); //-- Render VIP
		}

		x += 50;
		g_pRenderText->RenderText(x, y, this->rankingList[i].GetClass(), 60, HEIGH_SPACING, RT3_SORT_CENTER);

		x += 60;
		sprintf_s(pszText, "%02d", this->rankingList[i].GetScore());
		g_pRenderText->RenderText(x, y, pszText, 60, HEIGH_SPACING, RT3_SORT_CENTER);

		y += HEIGH_SPACING;
	}

	if (good_count > this->TotalView && m_pScrollBar)
	{
		m_pScrollBar->Render();
	}
}

void SEASON3B::CNewUIRankingTop::ReceiveRankingInfo(BYTE* ReceiveBuffer)
{
	LPPWHEADER_DEFAULT_WORD Data = (LPPWHEADER_DEFAULT_WORD)ReceiveBuffer;

	this->SetCharacterIndex(-1);
	if (this->TotalSize == 0 && Data->Value != 0)
	{
		this->TotalSize = Data->Value;
		RequestServerRankingInfo((BYTE)this->SelectIndex);
	}
	else
	{
		this->TotalSize = Data->Value;
	}
}

void SEASON3B::CNewUIRankingTop::ReceiveCharacterInfo(BYTE* ReceiveBuffer)
{
	LPPHEADER_RANKING_CHARACTER Data = (LPPHEADER_RANKING_CHARACTER)ReceiveBuffer;

	if (Data->characterIndex != this->GetCharacterIndex())
	{
		return;
	}

	this->is_loading = false;
	m_RenderCharacter.SetAutoupdatePlayer(FALSE);

	gCustomPreview.DeleteCustomPreview(RANKING_PREVIEW_KEY);
	m_RenderCharacter.GetPhotoChar()->Object.m_BuffMap.ClearBuff();
	m_RenderCharacter.GetPhotoChar()->CtlCode = 0;

	if (Data->result == 0)
	{
		return;
	}

	WORD Element[2] = { 0, 0 };
	char CharacterName[MAX_ID_SIZE + 1] = { 0 };
	memcpy(CharacterName, Data->Name, MAX_ID_SIZE);

	gCustomPreview.InsertCustomPreviewList(0, CharacterName, Data->PetIndex, Data->WingIndex, RANKING_PREVIEW_KEY, 0, Element);

	m_RenderCharacter.GetPhotoChar()->Key = RANKING_PREVIEW_KEY;
	if ((Data->CtlCode & CTLCODE_20OPERATOR) != 0)
	{
		m_RenderCharacter.GetPhotoChar()->CtlCode = CTLCODE_20OPERATOR;
	}
	else if ((Data->CtlCode & CTLCODE_08OPERATOR) != 0)
	{
		m_RenderCharacter.GetPhotoChar()->CtlCode = CTLCODE_08OPERATOR;
	}
	m_RenderCharacter.SetID(CharacterName);
	m_RenderCharacter.SetClass(RankingDBClassToClientClass(Data->PlayerClass));
	m_RenderCharacter.SetEquipmentPacket(Data->Equipment);
}

void SEASON3B::CNewUIRankingTop::ReceiveRankingListInfo(BYTE* ReceiveBuffer)
{
	LPPHEADER_RANKING_LIST Data = (LPPHEADER_RANKING_LIST)ReceiveBuffer;

	this->rankingList.clear();
	this->SetCharacterIndex(-1);
	if (m_pScrollBar)
	{
		m_pScrollBar->SetPercent(0.0f);
	}
	this->SelectIndex = Data->index;
	this->RankingName = Data->rankname;
	this->ScoreName = Data->col2;

	int offset = sizeof(PHEADER_RANKING_LIST);

	for (int n = 0; n < Data->count; n++)
	{
		LPPCREATE_RANKING_INFO Data2 = (LPPCREATE_RANKING_INFO)(ReceiveBuffer + offset);

		BYTE Class = RankingDBClassToClientClass(Data2->PlayerClass);

		this->rankingList.push_back(TEMPLATE_RANKING(Data2->Name, gCharacterManager.GetCharacterClassText(Class), Class, Data2->LevelVip, Data2->TotalScore));

		offset += (sizeof(PCREATE_RANKING_INFO));
	}

	std::sort(this->rankingList.begin(), this->rankingList.end(), [](const TEMPLATE_RANKING& a, const TEMPLATE_RANKING& b) {
		return a.Score > b.Score;
		});

	this->is_request = false;
}

void SEASON3B::CNewUIRankingTop::RequestCharacterInfo(int characterIndex)
{
	if (characterIndex < 0 || characterIndex >= this->GetTotalCharacter())
	{
		return;
	}

	this->SetCharacterIndex(characterIndex);
	this->is_loading = true;
	gCustomPreview.DeleteCustomPreview(RANKING_PREVIEW_KEY);
	m_RenderCharacter.GetPhotoChar()->Object.m_BuffMap.ClearBuff();
	m_RenderCharacter.GetPhotoChar()->CtlCode = 0;
	m_RenderCharacter.SetAutoupdatePlayer(FALSE);
	m_RenderCharacter.SetID(this->rankingList[characterIndex].GetName());
	m_RenderCharacter.SetClass(this->rankingList[characterIndex].GetPlayerClass());
	BYTE emptyEquipment[EQUIPMENT_LENGTH];
	BuildEmptyRankingEquipment(emptyEquipment);
	m_RenderCharacter.SetEquipmentPacket(emptyEquipment);
	SendRequestRankingPlayer(characterIndex, this->rankingList[characterIndex].GetName());
}

void SEASON3B::CNewUIRankingTop::RequestServerRankingInfo(BYTE RankingIndex)
{
	size_t rankingCount = (this->TotalSize == 0) ? 10 : this->TotalSize;

	if (!this->is_request && RankingIndex < rankingCount)
	{
		this->is_request = true;
		SendRequestRankingInfo(RankingIndex);
	}
}

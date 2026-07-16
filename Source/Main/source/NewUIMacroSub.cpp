#include "stdafx.h"
#include "NewUISystem.h"
#include "DSPlaySound.h"
#include "CAIController.h"
#include "NewUIMacroSub.h"
#include "CharacterManager.h"

SEASON3B::CNewUIMacroSub::CNewUIMacroSub()
{
	m_pNewUIMng = NULL;
	Initialize();
}

SEASON3B::CNewUIMacroSub::~CNewUIMacroSub()
{
	Release();
}

bool SEASON3B::CNewUIMacroSub::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	bool success = false;

	if (pNewUIMng)
	{
		m_pNewUIMng = pNewUIMng;
		m_pNewUIMng->AddUIObj(INTERFACE_MACRO_OFICIAL_SUB, this);
		SetPos(x, y);
		LoadImages();

		InitOptionButtonGroup();
		m_hp_Party.Create(IMAGE_OPTION_VOLUME_BACK, IMAGE_OPTION_VOLUME_COLOR, x + 33, y + 104, 124, 16, 10, 1);
		m_hp_Potion.Create(IMAGE_OPTION_VOLUME_BACK, IMAGE_OPTION_VOLUME_COLOR, x + 33, y + 78, 124, 16, 10, 1);
		m_hp_Healt.Create(IMAGE_OPTION_VOLUME_BACK, IMAGE_OPTION_VOLUME_COLOR, x + 33, y + 140, 124, 16, 10, 1);
		m_hp_DrainLife.Create(IMAGE_OPTION_VOLUME_BACK, IMAGE_OPTION_VOLUME_COLOR, x + 33, y + 140, 124, 16, 10, 1);

#if MAIN_UPDATE > 603
		m_preferenceParty.Create(BITMAP_check_box, x + 20, y + 79, 15, 15, GlobalText[3539]); //-- Preference of Party Heal
		m_durationParty[0].Create(BITMAP_check_box, x + 20, y + 171, 15, 15, GlobalText[3540]); //-- Buff Duration for All Party Members
		m_durationParty[1].Create(BITMAP_check_box, x + 20, y + 81, 15, 15, GlobalText[3540]); //-- Buff Duration for All Party Members

		m_preferenceParty.ChangeCheckBoxInfo(x + 20, y + 79, 15, 15, 45.f);
		m_durationParty[0].ChangeCheckBoxInfo(x + 20, y + 171, 15, 15, 45.f);
		m_durationParty[1].ChangeCheckBoxInfo(x + 20, y + 81, 15, 15, 45.f);
#else
		m_preferenceParty.Create(IMAGE_MACRO_MAIN_CHECK, x + 20, y + 79, 15, 15, GlobalText[3539]); //-- Preference of Party Heal
		m_durationParty[0].Create(IMAGE_MACRO_MAIN_CHECK, x + 20, y + 171, 15, 15, GlobalText[3540]); //-- Buff Duration for All Party Members
		m_durationParty[1].Create(IMAGE_MACRO_MAIN_CHECK, x + 20, y + 81, 15, 15, GlobalText[3540]); //-- Buff Duration for All Party Members
#endif

		unicode::t_char Src[0xFF];
		memset(Src, 0, 0xFFu);
		unicode::_sprintf(Src, "%d", 0);
		AddTextInputBox(&m_timespace[0], x + 114, y + 190, 30, 14, 3, Src);
		AddTextInputBox(&m_timespace[1], x + 114, y + 100, 30, 14, 3, Src);

#if MAIN_UPDATE > 603
		AddButton(&m_save, IMAGE_IGS_VIEWDETAIL_BTN, x + 120, y + 382, 52, 26, 1, 0, 1, 1u, GlobalText[3541], "", true);
		AddButton(&m_reset, IMAGE_IGS_VIEWDETAIL_BTN, x + 65, y + 382, 52, 26, 1, 0, 1, 1u, GlobalText[3542], "", true);
		AddButton(&m_exit, IMAGE_MACRO_MAIN_EXIT_BTN, x + 22, y + 384, 20, 20, 0, 0, 0, 0, "", GlobalText[3541], true);
#else
		AddButton(&m_save, IMAGE_IGS_VIEWDETAIL_BTN, x + 120, y + 388, 52, 26, 1, 0, 1, 1u, GlobalText[3541], "", true);
		AddButton(&m_reset, IMAGE_IGS_VIEWDETAIL_BTN, x + 65, y + 388, 52, 26, 1, 0, 1, 1u, GlobalText[3542], "", true);
		AddButton(&m_exit, IMAGE_MACRO_MAIN_EXIT_BTN, x + 20, y + 388, 36, 29, 0, 0, 0, 0, "", GlobalText[3541], true);
#endif

		Show(false);

		success = true;
	}

	return success;
}

void SEASON3B::CNewUIMacroSub::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIMacroSub::Initialize()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_WindowsType = 0;
	m_Character = 0;
	unknown60 = 0;
	unknown64 = 0;
}

void SEASON3B::CNewUIMacroSub::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void SEASON3B::CNewUIMacroSub::LoadImages()
{
}

void SEASON3B::CNewUIMacroSub::UnloadImages()
{
}

bool SEASON3B::CNewUIMacroSub::Update()
{
	return true;
}

bool SEASON3B::CNewUIMacroSub::Render()
{
	EnableAlphaTest();
	glColor4f(1.0, 1.0, 1.0, 1.0);

	float RenderFrameX = m_Pos.x;
	float RenderFrameY = m_Pos.y;

	SEASON3B::window_nomal(RenderFrameX, RenderFrameY, 0);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(-1);
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->RenderText(RenderFrameX + WIN_WINDOW_SIZEX / 2.f, RenderFrameY + 14, m_Name.c_str(), 190, 0, RT3_WRITE_CENTER, NULL);
	g_pRenderText->SetFont(g_hFont);

	switch (m_WindowsType)
	{
	case 1:
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 53, 170, 61, 60, 20);
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 112, 170, 65, 60, 20);
		m_optionGroup[0].Render();
		m_optionGroup[1].Render();

		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 61, GlobalText[3543], 0, 0, 3, 0);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 120, GlobalText[3544], 0, 0, 3, 0);
		break;
	case 2:
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 53, 170, 61, 60, 20);
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 112, 170, 65, 60, 20);

		m_optionGroup[2].Render();
		m_optionGroup[3].Render();

		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 61, GlobalText[3543], 0, 0, 3, 0);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 120, GlobalText[3544], 0, 0, 3, 0);
		break;
	case 3:
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 53, 170, 62, 60, 20);
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 115, 170, 62, 60, 20);

		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 61, GlobalText[3545], 0, 0, 3, 0);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 122, GlobalText[3546], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 77, RenderFrameY + 98, GlobalText[3547], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 77, RenderFrameY + 160, GlobalText[3547], 0, 0, 3, 0);

		m_hp_Potion.Render();
		m_hp_Healt.Render();
		break;
	case 4:
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 53, 170, 62, 60, 20);
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 115, 170, 62, 60, 20);

		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 61, GlobalText[3545], 0, 0, 3, 0);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 122, GlobalText[3517], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 77, RenderFrameY + 98, GlobalText[3547], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 77, RenderFrameY + 160, GlobalText[3547], 0, 0, 3, 0);

		m_hp_Potion.Render();
		m_hp_DrainLife.Render();
		break;
	case 5:
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 53, 170, 62, 60, 20);
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 61, GlobalText[3545], 0, 0, 3, 0);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(RenderFrameX + 77, RenderFrameY + 98, GlobalText[3547], 0, 0, 3, 0);

		m_hp_Potion.Render();
		break;
	case 6:
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 53, 170, 90, 60, 20);
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 145, 170, 67, 60, 20);
		RenderImage(Image_InputNumber, RenderFrameX + 109, RenderFrameY + 186, 28.0, 15.0);

		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 61, GlobalText[3548], 0, 0, 3, 0);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 153, GlobalText[3549], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 66, RenderFrameY + 123, GlobalText[3550], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 40, RenderFrameY + 190, GlobalText[3551], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 136, RenderFrameY + 190, GlobalText[3528], 0, 0, 3, 0);
		m_hp_Party.Render();
		m_preferenceParty.Render();
		m_durationParty[0].Render();
		m_timespace[0].Render();
		break;
	case 7:
		RenderGroupBox(RenderFrameX + 12, RenderFrameY + 55, 170, 67, 60, 20);
		RenderImage(Image_InputNumber, RenderFrameX + 109, RenderFrameY + 96, 28.0, 15.0);
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText(RenderFrameX + 24, RenderFrameY + 63, GlobalText[3549], 0, 0, 3, 0);
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->RenderText(RenderFrameX + 40, RenderFrameY + 100, GlobalText[3551], 0, 0, 3, 0);
		g_pRenderText->RenderText(RenderFrameX + 136, RenderFrameY + 100, GlobalText[3528], 0, 0, 3, 0);
		m_durationParty[1].Render();
		m_timespace[1].Render();
		break;
	default:
		break;
	}

	m_save.Render();
	m_reset.Render();
	m_exit.Render();

	DisableAlphaBlend();
	return true;
}

bool SEASON3B::CNewUIMacroSub::UpdateMouseEvent()
{
	if (BtnProcess() == true)
	{
		return false;
	}
	else
	{
		m_hp_Party.MouseUpdate();
		m_hp_Potion.MouseUpdate();
		m_hp_Healt.MouseUpdate();
		m_hp_DrainLife.MouseUpdate();
		m_timespace[0].DoAction();
		m_timespace[1].DoAction();
	}

	return !CheckMouseIn(m_Pos.x, m_Pos.y, 190, 429);
}

bool SEASON3B::CNewUIMacroSub::UpdateKeyEvent()
{
	if (IsPress(VK_ESCAPE) && g_pNewUISystem->IsVisible(INTERFACE_MACRO_OFICIAL_SUB))
	{
		PlayBuffer(SOUND_CLICK01);
		g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
		return false;
	}
	return true;
}

float SEASON3B::CNewUIMacroSub::GetLayerDepth()
{
	return 3.6f;
}

void SEASON3B::CNewUIMacroSub::EventOrderWindows(double WindowsX, double WindowsY)
{
	float RenderFrameX = (GetWindowsX);

	RenderFrameX -= (WindowsX - m_Pos.x);

	this->SetPos((int)RenderFrameX, m_Pos.y);


	m_hp_Party.SetPos(RenderFrameX + 33, m_Pos.y + 104);
	m_hp_Potion.SetPos(RenderFrameX + 33, m_Pos.y + 78);
	m_hp_Healt.SetPos(RenderFrameX + 33, m_Pos.y + 140);
	m_hp_DrainLife.SetPos(RenderFrameX + 33, m_Pos.y + 140);


	m_preferenceParty.SetPos(RenderFrameX + 20, m_Pos.y + 79); //-- Preference of Party Heal
	m_durationParty[0].SetPos(RenderFrameX + 20, m_Pos.y + 171); //-- Buff Duration for All Party Members
	m_durationParty[1].SetPos(RenderFrameX + 20, m_Pos.y + 81); //-- Buff Duration for All Party Members

	m_timespace[0].SetPosition(RenderFrameX + 114, m_Pos.y + 190);
	m_timespace[1].SetPosition(RenderFrameX + 114, m_Pos.y + 100);

	m_optionGroup[0].SetPos(RenderFrameX + 20, m_Pos.y + 74);
	m_optionGroup[0].SetOptionText(0, 0, 0, GlobalText[3555]);
	m_optionGroup[0].SetOptionText(1, 0, 17, GlobalText[3555]);

	m_optionGroup[1].SetPos(RenderFrameX + 20, m_Pos.y + 134);
	m_optionGroup[1].SetOptionText(0, 0, 0, GlobalText[3555]);
	m_optionGroup[1].SetOptionText(1, 80, 0, GlobalText[3558]);
	m_optionGroup[1].SetOptionText(2, 0, 17, GlobalText[3559]);
	m_optionGroup[1].SetOptionText(3, 80, 17, GlobalText[3560]);

	m_optionGroup[2].SetPos(RenderFrameX + 20, m_Pos.y + 74);
	m_optionGroup[2].SetOptionText(0, 0, 0, GlobalText[3555]);
	m_optionGroup[2].SetOptionText(1, 0, 17, GlobalText[3556]);

	m_optionGroup[3].SetPos(RenderFrameX + 20, m_Pos.y + 134);
	m_optionGroup[3].SetOptionText(0, 0, 0, GlobalText[3557]);
	m_optionGroup[3].SetOptionText(1, 80, 0, GlobalText[3558]);
	m_optionGroup[3].SetOptionText(2, 0, 17, GlobalText[3559]);
	m_optionGroup[3].SetOptionText(3, 80, 17, GlobalText[3560]);

	m_save.SetPos(RenderFrameX + 120, m_Pos.y + 388);
	m_reset.SetPos(RenderFrameX + 65, m_Pos.y + 388);
	m_exit.SetPos(RenderFrameX + 20, m_Pos.y + 388);
}

int SEASON3B::CNewUIMacroSub::GetWindowsType()
{
	return m_WindowsType;
}

void SEASON3B::CNewUIMacroSub::OpenningProcess(int WindowsType)
{
	m_WindowsType = WindowsType;
	m_Character = gCharacterManager.GetBaseClass(Hero->Class);

	switch (m_WindowsType)
	{
	case 1:
	case 2:
		m_Name = GlobalText[3552];
		break;
	case 3:
	case 4:
	case 5:
		m_Name = GlobalText[3553];
		break;
	case 6:
	case 7:
		m_Name = GlobalText[3554];
		break;
	}

	LoadConfig();
}

bool SEASON3B::CNewUIMacroSub::BtnProcess()
{
	if (IsRelease(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 169, m_Pos.y + 7, 13, 12))
	{
		return true;
	}
	else if (m_exit.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
		return true;
	}
	else if (m_save.UpdateMouseEvent() == true)
	{
		SaveConfig();
		g_pNewUISystem->Hide(INTERFACE_MACRO_OFICIAL_SUB);
		return true;
	}
	else if (m_reset.UpdateMouseEvent() == true)
	{
		ResetConfig();
		return true;
	}
	else
	{
		switch (m_WindowsType)
		{
		case 1:
			m_optionGroup[0].UpdateMouseEvent();
			m_optionGroup[1].UpdateMouseEvent();
			break;
		case 2:
			m_optionGroup[2].UpdateMouseEvent();
			m_optionGroup[3].UpdateMouseEvent();
			break;
		case 6:
			m_preferenceParty.UpdateMouseEvent();
			m_durationParty[0].UpdateMouseEvent();
			break;
		case 7:
			m_durationParty[1].UpdateMouseEvent();
			break;
		default:
			break;
		}
	}
	return false;
}

void SEASON3B::CNewUIMacroSub::RenderGroupBox(int x, int y, int width, int height, int headwidth, int headheight)
{
#if MAIN_UPDATE > 603
	EnableAlphaTest();
	RenderColor(x, y + headheight, width, (height - headheight), 0.70000001, 1);
	EndRenderColor();
#else

	EnableAlphaTest();
	glColor4f(0.0, 0.0, 0.0, 0.40000001);
	RenderColor((x + 3), (y + 2), (headwidth - 8), headheight, 0.0, 0);
	glColor4f(0.0, 0.0, 0.0, 0.40000001);
	RenderColor((x + 3), (y + headheight + 2), (width - 7), (height - headheight - 7), 0.0, 0);
	EndRenderColor();

	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_LEFT, x, y, 14.0, 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_RIGHT, (x + headwidth - 14), y, 14.0, 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_RIGHT, (x + width - 14), (headheight + y), 14.0, 14.0);

	RenderImage(IMAGE_MACRO_MAIN_TABLE_BOTTOM_LEFT, x, (y + height - 14), 14.0, 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_BOTTOM_RIGHT, (x + width - 14), (y + height - 14), 14.0, 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_PIXEL, (x + 6), y, (headwidth - 12), 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_PIXEL, (x + 3), (headheight + y), (headwidth - 8), 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_RIGHT_PIXEL, (x + headwidth - 14), (y + 6), 14.0, (headheight - 6));
	RenderImage(IMAGE_MACRO_MAIN_TABLE_TOP_PIXEL, (x + headwidth - 5), (headheight + y), (width - headwidth - 6), 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_RIGHT_PIXEL, (x + width - 14), (y + headheight + 6), 14.0, (height - headheight - 14));
	RenderImage(IMAGE_MACRO_MAIN_TABLE_BOTTOM_PIXEL, (x + 6), (y + height - 14), (width - 12), 14.0);
	RenderImage(IMAGE_MACRO_MAIN_TABLE_LEFT_PIXEL, x, (y + 6), 14.0, (height - 14));
#endif
}

void SEASON3B::CNewUIMacroSub::InitOptionButtonGroup()
{
	m_optionGroup[0].Create(Image_OptionButton, m_Pos.x + 20, m_Pos.y + 74, 15, 15, 2);
	m_optionGroup[0].SetOptionText(0, 0, 0, GlobalText[3555]);
	m_optionGroup[0].SetOptionText(1, 0, 17, GlobalText[3555]);

	m_optionGroup[1].Create(Image_OptionButton, m_Pos.x + 20, m_Pos.y + 134, 15, 15, 4);
	m_optionGroup[1].SetOptionText(0, 0, 0, GlobalText[3555]);
	m_optionGroup[1].SetOptionText(1, 80, 0, GlobalText[3558]);
	m_optionGroup[1].SetOptionText(2, 0, 17, GlobalText[3559]);
	m_optionGroup[1].SetOptionText(3, 80, 17, GlobalText[3560]);

	m_optionGroup[2].Create(Image_OptionButton, m_Pos.x + 20, m_Pos.y + 74, 15, 15, 2);
	m_optionGroup[2].SetOptionText(0, 0, 0, GlobalText[3555]);
	m_optionGroup[2].SetOptionText(1, 0, 17, GlobalText[3556]);

	m_optionGroup[3].Create(Image_OptionButton, m_Pos.x + 20, m_Pos.y + 134, 15, 15, 4);
	m_optionGroup[3].SetOptionText(0, 0, 0, GlobalText[3557]);
	m_optionGroup[3].SetOptionText(1, 80, 0, GlobalText[3558]);
	m_optionGroup[3].SetOptionText(2, 0, 17, GlobalText[3559]);
	m_optionGroup[3].SetOptionText(3, 80, 17, GlobalText[3560]);
}

void SEASON3B::CNewUIMacroSub::AddTextInputBox(CUITextInputBox* Input, int x, int y, int width, int height, int iMaxLength, const char* text)
{
	Input->Init(g_hWnd, width, height, iMaxLength, FALSE);
	Input->SetPosition(x, y);
	Input->SetText(text);
	Input->SetTextColor(255, 0, 0, 0);
	Input->SetBackColor(0, 0, 0, 0);
	Input->SetFont(g_hFont);
	Input->SetOption(1);
	Input->SetState(UISTATE_NORMAL);
}

void SEASON3B::CNewUIMacroSub::AddButton(CNewUIButton* btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, const char* btname, const char* tooltiptext, bool istoppos)
{

#if MAIN_UPDATE > 603
	if (imgindex == IMAGE_IGS_VIEWDETAIL_BTN)
	{
		sy -= 2;
		btn->ChangeButtonImgState(true, BITMAP_ingame_bt01, overflg, isimgwidth, bClickEffect, 0.f, 0.f, 208, 77.f);
	}
	else if (imgindex == IMAGE_MACRO_MAIN_EXIT_BTN)
	{
		sx = 20; sy = 20;
		btn->ChangeButtonImgState(true, BITMAP_btn_exit_00, overflg, isimgwidth, bClickEffect, 0.f, 0.f, 69.f, 72.f);
	}
	else
	{
		btn->ChangeButtonImgState(true, imgindex, overflg, isimgwidth, bClickEffect);
	}
#else
	btn->ChangeButtonImgState(true, imgindex, overflg, isimgwidth, bClickEffect);
#endif

	btn->ChangeButtonInfo(x, y, sx, sy);

	btn->ChangeText(btname);
	btn->ChangeToolTipText(tooltiptext, istoppos);

	if (MoveTxt) btn->MoveTextPos(0, -1);
}

bool SEASON3B::CNewUIMacroSub::GetCheckBoxValue(int index)
{
	bool value = false;

	if (index)
	{
		if (index == 1)
		{
			if (m_Character == Dark_Wizard)
				value = m_durationParty[1].IsSelected();
			else
				value = m_durationParty[0].IsSelected();
		}
	}
	else
	{
		value = m_preferenceParty.IsSelected();
	}
	return value;
}

bool SEASON3B::CNewUIMacroSub::SetCheckBoxValue(int index, bool value)
{
	if (index)
	{
		if (index == 1)
		{
			if (m_Character == Dark_Wizard)
				m_durationParty[1].SetSelected(value);
			else
				m_durationParty[0].SetSelected(value);
		}
	}
	else
	{
		m_preferenceParty.SetSelected(value);
	}
	return value;
}

int SEASON3B::CNewUIMacroSub::GetOptionButtonValue(int index)
{
	return 	m_optionGroup[index].GetIndex();
}

int SEASON3B::CNewUIMacroSub::SetOptionButtonValue(int index, int value)
{
	m_optionGroup[index].SetIndex(value);
	return value;
}

int SEASON3B::CNewUIMacroSub::GetSlideBarValue(int index)
{
	int value = index;

	switch (index)
	{
	case 0:
		value = m_hp_Party.GetSlideLevel();
		break;
	case 1:
		value = m_hp_Potion.GetSlideLevel();
		break;
	case 2:
		value = m_hp_Healt.GetSlideLevel();
		break;
	case 3:
		value = m_hp_DrainLife.GetSlideLevel();
		break;
	default:
		break;
	}
	return value;
}

int SEASON3B::CNewUIMacroSub::SetSlideBarValue(int index, int value)
{
	switch (index)
	{
	case 0:
		m_hp_Party.SetSlideLevel(value);
		break;
	case 1:
		m_hp_Potion.SetSlideLevel(value);
		break;
	case 2:
		m_hp_Healt.SetSlideLevel(value);
		break;
	case 3:
		m_hp_DrainLife.SetSlideLevel(value);
		break;
	default:
		break;
	}
	return value;
}

int SEASON3B::CNewUIMacroSub::GetInputBoxNumber(int index)
{
	if (index == 0)
	{
		char pszText[255];
		memset(pszText, 0, sizeof(pszText));

		if (m_Character == Dark_Wizard)
			m_timespace[1].GetText(pszText);
		else
			m_timespace[0].GetText(pszText);
		return atoi(pszText);
	}
	return 0;
}

int SEASON3B::CNewUIMacroSub::SetInputBoxNumber(int index, int value)
{
	char pszText[255];
	memset(pszText, 0, sizeof(pszText));
	itoa(value, pszText, 0xAu);

	if (!index)
	{
		if (m_Character == Dark_Wizard)
			m_timespace[1].SetText(pszText);
		else
			m_timespace[0].SetText(pszText);
	}

	return value;
}

void SEASON3B::CNewUIMacroSub::LoadConfig()
{
	MUSkill_info* settings;
	MUAction_info* setting001;

	SetCheckBoxValue(1, gmAIController->buff_duration_all_work);

	setting001 = &gmAIController->healParty;
	SetCheckBoxValue(0, setting001->Shutdown);
	SetSlideBarValue(0, (setting001->bRate / 10));

	SetInputBoxNumber(0, gmAIController->space_of_casting_buff);

	SetSlideBarValue(1, (gmAIController->healPotion.bRate / 10));
	SetSlideBarValue(2, (gmAIController->healskillLife.bRate / 10));
	SetSlideBarValue(3, (gmAIController->healskilldrain.bRate / 10));

	settings = &gmAIController->SkillGroup[0];
	SetOptionButtonValue(0, settings->CounterPre);
	SetOptionButtonValue(1, settings->CounterSub);

	settings = &gmAIController->SkillGroup[1];
	SetOptionButtonValue(2, settings->CounterPre);
	SetOptionButtonValue(3, settings->CounterSub);
}

void SEASON3B::CNewUIMacroSub::SaveConfig()
{
	gmAIController->buff_duration_all_work =  this->GetCheckBoxValue(1);
	gmAIController->healParty.Shutdown = this->GetCheckBoxValue(0);
	gmAIController->space_of_casting_buff = this->GetInputBoxNumber(0);

	gmAIController->healPotion.bRate = 10 * this->GetSlideBarValue(1);
	gmAIController->healskillLife.bRate = 10 * this->GetSlideBarValue(2);
	gmAIController->healParty.bRate = 10 * this->GetSlideBarValue(0);
	gmAIController->healskilldrain.bRate = 10 * this->GetSlideBarValue(3);

	gmAIController->SkillGroup[0].CounterPre = this->GetOptionButtonValue(0);
	gmAIController->SkillGroup[0].CounterSub = this->GetOptionButtonValue(1);
	gmAIController->SkillGroup[1].CounterPre = this->GetOptionButtonValue(2);
	gmAIController->SkillGroup[1].CounterSub = this->GetOptionButtonValue(3);
}

void SEASON3B::CNewUIMacroSub::ResetConfig()
{
	switch (m_WindowsType)
	{
	case 1:
		this->SetOptionButtonValue(0, 0);
		this->SetOptionButtonValue(1, 0);
		break;
	case 2:
		this->SetOptionButtonValue(2, 0);
		this->SetOptionButtonValue(3, 0);
		break;
	case 3:
		this->SetSlideBarValue(1, 5);
		this->SetSlideBarValue(2, 5);
		break;
	case 4:
		this->SetSlideBarValue(1, 5);
		this->SetSlideBarValue(3, 5);
		break;
	case 5:
		this->SetSlideBarValue(1, 5);
		break;
	case 6:
		this->SetCheckBoxValue(0, false);
		this->SetCheckBoxValue(1, false);
		this->SetInputBoxNumber(0, 0);
		this->SetSlideBarValue(0, 5);
		break;
	case 7:
		this->SetCheckBoxValue(1, false);
		this->SetInputBoxNumber(0, 0);
		break;
	default:
		break;
	}
}

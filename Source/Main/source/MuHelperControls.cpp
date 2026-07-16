#include "stdafx.h"
#include "MuHelperControls.h"
#include "NewUIMyInventory.h"
#include "ZzzInterface.h"

void SEASON3B::window_nomal(float x, float y, int type, float division)
{
	// The reference 5.2 Helper uses the normal inventory skin for its two windows.
	// Reusing the MuBattle inventory bitmaps preserves the current client theme.
	float frameY = y;
	RenderImage(CNewUIMyInventory::IMAGE_INVENTORY_BACK, x, frameY, WIN_WINDOW_SIZEX, WIN_WINDOW_SIZEY);
	RenderImage((type == 0) ? CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP : CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP2,
		x, frameY, WIN_WINDOW_SIZEX, 64.f);
	frameY += 64.f;
	RenderImage(CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT, x, frameY, 21.f, WIN_WINDOW_SIZEY - 109.f);
	RenderImage(CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT, x + WIN_WINDOW_SIZEX - 21.f, frameY, 21.f, WIN_WINDOW_SIZEY - 109.f);
	frameY += WIN_WINDOW_SIZEY - 109.f;
	RenderImage(CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM, x, frameY, WIN_WINDOW_SIZEX, 45.f);
}

SEASON3B::CCheckBox::CCheckBox()
{
	Initialize();
}

void SEASON3B::CCheckBox::Initialize()
{
	m_TextFont = g_hFont;
	m_Image = 0;
	m_Su = m_Sv = 0.f;
	m_Pos.x = m_Pos.y = 0;
	m_Size.cx = m_Size.cy = 0;
	m_Selected = false;
}

void SEASON3B::CCheckBox::RegisterImgIndex(int image) { m_Image = image; }
void SEASON3B::CCheckBox::ChangeCheckBoxText(unicode::t_string text) { m_Text = text; }
void SEASON3B::CCheckBox::SetPos(int x, int y) { m_Pos.x = x; m_Pos.y = y; }
void SEASON3B::CCheckBox::SetSize(int width, int height) { m_Size.cx = width; m_Size.cy = height; }
const POINT& SEASON3B::CCheckBox::GetPos() { return m_Pos; }

void SEASON3B::CCheckBox::ChangeCheckBoxInfo(int x, int y, int width, int height, float su, float sv)
{
	SetPos(x, y);
	SetSize(width, height);
	m_Su = su;
	m_Sv = sv;
}

bool SEASON3B::CCheckBox::Create(int image, int x, int y, int width, int height, unicode::t_string text)
{
	RegisterImgIndex(image);
	ChangeCheckBoxInfo(x, y, width, height);
	ChangeCheckBoxText(text);
	return true;
}

bool SEASON3B::CCheckBox::Render()
{
	const float stateV = m_Sv + (m_Selected ? 0.f : (float)m_Size.cy);
	SEASON3B::RenderImage(m_Image, (float)m_Pos.x, (float)m_Pos.y,
		(float)m_Size.cx, (float)m_Size.cy, m_Su, stateV);
	g_pRenderText->SetFont(m_TextFont);
	g_pRenderText->RenderText(m_Pos.x + m_Size.cx, m_Pos.y + 4, m_Text.c_str(), 0, 0, RT3_SORT_LEFT, 0);
	return true;
}

bool SEASON3B::CCheckBox::UpdateMouseEvent()
{
	if (SEASON3B::IsRelease(VK_LBUTTON) && SEASON3B::CheckMouseIn(m_Pos.x, m_Pos.y, m_Size.cx, m_Size.cy))
	{
		m_Selected = !m_Selected;
		return true;
	}
	return false;
}

SEASON3B::stMacroUIImage::stMacroUIImage()
	: output_width(0.f), output_height(0.f), m_ImgIndex(-1)
{
	m_Pos.x = m_Pos.y = 0;
}
const POINT& SEASON3B::stMacroUIImage::GetPos() { return m_Pos; }
void SEASON3B::stMacroUIImage::SetPos(int x, int y) { m_Pos.x = x; m_Pos.y = y; }
void SEASON3B::stMacroUIImage::Register(int image, int x, int y, int width, int height)
{
	m_ImgIndex = image; SetPos(x, y); output_width = (float)width; output_height = (float)height;
}
void SEASON3B::stMacroUIImage::Render()
{
	SEASON3B::RenderImage(m_ImgIndex, (float)m_Pos.x, (float)m_Pos.y, output_width, output_height);
}

SEASON3B::stMacroUIText::stMacroUIText() { m_Pos.x = m_Pos.y = 0; }
const POINT& SEASON3B::stMacroUIText::GetPos() { return m_Pos; }
void SEASON3B::stMacroUIText::SetPos(int x, int y) { m_Pos.x = x; m_Pos.y = y; }
void SEASON3B::stMacroUIText::Register(int x, int y, const char* text) { SetPos(x, y); m_Name = text; }
void SEASON3B::stMacroUIText::Render()
{
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y, m_Name.c_str(), 0, 0, RT3_SORT_LEFT, 0);
}

SEASON3B::CSlideBar::CSlideBar()
	: m_Value(0), m_MaxLength(0), m_MinLength(0), m_Image(0), m_Background(0), m_Width(0), m_Height(0)
{
	m_Pos.x = m_Pos.y = 0;
}
const POINT& SEASON3B::CSlideBar::GetPos() { return m_Pos; }
void SEASON3B::CSlideBar::SetPos(int x, int y) { m_Pos.x = x; m_Pos.y = y; }
void SEASON3B::CSlideBar::Create(int background, int image, int x, int y, short width, short height, short maxLength, short start)
{
	m_Background = background; m_Image = image; SetPos(x, y); m_Width = width; m_Height = height;
	m_MaxLength = maxLength; m_MinLength = start; m_Value = start;
}
bool SEASON3B::CSlideBar::MouseUpdate()
{
	if (m_MaxLength <= 0 || m_Width <= 0) return false;
	const int step = m_Width / m_MaxLength;
	if (SEASON3B::CheckMouseIn(m_Pos.x - step, m_Pos.y, step + m_Width, m_Height) && SEASON3B::IsRepeat(VK_LBUTTON))
	{
		const int current = MouseX - m_Pos.x;
		m_Value = (short)(current >= m_MinLength ? (current * m_MaxLength / m_Width) + 1 : m_MinLength);
		m_Value = (short)max((int)m_MinLength, min((int)m_MaxLength, (int)m_Value));
	}
	return true;
}
void SEASON3B::CSlideBar::Render()
{
	SEASON3B::RenderImage(m_Background, (float)m_Pos.x, (float)m_Pos.y, (float)m_Width, (float)m_Height);
	if (m_Value > 0)
		SEASON3B::RenderImage(m_Image, (float)m_Pos.x, (float)m_Pos.y, (float)m_Width * m_Value / m_MaxLength, (float)m_Height);
}
int SEASON3B::CSlideBar::GetSlideLevel() { return m_Value; }
void SEASON3B::CSlideBar::SetSlideLevel(short value) { m_Value = (short)max((int)m_MinLength, min((int)m_MaxLength, (int)value)); }

SEASON3B::COptionButtonGroup::COptionButtonGroup() : m_Index(0) { m_Pos.x = m_Pos.y = 0; }
SEASON3B::COptionButtonGroup::~COptionButtonGroup()
{
	for (size_t i = 0; i < m_Boxes.size(); ++i) delete m_Boxes[i];
	m_Boxes.clear();
}
const POINT& SEASON3B::COptionButtonGroup::GetPos() { return m_Pos; }
void SEASON3B::COptionButtonGroup::SetPos(int x, int y) { m_Pos.x = x; m_Pos.y = y; }
void SEASON3B::COptionButtonGroup::Create(int image, int x, int y, int width, int height, BYTE count, float su, float sv)
{
	SetPos(x, y);
	for (size_t i = 0; i < m_Boxes.size(); ++i) delete m_Boxes[i];
	m_Boxes.clear();
	for (BYTE i = 0; i < count; ++i)
	{
		CCheckBox* box = new CCheckBox;
		box->RegisterImgIndex(image);
		box->ChangeCheckBoxInfo(0, 0, width, height, su, sv);
		m_Boxes.push_back(box);
	}
}
void SEASON3B::COptionButtonGroup::SetOptionText(BYTE index, int offsetX, int offsetY, const char* text)
{
	if (index >= m_Boxes.size()) return;
	m_Boxes[index]->SetPos(m_Pos.x + offsetX, m_Pos.y + offsetY);
	m_Boxes[index]->ChangeCheckBoxText(text);
}
bool SEASON3B::COptionButtonGroup::UpdateMouseEvent()
{
	for (size_t i = 0; i < m_Boxes.size(); ++i)
		if (m_Boxes[i]->UpdateMouseEvent() && m_Boxes[i]->IsSelected()) { SetIndex((int)i); return true; }
	return false;
}
bool SEASON3B::COptionButtonGroup::Render()
{
	for (size_t i = 0; i < m_Boxes.size(); ++i) m_Boxes[i]->Render();
	return true;
}
void SEASON3B::COptionButtonGroup::SetIndex(int index)
{
	m_Index = (index >= 0 && index < (int)m_Boxes.size()) ? index : 0;
	for (size_t i = 0; i < m_Boxes.size(); ++i) m_Boxes[i]->SetSelected((int)i == m_Index);
}
int SEASON3B::COptionButtonGroup::GetIndex() { return m_Index; }

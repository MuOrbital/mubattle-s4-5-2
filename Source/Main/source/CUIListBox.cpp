#include "stdafx.h"
#include "UIControls.h"
#include "CUIListBox.h"

SEASON3B::CUIListBox::CUIListBox()
	: m_iViewCount(0), m_iMaxLength(0), m_iPosX(0), m_iPosY(0),
	  m_Width(0), m_Height(0), m_SelectedIndex(-1)
{
}

SEASON3B::CUIListBox::~CUIListBox()
{
	m_Items.clear();
}

bool SEASON3B::CUIListBox::Create(int x, int y, int width, int height, int viewcount, int maxLength)
{
	SetPos(x, y);
	SetSize(width, height);
	m_iViewCount = viewcount;
	m_iMaxLength = maxLength;
	m_SelectedIndex = -1;
	m_ScrollBar.Create(x + width - 10, y, height - 5);
	m_ScrollBar.SetPercent(0.0);
	return true;
}

void SEASON3B::CUIListBox::ClearText()
{
	m_Items.clear();
	m_SelectedIndex = -1;
	m_ScrollBar.SetPercent(0.0);
}

bool SEASON3B::CUIListBox::AddText(const unicode::t_char* text)
{
	if (text == NULL || text[0] == '\0' || CheckText(text))
		return false;

	if (m_Items.size() >= (size_t)m_iMaxLength)
	{
		unicode::t_char message[255] = { 0 };
		unicode::_sprintf(message, GlobalText[3570], m_iMaxLength);
		SEASON3B::CreateOkMessageBox(message, -1);
		return false;
	}

	m_Items.push_back(text);
	return true;
}

bool SEASON3B::CUIListBox::CheckText(const unicode::t_char* text)
{
	return std::find(m_Items.begin(), m_Items.end(), text) != m_Items.end();
}

const unicode::t_char* SEASON3B::CUIListBox::GetText(int index)
{
	return (index >= 0 && index < (int)m_Items.size()) ? m_Items[index].c_str() : NULL;
}

const std::vector<unicode::t_string>& SEASON3B::CUIListBox::GetData()
{
	return m_Items;
}

void SEASON3B::CUIListBox::DelSelectedListBoxText()
{
	if (m_SelectedIndex >= 0 && m_SelectedIndex < (int)m_Items.size())
	{
		m_Items.erase(m_Items.begin() + m_SelectedIndex);
		m_SelectedIndex = -1;
		return;
	}
	SEASON3B::CreateOkMessageBox(GlobalText[m_Items.empty() ? 3571 : 3572], -1);
}

void SEASON3B::CUIListBox::RenderTable(int x, int y, int width, int height)
{
	EnableAlphaTest();
	glColor4f(0.f, 0.f, 0.f, 0.4f);
	RenderColor(x + 3, y + 2, width - 7, height - 7, 0.f, 0);
	EndRenderColor();
	RenderImage(IMAGE_TABLE_TOP_LEFT, x, y, 14.f, 14.f);
	RenderImage(IMAGE_TABLE_TOP_RIGHT, x + width - 14, y, 14.f, 14.f);
	RenderImage(IMAGE_TABLE_BOTTOM_LEFT, x, y + height - 14, 14.f, 14.f);
	RenderImage(IMAGE_TABLE_BOTTOM_RIGHT, x + width - 14, y + height - 14, 14.f, 14.f);
	RenderImage(IMAGE_TABLE_TOP_PIXEL, x + 6, y, width - 12, 14.f);
	RenderImage(IMAGE_TABLE_RIGHT_PIXEL, x + width - 14, y + 6, 14.f, height - 14);
	RenderImage(IMAGE_TABLE_BOTTOM_PIXEL, x + 6, y + height - 14, width - 12, 14.f);
	RenderImage(IMAGE_TABLE_LEFT_PIXEL, x, y + 6, 14.f, height - 14);
}

bool SEASON3B::CUIListBox::UpdateMouseEvent()
{
	const int itemCount = (int)m_Items.size();
	int first = itemCount > m_iViewCount
		? (int)((itemCount - m_iViewCount) * m_ScrollBar.GetPercent()) : 0;

	for (int row = 0, i = first; i < itemCount && row < m_iViewCount; ++i, ++row)
	{
		if (IsPress(VK_LBUTTON) && CheckMouseIn(m_iPosX + 10, m_iPosY + 12 * row + 9, m_Width, 12))
			m_SelectedIndex = i;
	}

	if (CheckMouseIn(m_iPosX, m_iPosY, m_Width, m_Height))
	{
		double percent = m_ScrollBar.GetPercent();
		if (MouseWheel < 0) { MouseWheel = 0; percent += 0.1; }
		else if (MouseWheel > 0) { MouseWheel = 0; percent -= 0.1; }
		m_ScrollBar.SetPercent(max(0.0, min(1.0, percent)));
	}

	if (itemCount > m_iViewCount)
		m_ScrollBar.UpdateMouseEvent();
	return false;
}

bool SEASON3B::CUIListBox::Update()
{
	return true;
}

bool SEASON3B::CUIListBox::Render(bool renderTable)
{
	if (renderTable)
		RenderTable(m_iPosX, m_iPosY, m_Width, m_Height);

	const int itemCount = (int)m_Items.size();
	const int first = itemCount > m_iViewCount
		? (int)((itemCount - m_iViewCount) * m_ScrollBar.GetPercent()) : 0;
	g_pRenderText->SetTextColor(-1);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	for (int row = 0, i = first; i < itemCount && row < m_iViewCount; ++i, ++row)
	{
		if (m_SelectedIndex == i)
		{
			EnableAlphaTest();
			glColor4f(1.f, 1.f, 0.f, 0.5f);
			RenderColor(m_iPosX + 8, m_iPosY + 12 * row + 7, m_Width - 16, 14, 0.f, 0);
			EndRenderColor();
		}
		g_pRenderText->RenderText(m_iPosX + 10, m_iPosY + 12 * row + 9, m_Items[i].c_str());
	}

	if (itemCount > m_iViewCount)
		m_ScrollBar.Render();
	return true;
}

void SEASON3B::CUIListBox::SetPos(int x, int y)
{
	m_iPosX = x;
	m_iPosY = y;
}

void SEASON3B::CUIListBox::SetSize(int width, int height)
{
	m_Width = width;
	m_Height = height;
}


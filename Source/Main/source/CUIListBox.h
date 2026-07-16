#pragma once
#include "NewUIScrollBar.h"

namespace SEASON3B
{
	class CUIListBox
	{
		enum IMAGE_LIST
		{
			IMAGE_TABLE_TOP_LEFT = BITMAP_INTERFACE_NEW_INVENTORY_BASE_BEGIN + 1,
			IMAGE_TABLE_TOP_RIGHT,
			IMAGE_TABLE_BOTTOM_LEFT,
			IMAGE_TABLE_BOTTOM_RIGHT,
			IMAGE_TABLE_TOP_PIXEL,
			IMAGE_TABLE_BOTTOM_PIXEL,
			IMAGE_TABLE_LEFT_PIXEL,
			IMAGE_TABLE_RIGHT_PIXEL,
		};

	public:
		CUIListBox();
		virtual ~CUIListBox();
		bool Create(int x, int y, int width, int height, int viewcount, int maxLength);
		void ClearText();
		bool AddText(const unicode::t_char* text);
		bool CheckText(const unicode::t_char* text);
		const unicode::t_char* GetText(int index);
		const std::vector<unicode::t_string>& GetData();
		void DelSelectedListBoxText();
		void RenderTable(int x, int y, int width, int height);
		bool UpdateMouseEvent();
		bool Update();
		bool Render(bool renderTable = true);
		void SetPos(int x, int y);
		void SetSize(int width, int height);

	private:
		CNewUIScrollBar m_ScrollBar;
		int m_iViewCount;
		int m_iMaxLength;
		std::vector<unicode::t_string> m_Items;
		int m_iPosX;
		int m_iPosY;
		int m_Width;
		int m_Height;
		int m_SelectedIndex;
	};
}


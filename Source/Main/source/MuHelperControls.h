#pragma once
#include "MuHelperDefines.h"
#include "NewUICommon.h"
#include "UIControls.h"

namespace SEASON3B
{
	// Draws the standard 190x429 window used by the official Helper configuration.
	void window_nomal(float x, float y, int type = 0, float division = 0.f);

	class stSkillList
	{
	public:
		stSkillList() : iIndex(-1), iX(0), iY(0), Type(0) {}
		int iIndex;
		int iX;
		int iY;
		int Type;
	};

	class CCheckBox
	{
	public:
		CCheckBox();
		void Initialize();
		void RegisterImgIndex(int image);
		void ChangeCheckBoxInfo(int x, int y, int width, int height, float su = 0.f, float sv = 0.f);
		void ChangeCheckBoxText(unicode::t_string text);
		bool Create(int image, int x, int y, int width, int height, unicode::t_string text);
		bool Render();
		bool UpdateMouseEvent();
		const POINT& GetPos();
		void SetPos(int x, int y);
		void SetSize(int width, int height);
		bool IsSelected() const { return m_Selected; }
		void SetSelected(bool selected) { m_Selected = selected; }

	private:
		HFONT m_TextFont;
		int m_Image;
		float m_Su;
		float m_Sv;
		POINT m_Pos;
		SIZE m_Size;
		unicode::t_string m_Text;
		bool m_Selected;
	};

	class stMacroUIImage
	{
	public:
		stMacroUIImage();
		const POINT& GetPos();
		void SetPos(int x, int y);
		void Register(int image, int x, int y, int width, int height);
		void Render();
		POINT m_Pos;
		float output_width;
		float output_height;
		int m_ImgIndex;
	};

	class stMacroUIText
	{
	public:
		stMacroUIText();
		const POINT& GetPos();
		void SetPos(int x, int y);
		void Register(int x, int y, const char* text);
		void Render();
		POINT m_Pos;
		unicode::t_string m_Name;
	};

	class CSlideBar
	{
	public:
		CSlideBar();
		const POINT& GetPos();
		void SetPos(int x, int y);
		void Create(int background, int image, int x, int y, short width, short height, short maxLength, short start);
		bool MouseUpdate();
		void Render();
		int GetSlideLevel();
		void SetSlideLevel(short value);

	private:
		POINT m_Pos;
		short m_Value;
		short m_MaxLength;
		short m_MinLength;
		int m_Image;
		int m_Background;
		short m_Width;
		short m_Height;
	};

	class COptionButtonGroup
	{
	public:
		COptionButtonGroup();
		~COptionButtonGroup();
		const POINT& GetPos();
		void SetPos(int x, int y);
		void Create(int image, int x, int y, int width, int height, BYTE count, float su = 0.f, float sv = 0.f);
		void SetOptionText(BYTE index, int offsetX, int offsetY, const char* text);
		bool UpdateMouseEvent();
		bool Render();
		void SetIndex(int index);
		int GetIndex();

	private:
		std::vector<CCheckBox*> m_Boxes;
		int m_Index;
		POINT m_Pos;
	};
}

#if !defined(AFX_NEWUIMACROSUB_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_)
#define AFX_NEWUIMACROSUB_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_
#pragma once
#include "MuHelperDefines.h"
#include "NewUIBase.h"
#include "NewUIButton.h"
#include "NewUIManager.h"
#include "UIControls.h"
#include "NewUIMyInventory.h"
#include "NewUIInventoryCtrl.h"
#include "MuHelperControls.h"



namespace SEASON3B
{
	class CNewUIMacroSub : public CNewUIObj
	{
		enum IMAGE_LIST
		{
			IMAGE_MACRO_MAIN_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,	// Reference
			IMAGE_MACRO_MAIN_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_MACRO_MAIN_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_MACRO_MAIN_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_MACRO_MAIN_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_MACRO_MAIN_EXIT_BTN = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,
			IMAGE_MACRO_MAIN_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,	//. newui_item_table01(L).tga (14,14)
			IMAGE_MACRO_MAIN_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,	//. newui_item_table01(R).tga (14,14)
			IMAGE_MACRO_MAIN_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,	//. newui_item_table02(L).tga (14,14)
			IMAGE_MACRO_MAIN_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,	//. newui_item_table02(R).tga (14,14)
			IMAGE_MACRO_MAIN_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,			//. newui_item_table03(up).tga (1, 14)
			IMAGE_MACRO_MAIN_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,	//. newui_item_table03(dw).tga (1,14)
			IMAGE_MACRO_MAIN_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,		//. newui_item_table03(L).tga (14,1)
			IMAGE_MACRO_MAIN_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,
			IMAGE_MACRO_MAIN_CHECK = BITMAP_OPTION_BEGIN + 5,
			IMAGE_OPTION_VOLUME_BACK = BITMAP_OPTION_BEGIN + 8,
			IMAGE_OPTION_VOLUME_COLOR = BITMAP_OPTION_BEGIN + 9,
			IMAGE_CHAINFO_BTN_STAT = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN + 1,
			Image_RangeMinus = BITMAP_MACRO_MAIN_BEGIN,
			Image_OptionButton = BITMAP_MACRO_MAIN_BEGIN + 1,
			Image_InputNumber = BITMAP_MACRO_MAIN_BEGIN + 2,
			Image_InputString = BITMAP_MACRO_MAIN_BEGIN + 3,

			IMAGE_IGS_VIEWDETAIL_BTN = BITMAP_INGAMESHOP_FRAME + 10,
			IMAGE_MINIMAPALPBTN = BITMAP_CURSEDTEMPLE_BEGIN + 4,
			//IMAGE_SKILLBOX = CNewUISkillList::IMAGE_SKILLBOX,
			IMAGE_MVP_NUMBER = BITMAP_INTERFACE_CRYWOLF_BEGIN + 34,
		};
	public:
		CNewUIMacroSub();
		virtual~CNewUIMacroSub();
	private:
		/*+0012*/ CNewUIManager* m_pNewUIMng;
		/*+0016*/ unicode::t_string m_Name;
		/*+0044*/ POINT m_Pos;
		/*+0052*/ int m_WindowsType;
		/*+0056*/ int m_Character;
		/*+0060*/ int unknown60;
		/*+0064*/ int unknown64;
		/*+0068*/ CSlideBar m_hp_Party;
		/*+0100*/ CSlideBar m_hp_Potion;
		/*+0132*/ CSlideBar m_hp_Healt;
		/*+0164*/ CSlideBar m_hp_DrainLife;
		/*+0196*/ CCheckBox m_preferenceParty;
		/*+0284*/ CCheckBox m_durationParty[2];
		/*+0372*/ CUITextInputBox m_timespace[2];
		/*+0372*/ COptionButtonGroup m_optionGroup[4];
		/*+0908*/ CNewUIButton m_save;
		/*+1080*/ CNewUIButton m_reset;
		/*+1252*/ CNewUIButton m_exit;
	public:
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		void Initialize();

		void SetPos(int x, int y);
		void LoadImages();
		void UnloadImages();

		bool Update();
		bool Render();
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		float GetLayerDepth();
		void EventOrderWindows(double WindowsX, double WindowsY);
		int GetWindowsType();
		void OpenningProcess(int WindowsType);
	private:
		bool BtnProcess();
		void RenderGroupBox(int x, int y, int width, int height, int headwidth, int headheight);

		void InitOptionButtonGroup();
		void AddTextInputBox(CUITextInputBox* Input, int x, int y, int width, int height, int iMaxLength, const char* text);
		void AddButton(CNewUIButton* btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, const char* btname, const char* tooltiptext, bool istoppos);

		bool GetCheckBoxValue(int index);
		bool SetCheckBoxValue(int index, bool value);
		int GetOptionButtonValue(int index);
		int SetOptionButtonValue(int index, int value);
		int GetSlideBarValue(int index);
		int SetSlideBarValue(int index, int value);
		int GetInputBoxNumber(int index);
		int SetInputBoxNumber(int index, int value);
	public:
		void LoadConfig();
		void SaveConfig();
		void ResetConfig();
	};
}

#endif //AFX_NEWUIMACROSUB_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_

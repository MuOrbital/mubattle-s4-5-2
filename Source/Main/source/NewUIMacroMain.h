#if !defined(AFX_NEWUIMACROMAIN_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_)
#define AFX_NEWUIMACROMAIN_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_
#pragma once
#include "MuHelperDefines.h"
#include "NewUIBase.h"
#include "NewUIButton.h"
#include "NewUIManager.h"
#include "UIControls.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "CUIListBox.h"
#include "MuHelperControls.h"

namespace SEASON3B
{

	class CNewUIMacroMain : public CNewUIObj
	{
		enum __ButtonMacro
		{
			button_Hunting_Max = 0,
			button_Hunting_Min = 1,
			button_PickUp_Max = 9,
			button_PickUp_Min = 10,
			button_AddText = 11,
			button_DelText = 12,
			button_Save = 13,
			button_Initialize = 14,
			button_Exit = 15,
		};
		enum __stCheckBoxGroup
		{
			checkbox_AutoPotion = 0,
			checkbox_LongDistance,
			checkbox_ReturnOriginal,
			checkbox_DelayActiveSkill2,
			checkbox_ConAtiveSkill2,
			checkbox_DelayActiveSkill3,
			checkbox_ConAtiveSkill3,
			checkbox_ComboATK,
			checkbox_BuffDuration,
			checkbox_Use_DarkSpirit,
			checkbox_PartyBuffDuration,
			checkbox_AutoHeal,
			checkbox_DrainLife,
			//--
			checkbox_RepairItem,
			checkbox_Pick_NearItems,
			checkbox_Pick_SelectItems,
			checkbox_Pick_JewelGem,
			checkbox_Pick_SetItem,
			checkbox_Pick_Zen,
			checkbox_Pick_Excellent,
			checkbox_Pick_AddExtra,
			//--
			checkbox_PartyAutoRequestFriend,
			checkbox_UseElitePotion,
			checkbox_PartyAutoRequestGuild,
			checkbox_follow_party_member,
		};
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
			IMAGE_CHAINFO_BTN_STAT = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN + 1,
			Image_RangeMinus = BITMAP_MACRO_MAIN_BEGIN,
			Image_OptionButton = BITMAP_MACRO_MAIN_BEGIN + 1,
			Image_InputNumber = BITMAP_MACRO_MAIN_BEGIN + 2,
			Image_InputString = BITMAP_MACRO_MAIN_BEGIN + 3,

			IMAGE_IGS_VIEWDETAIL_BTN = BITMAP_INGAMESHOP_FRAME + 10,
			IMAGE_MINIMAPALPBTN = BITMAP_CURSEDTEMPLE_BEGIN + 4,
			IMAGE_SKILLBOX = CNewUISkillList::IMAGE_SKILLBOX,
			IMAGE_MVP_NUMBER = BITMAP_INTERFACE_CRYWOLF_BEGIN + 34,
		};

		typedef std::map<int, CCheckBox*> MAPE_CHECKBOX;
		typedef std::map<int, CNewUIButton*> MAPE_BUTTON;
		typedef std::map<int, CUITextInputBox*> MAPE_TXTINPUT;
		typedef std::map<int, stMacroUIText*> MAPE_MACROTXT;
		typedef std::map<int, stMacroUIImage*> MAPE_MACROIMG;
		typedef std::map<int, COptionButtonGroup*> MAPE_BTNGROUP;
	public:
		CNewUIMacroMain();
		virtual~CNewUIMacroMain();
	private:
		CNewUIManager* m_pNewUIMng;
		CNewUIRadioGroupButton m_iTab;
		CUIListBox m_ListBox;
		POINT m_Pos;
		MAPE_CHECKBOX m_checkBox;
		MAPE_CHECKBOX m_checkBoxGroup[MAX_CLASS_PLAYER];
		MAPE_BTNGROUP m_RadioOption;
		MAPE_BTNGROUP m_RadioOptionGroup[MAX_CLASS_PLAYER];
		MAPE_TXTINPUT m_InputBox;
		MAPE_TXTINPUT m_InputBoxGroup[MAX_CLASS_PLAYER];
		MAPE_BUTTON m_Buttons;
		MAPE_BUTTON m_buttonGroup[MAX_CLASS_PLAYER];
		MAPE_MACROTXT m_stMacroUIText;
		MAPE_MACROTXT m_MacroUITextGroup[MAX_CLASS_PLAYER];
		MAPE_MACROIMG m_stMacroUIImage;
		MAPE_MACROIMG m_MacroUIImageGroup[MAX_CLASS_PLAYER];
		std::list<stSkillList*> m_stSkillList[2];
		POINT m_stSkillListPos[2];
		int m_SkillATTK[3];
		int m_SkillBuff[3];
		bool IsUsedSkill[6];
		bool tooltipvisible;
		int m_iRenderSkillInfoType;
		int m_iRenderSkillInfoPosX;
		int m_iRenderSkillInfoPosY;
		int m_iRenderListType;
		int m_temp_skills;
		int m_RangeHunting;
		int m_RangePickUp;
		int m_iNumCurOpenTab;
		int m_iCurrentClass;
	public:
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		//--
		bool UpdateKeyEvent();
		bool UpdateMouseEvent();
		bool UpdateButtons();
		bool Update();
		bool Render();
		float GetLayerDepth();

		void EventOrderWindows(double WindowsX, double WindowsY);
		void RenderFrame();

		void SetPos(int x, int y);
		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);
		bool RenderSkillInfo();
		void OpenningProcess();
		void ClosingProcess();
	private:
		void InitButtons();
		void InitCheckBoxs();
		void InitInputBoxs();
		void InitImages();
		void InitTexts();
		void InitOptionButtonGroup();
		//----------------------------------
		void RenderTable(float X, float Y, float Width, float Height);
		void LoadImages();
		void UnloadImages();
		void LoadSkillList(int sType);
		//----------------------------------
		void SetPosButtons(int x, int y, BYTE Number);
		void SetPosCheckBoxs(int x, int y, BYTE Number);
		void SetPosInputBoxs(int x, int y, BYTE Number);
		void SetPosImages(int x, int y, BYTE Number);
		void SetPosTexts(int x, int y, BYTE Number);
		void SetPosOptionButtonGroup(int x, int y);


		int GetSkillIndex(int sType);
		bool GetCheckBoxValue(int Number);
		void SetCheckBoxValue(int Number, bool iVal);
		int GetOptionButtonValue(int Number);
		void SetOptionButtonValue(int Number, int iVal);
		int GetInputBoxNumber(int Number);
		void GetInputBoxText(int Number, char* pszText);
		void SetInputBoxNumber(int Number, int iVal);
		void SetInputBoxText(int Number, const char* pszText);
		//----------------------------------
		void AddOptionButtonMapInGroup(COptionButtonGroup* option, BYTE Number);
		void AddCheckBoxMap(int imgindex, int x, int y, int width, int height, const char* text, BYTE Number);
		void AddTextInputMap(int x, int y, int width, int height, int iMaxLength, int iOption, const char* text, BYTE Number);
		void AddButtonMap(int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, const char* btname, const char* tooltiptext, BYTE Number);
		void AddTextMap(int x, int y, const char* text, BYTE Number);
		void AddImageMap(int imgindex, float x, float y, float width, float height, int Number);
		//----------------------------------
		void AddOptionButtonGroupMapByClass(int sClass, BYTE Number);
		void AddCheckBoxMapByClass(int sClass, BYTE Number);
		void AddTextInputMapByClass(int sClass, BYTE Number);
		void AddButtonMapByClass(int sClass, BYTE Number);
		void AddTextMapByClass(int sClass, BYTE Number);
		void AddImageMapByClass(int sClass, BYTE Number);
		//----------------------------------
		bool ImageMapUpdateMouseEvent(BYTE Number);
		bool UpdateMouseEventSkillList(int* skillIndex);
		bool UpdateMouseEventSelectedSkill();
		void RenderSkillList();
		void CloseSkillList();
		void CloseSkillListExceptOne(bool* success);
		bool IsUsedSkillList();
		void RegisterSkillIndex(int skillIndex);
		void GetImagePositionInImageMap(POINT* pos, BYTE Number);
		void LoadConfig();
		void SaveConfig();
		void ResetConfig();
		int GetSkillListOffset();

		template<typename T>
		void ClearMap(T& map) {
			for (T::iterator& pair = map.begin(); pair != map.end(); pair++)
			{
				SAFE_DELETE(pair->second);
			}
			map.clear();
		};
		//void ClearMap(MAPE_BTNGROUP& map);
		//void ClearMap(MAPE_TXTINPUT& map);
		//void ClearMap(MAPE_BUTTON& map);
		//void ClearMap(MAPE_MACROTXT& map);
		//void ClearMap(MAPE_MACROIMG& map);
		/*void SEASON3B::CNewUIButton::RegisterButtonState(BUTTON_STATE eventstate, int imgindex, int btstate, float su, float sv, float uw, float vh)
{
	ButtonInfo btinfo;
	btinfo.s_ImgIndex = imgindex;
	btinfo.s_BTstate = btstate;

	btinfo.su = su;
	btinfo.sv = sv;
	btinfo.uw = uw;
	btinfo.vh = vh;

	m_ButtonInfo.insert(ButtonStateMap::value_type(eventstate, btinfo));
}*/
	};
}

#endif //AFX_NEWUIMACROMAIN_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_

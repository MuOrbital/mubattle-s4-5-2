// NewUIMyQuestInfoWindow.h: interface for the CNewUIMyQuestInfoWindow class.
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
#include "UIControls.h"
#include "MoveWindow.h"

#ifdef PJH_NEW_SERVER_SELECT_MAP
typedef std::list<DWORD> DWordList;
#endif

namespace SEASON3B
{
	class CNewUIMyQuestInfoWindow  : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_MYQUEST_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,			// newui_msgbox_back.jpg
			#ifdef PJH_NEW_SERVER_SELECT_MAP
			IMAGE_MYQUEST_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,		//. newui_item_back01.tga	(190,64)
			#else
			IMAGE_MYQUEST_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP2,		// newui_item_back04.tga	(190,64)
			#endif
			IMAGE_MYQUEST_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,		// newui_item_back02-l.tga	(21,320)
			IMAGE_MYQUEST_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,	// newui_item_back02-r.tga	(21,320)
			IMAGE_MYQUEST_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,	// newui_item_back03.tga	(190,45)
			IMAGE_MYQUEST_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,	//. newui_exit_00.tga
			#ifndef PJH_NEW_SERVER_SELECT_MAP
			IMAGE_MYQUEST_TAB_BUTTON = CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,	
			#endif	// ASG_MOD_UI_QUEST_INFO
			IMAGE_MYQUEST_LINE = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN,
			#ifdef PJH_NEW_SERVER_SELECT_MAP
			IMAGE_MYQUEST_BTN_OPEN = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+1,
			IMAGE_MYQUEST_BTN_GIVE_UP = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+2,
			IMAGE_MYQUEST_TAB_BACK = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+3,
			IMAGE_MYQUEST_TAB_SMALL = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+4,
			IMAGE_MYQUEST_TAB_BIG = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+5,
			#endif
		};
		enum
		{
			MYQUESTINFO_WINDOW_WIDTH = 190,
			MYQUESTINFO_WINDOW_HEIGHT = 429,
		};

		enum TAB_BUTTON_INDEX
		{
			TAB_NON	= -1,
			TAB_QUEST = 0,
			TAB_JOB_CHANGE,
			TAB_CASTLE_TEMPLE,
		};
		
	public:
		CNewUIMyQuestInfoWindow();
		virtual ~CNewUIMyQuestInfoWindow();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		
		float GetLayerDepth();	//. 3.3f
		
		void OpenningProcess();
		void ClosingProcess();
		#ifdef PJH_NEW_SERVER_SELECT_MAP
		void UnselectQuestList();
		void SetCurQuestList(DWordList* pDWordList);
		void SetSelQuestSummary();
		void SetSelQuestRequestReward();

		void QuestOpenBtnEnable(bool bEnable);
		void QuestGiveUpBtnEnable(bool bEnable);

		DWORD GetSelQuestIndex();
		#endif

	private:
		void LoadImages();
		void UnloadImages();
		
		void SetButtonInfo();
		
		bool BtnProcess();

		void RenderFrame();
		void RenderSubjectTexts();
		#ifdef PJH_NEW_SERVER_SELECT_MAP
		void RenderQuestInfo();
		void RenderJobChangeContents();
		void RenderJobChangeState();
		void RenderCastleInfo();
		void RenderTempleInfo();
		#else
		void RenderQuestTexts();
		void RenderTabQuest();
		void RenderTabCastle();
		void RenderTabTemple();
		void RenderButtons();
		#endif

		#ifdef PJH_NEW_SERVER_SELECT_MAP
		TAB_BUTTON_INDEX UpdateTabBtn();
		void RenderTabBtn();

		void SetMessage(int nGlobalTextIndex);
		#endif

	private:
		CNewUIManager* m_pNewUIMng;
		cMoveWindow m_MoveWindow;
		POINT m_Pos;
		#ifndef PJH_NEW_SERVER_SELECT_MAP
		CNewUIRadioGroupButton m_BtnTab;
		#endif
		CNewUIButton m_BtnExit;
		#ifdef PJH_NEW_SERVER_SELECT_MAP
		CNewUIButton	m_btnQuestOpen;
		CNewUIButton	m_btnQuestGiveUp;

		TAB_BUTTON_INDEX m_eTabBtnIndex;

		CUICurQuestListBox		m_CurQuestListBox;
		CUIQuestContentsListBox	m_QuestContentsListBox;
		char			m_aszMsg[2][64];
		int				m_nMsgLine;
		#endif
	};
}

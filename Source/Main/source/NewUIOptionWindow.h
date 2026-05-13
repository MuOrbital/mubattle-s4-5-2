// NewUIOptionWindow.h: interface for the CNewUIOptionWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIOPTIONWINDOW_H__1469FA1D_7C15_4AFE_AD6E_59C303E72BC0__INCLUDED_)
#define AFX_NEWUIOPTIONWINDOW_H__1469FA1D_7C15_4AFE_AD6E_59C303E72BC0__INCLUDED_

#pragma once

#include "NewUIManager.h"
#include "NewUIMyInventory.h"
#include "ZzzScene.h"
#include "MoveWindow.h"

namespace SEASON3B
{
    class CNewUIOptionWindow : public CNewUIObj
    {
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
    public:
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
        enum IMAGE_LIST
        {
            IMAGE_OPTION_FRAME_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
            IMAGE_OPTION_BTN_CLOSE = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_CLOSE,
            IMAGE_OPTION_FRAME_DOWN = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
            IMAGE_OPTION_FRAME_UP = BITMAP_OPTION_BEGIN,
            IMAGE_OPTION_FRAME_LEFT,
            IMAGE_OPTION_FRAME_RIGHT,
            IMAGE_OPTION_LINE,
            IMAGE_OPTION_POINT,
            IMAGE_OPTION_BTN_CHECK,
            IMAGE_OPTION_EFFECT_BACK,
            IMAGE_OPTION_EFFECT_COLOR,
            IMAGE_OPTION_VOLUME_BACK,
            IMAGE_OPTION_VOLUME_COLOR,
            IMAGE_OPTION_FOOTER,
        };

        enum Tab
        {
            TAB_GAME,
            TAB_GRAPHICS,
            TAB_PERFORMANCE
        };

    public:
        CNewUIOptionWindow();
        virtual ~CNewUIOptionWindow();

        bool Create(CNewUIManager* pNewUIMng, int x, int y);
        void Release();
        void SetPos(int x, int y);
        bool UpdateMouseEvent();
        bool UpdateKeyEvent();
        bool Update();
        bool Render();
        float GetLayerDepth();
        float GetKeyEventOrder();
        void OpenningProcess();
        void ClosingProcess();
        void SetAutoAttack(bool bAuto);
        bool IsAutoAttack();
        void SetWhisperSound(bool bSound);
        bool IsWhisperSound();
        void SetSlideHelp(bool bHelp);
        bool IsSlideHelp();
        void SetVolumeLevel(int iVolume);
        int GetVolumeLevel();
        void SetRenderLevel(int iRender);
        int GetRenderLevel();

    public:
        bool m_EffectSprite;
        bool m_EffectParticle;
        bool m_Terrain;
        bool m_Wings;
        bool m_Swords;
        bool m_Equipments;
        bool m_Music;
        bool m_bAutoAttack;
        bool m_bWhisperSound;
        bool m_bSlideHelp;
        int m_iVolumeLevel;
        int m_iRenderLevel;
        bool m_Custom;
        bool m_Window;
        bool m_Object;
        bool m_Shadows;
        bool m_DefaultItem;
        bool m_Monsters;
        bool m_Players;
        bool m_Rotation;
        bool m_NSkillCollor;
        bool SystemFPS;
        bool m_Fps30;
        bool m_Fps60;
        bool m_Fps120;
        float m_highestFps;
        bool m_MiniSafeZone;
        bool m_FogSystem;
        bool m_HideInterface;
        bool m_DisableSkill;
        bool m_NShadows;
        bool m_MonsterHPBar;

    private:
        void LoadImages();
        void UnloadImages();
        void SetButtonInfo();
        void RenderFrame();
        void RenderContents();
        void RenderButtons();
        void RenderTabs();
        void UpdateFpsLimitInIni(int limit);
        int ReadFpsLimitFromIni();
        void SaveOptionToIni(const char* key, int value);
        int ReadOptionFromIni(const char* key, int defaultValue);
    private:
        cMoveWindow m_MoveWindow;
        CNewUIManager* m_pNewUIMng;
        POINT m_Pos;
        int m_NewY;
        CNewUIButton m_BtnClose;
        Tab m_ActiveTab;
    };
}

#endif // !defined(AFX_NEWUIOPTIONWINDOW_H__1469FA1D_7C15_4AFE_AD6E_59C303E72BC0__INCLUDED_)
// NewUIHeroPositionInfo.h
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_NEWUIHEROPOSITIONINFO_H__5D452B62_B315_41EE_A862_5929286925F7__INCLUDED_)
#define AFX_NEWUIHEROPOSITIONINFO_H__5D452B62_B315_41EE_A862_5929286925F7__INCLUDED_
#pragma once
#include "MuHelperDefines.h"
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"

namespace SEASON3B
{
    class CNewUIHeroPositionInfo : public CNewUIObj
    {
    public:
        enum IMAGE_LIST
        {
            // Keep the MuBattle coordinate texture at the original index.  The
            // official Helper textures use private adjacent slots and are only
            // loaded when MuHelperEnabled is active in info.bmd.
            IMAGE_HERO_POSITION_INFO_CORD = BITMAP_HERO_POSITION_INFO_BEGIN,
            IMAGE_MUHELPER_POSITION_A = BITMAP_MUHELPER_POSITION_A,
            IMAGE_MUHELPER_POSITION_B,
            IMAGE_MUHELPER_POSITION_C,
            IMAGE_MUHELPER_SETUP,
            IMAGE_MUHELPER_START,
            IMAGE_MUHELPER_STOP,
        };

    private:
        enum HERO_POSITION_INFO_BASE_WINDOW_SIZE
        {
            HERO_POSITION_INFO_BASE_WINDOW_WIDTH = 0,   // ajuste se a nova imagem tiver tamanho diferente
            HERO_POSITION_INFO_BASE_WINDOW_HEIGHT = 0,
            MUHELPER_POSITION_A_WIDTH = 22,
            MUHELPER_POSITION_B_WIDTH = 80,
            MUHELPER_POSITION_C_WIDTH = 73,
            MUHELPER_POSITION_HEIGHT = 25,
        };

    private:
        CNewUIManager* m_pNewUIMng;
        POINT m_Pos;
        POINT m_CurHeroPosition;
        int m_LastHelperAlertLevel;
        CNewUIButton m_BtnSetup;
        CNewUIButton m_BtnStart;
        CNewUIButton m_BtnPause;

    public:
        CNewUIHeroPositionInfo();
        virtual ~CNewUIHeroPositionInfo();

        bool Create(CNewUIManager* pNewUIMng, int x, int y);
        void Release();
        void SetPos(int x, int y);

        bool UpdateMouseEvent();
        bool UpdateKeyEvent();
        bool Update();
        bool Render();
        bool BtnProcess();

        float GetLayerDepth();
        void OpenningProcess();
        void ClosingProcess();
        void SetCurHeroPosition(int x, int y);

    private:
        bool IsMuHelperEnabled() const;
        void SetHelperButtonInfo();
        void RenderHelperFrame();
        void RenderHelperButtons();
        void LoadImages();
        void UnloadImages();
    };
}
#endif

// NewUIHeroPositionInfo.h
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_NEWUIHEROPOSITIONINFO_H__5D452B62_B315_41EE_A862_5929286925F7__INCLUDED_)
#define AFX_NEWUIHEROPOSITIONINFO_H__5D452B62_B315_41EE_A862_5929286925F7__INCLUDED_
#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"

namespace SEASON3B
{
    class CNewUIHeroPositionInfo : public CNewUIObj
    {
    public:
        enum IMAGE_LIST
        {
            // IMAGE_HERO_POSITION_INFO_BASE_WINDOW = BITMAP_HERO_POSITION_INFO_BEGIN,  // removido
            IMAGE_HERO_POSITION_INFO_CORD = BITMAP_HERO_POSITION_INFO_BEGIN,   // nova imagem com alpha
        };

    private:
        enum HERO_POSITION_INFO_BASE_WINDOW_SIZE
        {
            HERO_POSITION_INFO_BASE_WINDOW_WIDTH = 0,   // ajuste se a nova imagem tiver tamanho diferente
            HERO_POSITION_INFO_BASE_WINDOW_HEIGHT = 0,
        };

    private:
        CNewUIManager* m_pNewUIMng;
        POINT m_Pos;
        POINT m_CurHeroPosition;

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
        void LoadImages();
        void UnloadImages();
    };
}
#endif
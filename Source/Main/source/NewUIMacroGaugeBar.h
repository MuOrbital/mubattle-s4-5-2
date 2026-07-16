#if !defined(AFX_NEWUIMACROGAUGEBAR_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_)
#define AFX_NEWUIMACROGAUGEBAR_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_

#pragma once
#include "MuHelperDefines.h"
#include "NewUIBase.h"
#include "NewUIManager.h"

namespace SEASON3B
{
	class CNewUIMacroGaugeBar : public CNewUIObj
	{
	public:
		CNewUIMacroGaugeBar();
		virtual~CNewUIMacroGaugeBar();
	private:
		/*+012*/ CNewUIManager* m_pNewUIMng;
		/*+016*/ POINT m_Pos;
		/*+024*/ CTimer m_Timer;
		/*+072*/ bool m_Start;
		/*+080*/ double m_initialTime;
		/*+088*/ int m_stage;
		/*+092*/ double m_Progress;
		/*+096*/ DWORD m_cost;
		/*+100*/ unicode::t_string m_tooltip;
		/*+128*/ bool m_RenderToolip;
		/*+132*/ int m_tooltipX;
		/*+136*/ int m_tooltipY;
	public:
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		void Initialize();

		void SetPos(int x, int y);
		void LoadImages();
		void UnloadImages();

		bool Render();
		bool Update();
		bool UpdateKeyEvent();
		bool UpdateMouseEvent();
		float GetLayerDepth();

		void StartAccumulate();
		void StopAccumulate();
		void SynceGaugeBar(time_t time);
		void SetMacroExpense(int Cost);
	};
}

#endif //AFX_NEWUIMACROGAUGEBAR_H__AA370602_D171_41DC_9A79_345D75F678D4__INCLUDED_

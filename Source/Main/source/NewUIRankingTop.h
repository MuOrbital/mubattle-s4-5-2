#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIScrollBar.h"

namespace SEASON3B
{
	struct TEMPLATE_RANKING
	{
		std::string Name;
		std::string Class;
		BYTE PlayerClass;
		BYTE Vip;
		DWORD Score;
		TEMPLATE_RANKING(const std::string& n, const std::string& c, BYTE p, BYTE v, DWORD s)
			: Name(n), Class(c), PlayerClass(p), Vip(v), Score(s)
		{
		}
		BYTE GetPlayerClass() {
			return PlayerClass;
		}
		BYTE GetVip() {
			return Vip;
		}
		DWORD GetScore() {
			return Score;
		}
		const char* GetName() {
			return Name.c_str();
		}
		const char* GetClass() {
			return Class.c_str();
		}
	};

	class CNewUIRankingTop : public CNewUIObj
	{
		enum IMAGE_LIST
		{
			IMAGE_TOP_BACK1 = 120100,
			IMAGE_TOP_BACK2 = 120101,
			IMAGE_TOP_BACK3 = 120102,
			IMAGE_TOP_LEVEL1 = 120103,
			IMAGE_TOP_LEVEL2 = 120104,
			IMAGE_TOP_LEVEL3 = 120105,
		};
	private:
		CNewUIManager* m_pNewUIMng;
		POINT m_Pos;
		CUIPhotoViewer m_RenderCharacter;

		bool is_request;
		bool is_loading;
		int CharacterIndex;
		size_t TotalView;
		size_t TotalSize;
		size_t SelectIndex;
		std::string RankingName;
		std::string ScoreName;
		CNewUIScrollBar* m_pScrollBar;
		std::vector<TEMPLATE_RANKING> rankingList;
	public:
		CNewUIRankingTop();
		virtual ~CNewUIRankingTop();

		bool Create(CNewUIManager* pNewUIMng, float x, float y);
		void Release();
		void SetInfo();
		void SetPos(float x, float y);
		void LoadImages();
		void UnloadImages();
		bool UpdateKeyEvent();
		bool UpdateMouseEvent();
		bool Render();
		bool Update();
		float GetLayerDepth(); //. 10.5f

		void OpenningProcess();
		void ClosingProcess();
		void ReceiveRankingInfo(BYTE* ReceiveBuffer);
		void ReceiveCharacterInfo(BYTE* ReceiveBuffer);
		void ReceiveRankingListInfo(BYTE* ReceiveBuffer);
	private:
		void RenderFrame();
		void RenderText();
		void SetCharacterIndex(int characterIndex);
		int GetCharacterIndex();
		int GetTotalCharacter();
		void RequestCharacterInfo(int characterIndex);
		void RequestServerRankingInfo(BYTE RankingIndex);
	};
}


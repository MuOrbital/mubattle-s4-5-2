// NewUIMixInventory.h: interface for the CChaosGenesis class.
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NewUIBase.h"
#include "NewUIInventoryCtrl.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "UIControls.h"
#include "ProtocolSend.h"
#include "LuaInterface.h"
#include "LuaGlobal.h"
#include "Lua.h"
#include "NProtocol.h"
//#include "CustomJewelStack.h"


namespace SEASON3B
{

	class CChaosGenesis : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_GENESISMIXINVENTORY_BACK_VOID = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_GENESISMIXINVENTORY_MIXBTN = BITMAP_INTERFACE_NEW_MIXINVENTORY_BEGIN,
		};


		class CHAOS_MACHINE_ITENS {
		public:
			CHAOS_MACHINE_ITENS() {

			};

			~CHAOS_MACHINE_ITENS() {

			};

			void Clear() {
				this->slot = -1;

				if (this->pItemObj)
				{
					DeleteItem(this->pItemObj);
					this->pItemObj = nullptr;
				}
			}


			ITEM* pItemObj;
			BYTE slot;
		};

	private:
		enum
		{
			INVENTORY_WIDTH = 190,
			INVENTORY_HEIGHT = 429,
		};

		CNewUIManager* m_pNewUIMng;


	public:
		POINT m_Pos;
		enum EVENT_STATE
		{
			EVENT_NONE = 0,
			EVENT_SCROLL_BTN_DOWN,
			EVENT_SCROLL_BTN_CLIKED,
		};

	public:
		CChaosGenesis();
		virtual ~CChaosGenesis();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		void RestartLua();
		void OpeningProcess();
		bool ClosingProcess();
		void SetPos(int x, int y);
		void Scrolling(int value);
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		float GetLayerDepth();	//. 3.4f
		void ProcessMyInvenItemAutoMove();
		bool IsItemAutoMove() { return m_bItemAutoMove; }
		void SetItemAutoMove(bool bItemAutoMove);
		void ProcessToReceiveGenesisItems(int nIndex, BYTE* pbyItemPacket);
		void ProcessToRemoveGenesisItems(int nIndex, BYTE* pbyItemPacket);
		static void DeleteItem(ITEM* Item);

		void Open(SDHP_CHAOS_BOX_GENESIS_OPEN_CLIENT_RECV* lpMsg);
		void ChaosBoxGenesisStateRecv(PMSG_CHAOS_BOX_GENESIS_STATE_RECV* lpMsg);
		void ChaosBoxGenesisListItemRecv(PMSG_CHAOS_BOX_GENESIS_LIST_RECV* lpMsg);

		void RenderItensChaosMachine(float x, float y, float height, int MaxLines);
		void CalcLinesRender();
		void InsertItemChaosMachine(BYTE slot, BYTE* ItemInfo);
		void RenderItemInformation(float x, float y, ITEM* pItemObj);
		void DelItem(BYTE slot);
		void DeleteAllItems();
		void RenderItemImage(ITEM* pItemObj);
		void RenderScroll(float x, float y, float width, float height, int MaxLines);
		void ScrollLines(int mode);
		int CheckCanMoveItemChaosBox(int ItemIndex);
		bool MoveItemToInventory();

	private:
		void LoadImages();
		void UnloadImages();

	public:

		bool CheckChaosBoxGenesisSourceEmptySlot()
		{
			if (this->m_ChaosMachineItens.size() < 120) {
				return 1;
			}

			return 0;
		}

		int getCountItensByIndexLevel(int ItemIndex, int Level, int Exc, int Luck, int Skill, int Life) {
			int count = 0;

			for (auto& itemTarget : this->m_ChaosMachineItens) {
				auto itemObj = itemTarget.pItemObj;

				if (itemObj == nullptr) {
					continue;
				}

				if (Exc != -1) {
					if (Exc == 0 && (itemObj->Option1 & 63) > 0) {
						continue;
					}
					else {

						int opt = itemObj->Option1 & 1;
						opt += (itemObj->Option1 >> 1) & 1;
						opt += (itemObj->Option1 >> 2) & 1;
						opt += (itemObj->Option1 >> 3) & 1;
						opt += (itemObj->Option1 >> 4) & 1;
						opt += (itemObj->Option1 >> 5) & 1;

						if (opt < Exc || itemObj->Jewel_Of_Harmony_OptionLevel > 0 || itemObj->Jewel_Of_Harmony_Option > 0) {
							continue;
						}
					}

				}

				if (Luck != -1) {
					if (((itemObj->Level >> 2) & 1) != Luck) {
						continue;
					}
				}

				if (Skill != -1) {
					if (Skill > 0) {
						if (((itemObj->Level >> 7) & 1) == 0) {
							continue;
						}
					}
					else {
						if (((itemObj->Level >> 7) & 1) != 0) {
							continue;
						}
					}

				}

				if (Life != -1) {
					int Option3 = ((itemObj->Level) & 3) + ((itemObj->Option1) & 64) / 64 * 4;
					if (Life == 0) {
						if (Option3 > 0) {
							continue;
						}
					}
					else {
						if (Option3 < Life) {
							continue;
						}
					}
				}

				if (ItemIndex != -1 && itemObj->Type != ItemIndex) {
					continue;
				}

				if (Level != -1) {
					if ((itemObj->Level / 8 & 15) < Level) {
						continue;
					}
				}

				//if (gJewelStack.CheckIsStack(ItemIndex)) {
				//	if (itemObj->Durability <= 0) {
				//		count += 1;
				//	}
				//	else {
				//		count += itemObj->Durability;
				//	}
				//}
				//else
				//{
				//	count++;
				//}

			}

			return count;
		}

		int getCountItensByIndex(int ItemIndex, int Level, int Exc, int Luck, int Skill, int Life, int Ancient, int MaxAcient, int Visual) {
			int count = 0;

			for (auto& itemTarget : this->m_ChaosMachineItens) {
				auto itemObj = itemTarget.pItemObj;

				if (itemObj == nullptr) {
					continue;
				}

				if (Exc != -1) {
					if (Exc == 0 && (itemObj->Option1 & 63) > 0) {
						continue;
					}
					else {

						int opt = itemObj->Option1 & 1;
						opt += (itemObj->Option1 >> 1) & 1;
						opt += (itemObj->Option1 >> 2) & 1;
						opt += (itemObj->Option1 >> 3) & 1;
						opt += (itemObj->Option1 >> 4) & 1;
						opt += (itemObj->Option1 >> 5) & 1;

						if (opt < Exc || itemObj->Jewel_Of_Harmony_OptionLevel > 0 || itemObj->Jewel_Of_Harmony_Option > 0) {
							continue;
						}
					}
				}

				if (Visual != -1) {
					if (itemObj->Jewel_Of_Harmony_OptionLevel > 0 || itemObj->Jewel_Of_Harmony_Option > 0) {
						if (Visual == 0) {
							continue;
						}
					}
					else {
						if (Visual == 1) {
							continue;
						}
					}
				}

				if (Ancient != -1) {
					if (Ancient == 0 && itemObj->ExtOption > 0) {
						continue;
					}
					else {
						if (itemObj->ExtOption < Ancient) {
							continue;
						}

						if (MaxAcient != -1 && itemObj->ExtOption > MaxAcient) {
							continue;
						}
					}
				}

				if (Luck != -1) {
					if (((itemObj->Level >> 2) & 1) != Luck) {
						continue;
					}
				}

				if (Skill != -1) {
					if (Skill > 0) {
						if (((itemObj->Level >> 7) & 1) == 0) {
							continue;
						}
					}
					else {
						if (((itemObj->Level >> 7) & 1) != 0) {
							continue;
						}
					}

				}

				if (Life != -1) {
					int Option3 = ((itemObj->Level) & 3) + ((itemObj->Option1) & 64) / 64 * 4;
					if (Life == 0) {
						if (Option3 > 0) {
							continue;
						}
					}
					else {
						if (Option3 < Life) {
							continue;
						}
					}
				}

				if (ItemIndex != -1 && itemObj->Type != ItemIndex) {
					continue;
				}

				if (Level != -1) {
					if ((itemObj->Level / 8 & 15) < Level) {
						continue;
					}
				}

				//if (gJewelStack.CheckIsStack(ItemIndex)) {
				//	if (itemObj->Durability <= 0) {
				//		count += 1;
				//	}
				//	else {
				//		count += itemObj->Durability;
				//	}
				//}
				//else
				//{
				//	count++;
				//}

			}

			return count;
		}

		int ChaosBoxGenesisState;
		int ChaosBoxGenesisSelectedKey;
		int ChaosBoxGenesisSelectedComb;
		int ItemPointer;

	private:
		bool ItemMove;
		int m_nBackupMouseX;
		int m_nBackupMouseY;
		bool m_bItemAutoMove;
		bool m_InventoryVisible;
		float m_PosX;
		float m_PosY;
		float m_HeightBox;
		int m_iMouseY;
		float ScrollBarPosX;
		float ScrollBarPosY;
		int ScrollBarOffset;
		int ScrollBarMaxOffset;
		float ScrollBarPos;
		float ScrollBarEsp;
		float ScrollBarMultiplier;
		int m_MaxLines;
		int m_TargetSlotBackup;
		EVENT_STATE m_EventState;

	private:
		Lua m_Lua;
		LuaInterface m_LuaInterface;
		LuaGlobal m_LuaGlobal;

	public:
		std::vector<CHAOS_MACHINE_ITENS> m_ChaosMachineItens;

	};
}


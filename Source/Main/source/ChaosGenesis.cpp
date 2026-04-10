// NewUIMixInventory.cpp: implementation of the CChaosGenesis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosGenesis.h"
#include "NewUISystem.h"
#include "NewUICustomMessageBox.h"
#include "MixMgr.h"
#include "ZzzBMD.h"
#include "ZzzEffect.h"
#include "ZzzObject.h"
#include "ZzzInventory.h"
#include "ZzzInterface.h"
#include "ZzzInfomation.h"
#include "ZzzCharacter.h"
#include "wsclientinline.h"
#include "DSPlaySound.h"
#include "SocketSystem.h"
#include "LuaStack.hpp"
#include "NProtocol.h"
#include "Protocol.h"
//#include "CustomJewelStack.h"

using namespace SEASON3B;

class ChaosMachineGenesisLua {
public:
	ChaosMachineGenesisLua() : index(-1) {

	};

	ChaosMachineGenesisLua(int index) {
		this->index = index;
	};

	~ChaosMachineGenesisLua() {

	};

	int getIndex() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return -1;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Type;
	};

	int getLevel() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return (g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level / 8) & 0xF;
	};

	int getOption1() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Option1;
	};

	int getExtOption() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->ExtOption;
	};

	int getHarmony() const { //OK
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		if (g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Jewel_Of_Harmony_OptionLevel > 0 || g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Jewel_Of_Harmony_Option > 0) {
			return 1;
		}

		return 0;
	}

	int getLuck() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return ((g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level >> 2) & 1);
	};

	int getSkill() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return ((g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level >> 7) & 1);
	};

	int getLife() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		int oplevel = g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level;
		int option1 = g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Option1;
		int Option3 = ((oplevel) & 3) + ((option1) & 64) / 64 * 4;

		return Option3;
	};

	int getDurability() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Durability;
	};

	int getStack() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		//if (gJewelStack.CheckIsStack(g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Type))
		//{
		//	return 1;
		//}
		//return 0;
	};

private:
	int index;
};

void RenderItensChaosMachineLua(float x, float y, float height, int MaxLines) {
	g_pChaosGenesis->RenderItensChaosMachine(x, y, height, MaxLines);
}

int getCountItensByIndexLua(int index, int level, int exc, int luck, int skill, int life, int ancient, int MaxAncient, int visual) {
	return g_pChaosGenesis->getCountItensByIndex(index, level, exc, luck, skill, life, ancient, MaxAncient, visual);
}

int getCountItensByIndexLevelLua(int index, int level, int exc, int luck, int skill, int life) {
	return g_pChaosGenesis->getCountItensByIndexLevel(index, level, exc, luck, skill, life);
}

int getChaosBoxGenesisState() {
	return g_pChaosGenesis->ChaosBoxGenesisState;
}

int getChaosBoxGenesisSelectedKey() {
	return g_pChaosGenesis->ChaosBoxGenesisSelectedKey;
}

int getChaosBoxGenesisSelectedComb() {
	return g_pChaosGenesis->ChaosBoxGenesisSelectedComb;
}

void SetChaosBoxGenesis(int selectedKey, int selectedComb) {
	g_pChaosGenesis->ChaosBoxGenesisSelectedKey = selectedKey;
	g_pChaosGenesis->ChaosBoxGenesisSelectedComb = selectedComb;

	SDHP_CHAOS_BOX_GENESIS_SET_INFO_SEND pMsg;

	pMsg.header.set(0xEF, 0x20, sizeof(pMsg));

	pMsg.state = 0;

	pMsg.selectedKey = selectedKey;

	pMsg.selectedComb = selectedComb;

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

int getMaxItens() {
	return (int)g_pChaosGenesis->m_ChaosMachineItens.size();
}

void SendChaosMachineMix() {
	SDHP_CHAOS_BOX_GENESIS_MIX_SEND pMsg;

	pMsg.header.set(0xEF, 0x22, sizeof(pMsg));

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

CChaosGenesis::CChaosGenesis() : m_Lua()
{
	m_pNewUIMng = NULL;

	m_Pos.x = m_Pos.y = 0;

	this->m_nBackupMouseX = 0;

	this->m_nBackupMouseY = 0;

	this->m_bItemAutoMove = false;

	this->m_iMouseY = 0;

	this->ChaosBoxGenesisState = 0;

	this->m_MaxLines = 9;

	this->m_HeightBox = 70;

	this->ItemPointer = -1;

	this->m_ChaosMachineItens.clear();

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	//this->m_Lua.DoFile("Data\\Lua\\Manager\\ChaosMachine\\ChaosMachine.lua");

	luaaa::LuaModule(this->m_Lua.GetState()).fun("RenderItensChaosMachine", &RenderItensChaosMachineLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndex", &getCountItensByIndexLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndexLevel", &getCountItensByIndexLevelLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisState", &getChaosBoxGenesisState);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedKey", &getChaosBoxGenesisSelectedKey);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedComb", &getChaosBoxGenesisSelectedComb);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SetChaosBoxGenesis", &SetChaosBoxGenesis);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getMaxItens", &getMaxItens);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SendChaosMachineMix", &SendChaosMachineMix);

	luaaa::LuaClass<ChaosMachineGenesisLua> luaChaosMachine(this->m_Lua.GetState(), "ChaosMachine");
	luaChaosMachine.ctor<int>();
	luaChaosMachine.fun("getIndex", &ChaosMachineGenesisLua::getIndex);
	luaChaosMachine.fun("getLevel", &ChaosMachineGenesisLua::getLevel);
	luaChaosMachine.fun("getOption1", &ChaosMachineGenesisLua::getOption1);
	luaChaosMachine.fun("getExtOption", &ChaosMachineGenesisLua::getExtOption);
	luaChaosMachine.fun("getLuck", &ChaosMachineGenesisLua::getLuck);
	luaChaosMachine.fun("getSkill", &ChaosMachineGenesisLua::getSkill);
	luaChaosMachine.fun("getLife", &ChaosMachineGenesisLua::getLife);
	luaChaosMachine.fun("getDurability", &ChaosMachineGenesisLua::getDurability);
	luaChaosMachine.fun("getStack", &ChaosMachineGenesisLua::getStack);
	luaChaosMachine.fun("getHarmony", &ChaosMachineGenesisLua::getHarmony);
}

CChaosGenesis::~CChaosGenesis() { Release(); }

bool CChaosGenesis::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;

	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHAOSGENESIS, this);

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void CChaosGenesis::RestartLua() {
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	//this->m_Lua.DoFile("Data\\Lua\\Manager\\ChaosMachine\\ChaosMachine.lua");

	luaaa::LuaModule(this->m_Lua.GetState()).fun("RenderItensChaosMachine", &RenderItensChaosMachineLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndex", &getCountItensByIndexLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndexLevel", &getCountItensByIndexLevelLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisState", &getChaosBoxGenesisState);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedKey", &getChaosBoxGenesisSelectedKey);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedComb", &getChaosBoxGenesisSelectedComb);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SetChaosBoxGenesis", &SetChaosBoxGenesis);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getMaxItens", &getMaxItens);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SendChaosMachineMix", &SendChaosMachineMix);

	luaaa::LuaClass<ChaosMachineGenesisLua> luaChaosMachine(this->m_Lua.GetState(), "ChaosMachine");
	luaChaosMachine.ctor<int>();
	luaChaosMachine.fun("getIndex", &ChaosMachineGenesisLua::getIndex);
	luaChaosMachine.fun("getLevel", &ChaosMachineGenesisLua::getLevel);
	luaChaosMachine.fun("getOption1", &ChaosMachineGenesisLua::getOption1);
	luaChaosMachine.fun("getExtOption", &ChaosMachineGenesisLua::getExtOption);
	luaChaosMachine.fun("getLuck", &ChaosMachineGenesisLua::getLuck);
	luaChaosMachine.fun("getSkill", &ChaosMachineGenesisLua::getSkill);
	luaChaosMachine.fun("getLife", &ChaosMachineGenesisLua::getLife);
	luaChaosMachine.fun("getDurability", &ChaosMachineGenesisLua::getDurability);
	luaChaosMachine.fun("getStack", &ChaosMachineGenesisLua::getStack);
	luaChaosMachine.fun("getHarmony", &ChaosMachineGenesisLua::getHarmony);
}

void CChaosGenesis::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CChaosGenesis::Open(SDHP_CHAOS_BOX_GENESIS_OPEN_CLIENT_RECV* lpMsg)
{
	m_EventState = EVENT_NONE;

	g_pChaosGenesis->ChaosBoxGenesisState = lpMsg->state;

	g_pChaosGenesis->ChaosBoxGenesisSelectedKey = lpMsg->selectedKey;

	g_pChaosGenesis->ChaosBoxGenesisSelectedComb = lpMsg->selectedComb;

	g_pNewUISystem->Show(SEASON3B::INTERFACE_CHAOSGENESIS);
}

void CChaosGenesis::ChaosBoxGenesisStateRecv(PMSG_CHAOS_BOX_GENESIS_STATE_RECV* lpMsg)
{
	this->ChaosBoxGenesisState = lpMsg->state;

	this->m_Lua.Generic_Call("ChaosMachineMixResult", "i>", this->ChaosBoxGenesisState);
}

void CChaosGenesis::ChaosBoxGenesisListItemRecv(PMSG_CHAOS_BOX_GENESIS_LIST_RECV* lpMsg)
{
	this->DeleteAllItems();


	if (lpMsg->count <= 0) {
		return;
	}

	for (int n = 0; n < lpMsg->count; n++) {
		auto lpInfo = (PMSG_CHAOS_BOX_GENESIS_ITEM_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_CHAOS_BOX_GENESIS_LIST_RECV) + (sizeof(PMSG_CHAOS_BOX_GENESIS_ITEM_INFO) * n));
		this->InsertItemChaosMachine(lpInfo->slot, lpInfo->ItemInfo);
	}

	this->CalcLinesRender();
}

void CChaosGenesis::RenderItensChaosMachine(float x, float y, float height, int MaxLines)
{
	EnableAlphaTest(1);

	float SizeY = 0;

	char buffer[256];
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(225, 225, 225, 255);

	int LineStart = 0;
	int LineCount = 0;

	if (this->m_ChaosMachineItens.size() <= 0)
	{
		//gDrawBarItem.ResetItemDrawBar(1);
	}

	for (auto& itemTarget : this->m_ChaosMachineItens)
	{
		if (LineStart >= ScrollBarOffset && LineCount <= MaxLines)
		{
			if (!itemTarget.pItemObj) {
				continue;
			}

			if (MouseX >= x && MouseX <= x + 130)
			{
				if (MouseY >= y + SizeY && MouseY <= y + (SizeY + 8))
				{
					this->ItemPointer = itemTarget.slot;

					RenderItemInformation(x - 100, y + 125, itemTarget.pItemObj);
				}
			}

			auto itemAttr = &ItemAttribute[itemTarget.pItemObj->Type];

			if (!itemAttr) {
				continue;
			}

			EnableAlphaTest(true);

			if (itemTarget.pItemObj->Jewel_Of_Harmony_Option > 0 || itemTarget.pItemObj->Jewel_Of_Harmony_OptionLevel > 0 || (itemAttr->m_byItemSlot >= 240 && itemAttr->m_byItemSlot <= 248)) {
				//sprintf_s(buffer, sizeof(buffer), "%s", gVisualInventory.GetItemHybridName(itemTarget.pItemObj->Type));
				sprintf_s(buffer, sizeof(buffer), "%s", itemAttr->Name);
			}
			else {
				if (((itemTarget.pItemObj->Level >> 3) & 0xF) == 0)
				{
					//if (gJewelStack.CheckIsStack(itemTarget.pItemObj->Type)) {
					//	int count = 1;
					//	if (itemTarget.pItemObj->Durability > 1) {
					//
					//		count = itemTarget.pItemObj->Durability;
					//	}
					//	sprintf_s(buffer, sizeof(buffer), "%s (%dx)", itemAttr->Name, count);
					//}
					//else {
					//	sprintf_s(buffer, sizeof(buffer), "%s", itemAttr->Name);
					//}
				}
				else {
					sprintf_s(buffer, sizeof(buffer), "%s +%d", itemAttr->Name, (itemTarget.pItemObj->Level >> 3) & 0xF);
				}
			}

			g_pRenderText->RenderText((int)(x), (int)(y + SizeY), buffer, 190, 0, RT3_SORT_LEFT);
			DisableAlphaBlend();

			SizeY += 10;
			LineCount++;
		}

		LineStart++;
	}

	DisableAlphaBlend();

	this->m_PosX = x;
	this->m_PosY = y;
	this->m_HeightBox = height - 30;

	if (this->m_MaxLines != MaxLines) {
		this->m_MaxLines = MaxLines;
		this->CalcLinesRender();
	}

	this->RenderScroll(x - 5, y - 15, 160, height, MaxLines);

}

void CChaosGenesis::CalcLinesRender()
{
	ScrollBarOffset = 0;
	ScrollBarPos = 0;
	ScrollBarMaxOffset = (int)(m_ChaosMachineItens.size() - (m_MaxLines + 1));
	ScrollBarMultiplier = (float)((float)m_HeightBox / (float)(m_ChaosMachineItens.size() - (m_MaxLines)));
}

void CChaosGenesis::InsertItemChaosMachine(BYTE slot, BYTE* ItemInfo)
{
	CHAOS_MACHINE_ITENS info;

	info.pItemObj = g_pNewItemMng->CreateItem(ItemInfo);

	info.slot = slot;

	this->m_ChaosMachineItens.push_back(info);

	this->CalcLinesRender();
}

void CChaosGenesis::RenderItemInformation(float x, float y, ITEM* pItemObj)
{
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	RenderColor(45, 40, 215.0f, 370.0f, 0.0, 0);

	EndRenderColor();

	EnableAlphaTest();

	RenderItemInfo(155, 130, pItemObj, 0);

	EndRenderColor();

	DisableAlphaBlend();

	this->RenderItemImage(pItemObj);

}

void CChaosGenesis::DelItem(BYTE slot)
{
	std::vector<CHAOS_MACHINE_ITENS>::iterator li = this->m_ChaosMachineItens.begin();

	for (; li != this->m_ChaosMachineItens.end(); li++) {
		auto pItem = &(*li);

		if (pItem->slot == slot) {
			pItem->Clear();
			this->m_ChaosMachineItens.erase(li);
			break;
		}
	}
}

void CChaosGenesis::DeleteAllItems()
{
	for (auto& itemChaosBox : this->m_ChaosMachineItens)
	{
		itemChaosBox.Clear();
	}

	this->m_ChaosMachineItens.clear();
}

void CChaosGenesis::RenderItemImage(ITEM* pItemObj)
{
	glMatrixMode(0x1701);
	glPushMatrix();
	glLoadIdentity();

	glViewport2(0, 0, WindowWidth, WindowHeight);

	gluPerspective2(1.f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);

	glMatrixMode(0x1700);
	glPushMatrix();
	glLoadIdentity();

	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	auto lpAttr = &ItemAttribute[pItemObj->Type];

	float width = lpAttr->Width;
	float height = lpAttr->Height;

	RenderItem3D((float)155, (float)100, (float)width * 3, (float)height * 4, (int)pItemObj->Type, (int)pItemObj->Level * 8, (int)pItemObj->Option1, (int)pItemObj->ExtOption, 0);

	glMatrixMode(0x1700);
	glPopMatrix();
	glMatrixMode(0x1701);
	glPopMatrix();

	glColor3f(1, 1, 1);
	EnableAlphaTest(false);
}

void CChaosGenesis::RenderScroll(float x, float y, float width, float height, int MaxLines)
{
	this->m_MaxLines = MaxLines;

	//Bar
	EnableAlphaTest(1);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderImage(31269, x + width - 11, y + 14, 7, 3);

	for (int i = 0; i < MaxLines; i++) {
		RenderImage(31270, x + width - 11, (y + 13) + (float)(i * 10 + 4), 7, 15);
	}

	RenderImage(31271, x + width - 11, y + height - 8, 7, 3);

	int Line = 0;

	if (m_EventState == EVENT_SCROLL_BTN_DOWN) {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else {
		glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	}

	RenderImage(31272, x + width - 15, y + 10 + ScrollBarPos, 15, 30);

	ScrollBarPosY = (float)(y + 10);
	ScrollBarPosX = (float)(x + width - 15);

	DisableAlphaBlend();
}

void CChaosGenesis::ScrollLines(int mode)
{
	if (mode == 0) { //Up
		if (ScrollBarOffset > 0) {
			ScrollBarOffset = ScrollBarOffset - 1;
			ScrollBarPos = ScrollBarPos - ScrollBarMultiplier;
		}
	}
	else if (mode == 1) { //Down
		if (ScrollBarOffset < ScrollBarMaxOffset) {
			ScrollBarOffset = ScrollBarOffset + 1;
			ScrollBarPos = ScrollBarPos + ScrollBarMultiplier;
		}
	}
}

int CChaosGenesis::CheckCanMoveItemChaosBox(int ItemIndex)
{
	int retorno = 0;

	this->m_Lua.Generic_Call("ChaosMachineCheckCanMoveItem", "i>i", ItemIndex, &retorno);

	return retorno;
}

void CChaosGenesis::OpeningProcess()
{
	m_EventState = EVENT_NONE;

	this->m_Lua.Generic_Call("ChaosMachineOpenning", ">");
}

bool CChaosGenesis::ClosingProcess()
{
	SendChaosGenesisClose();
	return true;
}

void CChaosGenesis::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void CChaosGenesis::Scrolling(int value)
{
	if (CheckMouseIn((int)this->m_PosX - 5, (int)this->m_PosY, 145, 100)) {
		if (value > 0) {
			this->ScrollLines(0);
		}

		if (value < 0) {
			this->ScrollLines(1);
		}
		return;
	}

	this->m_Lua.Generic_Call("ChaosMachineScrolling", "i>", value);
}

bool CChaosGenesis::UpdateMouseEvent()
{
	if (CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT))
	{
		if (SEASON3B::IsPress(VK_RBUTTON))
		{
			if (this->MoveItemToInventory()) {
				MouseRButton = false;
				MouseRButtonPop = false;
				MouseRButtonPush = false;
				return false;
			}

		}

		if (CheckMouseIn(ScrollBarPosX, ScrollBarPosY, 9, 25)) {
			if (IsPress(VK_LBUTTON)) {
				m_EventState = EVENT_SCROLL_BTN_CLIKED;
				MouseLButton = false;
				MouseLButtonPop = false;
				MouseLButtonPush = false;
			}

			m_EventState = EVENT_SCROLL_BTN_DOWN;
		}
		else {
			m_EventState = EVENT_NONE;
		}

		if (CheckMouseIn(ScrollBarPosX, (int)(this->m_PosY), 10, 96)) {
			if (IsRepeat(VK_LBUTTON)) {
				int iMoveValue = MouseY - m_iMouseY;

				if (iMoveValue < 0) {
					if (MouseY <= (this->m_PosY + 10 + ScrollBarPos)) {
						this->ScrollLines(0);
					}
				}
				else if (iMoveValue > 0) {
					if (MouseY >= (this->m_PosY + 10 + ScrollBarPos + 25)) {
						this->ScrollLines(1);
					}
				}

				m_iMouseY = MouseY;
			}
		}

		this->m_Lua.Generic_Call("ChaosMachineUpdateMouse", ">");


		if (SEASON3B::IsNone(VK_LBUTTON) == false)
		{
			return false;
		}
	}

	return true;
}

bool CChaosGenesis::UpdateKeyEvent()
{

	return true;
}

bool CChaosGenesis::Update()
{
	if (IsVisible())
	{
		this->ItemPointer = -1;

		this->m_Lua.Generic_Call("ChaosMachineUpdate", ">");
	}

	return true;
}

bool CChaosGenesis::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	this->m_Lua.Generic_Call("ChaosMachineRender", ">");

	DisableAlphaBlend();

	return true;
}

float CChaosGenesis::GetLayerDepth()
{
	return 3.4f;
}

void CChaosGenesis::LoadImages()
{
	//LoadBitmap("Custom\\NewInterface\\btn_mix.jpg", 31419, GL_LINEAR, GL_CLAMP);
}

void CChaosGenesis::UnloadImages()
{
	DeleteBitmap(31419);
}

void CChaosGenesis::SetItemAutoMove(bool bItemAutoMove)
{
	m_bItemAutoMove = bItemAutoMove;

	if (bItemAutoMove)
	{
		m_nBackupMouseX = MouseX;
		m_nBackupMouseY = MouseY;
	}
	else
		m_nBackupMouseX = m_nBackupMouseY = 0;
}

void CChaosGenesis::ProcessMyInvenItemAutoMove()
{
	if (g_pPickedItem)
	{
		if (g_pPickedItem->GetItem())
		{
			return;
		}
	}

	if (IsItemAutoMove())
	{
		return;
	}

	if (this->CheckChaosBoxGenesisSourceEmptySlot() == 0)
	{
		return;
	}

	CNewUIInventoryCtrl* pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();

	ITEM* pItemObj = pMyInvenCtrl->FindItemAtPt(MouseX, MouseY);
	if (pItemObj)
	{
		if (this->CheckCanMoveItemChaosBox(pItemObj->Type) == 0) {

			return;
		}

		SetItemAutoMove(true);

		int nSrcIndex = pItemObj->y * pMyInvenCtrl->GetNumberOfColumn() + pItemObj->x;
		SendRequestEquipmentItem(REQUEST_EQUIPMENT_INVENTORY, MAX_EQUIPMENT_INDEX + nSrcIndex, pItemObj, REQUEST_EQUIPMENT_CHAOS_GENESIS, -1);

		::PlayBuffer(SOUND_GET_ITEM01);
	}
}

void CChaosGenesis::ProcessToReceiveGenesisItems(int nIndex, BYTE* pbyItemPacket)
{
	SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();

	if (nIndex >= 0 && nIndex < 120)
	{
		if (IsItemAutoMove())
		{
			CNewUIInventoryCtrl* pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();
			ITEM* pItemObj = pMyInvenCtrl->FindItemAtPt(m_nBackupMouseX, m_nBackupMouseY);
			g_pMyInventory->GetInventoryCtrl()->RemoveItem(pItemObj);

			SetItemAutoMove(false);
		}

		InsertItemChaosMachine(nIndex, pbyItemPacket);
	}
}

void CChaosGenesis::ProcessToRemoveGenesisItems(int nIndex, BYTE* pbyItemPacket)
{
	g_pMyInventory->InsertItem(this->m_TargetSlotBackup, pbyItemPacket);
	this->DelItem(nIndex);
	this->CalcLinesRender();
	SetItemAutoMove(false);
}

void CChaosGenesis::DeleteItem(ITEM* Item)
{
	g_pNewItemMng->DeleteItem(Item);
}

bool CChaosGenesis::MoveItemToInventory()
{
	if (IsItemAutoMove())
	{
		return 0;
	}

	float SizeY = 0;
	int LineStart = 0;
	int LineCount = 0;

	for (auto& itemChaosBox : this->m_ChaosMachineItens)
	{
		if (LineStart >= ScrollBarOffset && LineCount <= 8)
		{
			if (!itemChaosBox.pItemObj)
			{
				continue;
			}

			if (MouseX >= this->m_PosX && MouseX <= this->m_PosX + 130)
			{
				if (MouseY >= this->m_PosY + SizeY && MouseY <= this->m_PosY + (SizeY + 8))
				{
					auto itemAttr = &ItemAttribute[itemChaosBox.pItemObj->Type];

					if (itemAttr)
					{
						int nDstIndex = g_pMyInventory->FindEmptySlot(itemChaosBox.pItemObj);

						if (-1 != nDstIndex)
						{
							SetItemAutoMove(true);

							m_TargetSlotBackup = nDstIndex;

							SendRequestEquipmentItem(REQUEST_EQUIPMENT_CHAOS_GENESIS, itemChaosBox.slot, itemChaosBox.pItemObj, REQUEST_EQUIPMENT_INVENTORY, MAX_EQUIPMENT_INDEX + nDstIndex);

							::PlayBuffer(SOUND_GET_ITEM01);
							return 1;
						}
					}
				}
			}

			SizeY += 10;
			LineCount++;
		}

		LineStart++;
	}

	return 0;
}

#include "stdafx.h"
#include "LuaInterface.h"
#include "LuaStack.hpp"
#include "ZzzOpenglUtil.h"
#include "UIControls.h"
#include "NewUISystem.h"
#include "WideScreen.h"
#include "ZzzInterface.h"
#include "DSPlaySound.h"
#include "Interfaces.h"
#include "NewUICommon.h"
#include <Windows.h>
using namespace luaaa;
bool g_bHideHeroBoolean = false;

void RenderImageLua(int imageID, float x, float y, float width, float height) 
{
	SEASON3B::RenderImage(imageID, x, y, width, height);
}

int MousePosX() {
	return MouseX;
}

int MousePosY() {
	return MouseY;
}

void LuaglColor3f(float r, float g, float b) {
	glColor3f(r, g, b);
}

void LuaglColor4f(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void LuaEnableAlphaTest() {
	EnableAlphaTest();
}

void LuaDisableAlphaBlend() {
	DisableAlphaBlend();
}

static BOOL LuapSetBlend(lua_State* Lua)
{
	EnableAlphaTest(true);
	return true;
};

void LuaDrawBar(float x, float y, float w, float h) {
	RenderColor(x, y, w, h);
}

void LuaEndDrawBar() {
	EndRenderColor();
}

void LuaSetFontType(int font) {
	switch (font) {
	case 0:
		g_pRenderText->SetFont(g_hFont);
		break;
	case 1:
		g_pRenderText->SetFont(g_hFontBold);
		break;
	case 2:
		g_pRenderText->SetFont(g_hFontBig);
		break;
	case 3:
		g_pRenderText->SetFont(g_hFixFont);
		break;
	default:
		g_pRenderText->SetFont(g_hFont);
		break;
	}
}

void LuaSetFontBg(int r, int g, int b, int a) {
	g_pRenderText->SetBgColor(RGBA(r,g,b,a));
}

void LuaSetTextColor(int r, int g, int b, int a) {
	g_pRenderText->SetTextColor(RGBA(r, g, b, a));
}

void LuaRenderText(int x, int y, char* text, int width, int sort) {
	g_pRenderText->RenderText(x, y, text, width, 0, sort);
}

static BOOL LuaRenderText3(lua_State* Lua)
{
	g_pRenderText->RenderText((int)lua_tonumber(Lua, 1), (int)lua_tonumber(Lua, 2), lua_tostring(Lua, 3), 0, 0, (int)lua_tonumber(Lua, 4), 0);
	lua_pop(Lua, 3);
	return true;
};

int LuaGetTextPosY(int font, char* text, int y, int height) {
	SIZE Fontsize;
	switch (font) {
	case 0:
		g_pRenderText->SetFont(g_hFont);
		break;
	case 1:
		g_pRenderText->SetFont(g_hFontBold);
		break;
	case 2:
		g_pRenderText->SetFont(g_hFontBig);
		break;
	case 3:
		g_pRenderText->SetFont(g_hFixFont);
		break;
	default:
		g_pRenderText->SetFont(g_hFont);
		break;
	}
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), text, sizeof(text), &Fontsize);

	Fontsize.cy = Fontsize.cy / ((float)WindowHeight / 480);

	int posy = y + ((height / 2) - (Fontsize.cy / 2));

	return posy;
}

void LuaRenderImage(int imageIndex, float x, float y, float w, float h) {
	SEASON3B::RenderImage(imageIndex, x, y, w, h);
}

void LuaRenderBitmap(int Texture, float x, float y, float w, float h, float u, float v, float uWidth, float vHeight, int Scale, int StartScale, float Alpha) {
	RenderBitmap(Texture, x, y, w, h, u, v, uWidth, vHeight, Scale, StartScale, Alpha);
}

void LuaRenderImageScale(int Texture, float x, float y, float w, float h, float u, float v, float uWidth, float vHeight, float Alpha) 
{
	SEASON3B::RenderImageScale(Texture, x, y, w, h, u, v, uWidth, vHeight, Alpha);
}

void LuaRenderImageScaleAuto(int Texture, float x, float y, float w, float h) {
	SEASON3B::RenderImageScaleAuto(Texture, x, y, w, h);
}

int LuaCheckIsRepeatKey(int Key) {
	return SEASON3B::IsRepeat(Key);
}

int LuaCheckPressedKey(int Key) {
	return SEASON3B::IsPress(Key);
}

void LuaSendKey(int key)
{
	// Pressiona a tecla
	keybd_event((BYTE)key, 0, 0, 0);

	// Solta a tecla
	keybd_event((BYTE)key, 0, KEYEVENTF_KEYUP, 0);
}

int LuaCheckReleasedKey(int Key) {
	return SEASON3B::IsRelease(Key);
}

int LuaCheckWindowOpen(int Key) {
	return g_pNewUISystem->IsVisible(Key);
}

void LuaCloseWindow(int Key) {
	g_pNewUISystem->Hide(Key);
}

void LuaOpenWindow(int Key) {
	g_pNewUISystem->Show(Key);
}

void LuaResetMouseL() {
	MouseLButton = false;
	MouseLButtonPop = false;
	MouseLButtonPush = false;
}

void LuaResetMouseR() {
	MouseRButton = false;
	MouseRButtonPop = false;
	MouseRButtonPush = false;
}

void LuaResetMouseM() {
	MouseMButton = false;
	MouseMButtonPop = false;
	MouseMButtonPush = false;
}

void LuaDisableCLick() 
{
	gInterface.interfaceLock = 1;
	LuaResetMouseL();
	LuaResetMouseR();
	LuaResetMouseM();
}

static BOOL LuaCheckClickClient(lua_State* Lua)
{
	lua_pushnumber(Lua, MouseLButton);
	return true;
};

int LuaGetWindowWidth() 
{
	return GWidescreen.g_WideWindowWidth;
}

int LuaGetWindowWidthAdd() {
	return GWidescreen.g_WideWindowWidthAdd;
}

float LuaGetImageWidth(int texture) {
	BITMAP_t* pImage = &Bitmaps[texture];
	return pImage->Width;
}

float LuaGetImageHeight(int texture) {
	BITMAP_t* pImage = &Bitmaps[texture];
	return pImage->Height;
}

void ItemByteConvert(BYTE* lpMsg, int ItemIndex, int Level, int dur, int Op1, int Op2, int Op3, int Exc, int Ancient, int JoH, int Socket, BYTE Sock1, BYTE Sock2, BYTE Sock3, BYTE Sock4, BYTE Sock5)
{
	lpMsg[0] = ItemIndex & 0xFF;

	lpMsg[1] = 0;
	lpMsg[1] |= Level * 8;
	lpMsg[1] |= Op1 * 128;
	lpMsg[1] |= Op2 * 4;
	lpMsg[1] |= Op3 & 3;

	lpMsg[2] = dur;

	lpMsg[3] = 0;
	lpMsg[3] |= (ItemIndex & 256) >> 1;
	lpMsg[3] |= ((Op3 > 3) ? 64 : 0);
	lpMsg[3] |= Exc;

	lpMsg[4] = Ancient;

	lpMsg[5] = 0;
	lpMsg[5] |= (ItemIndex & gItemManager->GetItem(15, 0)) >> 5;
	lpMsg[5] |= ((0 & 128) >> 4);
	lpMsg[5] |= ((0 & 1) << 1);
	lpMsg[5] |= ((0 & 1) << 2);

	if (JoH) {
		lpMsg[6] = JoH;
	}
	else {
		lpMsg[6] = Socket;
	}

	lpMsg[7] = Sock1;
	lpMsg[8] = Sock2;
	lpMsg[9] = Sock3;
	lpMsg[10] = Sock4;
	lpMsg[11] = Sock5;
}

void LuaRenderTooltipComplete(int x, int y, int ItemIndex, int Level, int dur, int Op1, int Op2, int Op3, int Exc, int Ancient, int JoH, int Item380, int Socket, int Sock1, int Sock2, int Sock3, int Sock4, int Sock5) {
	EnableAlphaTest(1);

	static BYTE ItemBuffer[12];
	std::memset(&ItemBuffer[0], 0, sizeof(ItemBuffer));

	ItemByteConvert(ItemBuffer, ItemIndex, Level, dur, Op1, Op2, Op3, Exc, Ancient, JoH, Socket, Sock1, Sock2, Sock3, Sock4, Sock5);

	auto m_Item = g_pNewItemMng->CreateItem(ItemBuffer);

	if (m_Item) {
		m_Item->option_380 = (Item380 != 0);
	}

	RenderItemInfo(x, y, m_Item, false);

	DisableAlphaBlend();

	if (m_Item) {
		g_pNewItemMng->DeleteItem(m_Item);
		m_Item = nullptr;
	}
}

static BOOL LuaRenderTooltip(lua_State* Lua)
{
	EnableAlphaTest(1);

	BYTE m_Buff[12];

	m_Buff[0] = lua_tointeger(Lua, 3) & 0xFF;

	m_Buff[1] = 0;
	m_Buff[1] |= (BYTE)lua_tointeger(Lua, 4) * 8;
	m_Buff[1] |= (BYTE)lua_tointeger(Lua, 5) * 128;
	m_Buff[1] |= (BYTE)lua_tointeger(Lua, 6) * 4;
	m_Buff[1] |= (BYTE)lua_tointeger(Lua, 7) & 3;

	m_Buff[2] = (BYTE)lua_tointeger(Lua, 8);

	m_Buff[3] = 0;
	m_Buff[3] |= ((short)lua_tointeger(Lua, 3) & 256) >> 1;
	m_Buff[3] |= (((BYTE)lua_tointeger(Lua, 7) > 3) ? 64 : 0);
	m_Buff[3] |= (BYTE)lua_tointeger(Lua, 9);

	m_Buff[4] = (BYTE)lua_tointeger(Lua, 10);

	m_Buff[5] = 0;
	m_Buff[5] |= (lua_tointeger(Lua, 3) & gItemManager->GetItem(15, 0)) >> 5;
	m_Buff[5] |= ((0 & 128) >> 4);
	m_Buff[5] |= ((0 & 1) << 1);
	m_Buff[5] |= ((0 & 1) << 2);

	m_Buff[6] = 0;

	m_Buff[7] = 0xFF;
	m_Buff[8] = 0xFF;
	m_Buff[9] = 0xFF;
	m_Buff[10] = 0xFF;
	m_Buff[11] = 0xFF;

	auto m_Item = g_pNewItemMng->CreateItem(m_Buff);

	RenderItemInfo((int)lua_tointeger(Lua, 1), (int)lua_tointeger(Lua, 2), m_Item, false);

	DisableAlphaBlend();

	if (m_Item) {
		g_pNewItemMng->DeleteItem(m_Item);
		m_Item = nullptr;
	}

	lua_pop(Lua, 9);
	return true;
}

void LuaRenderItem(float sx, float sy, float w, float h, int Type, int Level, int Option1, int ExtOption) {
	EndBitmap();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(1.f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	RenderItem3D(sx, sy, w, h, Type, Level*8, Option1, ExtOption, false);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();
}

void LuaSendMessageClient(char* text) {
	g_pChatListBox->AddText("", text, SEASON3B::TYPE_SYSTEM_MESSAGE);
}

void LuaLockPlayerWalk() {
	LockPlayerWalk = true;
}

void LuaUnlockPlayerWalk() {
	LockPlayerWalk = false;
}

void LuaDrawTooltip(int x, int y, char* text) {
	RenderTipText(x, y, text);
}

void LuaSetLockInterfaces() {
	ErrorMessage = 1;
}

void LuaSetUnlockInterfaces() {
	ErrorMessage = 0;
}

void LuaHideHeroBoolean() {
	g_bHideHeroBoolean = true;
}

void LuaShowHeroBoolean() {
	g_bHideHeroBoolean = false;
}

std::string LuaGetLanguage() {
	return g_strSelectedML;
}

void LuaPlaySound(int id) {
	PlayBuffer(id);
}

void LuaStopSound(int id) {
	StopBuffer(id, true);
}

static BOOL LuaGLSwitchBlend(lua_State* Lua)
{
	EnableAlphaBlend();
	return true;
};

static BOOL LuaGLSwitch(lua_State* Lua)
{
	DisableAlphaBlend();
	return true;
};

int GetWideX()
{
	return GetCenterX(640);	//	WIDESCREEM_COND
}

static BOOL LuaRenderText2(lua_State* Lua)
{
	g_pRenderText->RenderText((int)lua_tonumber(Lua, 1), (int)lua_tonumber(Lua, 2), lua_tostring(Lua, 3), (int)lua_tonumber(Lua, 4), 0, (int)lua_tonumber(Lua, 5), 0);
	lua_pop(Lua, 3);
	return true;
};

void LuaInterface::RegisterInterface(lua_State* Lua)
{
	LuaModule(Lua).fun("RenderText", &LuaRenderText);
	LuaModule(Lua).fun("RenderText2", &LuaRenderText);
	LuaModule(Lua).fun("RenderText3", &LuaRenderText);
	LuaModule(Lua).fun("RenderText4", &LuaRenderText3);
	LuaModule(Lua).fun("RenderText5", &LuaRenderText2);
	LuaModule(Lua).fun("EnableAlphaTest", &LuaEnableAlphaTest);
	LuaModule(Lua).fun("DisableAlphaBlend", &LuaDisableAlphaBlend);
	LuaModule(Lua).fun("glColor3f", &LuaglColor3f);
	LuaModule(Lua).fun("glColor4f", &LuaglColor4f);
	LuaModule(Lua).fun("DrawBar", &LuaDrawBar);
	LuaModule(Lua).fun("EndDrawBar", LuaEndDrawBar);
	LuaModule(Lua).fun("SetFontType", LuaSetFontType);
	LuaModule(Lua).fun("SetTextBg", LuaSetFontBg);
	LuaModule(Lua).fun("SetTextColor", LuaSetTextColor);
	LuaModule(Lua).fun("GetTextPosY", LuaGetTextPosY);
	LuaModule(Lua).fun("PlaySound", &LuaPlaySound);
	LuaModule(Lua).fun("StopSound", &LuaStopSound);
	LuaModule(Lua).fun("MousePosX", &MousePosX);
	LuaModule(Lua).fun("MousePosY", &MousePosY);
	LuaModule(Lua).fun("RenderImage", &LuaRenderImage);
	LuaModule(Lua).fun("RenderImage2", &LuaRenderBitmap);
	LuaModule(Lua).fun("RenderImage3", &LuaRenderImageScale);
	LuaModule(Lua).fun("RenderImageScale", &LuaRenderImageScale);
	LuaModule(Lua).fun("RenderImageAuto", &LuaRenderImageScaleAuto);
	LuaModule(Lua).fun("CheckRepeatKey", &LuaCheckIsRepeatKey);
	LuaModule(Lua).fun("CheckPressedKey", &LuaCheckPressedKey);
	LuaModule(Lua).fun("SendKey", &LuaSendKey);
	LuaModule(Lua).fun("CheckReleasedKey", &LuaCheckReleasedKey);
	LuaModule(Lua).fun("CheckWindowOpen", &LuaCheckWindowOpen);
	LuaModule(Lua).fun("CloseWindow", &LuaCloseWindow);
	LuaModule(Lua).fun("OpenWindow", &LuaOpenWindow);
	LuaModule(Lua).fun("ResetMouseL", &LuaResetMouseL);
	LuaModule(Lua).fun("ResetMouseR", &LuaResetMouseR);
	LuaModule(Lua).fun("ResetMouseM", &LuaResetMouseM);
	LuaModule(Lua).fun("GetImageWidth", &LuaGetImageWidth);
	LuaModule(Lua).fun("GetImageHeight", &LuaGetImageHeight);
	LuaModule(Lua).fun("WindowGetWidth", &LuaGetWindowWidth);
	LuaModule(Lua).fun("WindowGetWidthAdd", &LuaGetWindowWidthAdd);
	LuaModule(Lua).fun("WindowGetWidthAdd", &LuaGetWindowWidthAdd);
	LuaModule(Lua).fun("ShowDescriptionComplete", &LuaRenderTooltipComplete);
	LuaModule(Lua).fun("ShowItemDescription", &LuaRenderTooltip);
	LuaModule(Lua).fun("SendMessageClient", &LuaSendMessageClient);
	LuaModule(Lua).fun("DisableClickClient", &LuaDisableCLick);
	LuaModule(Lua).fun("LockPlayerWalk", &LuaLockPlayerWalk);
	LuaModule(Lua).fun("UnlockPlayerWalk", &LuaUnlockPlayerWalk);
	LuaModule(Lua).fun("CreateItem", &LuaRenderItem);
	LuaModule(Lua).fun("DrawTooltip", &LuaDrawTooltip);
	LuaModule(Lua).fun("SetLockInterfaces", &LuaSetLockInterfaces);
	LuaModule(Lua).fun("SetUnlockInterfaces", &LuaSetUnlockInterfaces);
	LuaModule(Lua).fun("GetLanguage", &LuaGetLanguage);
	lua_register(Lua, "CheckClickClient", LuaCheckClickClient);
	LuaModule(Lua).fun("GetWideX", &GetWideX);
	lua_register(Lua, "SetBlend", LuapSetBlend);
	LuaModule(Lua).fun("RenderImage", &RenderImageLua);
	lua_register(Lua, "GLSwitchBlend", LuaGLSwitchBlend);
	lua_register(Lua, "GLSwitch", LuaGLSwitch);
	LuaModule(Lua).fun("HideHeroBoolean", &LuaHideHeroBoolean);
	LuaModule(Lua).fun("ShowHeroBoolean", &LuaShowHeroBoolean);
}

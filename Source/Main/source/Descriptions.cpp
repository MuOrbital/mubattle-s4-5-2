#include "stdafx.h"
#include "Descriptions.h"
#include "LuaStack.hpp"
#include "LuaUser.h"
#include "LuaItemObject.h"
#include "Console.h"
#include "UIControls.h"

using namespace luaaa;

CDescriptions::CDescriptions() : m_Lua()
{

}

CDescriptions::~CDescriptions()
{

}
void CDescriptions::SetDescriptionsItem(int Line, ITEM* itemObject)
{
	this->DescriptionsCall(Line, itemObject);
}

int CDescriptions::DescriptionsCall(int Line, ITEM* itemObject)
{
	if (itemObject == 0) 
	{
		return Line;
	}

	gDescriptions->m_Lua.Generic_Call("SetDescriptions", "iij>i", Line, itemObject->Type, itemObject, &Line);

	return Line;
}

void CreateDescription(int Line, char* text, int color, int bold) 
{
	wsprintf(TextList[Line], text);

	TextListColor[Line] = color;

	TextBold[Line] = bold;
}

void AddNewColorDescription(int Index, int R, int G, int B) 
{
	NEWCOLOR_3f color;

	color.R = R;

	color.G = G;

	color.B = B;

	gDescriptions->m_Color3f.insert(std::make_pair(Index, color));
}

int SetNewColorDescription(int colorID) 
{
	if (gDescriptions->m_Color3f.count(colorID) > 0) 
	{
		glColor3f(gDescriptions->m_Color3f[colorID].R, gDescriptions->m_Color3f[colorID].G, gDescriptions->m_Color3f[colorID].B);

		return 1;
	}
	else 
	{
		return 0;
	}
}

void AddNewColorBackGroundDescription(int Index, int fR, int fG, int fB, int fA, int bR, int bG, int bB, int bA) 
{
	NEW_COLOR_FULL color;

	color.fR = fR;

	color.fG = fG;
	
	color.fB = fB;
	
	color.fA = fA;
	
	color.bR = bR;
	
	color.bG = bG;
	
	color.bB = bB;
	
	color.bA = bA;
	
	gDescriptions->m_ColorFull.insert(std::make_pair(Index, color));
}

int SetNewColorBackGroundDescription(int colorID) 
{
	if (gDescriptions->m_ColorFull.count(colorID) > 0) 
	{
		g_pRenderText->SetBgColor(gDescriptions->m_ColorFull[colorID].bR, gDescriptions->m_ColorFull[colorID].bG, gDescriptions->m_ColorFull[colorID].bB, gDescriptions->m_ColorFull[colorID].bA);
		
		g_pRenderText->SetTextColor(gDescriptions->m_ColorFull[colorID].fR, gDescriptions->m_ColorFull[colorID].fG, gDescriptions->m_ColorFull[colorID].fB, gDescriptions->m_ColorFull[colorID].fA);
		
		return 1;
	}
	else 
	{
		return 0;
	}
}

void CDescriptions::Init()
{
	this->m_Color3f.clear();

	this->m_ColorFull.clear();

	RegisterLuaItemObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("createDescription", &CreateDescription);

	LuaModule(this->m_Lua.GetState()).fun("AddNewColorDescription", &AddNewColorDescription);

	LuaModule(this->m_Lua.GetState()).fun("AddNewColorBackGroundDescription", &AddNewColorBackGroundDescription);

	this->m_LuaUser.RegisterLuaUser(this->m_Lua.GetState());

	this->m_Lua.DoFile("Data//Lua//ItemManager//Descriptions.lua");

	this->m_Lua.Generic_Call("SetNewColorDescription", "i>");

	this->m_Lua.Generic_Call("SetNewColorBackGroundDescription", "i>");
}

void CDescriptions::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_Color3f.clear();

	this->m_ColorFull.clear();

	RegisterLuaItemObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("createDescription", &CreateDescription);

	LuaModule(this->m_Lua.GetState()).fun("AddNewColorDescription", &AddNewColorDescription);

	LuaModule(this->m_Lua.GetState()).fun("AddNewColorBackGroundDescription", &AddNewColorBackGroundDescription);

	this->m_LuaUser.RegisterLuaUser(this->m_Lua.GetState());

	this->m_Lua.DoFile("Data//Lua//ItemManager//Descriptions.lua");

	this->m_Lua.Generic_Call("SetNewColorDescription", "i>");

	this->m_Lua.Generic_Call("SetNewColorBackGroundDescription", "i>");
}
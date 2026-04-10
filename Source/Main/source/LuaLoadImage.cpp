#include "stdafx.h"
#include "LuaLoadImage.h"
#include "LuaStack.hpp"
#include "ZzzTexture.h"

LuaLoadImage gLuaLoadImage;

int m_LuaStartImage = 200000;

LuaLoadImage::LuaLoadImage() : m_Lua()
{
	
}

LuaLoadImage::~LuaLoadImage()
{
}

void LuaImage(char* string, int textureID) 
{
	LoadBitmap(string, textureID);
}

int LoadImageByDir(char* string) 
{
	LoadBitmap(string, m_LuaStartImage);

	return m_LuaStartImage++;
}

void LuaUnloadImage(int id)
{
	DeleteBitmap(id);
}

void LuaLoadImage::SetFunctions(lua_State * lua)
{
	luaaa::LuaModule(lua).fun("LoadImage", &LuaImage);

	luaaa::LuaModule(lua).fun("LoadImageByDir", &LoadImageByDir);
}

void LuaLoadImage::Unload()
{
	this->m_Lua.Generic_Call("UnloadImages", ">");
}

void LuaLoadImage::Init()
{
	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImage", &LuaImage);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("UnloadImage", &LuaUnloadImage);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImageByDir", &LoadImageByDir);

	this->m_Lua.DoFile("Data\\Lua\\Controller\\LoadImages.lua");

	this->m_Lua.Generic_Call("LoadImages", ">");
}

void LuaLoadImage::RestartLua()
{
	this->m_Lua.Generic_Call("UnloadImages", ">");

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImage", &LuaImage);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("UnloadImage", &LuaUnloadImage);

	this->m_Lua.DoFile("Data\\Lua\\Controller\\LoadImages.lua");

	this->m_Lua.Generic_Call("LoadImages", ">");
}
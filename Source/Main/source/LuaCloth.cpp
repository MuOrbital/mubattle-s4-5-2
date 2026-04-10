#include "stdafx.h"
#include "LuaCloth.h"
#include "LuaStack.hpp"

LuaCloth::LuaCloth()
{
}

LuaCloth::~LuaCloth()
{
}

void LuaCloth::RegisterClassCloth(lua_State* lua)
{
	luaaa::LuaClass<ClothClass> LuaCloth(lua, "CapeStack");
	LuaCloth.ctor<DWORD>();
	LuaCloth.fun("Create", &ClothClass::Create);
	LuaCloth.fun("SetWindMinMax", &ClothClass::SetWindMax);
	LuaCloth.fun("Collision", &ClothClass::AddCollisionSphere);
}

#include "stdafx.h"
#include "ZzzBMD.h"
#include "LuaBMD.h"
#include "LuaStack.hpp"

LuaBMD::LuaBMD()
{
}

LuaBMD::~LuaBMD()
{
}

void LuaBMD::RegisterClassBMD(lua_State * lua)
{
	luaaa::LuaClass<BMDClass> luaBMD(lua, "BMD");
	luaBMD.ctor<DWORD>();
	luaBMD.fun("GetLight", &BMDClass::GetLight);
	luaBMD.fun("SetLight", &BMDClass::SetLight);
	luaBMD.fun("RenderBody", &BMDClass::RenderBody);
	luaBMD.fun("RenderMesh", &BMDClass::RenderMesh);
	luaBMD.fun("glColor3fv", &BMDClass::glColor3f);
	luaBMD.fun("BeginRender", &BMDClass::BeginRender);
	luaBMD.fun("EndRender", &BMDClass::EndRender);
	luaBMD.fun("setMesh", &BMDClass::setMesh);
	luaBMD.fun("RenderShadowModel", &BMDClass::RenderShadowModel);
	luaBMD.fun("TransformPosition", &BMDClass::TransformPosition);
	luaBMD.fun("TransformPosition2", &BMDClass::TransformPosition2);
	luaBMD.fun("CreateSprite", &BMDClass::CreateSprites);
	luaBMD.fun("CreateParticle", &BMDClass::CreateParticles);
	luaBMD.fun("CreateEffect", &BMDClass::CreateEffects);
	luaBMD.fun("CreateEffectsInFenrirUsingSkill", &BMDClass::CreateEffectsInFenrirUsingSkill);
}

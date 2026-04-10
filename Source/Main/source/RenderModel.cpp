#include "stdafx.h"
#include "RenderModel.h"
#include "LuaBMD.h"
#include "LuaObject.h"

CRenderModel gRenderModel;

CRenderModel::CRenderModel() : m_Lua()
{

}

CRenderModel::~CRenderModel()
{

}

int CRenderModel::RenderModelBodyLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct)
{
	int retorno = 0;

	gRenderModel.m_Lua.Generic_Call("RenderModelBody", "iii>i", BMDStruct, ObjectStruct, ItemIndex, &retorno);

	return retorno;
}

int CRenderModel::RenderModelBodyColorLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct)
{
	int retorno = 0;

	gRenderModel.m_Lua.Generic_Call("RenderModelBodyColor", "iii>i", BMDStruct, ObjectStruct, ItemIndex, &retorno);

	return retorno;
}

int CRenderModel::RenderModelBodyColor2Lua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct)
{
	int retorno = 0;

	gRenderModel.m_Lua.Generic_Call("RenderModelBodyColor2", "iii>i", BMDStruct, ObjectStruct, ItemIndex, &retorno);

	return retorno;
}

void CRenderModel::Init()
{
	this->m_LuaLoadImage.SetFunctions(this->m_Lua.GetState());

	this->m_Lua.DoFile("Data//Lua//EffectSystem//RenderModel.lua");

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());
}

void CRenderModel::LoadImageRenderModel()
{
	this->m_Lua.Generic_Call("StartLoadImages", ">");
}

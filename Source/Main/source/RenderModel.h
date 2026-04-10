#pragma once

#include "Lua.h"
#include "LuaLoadImage.h"
#include "LuaBMD.h"

class CRenderModel {
public:
	CRenderModel();
	~CRenderModel();

	void Init();

	int RenderModelBodyLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct);
	int RenderModelBodyColorLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct);
	int RenderModelBodyColor2Lua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct);
	void LoadImageRenderModel();

public:
	Lua m_Lua;

private:
	LuaLoadImage m_LuaLoadImage;
	LuaBMD m_LuaBMD;
};

extern CRenderModel gRenderModel;
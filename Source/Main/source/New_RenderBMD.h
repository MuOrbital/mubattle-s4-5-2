#pragma once

#if jdk_shader_local330

#include "New_ModelBMD.h"

class BMD;
class CGMNewRenderBMD
{
public:
	virtual void Render(OGL330MODEL::RenderMeshVAO&) = 0;
};

extern CGMNewRenderBMD* g_NewRenderBMD;

class CGMShaderBMD : public CGMNewRenderBMD
{
public:
	CGMShaderBMD() {}
	~CGMShaderBMD() {}
	void Render(OGL330MODEL::RenderMeshVAO&);
};

#endif
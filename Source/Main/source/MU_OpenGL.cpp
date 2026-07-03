#include "stdafx.h"
#include "MU_OpenGL.h"

#if jdk_shader_local330
#include "ZzzBMD.h"
#include "New_ModelBMD.h"
#include "New_RenderBMD.h"

typedef std::vector<int> VectorInt;

namespace OGL330
{
	bool m_ShaderState = false;

	typedef struct _TypeRender_t
	{
		int m_TexureID; int m_ShaderID;
		_TypeRender_t() : m_TexureID(-1), m_ShaderID(SHADER_330_NONE) {}
		_TypeRender_t(int iTex, int iShd) : m_TexureID(iTex), m_ShaderID(iShd) {}
	}TypeRender_t;

	typedef std::map<int, TypeRender_t> TextureManager;

	TextureManager m_RenderTexture;
	VectorInt m_FlagRenderContainer;
}

void OGL330::Init()
{
	m_ShaderState = true;

	g_NewRenderBMD = new CGMShaderBMD();

	OGL330MODEL::Init();

	AddTexturShader(RENDER_CHROME, BITMAP_CHROME, SHADER_330_CHROME1);
	AddTexturShader(RENDER_CHROME2, BITMAP_CHROME2, SHADER_330_CHROME2);
	AddTexturShader(RENDER_CHROME3, BITMAP_CHROME2, SHADER_330_CHROME3);
	AddTexturShader(RENDER_CHROME4, BITMAP_CHROME2, SHADER_330_CHROME4);
	AddTexturShader(RENDER_CHROME5, -1, SHADER_330_CHROME5);
	AddTexturShader(RENDER_CHROME6, BITMAP_CHROME6, SHADER_330_CHROME6);
	AddTexturShader(RENDER_CHROME7, -1, SHADER_330_CHROME7);
	AddTexturShader(RENDER_OIL, -1, SHADER_330_OIL);
	AddTexturShader(RENDER_METAL, BITMAP_SHINY, SHADER_330_METAL);
}

bool OGL330::IsShader()
{
	return m_ShaderState;
}

void OGL330::SetShaderState(bool Enable)
{
	m_ShaderState = Enable;
}

void OGL330::SwitchStatePipeline()
{
	if (!IsShader())
	{
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	OGL330MODEL::InvalidateShaderCache();
}

void OGL330::Release()
{
	SAFE_DELETE(g_NewRenderBMD);
}

bool OGL330::AddTexturShader(int FlagRender, int Texture, int IDShader)
{
	if (Texture >= BITMAP_NONAMED_TEXTURES_END) return false;

	if (IDShader < 0 || IDShader >= SHADER_330_ALL) return false;

	m_FlagRenderContainer.push_back(FlagRender);
	m_RenderTexture.insert(TextureManager::value_type(FlagRender, TypeRender_t(Texture, IDShader)));
	return true;
}

int OGL330::GetFlagRenderTexrute(int iFlag)
{
	for (int i = 0; i < static_cast<int>(m_FlagRenderContainer.size()); ++i)
	{
		if (m_FlagRenderContainer[i] & iFlag)
		{
			return m_FlagRenderContainer[i];
		}
	}
	return -1;
}

void OGL330::GetTexturShader(int FlagRender, int& outTexrute, int& outShader)
{
	int iEff = GetFlagRenderTexrute(FlagRender);

	if (iEff == -1)
	{
		outTexrute = -1;
		outShader = SHADER_330_NONE;
		return;
	}
	TypeRender_t& rOut = m_RenderTexture[iEff];
	outTexrute = rOut.m_TexureID;
	outShader = rOut.m_ShaderID;
}

#endif

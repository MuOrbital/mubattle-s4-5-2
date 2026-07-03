#pragma once
#if jdk_shader_local330

namespace OGL330
{
	typedef enum _Shader330_t
	{
		SHADER_330_NONE = 0,
		SHADER_330_BLENDMESH,
		SHADER_330_CHROME1,
		SHADER_330_CHROME2,
		SHADER_330_CHROME3,
		SHADER_330_CHROME4,
		SHADER_330_CHROME5,
		SHADER_330_CHROME6,
		SHADER_330_CHROME7,
		SHADER_330_OIL,
		SHADER_330_METAL,
		SHADER_330_ALL
	} Shader330_t;

	void Init();
	void Release();
	bool IsShader();
	void SetShaderState(bool Enable);
	void SwitchStatePipeline();
	bool AddTexturShader(int FlagRender, int Texture, int IDShader);
	void GetTexturShader(int FlagRender, int& outTexrute, int& outShader);
	int	GetFlagRenderTexrute(int iFlag);
}

#endif

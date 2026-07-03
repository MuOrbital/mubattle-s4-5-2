#include "StdAfx.h"
#include "New_ModelBMD.h"
#include <filesystem> // C++17
#include <iostream>
#include <sstream>
#if jdk_shader_local330
#include "ZzzBMD.h"
#include "ZzzObject.h"
#include "New_RenderBMD.h"

#include "Utilities/Log/muConsoleDebug.h"

using namespace OGL330;
extern int WaterTextureNumber;

namespace OGL330MODEL
{
	std::unordered_map<int, GLuint> shaderProgramMap;
	GLuint g_CurrentShaderID = 0;

	struct ShaderUniformLocations
	{
		GLint projection;
		GLint view;
		GLint bodyLight;
		GLint lightPosition;
		GLint meshUV;
		GLint setting1;
		GLint setting2;
		GLint enableLight;
		GLint texture;
		GLint shadowMode;
		GLint bodyOrigin;
		GLint alphaCutoff;
		GLint bones;

		ShaderUniformLocations()
			: projection(-1), view(-1), bodyLight(-1), lightPosition(-1), meshUV(-1),
			  setting1(-1), setting2(-1), enableLight(-1), texture(-1),
			  shadowMode(-1), bodyOrigin(-1), alphaCutoff(-1), bones(-1)
		{
		}
	};

	static std::unordered_map<GLuint, ShaderUniformLocations> g_UniformLocations;
	static float g_ProjectionMatrix[16];
	static float g_ViewMatrix[16];
	static bool g_HasBatchMatrices = false;
	static std::unordered_map<GLuint, bool> g_MatrixUploaded;
	static std::unordered_map<GLuint, const void*> g_BonePaletteUploaded;

	static ShaderUniformLocations& GetUniformLocations(GLuint shaderID)
	{
		std::unordered_map<GLuint, ShaderUniformLocations>::iterator it = g_UniformLocations.find(shaderID);
		if (it != g_UniformLocations.end())
		{
			return it->second;
		}

		ShaderUniformLocations locations;
		locations.projection = glGetUniformLocation(shaderID, "uProj");
		locations.view = glGetUniformLocation(shaderID, "uView");
		locations.bodyLight = glGetUniformLocation(shaderID, "u_bodyLight");
		locations.lightPosition = glGetUniformLocation(shaderID, "u_lightPosition");
		locations.meshUV = glGetUniformLocation(shaderID, "u_meshUV");
		locations.setting1 = glGetUniformLocation(shaderID, "u_setting1");
		locations.setting2 = glGetUniformLocation(shaderID, "u_setting2");
		locations.enableLight = glGetUniformLocation(shaderID, "u_enableLight");
		locations.texture = glGetUniformLocation(shaderID, "uTexture");
		locations.shadowMode = glGetUniformLocation(shaderID, "u_shadowMode");
		locations.bodyOrigin = glGetUniformLocation(shaderID, "u_bodyOrigin");
		locations.alphaCutoff = glGetUniformLocation(shaderID, "u_alphaCutoff");
		locations.bones = glGetUniformLocation(shaderID, "u_Bones");

		return g_UniformLocations.insert(std::make_pair(shaderID, locations)).first->second;
	}

	static void BuildModelBonePalette(BMD* model, const float* bone, bool translate, std::vector<float>& palette)
	{
		palette.clear();

		if (model == NULL || bone == NULL || model->NumBones <= 0)
		{
			return;
		}

		int boneCount = min(static_cast<int>(model->NumBones), 200);
		float resultScale = translate ? model->BodyScale : 1.0f;
		float scalePre = resultScale;
		bool appScale = (model->m_fRequestScale != 1.0f && model->m_fRequestScale != 0.0f);
		vec3_t origin;
		Vector(0.0f, 0.0f, 0.0f, origin);

		if (translate)
		{
			VectorCopy(model->BodyOrigin, origin);
		}

		if (appScale)
		{
			resultScale *= model->m_fRequestScale;
		}

		if (palette.capacity() < static_cast<size_t>(boneCount * 12))
			palette.reserve(boneCount * 12);
		for (int i = 0; i < boneCount; ++i)
		{
			int iMatIdx = i * 12;
			for (int j = 0; j < 3; ++j)
			{
				const float* target = bone + iMatIdx + j * 4;
				palette.push_back(target[0] * resultScale);
				palette.push_back(target[1] * resultScale);
				palette.push_back(target[2] * resultScale);
				palette.push_back(target[3] * (appScale ? scalePre : resultScale) + origin[j]);
			}
		}
	}
}

GLuint LoadShaderProgramFromFiles(const char* vertexPath, const char* fragmentPath)
{
	auto LoadShaderSource = [](const char* path) -> std::string
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cerr << "Failed to open shader: " << path << std::endl;
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	};

	std::string vertCode = LoadShaderSource(vertexPath);
	const char* vertSrc = vertCode.c_str();
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSrc, nullptr);
	glCompileShader(vertShader);

	GLint success;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(vertShader, 512, nullptr, log);
		std::cerr << "Vertex shader error [" << vertexPath << "]: " << log << std::endl;
	}

	std::string fragCode = LoadShaderSource(fragmentPath);
	const char* fragSrc = fragCode.c_str();
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSrc, nullptr);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(fragShader, 512, nullptr, log);
		std::cerr << "Fragment shader error [" << fragmentPath << "]: " << log << std::endl;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(program, 512, nullptr, log);
		std::cerr << "Program link error: " << log << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

void OGL330MODEL::Init()
{
	const char* shaderNames[SHADER_330_ALL] = {
	"Model",
	"BlendMesh",
	"Chrome1",
	"Chrome2",
	"Chrome3",
	"Chrome4",
	"Chrome5",
	"Chrome6",
	"Chrome7",
	"Oil",
	"Metal"
	};

	char pathBuffer[128];
	char pathBuffer2[128];

	for (int i = 0; i < SHADER_330_ALL; ++i)
	{
		snprintf(pathBuffer, sizeof(pathBuffer), "Data\\Effect\\VBO\\%s.vs", shaderNames[i]);
		snprintf(pathBuffer2, sizeof(pathBuffer2), "Data\\Effect\\VBO\\%s.fs", shaderNames[i]);
		shaderProgramMap[i] = LoadShaderProgramFromFiles(pathBuffer, pathBuffer2);
	}
}


void OGL330MODEL::ConvertOldMeshToVaoMesh(GLuint iModel, bool DelMesh)
{
	UNREFERENCED_PARAMETER(iModel);
	UNREFERENCED_PARAMETER(DelMesh);
}

void OGL330MODEL::UseShader(GLuint shaderID)
{
	if (g_CurrentShaderID != shaderID)
	{
		glUseProgram(shaderID);
		g_CurrentShaderID = shaderID;
	}
}

void OGL330MODEL::UnUseShader()
{
	glUseProgram(0);
	g_CurrentShaderID = -1;
}

void OGL330MODEL::InvalidateShaderCache()
{
	g_CurrentShaderID = static_cast<GLuint>(-1);
}

void OGL330MODEL::SetTargetRender(OBJECT* pObj)
{
	if (!OGL330::IsShader()) return;

	if (pObj != NULL)
	{
		GMMeshShader->BeginBatch();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GMMeshShader->EndBatch())
	{
		GMMeshShader->FlushAllMesh();
		OGL330::SwitchStatePipeline();
	}
}

void OGL330MODEL::BeginUniformBatch()
{
	glGetFloatv(GL_PROJECTION_MATRIX, g_ProjectionMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, g_ViewMatrix);
	g_MatrixUploaded.clear();
	g_BonePaletteUploaded.clear();
	g_HasBatchMatrices = true;
}

void OGL330MODEL::EndUniformBatch()
{
	g_HasBatchMatrices = false;
	g_MatrixUploaded.clear();
}

GLint OGL330MODEL::GetBonesUniform(GLuint shaderID)
{
	return GetUniformLocations(shaderID).bones;
}

GLint OGL330MODEL::GetAlphaCutoffUniform(GLuint shaderID)
{
	return GetUniformLocations(shaderID).alphaCutoff;
}

bool OGL330MODEL::SendBonePalette(GLuint shaderID, const std::shared_ptr<std::vector<float> >& palette)
{
	if (!palette || palette->empty())
	{
		return false;
	}

	ShaderUniformLocations& locations = GetUniformLocations(shaderID);
	if (locations.bones < 0)
	{
		return false;
	}

	const void* paletteId = palette.get();
	std::unordered_map<GLuint, const void*>::iterator it = g_BonePaletteUploaded.find(shaderID);
	if (it != g_BonePaletteUploaded.end() && it->second == paletteId)
	{
		return true;
	}

	glUniform4fv(locations.bones, static_cast<GLsizei>(palette->size() / 4), &(*palette)[0]);
	g_BonePaletteUploaded[shaderID] = paletteId;
	return true;
}

void OGL330MODEL::SendUniform(GLuint shaderID, const mvec4& bodyLight, const mvec4& lightPosition, const mvec4& meshUV, const mvec4& setting1, const mvec4& setting2, const bool enableLight, const bool shadow, vec3_t vBodyOrigin)
{
	ShaderUniformLocations& locations = GetUniformLocations(shaderID);

	if (!g_HasBatchMatrices)
	{
		glGetFloatv(GL_PROJECTION_MATRIX, g_ProjectionMatrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, g_ViewMatrix);
	}

	if (!g_MatrixUploaded[shaderID])
	{
		if (locations.projection != -1)
			glUniformMatrix4fv(locations.projection, 1, GL_FALSE, g_ProjectionMatrix);
		if (locations.view != -1)
			glUniformMatrix4fv(locations.view, 1, GL_FALSE, g_ViewMatrix);
		if (locations.texture != -1)
			glUniform1i(locations.texture, 0);
		g_MatrixUploaded[shaderID] = true;
	}

	if (locations.bodyLight != -1)
		glUniform4f(locations.bodyLight, bodyLight.x, bodyLight.y, bodyLight.z, bodyLight.w);
	if (locations.lightPosition != -1)
		glUniform4f(locations.lightPosition, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
	if (locations.meshUV != -1)
		glUniform4f(locations.meshUV, meshUV.x, meshUV.y, meshUV.z, meshUV.w);
	if (locations.setting1 != -1)
		glUniform4f(locations.setting1, setting1.x, setting1.y, setting1.z, setting1.w);
	if (locations.setting2 != -1)
		glUniform4f(locations.setting2, setting2.x, setting2.y, setting2.z, setting2.w);
	if (locations.enableLight != -1)
		glUniform1i(locations.enableLight, enableLight ? 1 : 0);
	if (locations.shadowMode != -1)
		glUniform1i(locations.shadowMode, shadow ? 1 : 0);
	if (locations.bodyOrigin != -1)
		glUniform3f(locations.bodyOrigin, vBodyOrigin[0], vBodyOrigin[1], vBodyOrigin[2]);
	CoreGLCompat::ApplyFogUniforms(shaderID);
}

using namespace OGL330MODEL;

CGMMeshShader::CGMMeshShader()
{
	m_Transfrom = false;
	m_Data.reserve(64);
	m_Lock = false;
	m_Enabled = true;
	m_BatchDepth = 0;
	memset(m_vLightPosOrg, 0, sizeof(vec3_t));
	memset(m_vLightDirOrg, 0, sizeof(vec3_t));
	memset(m_vLightPos, 0, sizeof(vec3_t));
	memset(m_vLightDir, 0, sizeof(vec3_t));
	m_finalBone = NULL;
}

CGMMeshShader::~CGMMeshShader()
{
	Release();
}

void CGMMeshShader::Release()
{
	m_Data.clear();
	m_CurrentBonePalette.reset();
}

bool CGMMeshShader::IsAlpha(int iType)
{
	if (iType & RENDER_BRIGHT) return true;

	if (iType & RENDER_CHROME3) return true;
	else if (iType & RENDER_CHROME4) return true;
	else if (iType & RENDER_CHROME5) return true;
	else if (iType & RENDER_CHROME7) return true;

	return false;
}

void CGMMeshShader::MakeShaderType(int iShaderType, bool enableLight, bool bAlphaNoUse, float BlendU, float BlendV, RenderMeshVAO& r)
{
	mvec4 bodyLight;

	if (bAlphaNoUse)
	{
		bodyLight.x = r.m_isColor.x * r.m_isAlpha;
		bodyLight.y = r.m_isColor.y * r.m_isAlpha;
		bodyLight.z = r.m_isColor.z * r.m_isAlpha;
		bodyLight.w = 1.f;
	}
	else
	{
		bodyLight.x = r.m_isColor.x;
		bodyLight.y = r.m_isColor.y;
		bodyLight.z = r.m_isColor.z;
		bodyLight.w = r.m_isAlpha;
	}

	// Fixed-function glColor clamps RGB before the texture stage.
	bodyLight.x = bodyLight.x < 0.f ? 0.f : (bodyLight.x > 1.f ? 1.f : bodyLight.x);
	bodyLight.y = bodyLight.y < 0.f ? 0.f : (bodyLight.y > 1.f ? 1.f : bodyLight.y);
	bodyLight.z = bodyLight.z < 0.f ? 0.f : (bodyLight.z > 1.f ? 1.f : bodyLight.z);

	mvec4 lightPosition;

	lightPosition.w = r.m_isAlpha;

	if (enableLight)
	{
		lightPosition.x = m_vLightDir[0];
		lightPosition.y = m_vLightDir[1];
		lightPosition.z = m_vLightDir[2];
	}
	else
	{
		lightPosition.x = 0.f;
		lightPosition.y = 0.f;
		lightPosition.z = 0.f;
	}

	mvec4 meshUV;
	mvec4 setting1;
	mvec4 setting2;

	switch (iShaderType)
	{
	case SHADER_330_BLENDMESH:
	{
		meshUV.x = BlendU;
		meshUV.y = BlendV;
		meshUV.z = 1.f;
		meshUV.w = 0.f;
	}
	break;
	case SHADER_330_CHROME1:
	{
		setting1.x = 1.f;
		setting1.y = 0.f;
		setting1.z = (int)WorldTime % 10000 * 0.0001f;
		setting1.w = 0.f;

		setting2.x = 0.5f;
		setting2.y = 0.5f;
		setting2.z = 2.0f;
		setting2.w = 1.f;
	}
	break;
	case SHADER_330_CHROME2:
	{
		setting1.x = 1.f;
		setting1.y = 0.f;
		setting1.z = 0.f;
		setting1.w = (int)WorldTime % 5000 * 0.00024f - 0.4f;

		setting2.x = 0.8f;
		setting2.y = 2.f;
		setting2.z = 1.f;
		setting2.w = 3.f;
	}
	break;
	case SHADER_330_CHROME3:
	{
		setting2.x = 0.0;
		setting2.y = -0.1f;
		setting2.z = -0.8f;
		setting2.w = 1.f;
	}
	break;
	case SHADER_330_CHROME4:
	{
		setting1.x = cosf(WorldTime * 0.001f);
		setting1.y = sinf(WorldTime * 0.002f);
		setting1.z = 1.f;
		setting1.w = (int)WorldTime % 10000 * 0.0001f;

		meshUV.x = BlendU;
		meshUV.y = BlendV;
		meshUV.z = 1.f;
		meshUV.w = 0.f;

		setting2.x = 0.5f;
		setting2.y = 3.f;
		setting2.z = 0.5f;
		setting2.w = 3.f;
	}
	break;
	case SHADER_330_CHROME5:
	{
		setting1.x = cosf(WorldTime * 0.001f);
		setting1.y = sinf(WorldTime * 0.002f);
		setting1.z = 1.f;
		setting1.w = (int)WorldTime % 10000 * 0.0001f;

		setting2.x = 2.5f;
		setting2.y = 1.f;
		setting2.z = 3.f;
		setting2.w = 5.f;
	}
	break;
	case SHADER_330_CHROME6:
	{
		setting1.x = 0.8f;
		setting1.y = 2.f;
		setting1.z = (int)WorldTime % 5000 * 0.00024f - 0.4f;
		setting1.w = 0.f;
	}
	break;
	case SHADER_330_CHROME7:
	{
		setting1.x = 0.8f;
		setting1.y = 0.8f;
		setting1.z = WorldTime;
		setting1.w = 0.00006f;
	}
	break;
	case SHADER_330_METAL:
	{
		setting2.x = 0.5f;
		setting2.y = 0.2f;
		setting2.z = 0.5f;
		setting2.w = 0.5f;
	}
	break;
	case SHADER_330_OIL:
	{
		meshUV.x = BlendU;
		meshUV.y = BlendV;
		meshUV.z = 1.f;
		meshUV.w = 0.f;
	}
	break;
	}

	r.m_bodyLight = bodyLight;
	r.m_lightPosition = lightPosition;
	r.m_meshUV = meshUV;
	r.m_setting1 = setting1;
	r.m_setting2 = setting2;

	r.m_Shader = shaderProgramMap[iShaderType];
}

void CGMMeshShader::SetHighLight(bool bHighLight, bool bBattleCastle)
{
	if (bHighLight)
	{
		Vector(1.3f, 0.f, 2.f, m_vLightPosOrg);
	}
	else if (bBattleCastle)
	{
		Vector(0.5f, -1.f, 1.f, m_vLightPosOrg);
	}
	else
	{
		Vector(0.f, -1.5f, 0.f, m_vLightPosOrg);
	}

	VectorCopy(m_vLightPosOrg, m_vLightDirOrg);
}

void CGMMeshShader::AddMeshCommand(BMD* pSrc, int idx, int RFlag, float Alpha, int BlendMesh, float Light, float BlendU, float BlendV, int Texture)
{
	if (!pSrc || idx < 0 || idx >= pSrc->NumMeshs)
	{
		return;
	}

	if (pSrc->New_Meshs.size() != static_cast<size_t>(pSrc->NumMeshs) || pSrc->New_Meshs[idx].VAO == 0)
	{
		pSrc->LoadMeshToVAO();
		pSrc->UploadAllToGPU();
	}

	if (pSrc->New_Meshs.size() != static_cast<size_t>(pSrc->NumMeshs) || pSrc->New_Meshs[idx].VAO == 0)
	{
		return;
	}

	VAOMesh& rMesh = pSrc->New_Meshs[idx];

	int iSourceTex = pSrc->IndexTexture[rMesh.Texture];

	if (iSourceTex == BITMAP_HIDE || (RFlag != RENDER_TEXTURE && (iSourceTex == BITMAP_SKIN
		|| iSourceTex == BITMAP_HAIR))
		)
	{
		return;
	}

	switch (iSourceTex)
	{
	case BITMAP_SKIN:
		if (pSrc->HideSkin == true)
			return;
		iSourceTex = pSrc->Skin + BITMAP_SKIN;
		break;
	case BITMAP_WATER:
		iSourceTex = (int)WaterTextureNumber + BITMAP_WATER;
		break;
	case BITMAP_HAIR:
		if (pSrc->HideSkin == true)
			return;
		iSourceTex = BITMAP_HAIR + (pSrc->Skin - 8);
		break;
	}

	if (Texture != -1)
		iSourceTex = Texture;

	if (iSourceTex != -1)
	{
		Mesh_t& rOldMesh = pSrc->Meshs[idx];
		m_Data.push_back(RenderMeshVAO());
		RenderMeshVAO& rNew = m_Data[m_Data.size() - 1];
		rNew.m_OldBMD = pSrc;
		rNew.m_IndexMesh = idx;
		rNew.m_FlagRender = RFlag;
		rNew.m_isAlpha = Alpha > 0.99 ? 1.f : Alpha;
		rNew.m_isLight = !pSrc->bOffLight;
		rNew.m_isLight = pSrc->LightEnable;

		memcpy(&rNew.m_isColor.x, pSrc->BodyLight, sizeof(float) * 3);

		int iShaderType = SHADER_330_NONE;

		if (BITMAP_HIDE == iSourceTex && RFlag != (RENDER_SHADOWMAP | RENDER_TEXTURE))
		{
			return;
		}

		OGL330::GetTexturShader(rNew.m_FlagRender, rNew.m_TextureID, iShaderType);

		if (rNew.m_TextureID == -1)
			rNew.m_TextureID = iSourceTex;

		if (Texture != -1)
			rNew.m_TextureID = Texture;

		if (iShaderType == SHADER_330_NONE)
		{
			if (rMesh.m_csTScript)
			{
				if (rMesh.m_csTScript->getStreamMesh())
				{
					iShaderType = SHADER_330_BLENDMESH;
				}
			}
			else if (idx == BlendMesh || idx == pSrc->StreamMesh)
			{
				iShaderType = SHADER_330_BLENDMESH;
				rNew.m_isLight = false;
			}

			if (BlendMesh <= -2 || rMesh.Texture == BlendMesh)
			{
				if (!(rNew.m_FlagRender & RENDER_BRIGHT))
					rNew.m_FlagRender |= RENDER_BRIGHT;

				if (BlendU != 0.f || BlendV != 0.f)
				{
					iShaderType = SHADER_330_BLENDMESH;
				}

				rNew.m_isColor.x *= Light;
				rNew.m_isColor.y *= Light;
				rNew.m_isColor.z *= Light;
				rNew.m_isAlpha = 1.f;
			}
		}
		else
		{
			if (rOldMesh.NoneBlendMesh)
			{
				m_Data.pop_back();
				return;
			}

			if (rMesh.m_csTScript)
			{
				if (rMesh.m_csTScript->getNoneBlendMesh())
				{
					m_Data.pop_back();
					return;
				}
			}

			rNew.m_isLight = false;
		}

		if (rNew.m_isLight)
		{
			vec34_t mat;
			Vector(0.f, 0.f, -45.f, pSrc->ShadowAngle);
			AngleMatrix(pSrc->ShadowAngle, mat);
			VectorIRotate(m_vLightDirOrg, mat, m_vLightDir);
		}

		MakeShaderType(iShaderType, rNew.m_isLight, IsAlpha(RFlag), BlendU, BlendV, rNew);
		rNew.m_BonePalette = m_CurrentBonePalette;
	}
}

void CGMMeshShader::AddBoneTransform(BMD* model, float(*BoneMatrix)[3][4], bool trans)
{
	m_Transfrom = trans;
	m_finalBone = BoneMatrix[0][0];
	if (!m_CurrentBonePalette || m_CurrentBonePalette.use_count() != 1)
		m_CurrentBonePalette.reset(new std::vector<float>());
	BuildModelBonePalette(model, m_finalBone, trans, *m_CurrentBonePalette);
}

void CGMMeshShader::FlushAllMesh()
{
	if (m_Data.empty())
	{
		return;
	}

	OGL330MODEL::BeginUniformBatch();
	for (MeshVAO::iterator iter = m_Data.begin(); iter != m_Data.end(); ++iter)
	{
		g_NewRenderBMD->Render(*iter);
	}

	m_Data.clear();
	OGL330MODEL::UnUseShader();
	OGL330MODEL::EndUniformBatch();
}

#endif

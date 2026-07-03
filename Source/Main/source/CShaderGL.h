#pragma once



struct MeshRenderState
{
	alignas(16) int RenderFlag;          // ocupa 4 bytes
	alignas(16) float Alpha;             // ocupa 4 bytes
	alignas(16) float BlendMeshTexCoordU;// ocupa 4 bytes
	alignas(16) float BlendMeshTexCoordV;// ocupa 4 bytes
	alignas(16) float BlendMeshLight;    // ocupa 4 bytes
	float padding[3]; // relleno para que el tamaño total sea múltiplo de 16
};

class CShaderGL
{
public:
	CShaderGL();
	virtual~CShaderGL();

	void Init();
	void RenderShader();
	bool CheckedShader();
	GLuint GetShaderId();
	GLuint GetUBOId();
	void InitUBO();
	bool readshader(const char* filename, std::string& shader_text);
	GLuint run_shader(const char* shader_text, GLenum type);

	void Projection();
	void SetPerspective(float Fov, float Aspect, float ZNear, float ZFar);

	// Funciones para establecer uniforms
	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec2(const char* name, float x, float y) const;
	void setVec3(const char* name, float x, float y, float z) const;
	void setVec4(const char* name, float x, float y, float z, float w) const;
	void setMat4(const char* name, const float matrix[4][4]) const;
	void BuildModelMatrix(const vec3_t position, const vec3_t angle, float scale, float outMatrix[4][4]);
	void BuildModelMatrixFrom34(float matrix34[3][4], float outMatrix[4][4]);
	static CShaderGL* Instance();
private:
	GLuint shaderid;
	GLuint uboMeshState;
};

#define gShaderGL				(CShaderGL::Instance())

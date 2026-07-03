#include "stdafx.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Necesario para glm::value_ptr
#include "CShaderGL.h"
#include "Utilities/Log/muConsoleDebug.h"

CShaderGL::CShaderGL()
{
	shaderid = 0;
	uboMeshState = 0;
}

CShaderGL::~CShaderGL()
{
	glDeleteProgram(shaderid);
}

void CShaderGL::Init()
{
	std::string vertex_shader;

	if (!readshader("Data\\Effect\\Shader\\vertex_shader.glsl", vertex_shader))
	{
		return;
	}

	std::string frgmen_shader;

	if (!readshader("Data\\Effect\\Shader\\fragment_shader.glsl", frgmen_shader))
	{
		return;
	}

	GLuint shader_vertex = run_shader(vertex_shader.data(), GL_VERTEX_SHADER);

	GLuint shader_frgmen = run_shader(frgmen_shader.data(), GL_FRAGMENT_SHADER);

	shaderid = glCreateProgram();
	glAttachShader(shaderid, shader_vertex);
	glAttachShader(shaderid, shader_frgmen);
	glLinkProgram(shaderid);

	int success;
	glGetProgramiv(shaderid, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderid, 512, NULL, infoLog);
		//g_ConsoleDebug->Write(5, "Error al enlazar el Shader Program:");
		//g_ConsoleDebug->Write(5, infoLog);
	}
	else
	{
		this->InitUBO();
	}

	// Eliminar los shaders compilados
	glDeleteShader(shader_vertex);
	glDeleteShader(shader_frgmen);
}

void CShaderGL::RenderShader()
{
	if (this->CheckedShader())
	{
		glUseProgram(shaderid);
	}
}

bool CShaderGL::CheckedShader()
{
	return (shaderid != 0);
}

GLuint CShaderGL::GetShaderId()
{
	return shaderid;
}

GLuint CShaderGL::GetUBOId()
{
	return uboMeshState;
}

void CShaderGL::InitUBO()
{
	glGenBuffers(1, &uboMeshState);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMeshState);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MeshRenderState), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMeshState);

	//GLuint blockIndex = glGetUniformBlockIndex(shaderid, "MeshState");
	//glUniformBlockBinding(shaderid, blockIndex, 0);
}

bool CShaderGL::readshader(const char* filename, std::string& shader_text)
{
	FILE* compressedFile = fopen(filename, "rb");

	if (compressedFile)
	{
		fseek(compressedFile, 0, SEEK_END);
		long fileSize = ftell(compressedFile);
		fseek(compressedFile, 0, SEEK_SET);

		shader_text.resize(fileSize, 0);
		fread(shader_text.data(), 1, fileSize, compressedFile);
		fclose(compressedFile);

		return true;
	}

	return false;
}

GLuint CShaderGL::run_shader(const char* shader_text, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shader_text, NULL);
	glCompileShader(shader);

	// Verificar errores de compilación
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		g_ConsoleDebug->Write(5, "Error al compilar shader:");
		g_ConsoleDebug->Write(5, infoLog);
	}

	return shader;
}

void CShaderGL::Projection()
{
	if (shaderid != 0)
	{
		glUseProgram(shaderid);

		float proj[4][4];
		float clip[4][4];

		glGetFloatv(GL_MODELVIEW_MATRIX, &clip[0][0]);
		glGetFloatv(GL_PROJECTION_MATRIX, &proj[0][0]);

		this->setMat4("view", clip);
		this->setMat4("projection", proj);

		glUniform1i(glGetUniformLocation(shaderid, "texture1"), 0);
		glUseProgram(0);
	}
}

void CShaderGL::SetPerspective(float Fov, float Aspect, float ZNear, float ZFar)
{
	//if (shaderid != 0)
	//{
	//	glUseProgram(shaderid);
	//	glm::mat4 projection;
	//	glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));
	//	this->setMat4("projection", projection);
	//	glUseProgram(0);
	//}
}

// Funciones para establecer uniforms
void CShaderGL::setBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderid, name), (int)value);
}

void CShaderGL::setInt(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderid, name), value);
}

void CShaderGL::setFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderid, name), value);
}

void CShaderGL::setVec2(const char* name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(shaderid, name), x, y);
}

void CShaderGL::setVec3(const char* name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(shaderid, name), x, y, z);
}

void CShaderGL::setVec4(const char* name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(shaderid, name), x, y, z, w);
}

void CShaderGL::setMat4(const char* name, const float matrix[4][4]) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderid, name), 1, GL_FALSE, &matrix[0][0]);
}

void CShaderGL::BuildModelMatrix(const vec3_t position, const vec3_t angle, float scale, float outMatrix[4][4])
{
	glm::mat4 model = glm::mat4(1.0f);

	// Traslación al final (primero en multiplicación)
	model = glm::translate(model, glm::vec3(position[0], position[1], position[2]));

	// Rotación ZYX (en orden inverso, se aplicará Z  X  Y)
	model = glm::rotate(model, glm::radians(angle[2]), glm::vec3(0.0f, 0.0f, 1.0f)); // Z
	model = glm::rotate(model, glm::radians(angle[0]), glm::vec3(1.0f, 0.0f, 0.0f)); // X
	model = glm::rotate(model, glm::radians(angle[1]), glm::vec3(0.0f, 1.0f, 0.0f)); // Y

	// Escalado uniforme al principio (último en multiplicación)
	model = glm::scale(model, glm::vec3(scale));

	// Convertir a float[4][4]
	memcpy(outMatrix, glm::value_ptr(model), sizeof(float) * 16);
}

void CShaderGL::BuildModelMatrixFrom34(float matrix34[3][4], float outMatrix[4][4])
{
	glm::mat4 model = glm::mat4(1.0f);

	model[0][0] = matrix34[0][0];
	model[0][1] = matrix34[0][1];
	model[0][2] = matrix34[0][2];
	model[0][3] = 0.0f;

	model[1][0] = matrix34[1][0];
	model[1][1] = matrix34[1][1];
	model[1][2] = matrix34[1][2];
	model[1][3] = 0.0f;

	model[2][0] = matrix34[2][0];
	model[2][1] = matrix34[2][1];
	model[2][2] = matrix34[2][2];
	model[2][3] = 0.0f;

	model[3][0] = matrix34[0][3];
	model[3][1] = matrix34[1][3];
	model[3][2] = matrix34[2][3];
	model[3][3] = 1.0f;

	memcpy(outMatrix, glm::value_ptr(model), sizeof(float) * 16);
}

CShaderGL* CShaderGL::Instance()
{
	static CShaderGL sInstance;
	return &sInstance;
}




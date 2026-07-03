#define CORE_GL_COMPAT_IMPLEMENTATION
#include "stdafx.h"

#undef glBegin
#undef glEnd
#undef glColor3f
#undef glColor3fv
#undef glColor3ub
#undef glColor4f
#undef glColor4ub
#undef glTexCoord2f
#undef glVertex2f
#undef glVertex2i
#undef glVertex3f
#undef glVertex3fv
#undef glNormal3f
#undef glMatrixMode
#undef glLoadIdentity
#undef glPushMatrix
#undef glPopMatrix
#undef glTranslatef
#undef glRotatef
#undef glScalef
#undef glOrtho
#undef gluPerspective
#undef gluOrtho2D
#undef glGetFloatv
#undef glEnable
#undef glDisable
#undef glAlphaFunc
#undef glFogf
#undef glFogfv
#undef glTexEnvi
#undef glTexEnvf
#undef glTexParameteri
#undef glPolygonMode
#undef glBindTexture
#undef glDeleteTextures
#undef glBlendFunc
#undef glDepthMask
#undef glDepthFunc
#undef glFrontFace
#undef glStencilFunc
#undef glStencilOp
#undef glColorMask
#undef glLineWidth
#undef glPointSize
#undef glViewport
#undef glClear
#undef glFlush
#undef glUseProgram
#undef glDrawRangeElements
#undef glReadPixels
#undef glTexImage2D
#undef glTexSubImage2D
#undef glPushAttrib
#undef glPopAttrib
#undef glEnableClientState
#undef glDisableClientState
#undef glVertexPointer
#undef glDrawArrays

#include "CoreGLCompat.h"
#if jdk_shader_local330
#include "New_ModelBMD.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
	struct CoreVertex
	{
		float position[3];
		float color[4];
		float texCoord[2];
	};

	struct ClientVertexArray
	{
		bool enabled;
		GLint size;
		GLenum type;
		GLsizei stride;
		const unsigned char* pointer;

		ClientVertexArray() : enabled(false), size(3), type(GL_FLOAT), stride(0), pointer(NULL) {}
	};

	GLuint g_program = 0;
	GLuint g_vao = 0;
	GLuint g_vbo = 0;
	const size_t STREAM_BUFFER_SIZE = 8 * 1024 * 1024;
	size_t g_streamOffset = 0;
	bool g_initialized = false;
	bool g_insideBegin = false;
	GLenum g_primitive = GL_TRIANGLES;
	std::vector<CoreVertex> g_vertices;
	std::vector<CoreVertex> g_drawVertices;
	std::vector<CoreVertex> g_clientVertices;
	std::vector<CoreVertex> g_pendingVertices;
	GLenum g_pendingPrimitive = GL_TRIANGLES;
	GLuint g_boundTexture2D = 0;
	bool g_textureBindingKnown = false;
	std::unordered_map<GLenum, bool> g_capabilityState;

	struct PipelineState
	{
		bool blendKnown;
		GLenum blendSource;
		GLenum blendDestination;
		bool depthMaskKnown;
		GLboolean depthMask;
		bool depthFuncKnown;
		GLenum depthFunc;
		bool frontFaceKnown;
		GLenum frontFace;
		bool stencilFuncKnown;
		GLenum stencilFunction;
		GLint stencilReference;
		GLuint stencilMask;
		bool stencilOpKnown;
		GLenum stencilFail;
		GLenum stencilDepthFail;
		GLenum stencilDepthPass;
		bool colorMaskKnown;
		GLboolean colorMask[4];
		bool lineWidthKnown;
		GLfloat lineWidth;
		bool pointSizeKnown;
		GLfloat pointSize;
		bool viewportKnown;
		GLint viewport[4];
		bool polygonModeKnown;
		GLenum polygonMode;

		PipelineState()
			: blendKnown(false), blendSource(GL_ONE), blendDestination(GL_ZERO),
			depthMaskKnown(false), depthMask(GL_TRUE), depthFuncKnown(false), depthFunc(GL_LESS),
			frontFaceKnown(false), frontFace(GL_CCW), stencilFuncKnown(false),
			stencilFunction(GL_ALWAYS), stencilReference(0), stencilMask(~0u),
			stencilOpKnown(false), stencilFail(GL_KEEP), stencilDepthFail(GL_KEEP),
			stencilDepthPass(GL_KEEP), colorMaskKnown(false), lineWidthKnown(false),
			lineWidth(1.0f), pointSizeKnown(false), pointSize(1.0f), viewportKnown(false),
			polygonModeKnown(false), polygonMode(GL_FILL)
		{
			colorMask[0] = colorMask[1] = colorMask[2] = colorMask[3] = GL_TRUE;
			viewport[0] = viewport[1] = viewport[2] = viewport[3] = 0;
		}
	} g_pipelineState;

	struct ImmediateUniformLocations
	{
		GLint projection;
		GLint modelView;
		GLint texture;
		GLint textureEnabled;
		GLint alphaTestEnabled;
		GLint alphaReference;
		GLint textureEnvironment;
		GLint fogEnabled;
		GLint fogColor;
		GLint fogStart;
		GLint fogEnd;
	} g_uniforms;

	bool g_matrixUniformsDirty = true;
	bool g_stateUniformsDirty = true;
	unsigned long long g_fogVersion = 1;
	unsigned long long g_immediateFogVersion = 0;

	struct FogUniformState
	{
		GLint enabled;
		GLint color;
		GLint start;
		GLint end;
		bool initialized;
		unsigned long long appliedVersion;

		FogUniformState() : enabled(-1), color(-1), start(-1), end(-1), initialized(false), appliedVersion(0) {}
	};
	std::unordered_map<GLuint, FogUniformState> g_fogUniformCache;

	glm::mat4 g_projection(1.0f);
	glm::mat4 g_modelView(1.0f);
	std::vector<glm::mat4> g_projectionStack;
	std::vector<glm::mat4> g_modelViewStack;
	GLenum g_matrixMode = GL_MODELVIEW;

	float g_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float g_texCoord[2] = { 0.0f, 0.0f };
	bool g_textureEnabled = false;
	bool g_alphaTestEnabled = false;
	bool g_fogEnabled = false;
	float g_alphaReference = 0.0f;
	GLint g_textureEnvironment = GL_MODULATE;
	float g_fogColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float g_fogStart = 1200.0f;
	float g_fogEnd = 2700.0f;
	float g_fogDensity = 1.0f;
	ClientVertexArray g_clientVertexArray;

	struct AttributeState
	{
		bool textureEnabled;
		bool alphaTestEnabled;
		bool fogEnabled;
		bool blendEnabled;
		bool depthTestEnabled;
		bool cullFaceEnabled;
		bool stencilTestEnabled;
		GLboolean depthMask;
		GLint blendSource;
		GLint blendDestination;
		GLint textureBinding;
		GLint textureEnvironment;
		float alphaReference;
		float color[4];
		glm::mat4 projection;
		glm::mat4 modelView;
	};
	std::vector<AttributeState> g_attributeStack;

	void MarkMatrixDirty()
	{
		g_matrixUniformsDirty = true;
	}

	void MarkStateDirty()
	{
		g_stateUniformsDirty = true;
	}

	void MarkFogDirty()
	{
		++g_fogVersion;
		if (g_fogVersion == 0)
			g_fogVersion = 1;
	}

	glm::mat4& CurrentMatrix()
	{
		return (g_matrixMode == GL_PROJECTION) ? g_projection : g_modelView;
	}

	std::vector<glm::mat4>& CurrentMatrixStack()
	{
		return (g_matrixMode == GL_PROJECTION) ? g_projectionStack : g_modelViewStack;
	}

	GLuint CompileShader(GLenum type, const char* source)
	{
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		GLint compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE)
		{
			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	bool BuildProgram()
	{
		static const char* vertexSource =
			"#version 330 core\n"
			"layout(location=0) in vec3 aPosition;\n"
			"layout(location=1) in vec4 aColor;\n"
			"layout(location=2) in vec2 aTexCoord;\n"
			"uniform mat4 uProjection;\n"
			"uniform mat4 uModelView;\n"
			"out vec4 vColor;\n"
			"out vec2 vTexCoord;\n"
			"out float vFogDepth;\n"
			"void main(){\n"
			" vec4 viewPosition=uModelView*vec4(aPosition,1.0);\n"
			" gl_Position=uProjection*viewPosition;\n"
			" vColor=aColor;\n"
			" vTexCoord=aTexCoord;\n"
			" vFogDepth=abs(viewPosition.z);\n"
			"}\n";

		static const char* fragmentSource =
			"#version 330 core\n"
			"in vec4 vColor;\n"
			"in vec2 vTexCoord;\n"
			"in float vFogDepth;\n"
			"uniform sampler2D uTexture;\n"
			"uniform bool uTextureEnabled;\n"
			"uniform bool uAlphaTestEnabled;\n"
			"uniform float uAlphaReference;\n"
			"uniform int uTextureEnvironment;\n"
			"uniform bool uFogEnabled;\n"
			"uniform vec4 uFogColor;\n"
			"uniform float uFogStart;\n"
			"uniform float uFogEnd;\n"
			"out vec4 FragColor;\n"
			"void main(){\n"
			" vec4 result=vColor;\n"
			" if(uTextureEnabled){\n"
			"  vec4 texel=texture(uTexture,vTexCoord);\n"
			"  if(uTextureEnvironment==1) result=texel;\n"
			"  else if(uTextureEnvironment==2) result=clamp(texel+vColor,0.0,1.0);\n"
			"  else result=texel*vColor;\n"
			" }\n"
			" if(uAlphaTestEnabled && result.a<=uAlphaReference) discard;\n"
			" if(uFogEnabled){\n"
			"  float fogFactor=clamp((uFogEnd-vFogDepth)/max(uFogEnd-uFogStart,0.0001),0.0,1.0);\n"
			"  result=mix(uFogColor,result,fogFactor);\n"
			" }\n"
			" FragColor=result;\n"
			"}\n";

		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
		if (vertexShader == 0 || fragmentShader == 0)
		{
			if (vertexShader != 0) glDeleteShader(vertexShader);
			if (fragmentShader != 0) glDeleteShader(fragmentShader);
			return false;
		}

		g_program = glCreateProgram();
		glAttachShader(g_program, vertexShader);
		glAttachShader(g_program, fragmentShader);
		glLinkProgram(g_program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		GLint linked = GL_FALSE;
		glGetProgramiv(g_program, GL_LINK_STATUS, &linked);
		return linked == GL_TRUE;
	}

	void AppendVertex(float x, float y, float z)
	{
		if (!g_insideBegin)
			return;

		CoreVertex vertex;
		vertex.position[0] = x;
		vertex.position[1] = y;
		vertex.position[2] = z;
		memcpy(vertex.color, g_color, sizeof(g_color));
		memcpy(vertex.texCoord, g_texCoord, sizeof(g_texCoord));
		g_vertices.push_back(vertex);
	}

	void AppendTriangle(const CoreVertex& a, const CoreVertex& b, const CoreVertex& c)
	{
		g_drawVertices.push_back(a);
		g_drawVertices.push_back(b);
		g_drawVertices.push_back(c);
	}

	void AppendLine(const CoreVertex& a, const CoreVertex& b)
	{
		g_drawVertices.push_back(a);
		g_drawVertices.push_back(b);
	}

	GLenum ConvertPrimitive(const CoreVertex*& drawData, size_t& drawCount)
	{
		g_drawVertices.clear();

		if (g_primitive == GL_QUADS)
		{
			for (size_t i = 0; i + 3 < g_vertices.size(); i += 4)
			{
				AppendTriangle(g_vertices[i], g_vertices[i + 1], g_vertices[i + 2]);
				AppendTriangle(g_vertices[i], g_vertices[i + 2], g_vertices[i + 3]);
			}
			drawData = g_drawVertices.data();
			drawCount = g_drawVertices.size();
			return GL_TRIANGLES;
		}

		if (g_primitive == GL_QUAD_STRIP)
		{
			for (size_t i = 0; i + 3 < g_vertices.size(); i += 2)
			{
				AppendTriangle(g_vertices[i], g_vertices[i + 1], g_vertices[i + 3]);
				AppendTriangle(g_vertices[i], g_vertices[i + 3], g_vertices[i + 2]);
			}
			drawData = g_drawVertices.data();
			drawCount = g_drawVertices.size();
			return GL_TRIANGLES;
		}

		if (g_primitive == GL_POLYGON)
		{
			for (size_t i = 1; i + 1 < g_vertices.size(); ++i)
				AppendTriangle(g_vertices[0], g_vertices[i], g_vertices[i + 1]);
			drawData = g_drawVertices.data();
			drawCount = g_drawVertices.size();
			return GL_TRIANGLES;
		}

		if (g_primitive == GL_TRIANGLE_FAN)
		{
			for (size_t i = 1; i + 1 < g_vertices.size(); ++i)
				AppendTriangle(g_vertices[0], g_vertices[i], g_vertices[i + 1]);
			drawData = g_drawVertices.data();
			drawCount = g_drawVertices.size();
			return GL_TRIANGLES;
		}

		if (g_primitive == GL_TRIANGLE_STRIP)
		{
			for (size_t i = 0; i + 2 < g_vertices.size(); ++i)
			{
				if ((i & 1) == 0)
					AppendTriangle(g_vertices[i], g_vertices[i + 1], g_vertices[i + 2]);
				else
					AppendTriangle(g_vertices[i + 1], g_vertices[i], g_vertices[i + 2]);
			}
			drawData = g_drawVertices.data();
			drawCount = g_drawVertices.size();
			return GL_TRIANGLES;
		}

		if (g_primitive == GL_LINE_STRIP || g_primitive == GL_LINE_LOOP)
		{
			for (size_t i = 0; i + 1 < g_vertices.size(); ++i)
				AppendLine(g_vertices[i], g_vertices[i + 1]);
			if (g_primitive == GL_LINE_LOOP && g_vertices.size() > 2)
				AppendLine(g_vertices.back(), g_vertices.front());
			drawData = g_drawVertices.data();
			drawCount = g_drawVertices.size();
			return GL_LINES;
		}

		drawData = g_vertices.data();
		drawCount = g_vertices.size();
		return g_primitive;
	}

	void UploadCommonUniforms()
	{
		if (g_matrixUniformsDirty)
		{
			glUniformMatrix4fv(g_uniforms.projection, 1, GL_FALSE, glm::value_ptr(g_projection));
			glUniformMatrix4fv(g_uniforms.modelView, 1, GL_FALSE, glm::value_ptr(g_modelView));
			g_matrixUniformsDirty = false;
		}

		if (g_stateUniformsDirty)
		{
			int textureEnvironment = 0;
			if (g_textureEnvironment == GL_REPLACE) textureEnvironment = 1;
			else if (g_textureEnvironment == GL_ADD) textureEnvironment = 2;

			glUniform1i(g_uniforms.textureEnabled, g_textureEnabled ? 1 : 0);
			glUniform1i(g_uniforms.alphaTestEnabled, g_alphaTestEnabled ? 1 : 0);
			glUniform1f(g_uniforms.alphaReference, g_alphaReference);
			glUniform1i(g_uniforms.textureEnvironment, textureEnvironment);
			g_stateUniformsDirty = false;
		}

		if (g_immediateFogVersion != g_fogVersion)
		{
			glUniform1i(g_uniforms.fogEnabled, g_fogEnabled ? 1 : 0);
			glUniform4fv(g_uniforms.fogColor, 1, g_fogColor);
			glUniform1f(g_uniforms.fogStart, g_fogStart);
			glUniform1f(g_uniforms.fogEnd, g_fogEnd);
			g_immediateFogVersion = g_fogVersion;
		}
	}

	void DrawVertexBuffer(GLenum mode, const CoreVertex* vertices, size_t count)
	{
		if (!g_initialized || vertices == NULL || count == 0)
			return;

		GLEW_GET_FUN(__glewUseProgram)(g_program);
		UploadCommonUniforms();
		glBindVertexArray(g_vao);
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
		const size_t uploadSize = count * sizeof(CoreVertex);
		if (uploadSize > STREAM_BUFFER_SIZE)
		{
			glBufferData(GL_ARRAY_BUFFER, uploadSize, vertices, GL_STREAM_DRAW);
			glDrawArrays(mode, 0, static_cast<GLsizei>(count));
			glBufferData(GL_ARRAY_BUFFER, STREAM_BUFFER_SIZE, NULL, GL_STREAM_DRAW);
			g_streamOffset = 0;
		}
		else
		{
			if (g_streamOffset + uploadSize > STREAM_BUFFER_SIZE)
			{
				glBufferData(GL_ARRAY_BUFFER, STREAM_BUFFER_SIZE, NULL, GL_STREAM_DRAW);
				g_streamOffset = 0;
			}

			glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(g_streamOffset), uploadSize, vertices);
			const GLint firstVertex = static_cast<GLint>(g_streamOffset / sizeof(CoreVertex));
			glDrawArrays(mode, firstVertex, static_cast<GLsizei>(count));
			g_streamOffset += uploadSize;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		GLEW_GET_FUN(__glewUseProgram)(0);
#if jdk_shader_local330
		OGL330MODEL::InvalidateShaderCache();
#endif
	}

	bool IsLegacyCapability(GLenum capability)
	{
		return capability == GL_TEXTURE_2D ||
			capability == GL_ALPHA_TEST ||
			capability == GL_FOG ||
			capability == GL_LIGHTING ||
			capability == GL_POINT_SMOOTH ||
			capability == GL_VERTEX_PROGRAM_ARB ||
			capability == GL_FRAGMENT_PROGRAM_ARB;
	}
}

bool CoreGLCompat::Initialize()
{
	if (g_initialized)
		return true;
	if (!BuildProgram())
		return false;

	glGenVertexArrays(1, &g_vao);
	glGenBuffers(1, &g_vbo);
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, STREAM_BUFFER_SIZE, NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CoreVertex), reinterpret_cast<void*>(offsetof(CoreVertex, position)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(CoreVertex), reinterpret_cast<void*>(offsetof(CoreVertex, color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CoreVertex), reinterpret_cast<void*>(offsetof(CoreVertex, texCoord)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	g_uniforms.projection = glGetUniformLocation(g_program, "uProjection");
	g_uniforms.modelView = glGetUniformLocation(g_program, "uModelView");
	g_uniforms.texture = glGetUniformLocation(g_program, "uTexture");
	g_uniforms.textureEnabled = glGetUniformLocation(g_program, "uTextureEnabled");
	g_uniforms.alphaTestEnabled = glGetUniformLocation(g_program, "uAlphaTestEnabled");
	g_uniforms.alphaReference = glGetUniformLocation(g_program, "uAlphaReference");
	g_uniforms.textureEnvironment = glGetUniformLocation(g_program, "uTextureEnvironment");
	g_uniforms.fogEnabled = glGetUniformLocation(g_program, "uFogEnabled");
	g_uniforms.fogColor = glGetUniformLocation(g_program, "uFogColor");
	g_uniforms.fogStart = glGetUniformLocation(g_program, "uFogStart");
	g_uniforms.fogEnd = glGetUniformLocation(g_program, "uFogEnd");

	GLEW_GET_FUN(__glewUseProgram)(g_program);
	glUniform1i(g_uniforms.texture, 0);
	GLEW_GET_FUN(__glewUseProgram)(0);

	g_vertices.reserve(4096);
	g_drawVertices.reserve(6144);
	g_clientVertices.reserve(4096);
	g_pendingVertices.reserve(65536);
	g_streamOffset = 0;
	g_matrixUniformsDirty = true;
	g_stateUniformsDirty = true;
	g_immediateFogVersion = 0;
	g_fogUniformCache.clear();
	g_initialized = true;
	return true;
}

void CoreGLCompat::Shutdown()
{
	FlushPending();
	if (g_vbo != 0) glDeleteBuffers(1, &g_vbo);
	if (g_vao != 0) glDeleteVertexArrays(1, &g_vao);
	if (g_program != 0) glDeleteProgram(g_program);
	g_vbo = 0;
	g_vao = 0;
	g_program = 0;
	g_streamOffset = 0;
	g_pendingVertices.clear();
	g_capabilityState.clear();
	g_textureBindingKnown = false;
	g_pipelineState = PipelineState();
	g_fogUniformCache.clear();
	g_initialized = false;
}

bool CoreGLCompat::IsInitialized()
{
	return g_initialized;
}

void CoreGLCompat::FlushPending()
{
	if (g_pendingVertices.empty())
		return;

	DrawVertexBuffer(g_pendingPrimitive, g_pendingVertices.data(), g_pendingVertices.size());
	g_pendingVertices.clear();
}

void CoreGLCompat::Begin(GLenum mode)
{
	g_insideBegin = true;
	g_primitive = mode;
	g_vertices.clear();
}

void CoreGLCompat::End()
{
	if (!g_insideBegin)
		return;
	g_insideBegin = false;
	const CoreVertex* drawData = NULL;
	size_t drawCount = 0;
	GLenum mode = ConvertPrimitive(drawData, drawCount);
	if (drawData == NULL || drawCount == 0)
		return;

	if (!g_pendingVertices.empty() && g_pendingPrimitive != mode)
		FlushPending();
	if (g_pendingVertices.empty())
		g_pendingPrimitive = mode;

	g_pendingVertices.insert(g_pendingVertices.end(), drawData, drawData + drawCount);
	if (g_pendingVertices.size() >= 65536)
		FlushPending();
}

void CoreGLCompat::Color3f(GLfloat r, GLfloat g, GLfloat b) { Color4f(r, g, b, g_color[3]); }
void CoreGLCompat::Color3fv(const GLfloat* value) { if (value) Color3f(value[0], value[1], value[2]); }
void CoreGLCompat::Color3ub(GLubyte r, GLubyte g, GLubyte b) { Color3f(r / 255.0f, g / 255.0f, b / 255.0f); }
void CoreGLCompat::Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { g_color[0] = r; g_color[1] = g; g_color[2] = b; g_color[3] = a; }
void CoreGLCompat::Color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) { Color4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }
void CoreGLCompat::TexCoord2f(GLfloat u, GLfloat v) { g_texCoord[0] = u; g_texCoord[1] = v; }
void CoreGLCompat::Vertex2f(GLfloat x, GLfloat y) { AppendVertex(x, y, 0.0f); }
void CoreGLCompat::Vertex2i(GLint x, GLint y) { AppendVertex(static_cast<float>(x), static_cast<float>(y), 0.0f); }
void CoreGLCompat::Vertex3f(GLfloat x, GLfloat y, GLfloat z) { AppendVertex(x, y, z); }
void CoreGLCompat::Vertex3fv(const GLfloat* value) { if (value) AppendVertex(value[0], value[1], value[2]); }
void CoreGLCompat::Normal3f(GLfloat, GLfloat, GLfloat) {}

void CoreGLCompat::MatrixMode(GLenum mode) { if (mode == GL_MODELVIEW || mode == GL_PROJECTION) g_matrixMode = mode; }
void CoreGLCompat::LoadIdentity() { FlushPending(); CurrentMatrix() = glm::mat4(1.0f); MarkMatrixDirty(); }
void CoreGLCompat::PushMatrix() { CurrentMatrixStack().push_back(CurrentMatrix()); }
void CoreGLCompat::PopMatrix() { if (!CurrentMatrixStack().empty()) { FlushPending(); CurrentMatrix() = CurrentMatrixStack().back(); CurrentMatrixStack().pop_back(); MarkMatrixDirty(); } }
void CoreGLCompat::Translatef(GLfloat x, GLfloat y, GLfloat z) { FlushPending(); CurrentMatrix() = glm::translate(CurrentMatrix(), glm::vec3(x, y, z)); MarkMatrixDirty(); }
void CoreGLCompat::Rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) { FlushPending(); CurrentMatrix() = glm::rotate(CurrentMatrix(), glm::radians(angle), glm::vec3(x, y, z)); MarkMatrixDirty(); }
void CoreGLCompat::Scalef(GLfloat x, GLfloat y, GLfloat z) { FlushPending(); CurrentMatrix() = glm::scale(CurrentMatrix(), glm::vec3(x, y, z)); MarkMatrixDirty(); }
void CoreGLCompat::Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	FlushPending();
	CurrentMatrix() *= glm::ortho(static_cast<float>(left), static_cast<float>(right), static_cast<float>(bottom),
		static_cast<float>(top), static_cast<float>(zNear), static_cast<float>(zFar));
	MarkMatrixDirty();
}
void CoreGLCompat::Perspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	FlushPending();
	CurrentMatrix() *= glm::perspective(glm::radians(static_cast<float>(fovY)), static_cast<float>(aspect),
		static_cast<float>(zNear), static_cast<float>(zFar));
	MarkMatrixDirty();
}
void CoreGLCompat::Ortho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) { Ortho(left, right, bottom, top, -1.0, 1.0); }
void CoreGLCompat::GetFloatv(GLenum pname, GLfloat* values)
{
	if (!values) return;
	if (pname == GL_MODELVIEW_MATRIX) memcpy(values, glm::value_ptr(g_modelView), sizeof(float) * 16);
	else if (pname == GL_PROJECTION_MATRIX) memcpy(values, glm::value_ptr(g_projection), sizeof(float) * 16);
	else if (pname == GL_CURRENT_COLOR) memcpy(values, g_color, sizeof(g_color));
	else glGetFloatv(pname, values);
}

void CoreGLCompat::Enable(GLenum capability)
{
	if (capability == GL_TEXTURE_2D)
	{
		if (!g_textureEnabled) { FlushPending(); g_textureEnabled = true; MarkStateDirty(); }
	}
	else if (capability == GL_ALPHA_TEST)
	{
		if (!g_alphaTestEnabled) { FlushPending(); g_alphaTestEnabled = true; MarkStateDirty(); }
	}
	else if (capability == GL_FOG)
	{
		if (!g_fogEnabled) { FlushPending(); g_fogEnabled = true; MarkFogDirty(); }
	}
	else if (!IsLegacyCapability(capability))
	{
		std::unordered_map<GLenum, bool>::iterator it = g_capabilityState.find(capability);
		if (it == g_capabilityState.end() || !it->second)
		{
			FlushPending();
			glEnable(capability);
			g_capabilityState[capability] = true;
		}
	}
}
void CoreGLCompat::Disable(GLenum capability)
{
	if (capability == GL_TEXTURE_2D)
	{
		if (g_textureEnabled) { FlushPending(); g_textureEnabled = false; MarkStateDirty(); }
	}
	else if (capability == GL_ALPHA_TEST)
	{
		if (g_alphaTestEnabled) { FlushPending(); g_alphaTestEnabled = false; MarkStateDirty(); }
	}
	else if (capability == GL_FOG)
	{
		if (g_fogEnabled) { FlushPending(); g_fogEnabled = false; MarkFogDirty(); }
	}
	else if (!IsLegacyCapability(capability))
	{
		std::unordered_map<GLenum, bool>::iterator it = g_capabilityState.find(capability);
		if (it == g_capabilityState.end() || it->second)
		{
			FlushPending();
			glDisable(capability);
			g_capabilityState[capability] = false;
		}
	}
}
void CoreGLCompat::AlphaFunc(GLenum, GLfloat reference)
{
	if (g_alphaReference != reference)
	{
		FlushPending();
		g_alphaReference = reference;
		MarkStateDirty();
	}
}
void CoreGLCompat::Fogf(GLenum pname, GLfloat value)
{
	bool changed = false;
	if (pname == GL_FOG_START && g_fogStart != value) { g_fogStart = value; changed = true; }
	else if (pname == GL_FOG_END && g_fogEnd != value) { g_fogEnd = value; changed = true; }
	else if (pname == GL_FOG_DENSITY && g_fogDensity != value) { g_fogDensity = value; changed = true; }
	if (changed) { FlushPending(); MarkFogDirty(); }
}
void CoreGLCompat::Fogfv(GLenum pname, const GLfloat* values)
{
	if (pname == GL_FOG_COLOR && values && memcmp(g_fogColor, values, sizeof(g_fogColor)) != 0)
	{
		FlushPending();
		memcpy(g_fogColor, values, sizeof(g_fogColor));
		MarkFogDirty();
	}
}
void CoreGLCompat::TexEnvi(GLenum, GLenum pname, GLint value)
{
	if (pname == GL_TEXTURE_ENV_MODE && g_textureEnvironment != value)
	{
		FlushPending();
		g_textureEnvironment = value;
		MarkStateDirty();
	}
}
void CoreGLCompat::TexEnvf(GLenum, GLenum pname, GLfloat value) { TexEnvi(GL_TEXTURE_ENV, pname, static_cast<GLint>(value)); }
void CoreGLCompat::TexParameteri(GLenum target, GLenum pname, GLint value)
{
	FlushPending();
	if (value == GL_CLAMP) value = GL_CLAMP_TO_EDGE;
	glTexParameteri(target, pname, value);
}
void CoreGLCompat::PolygonMode(GLenum, GLenum mode)
{
	if (!g_pipelineState.polygonModeKnown || g_pipelineState.polygonMode != mode)
	{
		FlushPending();
		glPolygonMode(GL_FRONT_AND_BACK, mode);
		g_pipelineState.polygonMode = mode;
		g_pipelineState.polygonModeKnown = true;
	}
}
void CoreGLCompat::BindTexture(GLenum target, GLuint texture)
{
	if (target != GL_TEXTURE_2D || !g_textureBindingKnown || g_boundTexture2D != texture)
	{
		FlushPending();
		glBindTexture(target, texture);
		if (target == GL_TEXTURE_2D)
		{
			g_boundTexture2D = texture;
			g_textureBindingKnown = true;
		}
	}
}
GLuint CoreGLCompat::GetBoundTexture2D()
{
	return g_textureBindingKnown ? g_boundTexture2D : 0;
}
void CoreGLCompat::DeleteTextures(GLsizei count, const GLuint* textures)
{
	FlushPending();
	glDeleteTextures(count, textures);
	if (g_textureBindingKnown && textures)
	{
		for (GLsizei i = 0; i < count; ++i)
		{
			if (textures[i] == g_boundTexture2D)
			{
				g_boundTexture2D = 0;
				g_textureBindingKnown = false;
				break;
			}
		}
	}
}
void CoreGLCompat::BlendFunc(GLenum source, GLenum destination)
{
	if (!g_pipelineState.blendKnown || g_pipelineState.blendSource != source || g_pipelineState.blendDestination != destination)
	{
		FlushPending();
		glBlendFunc(source, destination);
		g_pipelineState.blendSource = source;
		g_pipelineState.blendDestination = destination;
		g_pipelineState.blendKnown = true;
	}
}
void CoreGLCompat::DepthMask(GLboolean enabled)
{
	if (!g_pipelineState.depthMaskKnown || g_pipelineState.depthMask != enabled)
	{
		FlushPending();
		glDepthMask(enabled);
		g_pipelineState.depthMask = enabled;
		g_pipelineState.depthMaskKnown = true;
	}
}
void CoreGLCompat::DepthFunc(GLenum function)
{
	if (!g_pipelineState.depthFuncKnown || g_pipelineState.depthFunc != function)
	{
		FlushPending();
		glDepthFunc(function);
		g_pipelineState.depthFunc = function;
		g_pipelineState.depthFuncKnown = true;
	}
}
void CoreGLCompat::FrontFace(GLenum mode)
{
	if (!g_pipelineState.frontFaceKnown || g_pipelineState.frontFace != mode)
	{
		FlushPending();
		glFrontFace(mode);
		g_pipelineState.frontFace = mode;
		g_pipelineState.frontFaceKnown = true;
	}
}
void CoreGLCompat::StencilFunc(GLenum function, GLint reference, GLuint mask)
{
	if (!g_pipelineState.stencilFuncKnown || g_pipelineState.stencilFunction != function ||
		g_pipelineState.stencilReference != reference || g_pipelineState.stencilMask != mask)
	{
		FlushPending();
		glStencilFunc(function, reference, mask);
		g_pipelineState.stencilFunction = function;
		g_pipelineState.stencilReference = reference;
		g_pipelineState.stencilMask = mask;
		g_pipelineState.stencilFuncKnown = true;
	}
}
void CoreGLCompat::StencilOp(GLenum stencilFail, GLenum depthFail, GLenum depthPass)
{
	if (!g_pipelineState.stencilOpKnown || g_pipelineState.stencilFail != stencilFail ||
		g_pipelineState.stencilDepthFail != depthFail || g_pipelineState.stencilDepthPass != depthPass)
	{
		FlushPending();
		glStencilOp(stencilFail, depthFail, depthPass);
		g_pipelineState.stencilFail = stencilFail;
		g_pipelineState.stencilDepthFail = depthFail;
		g_pipelineState.stencilDepthPass = depthPass;
		g_pipelineState.stencilOpKnown = true;
	}
}
void CoreGLCompat::ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	if (!g_pipelineState.colorMaskKnown || g_pipelineState.colorMask[0] != red ||
		g_pipelineState.colorMask[1] != green || g_pipelineState.colorMask[2] != blue ||
		g_pipelineState.colorMask[3] != alpha)
	{
		FlushPending();
		glColorMask(red, green, blue, alpha);
		g_pipelineState.colorMask[0] = red;
		g_pipelineState.colorMask[1] = green;
		g_pipelineState.colorMask[2] = blue;
		g_pipelineState.colorMask[3] = alpha;
		g_pipelineState.colorMaskKnown = true;
	}
}
void CoreGLCompat::LineWidth(GLfloat width)
{
	if (!g_pipelineState.lineWidthKnown || g_pipelineState.lineWidth != width)
	{
		FlushPending();
		glLineWidth(width);
		g_pipelineState.lineWidth = width;
		g_pipelineState.lineWidthKnown = true;
	}
}
void CoreGLCompat::PointSize(GLfloat size)
{
	if (!g_pipelineState.pointSizeKnown || g_pipelineState.pointSize != size)
	{
		FlushPending();
		glPointSize(size);
		g_pipelineState.pointSize = size;
		g_pipelineState.pointSizeKnown = true;
	}
}
void CoreGLCompat::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (!g_pipelineState.viewportKnown || g_pipelineState.viewport[0] != x || g_pipelineState.viewport[1] != y ||
		g_pipelineState.viewport[2] != width || g_pipelineState.viewport[3] != height)
	{
		FlushPending();
		glViewport(x, y, width, height);
		g_pipelineState.viewport[0] = x;
		g_pipelineState.viewport[1] = y;
		g_pipelineState.viewport[2] = width;
		g_pipelineState.viewport[3] = height;
		g_pipelineState.viewportKnown = true;
	}
}
void CoreGLCompat::Clear(GLbitfield mask) { FlushPending(); glClear(mask); }
void CoreGLCompat::Flush() { FlushPending(); glFlush(); }
void CoreGLCompat::UseProgram(GLuint program) { FlushPending(); GLEW_GET_FUN(__glewUseProgram)(program); }
void CoreGLCompat::DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices)
{
	FlushPending();
	GLEW_GET_FUN(__glewDrawRangeElements)(mode, start, end, count, type, indices);
}
void CoreGLCompat::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
{
	FlushPending();
	glReadPixels(x, y, width, height, format, type, pixels);
}
void CoreGLCompat::TexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height,
	GLint border, GLenum format, GLenum type, const void* pixels)
{
	FlushPending();
	if (internalFormat == 3) internalFormat = GL_RGB8;
	else if (internalFormat == 4) internalFormat = GL_RGBA8;
	glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);
}
void CoreGLCompat::TexSubImage2D(GLenum target, GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height,
	GLenum format, GLenum type, const void* pixels)
{
	FlushPending();
	glTexSubImage2D(target, level, xOffset, yOffset, width, height, format, type, pixels);
}

void CoreGLCompat::PushAttrib(GLbitfield)
{
	FlushPending();
	AttributeState state;
	state.textureEnabled = g_textureEnabled;
	state.alphaTestEnabled = g_alphaTestEnabled;
	state.fogEnabled = g_fogEnabled;
	state.blendEnabled = glIsEnabled(GL_BLEND) == GL_TRUE;
	state.depthTestEnabled = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	state.cullFaceEnabled = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
	state.stencilTestEnabled = glIsEnabled(GL_STENCIL_TEST) == GL_TRUE;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &state.depthMask);
	glGetIntegerv(GL_BLEND_SRC_RGB, &state.blendSource);
	glGetIntegerv(GL_BLEND_DST_RGB, &state.blendDestination);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &state.textureBinding);
	state.textureEnvironment = g_textureEnvironment;
	state.alphaReference = g_alphaReference;
	memcpy(state.color, g_color, sizeof(g_color));
	state.projection = g_projection;
	state.modelView = g_modelView;
	g_attributeStack.push_back(state);
}

void CoreGLCompat::PopAttrib()
{
	if (g_attributeStack.empty())
		return;

	FlushPending();
	const AttributeState state = g_attributeStack.back();
	g_attributeStack.pop_back();
	g_textureEnabled = state.textureEnabled;
	g_alphaTestEnabled = state.alphaTestEnabled;
	g_fogEnabled = state.fogEnabled;
	g_textureEnvironment = state.textureEnvironment;
	g_alphaReference = state.alphaReference;
	memcpy(g_color, state.color, sizeof(g_color));
	g_projection = state.projection;
	g_modelView = state.modelView;
	MarkMatrixDirty();
	MarkStateDirty();
	MarkFogDirty();
	state.blendEnabled ? Enable(GL_BLEND) : Disable(GL_BLEND);
	state.depthTestEnabled ? Enable(GL_DEPTH_TEST) : Disable(GL_DEPTH_TEST);
	state.cullFaceEnabled ? Enable(GL_CULL_FACE) : Disable(GL_CULL_FACE);
	state.stencilTestEnabled ? Enable(GL_STENCIL_TEST) : Disable(GL_STENCIL_TEST);
	DepthMask(state.depthMask);
	BlendFunc(state.blendSource, state.blendDestination);
	BindTexture(GL_TEXTURE_2D, state.textureBinding);
}

void CoreGLCompat::EnableClientState(GLenum array) { if (array == GL_VERTEX_ARRAY) g_clientVertexArray.enabled = true; }
void CoreGLCompat::DisableClientState(GLenum array) { if (array == GL_VERTEX_ARRAY) g_clientVertexArray.enabled = false; }
void CoreGLCompat::VertexPointer(GLint size, GLenum type, GLsizei stride, const void* pointer)
{
	g_clientVertexArray.size = size;
	g_clientVertexArray.type = type;
	g_clientVertexArray.stride = stride;
	g_clientVertexArray.pointer = static_cast<const unsigned char*>(pointer);
}
void CoreGLCompat::DrawArrays(GLenum mode, GLint first, GLsizei count)
{
	FlushPending();
	if (!g_clientVertexArray.enabled || !g_clientVertexArray.pointer || g_clientVertexArray.type != GL_FLOAT || count <= 0)
	{
		glDrawArrays(mode, first, count);
		return;
	}

	g_clientVertices.clear();
	g_clientVertices.reserve(count);
	const size_t stride = g_clientVertexArray.stride > 0 ? g_clientVertexArray.stride : g_clientVertexArray.size * sizeof(float);
	for (GLsizei i = 0; i < count; ++i)
	{
		const float* position = reinterpret_cast<const float*>(g_clientVertexArray.pointer + (first + i) * stride);
		CoreVertex vertex;
		vertex.position[0] = position[0];
		vertex.position[1] = position[1];
		vertex.position[2] = g_clientVertexArray.size > 2 ? position[2] : 0.0f;
		memcpy(vertex.color, g_color, sizeof(g_color));
		memcpy(vertex.texCoord, g_texCoord, sizeof(g_texCoord));
		g_clientVertices.push_back(vertex);
	}
	DrawVertexBuffer(mode, g_clientVertices.data(), g_clientVertices.size());
}

void CoreGLCompat::DrawSphere(float radius, int slices, int stacks)
{
	if (radius <= 0.0f || slices < 3 || stacks < 2)
		return;

	const float pi = 3.14159265358979323846f;
	for (int stack = 0; stack < stacks; ++stack)
	{
		const float latitude0 = -0.5f * pi + pi * static_cast<float>(stack) / static_cast<float>(stacks);
		const float latitude1 = -0.5f * pi + pi * static_cast<float>(stack + 1) / static_cast<float>(stacks);
		const float z0 = sinf(latitude0);
		const float zr0 = cosf(latitude0);
		const float z1 = sinf(latitude1);
		const float zr1 = cosf(latitude1);

		Begin(GL_TRIANGLE_STRIP);
		for (int slice = 0; slice <= slices; ++slice)
		{
			const float longitude = 2.0f * pi * static_cast<float>(slice) / static_cast<float>(slices);
			const float x = cosf(longitude);
			const float y = sinf(longitude);
			TexCoord2f(static_cast<float>(slice) / slices, static_cast<float>(stack) / stacks);
			Vertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
			TexCoord2f(static_cast<float>(slice) / slices, static_cast<float>(stack + 1) / stacks);
			Vertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
		}
		End();
	}
}

void CoreGLCompat::ApplyFogUniforms(GLuint program)
{
	if (program == 0)
		return;

	FogUniformState& state = g_fogUniformCache[program];
	if (!state.initialized)
	{
		state.enabled = glGetUniformLocation(program, "uFogEnabled");
		state.color = glGetUniformLocation(program, "uFogColor");
		state.start = glGetUniformLocation(program, "uFogStart");
		state.end = glGetUniformLocation(program, "uFogEnd");
		state.initialized = true;
	}

	if (state.appliedVersion == g_fogVersion)
		return;

	if (state.enabled >= 0) glUniform1i(state.enabled, g_fogEnabled ? 1 : 0);
	if (state.color >= 0) glUniform4fv(state.color, 1, g_fogColor);
	if (state.start >= 0) glUniform1f(state.start, g_fogStart);
	if (state.end >= 0) glUniform1f(state.end, g_fogEnd);
	state.appliedVersion = g_fogVersion;
}

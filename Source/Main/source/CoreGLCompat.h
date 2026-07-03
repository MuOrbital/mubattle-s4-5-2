#pragma once

namespace CoreGLCompat
{
	bool Initialize();
	void Shutdown();
	bool IsInitialized();
	void FlushPending();

	void Begin(GLenum mode);
	void End();
	void Color3f(GLfloat r, GLfloat g, GLfloat b);
	void Color3fv(const GLfloat* value);
	void Color3ub(GLubyte r, GLubyte g, GLubyte b);
	void Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void Color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	void TexCoord2f(GLfloat u, GLfloat v);
	void Vertex2f(GLfloat x, GLfloat y);
	void Vertex2i(GLint x, GLint y);
	void Vertex3f(GLfloat x, GLfloat y, GLfloat z);
	void Vertex3fv(const GLfloat* value);
	void Normal3f(GLfloat x, GLfloat y, GLfloat z);

	void MatrixMode(GLenum mode);
	void LoadIdentity();
	void PushMatrix();
	void PopMatrix();
	void Translatef(GLfloat x, GLfloat y, GLfloat z);
	void Rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	void Scalef(GLfloat x, GLfloat y, GLfloat z);
	void Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	void Perspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void Ortho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
	void GetFloatv(GLenum pname, GLfloat* values);

	void Enable(GLenum capability);
	void Disable(GLenum capability);
	void AlphaFunc(GLenum function, GLfloat reference);
	void Fogf(GLenum pname, GLfloat value);
	void Fogfv(GLenum pname, const GLfloat* values);
	void TexEnvi(GLenum target, GLenum pname, GLint value);
	void TexEnvf(GLenum target, GLenum pname, GLfloat value);
	void TexParameteri(GLenum target, GLenum pname, GLint value);
	void PolygonMode(GLenum face, GLenum mode);
	void BindTexture(GLenum target, GLuint texture);
	GLuint GetBoundTexture2D();
	void DeleteTextures(GLsizei count, const GLuint* textures);
	void BlendFunc(GLenum source, GLenum destination);
	void DepthMask(GLboolean enabled);
	void DepthFunc(GLenum function);
	void FrontFace(GLenum mode);
	void StencilFunc(GLenum function, GLint reference, GLuint mask);
	void StencilOp(GLenum stencilFail, GLenum depthFail, GLenum depthPass);
	void ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	void LineWidth(GLfloat width);
	void PointSize(GLfloat size);
	void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
	void Clear(GLbitfield mask);
	void Flush();
	void UseProgram(GLuint program);
	void DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
	void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
	void TexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height,
		GLint border, GLenum format, GLenum type, const void* pixels);
	void TexSubImage2D(GLenum target, GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height,
		GLenum format, GLenum type, const void* pixels);
	void PushAttrib(GLbitfield mask);
	void PopAttrib();

	void EnableClientState(GLenum array);
	void DisableClientState(GLenum array);
	void VertexPointer(GLint size, GLenum type, GLsizei stride, const void* pointer);
	void DrawArrays(GLenum mode, GLint first, GLsizei count);
	void DrawSphere(float radius, int slices, int stacks);

	void ApplyFogUniforms(GLuint program);
}

#ifndef CORE_GL_COMPAT_IMPLEMENTATION
#define glBegin CoreGLCompat::Begin
#define glEnd CoreGLCompat::End
#define glColor3f CoreGLCompat::Color3f
#define glColor3fv CoreGLCompat::Color3fv
#define glColor3ub CoreGLCompat::Color3ub
#define glColor4f CoreGLCompat::Color4f
#define glColor4ub CoreGLCompat::Color4ub
#define glTexCoord2f CoreGLCompat::TexCoord2f
#define glVertex2f CoreGLCompat::Vertex2f
#define glVertex2i CoreGLCompat::Vertex2i
#define glVertex3f CoreGLCompat::Vertex3f
#define glVertex3fv CoreGLCompat::Vertex3fv
#define glNormal3f CoreGLCompat::Normal3f
#define glMatrixMode CoreGLCompat::MatrixMode
#define glLoadIdentity CoreGLCompat::LoadIdentity
#define glPushMatrix CoreGLCompat::PushMatrix
#define glPopMatrix CoreGLCompat::PopMatrix
#define glTranslatef CoreGLCompat::Translatef
#define glRotatef CoreGLCompat::Rotatef
#define glScalef CoreGLCompat::Scalef
#define glOrtho CoreGLCompat::Ortho
#define gluPerspective CoreGLCompat::Perspective
#define gluOrtho2D CoreGLCompat::Ortho2D
#define glGetFloatv CoreGLCompat::GetFloatv
#define glEnable CoreGLCompat::Enable
#define glDisable CoreGLCompat::Disable
#define glAlphaFunc CoreGLCompat::AlphaFunc
#define glFogf CoreGLCompat::Fogf
#define glFogfv CoreGLCompat::Fogfv
#define glTexEnvi CoreGLCompat::TexEnvi
#define glTexEnvf CoreGLCompat::TexEnvf
#define glTexParameteri CoreGLCompat::TexParameteri
#define glPolygonMode CoreGLCompat::PolygonMode
#define glBindTexture CoreGLCompat::BindTexture
#define glDeleteTextures CoreGLCompat::DeleteTextures
#define glBlendFunc CoreGLCompat::BlendFunc
#define glDepthMask CoreGLCompat::DepthMask
#define glDepthFunc CoreGLCompat::DepthFunc
#define glFrontFace CoreGLCompat::FrontFace
#define glStencilFunc CoreGLCompat::StencilFunc
#define glStencilOp CoreGLCompat::StencilOp
#define glColorMask CoreGLCompat::ColorMask
#define glLineWidth CoreGLCompat::LineWidth
#define glPointSize CoreGLCompat::PointSize
#define glViewport CoreGLCompat::Viewport
#define glClear CoreGLCompat::Clear
#define glFlush CoreGLCompat::Flush
#define glUseProgram CoreGLCompat::UseProgram
#define glDrawRangeElements CoreGLCompat::DrawRangeElements
#define glReadPixels CoreGLCompat::ReadPixels
#define glTexImage2D CoreGLCompat::TexImage2D
#define glTexSubImage2D CoreGLCompat::TexSubImage2D
#define glPushAttrib CoreGLCompat::PushAttrib
#define glPopAttrib CoreGLCompat::PopAttrib
#define glEnableClientState CoreGLCompat::EnableClientState
#define glDisableClientState CoreGLCompat::DisableClientState
#define glVertexPointer CoreGLCompat::VertexPointer
#define glDrawArrays CoreGLCompat::DrawArrays
#endif

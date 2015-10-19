#include "GLTexture.h"

namespace ARehabGL
{

	GLTexture::GLTexture(QOpenGLFunctions_4_3_Core * gl) : 
		GLFunctions(gl),
		textureHandle(0),
		m_failed(false),
		height(0),
		width(0)
	{
		gl->glGenTextures(1, &textureHandle);
	}

	GLuint GLTexture::getHandle(void)
	{
		return this->textureHandle;
	}

	GLuint GLTexture::getHeight(void)
	{
		return this->height;
	}

	GLuint GLTexture::getWidth(void)
	{
		return this->width;
	}

	GLTexture::~GLTexture(void)
	{
		gl->glDeleteTextures(1, &textureHandle);
	}

};
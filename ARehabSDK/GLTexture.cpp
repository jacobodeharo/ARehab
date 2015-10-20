/*
This file is part of ARehab.

ARehab is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

ARehab is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.If not, see <http://www.gnu.org/licenses/>.

Copyright 2015 Jacobo de Haro
jacobodeharo@gmail.com
@jacobodeharo

*/

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
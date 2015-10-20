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

#include "GLFBO.h"


GLFBO::GLFBO(QOpenGLFunctions_4_3_Core * gl)
	:GLFunctions(gl),
	height(0),
	width(0),
	fboHandle(0), //Frame Buffer Object
	rboHandle(0), //Render Buffer Object
	ready(false)
{
	gl->glGenFramebuffers(1, &fboHandle); // Create a Frame Buffer Object
	gl->glGenRenderbuffers(1, &rboHandle); // Create a Render Buffer Object	
}


void GLFBO::atachTexture2D(GLTexture2D * textureVideo, GLenum attachement)
{	
	if (textureVideo != NULL)
	{
		this->height = textureVideo->getHeight();
		this->width = textureVideo->getWidth();
		gl->glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, GL_TEXTURE_2D, textureVideo->getHandle(), 0);

		gl->glBindRenderbuffer(GL_RENDERBUFFER, rboHandle);
		gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->height, this->width);
		gl->glBindRenderbuffer(GL_RENDERBUFFER, 0);

		gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboHandle);

		//Check status of the FBO
		if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw GLFBOException("FBO status not completed.");
			return;
		}
		this->ready = true;
	}
	else
	{
		throw GLFBOException("Texture can't be attached because is NULL.");
		return;
	}
}

void GLFBO::bind(void)
{
	gl->glBindFramebuffer(GL_FRAMEBUFFER, this->fboHandle);	
}

void GLFBO::unbind(void)
{
	gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


GLFBO::~GLFBO(void)
{
	if (this->fboHandle != 0)
	{
		gl->glDeleteFramebuffers(1, &fboHandle);
		this->fboHandle = 0;
	}
	if (this->rboHandle != 0)
	{
		gl->glDeleteRenderbuffers(1, &rboHandle);
		this->rboHandle = 0;
	}
}

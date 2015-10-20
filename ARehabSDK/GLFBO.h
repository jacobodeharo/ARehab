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

#ifndef GLFBO_H_
#define GLFBO_H_

#include "Toolkit.h"
#include "GLTexture2D.h"

namespace ARehabGL {

	class GLFBOException : public std::runtime_error {
	public:
		GLFBOException(const std::string & msg)
			: std::runtime_error(msg)
		{
		}
	};

	class GLFBO : public GLFunctions
	{
		public:
			GLFBO(QOpenGLFunctions_4_3_Core * gl);
			void atachTexture2D(GLTexture2D * textureVideo, GLenum attachement=GL_COLOR_ATTACHMENT0);
			void bind(void);
			void unbind(void);
			virtual ~GLFBO(void);

		protected:
			GLuint height;
			GLuint width;
			GLuint fboHandle; //Frame Buffer Object
			GLuint rboHandle; //Render Buffer Object
			bool ready;
	};

}

#endif

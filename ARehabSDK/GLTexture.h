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

#ifndef GLTEXTURE_H_
#define GLTEXTURE_H_

#include <QImage>

#include "Toolkit.h"

namespace ARehabGL
{
	class GLTextureException : public std::runtime_error {
		public:
			GLTextureException(const std::string & msg)
				: std::runtime_error(msg)
			{
			}
	};

	class GLTexture : public GLFunctions
	{
		public:
			GLTexture(QOpenGLFunctions_4_3_Core * gl);
			virtual ~GLTexture(void);
			virtual void bind(void) = 0;
			virtual void unbind(void) = 0;
			virtual bool failed(void) const { return m_failed; }
			GLuint getHandle(void);
			GLuint getHeight(void);
			GLuint getWidth(void);

		protected:
			GLuint textureHandle;
			GLuint height;
			GLuint width;
			GLint format;
			bool m_failed;
	};

};

#endif
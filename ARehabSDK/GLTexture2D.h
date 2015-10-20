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

#ifndef GLTEXTURE2D_H_
#define GLTEXTURE2D_H_

#include "GLTexture.h"

namespace ARehabGL
{
	class GLTexture2D : public GLTexture
	{
		public:
			explicit GLTexture2D(QOpenGLFunctions_4_3_Core * gl, int width, int height, unsigned int format = GL_RGBA, unsigned int type = GL_UNSIGNED_INT);
			explicit GLTexture2D(QOpenGLFunctions_4_3_Core * gl, const QString& fileName) throw (GLTextureException);
			explicit GLTexture2D(QOpenGLFunctions_4_3_Core * gl, uchar * buff, int width = 0, int height = 0, unsigned int format = GL_RGBA);
			//void load(int width, int height, QRgb *data);
			virtual void bind(void) Q_DECL_OVERRIDE;
			virtual void unbind(void) Q_DECL_OVERRIDE;
			void update(uchar * buff, int width = 0, int height = 0, unsigned int format = GL_RGBA);
	};

};

#endif
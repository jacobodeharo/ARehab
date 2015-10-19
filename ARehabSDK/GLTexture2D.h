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
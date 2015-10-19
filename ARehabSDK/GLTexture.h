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
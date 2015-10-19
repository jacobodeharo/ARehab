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

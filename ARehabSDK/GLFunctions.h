#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_

#include <QDebug>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLContext>

namespace ARehabGL
{
	class GLFunctions
	{
		public:	
			GLFunctions(void) :gl(0){}
			GLFunctions(QOpenGLFunctions_4_3_Core * gl) : gl(gl){}
			QOpenGLFunctions_4_3_Core * setupOpenGLFunctionsPtr(QOpenGLContext * context);
					
		protected:
			QOpenGLFunctions_4_3_Core * gl;
	};
};

#endif
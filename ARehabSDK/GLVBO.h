#ifndef GLVBO_H_
#define GLVBO_H_

#include "Toolkit.h"

#define BUFFER_OFFSET(i) ((void*)(i))
#define BUFFER_OFFSET_UINT(i) ((GLvoid*)(sizeof(GLuint) * i))

namespace ARehabGL {

	class GLVBO : public GLFunctions
	{
		public:
			GLVBO(QOpenGLFunctions_4_3_Core * gl);			
			virtual ~GLVBO(void);
			virtual void render(void) = 0;

		protected:
			
			unsigned int vaoHandle;
			unsigned int * vboHandles; //buffer handles (position, color, etc) 
			unsigned int numVBOHandles; //Number of VBO for free resources when the program exit.
	};

};

#endif

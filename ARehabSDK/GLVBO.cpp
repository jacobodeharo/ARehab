#include "GLVBO.h"

namespace ARehabGL
{
	
	GLVBO::GLVBO(QOpenGLFunctions_4_3_Core * gl) :
		GLFunctions(gl),
		vaoHandle(0),
		vboHandles(0),
		numVBOHandles(0)
	{
	}


	GLVBO::~GLVBO(void)
	{
		if (0 != this->vboHandles && vaoHandle && numVBOHandles)
		{			
			gl->glDeleteBuffers(numVBOHandles, this->vboHandles);
			delete[] this->vboHandles;
			this->vboHandles = 0;

			gl->glDeleteVertexArrays(1, &this->vaoHandle);			
		}
	}

};

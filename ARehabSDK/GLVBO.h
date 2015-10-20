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

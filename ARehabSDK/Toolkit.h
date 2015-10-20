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

#ifndef TOOLKIT_H_
#define TOOLKIT_H_

#include <string>
#include <sys/stat.h>

#define GLM_SWIZZLE //This enable swizzle operators, selecting components like foo.xyz
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

#include "GLFunctions.h"
using namespace ARehabGL;

namespace ARehabTools
{
	class Measures
	{
		public:
			/* Usage:
				long long start = milliseconds_now();
				// TASK TO BE MEASURED....
				long long elapsed = milliseconds_now() - start;
			*/
			static unsigned long long int milliseconds_now();
	};

	class GLToolkit : public GLFunctions
	{
		public:
			static std::string getExtension(const char * name);

			static bool fileExists(const std::string & fileName);
			
			static const char * getTypeString(GLenum type);

			static void rgb2ycbcr(float * pixel);

			static glm::vec3 rgb2ycbcr(const float r, const float g, const float b);

			static void ycbcr2rgb(float * pixel);

	};

}

#endif


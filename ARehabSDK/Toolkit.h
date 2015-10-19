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


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

#include "Toolkit.h"

namespace ARehabTools
{
	std::string GLToolkit::getExtension(const char * name) {
		std::string nameStr(name);

		size_t loc = nameStr.find_last_of('.');
		if (loc != std::string::npos) {
			return nameStr.substr(loc, std::string::npos);
		}
		return "";
	}

	bool GLToolkit::fileExists(const std::string & fileName)
	{
		struct stat info;
		int ret = -1;

		ret = stat(fileName.c_str(), &info);
		return 0 == ret;
	}

	const char * GLToolkit::getTypeString(GLenum type) {
		switch (type) {
		case GL_FLOAT:
			return "float";
		case GL_FLOAT_VEC2:
			return "vec2";
		case GL_FLOAT_VEC3:
			return "vec3";
		case GL_FLOAT_VEC4:
			return "vec4";
		case GL_DOUBLE:
			return "double";
		case GL_INT:
			return "int";
		case GL_UNSIGNED_INT:
			return "unsigned int";
		case GL_BOOL:
			return "bool";
		case GL_FLOAT_MAT2:
			return "mat2";
		case GL_FLOAT_MAT3:
			return "mat3";
		case GL_FLOAT_MAT4:
			return "mat4";
		default:
			return "?";
		}
	}

	void GLToolkit::rgb2ycbcr(float * pixel)
	{
		float pixelAux[3] = { 0.0f, 0.0f, 0.0f };

		pixelAux[0] = 0.299f * pixel[0] + 0.587f * pixel[1] + 0.114f * pixel[2]; //Y
		pixelAux[1] = 128.0f - 0.168736f * pixel[0] - 0.331264f * pixel[1] + 0.5f * pixel[2]; //Cb
		pixelAux[2] = 128.0f + 0.5f * pixel[0] - 0.418688f * pixel[1] - 0.081312f * pixel[2]; //Cr

		pixel[0] = pixelAux[0];
		pixel[1] = pixelAux[1];
		pixel[2] = pixelAux[2];
	}

	glm::vec3 GLToolkit::rgb2ycbcr(const float r, const float g, const float b)
	{
		return (glm::vec3(
			0.299f * r + 0.587f * g + 0.114f * b, //Y
			128.0f - 0.168736f * r - 0.331264f * g + 0.5f * b, //Cb
			128.0f + 0.5f * r - 0.418688f * g - 0.081312f * b //Cr
			));
	}

	void GLToolkit::ycbcr2rgb(float * pixel)
	{
		float pixelAux[3] = { 0.0f, 0.0f, 0.0f };

		pixelAux[0] = pixel[0] + 1.402f * (pixel[2] - 128.0f); //R
		pixelAux[1] = pixel[0] - 0.34414f * (pixel[1] - 128.0f) - 0.71414f * (pixel[2] - 128.0f); //G
		pixelAux[2] = pixel[0] + 1.772f * (pixel[1] - 128.0f); //B

		pixel[0] = pixelAux[0];
		pixel[1] = pixelAux[1];
		pixel[2] = pixelAux[2];
	}

	unsigned long long int Measures::milliseconds_now(void) {
		static LARGE_INTEGER s_frequency;
		static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
		if (s_use_qpc) {
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			return (1000LL * now.QuadPart) / s_frequency.QuadPart;
		}
		else {
			return GetTickCount();
		}
	}
};

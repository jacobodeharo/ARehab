#ifndef GLSHADER_H_
#define GLSHADER_H_

#include <cstdio>
#include <cstdlib>

#include <stdexcept>
#include <map>
#include <iostream>
#include <fstream>
using std::ifstream;
#include <sstream>
#include <string>
using std::string;
#include <sys/stat.h>

#include "Toolkit.h"
using namespace ARehabTools;

namespace ARehabGL
{

	struct ShaderFileExtension {
		const char * ext;
		GLenum type;
	};

	static ShaderFileExtension extensions[] = {
		{ ".vs", GL_VERTEX_SHADER },
		{ ".vert", GL_VERTEX_SHADER },
		{ ".gs", GL_GEOMETRY_SHADER },
		{ ".geom", GL_GEOMETRY_SHADER },
		{ ".tcs", GL_TESS_CONTROL_SHADER },
		{ ".tes", GL_TESS_EVALUATION_SHADER },
		{ ".fs", GL_FRAGMENT_SHADER },
		{ ".frag", GL_FRAGMENT_SHADER },
		{ ".cs", GL_COMPUTE_SHADER }
	};
	
	class GLShaderException : public std::runtime_error {
		public:
			GLShaderException(const std::string & msg)
				: std::runtime_error(msg)
			{
			}
	};

	class GLShader : public GLFunctions
	{
		private:
			GLuint  handle;
			bool linked;
			std::map<std::string, int> uniformLocations;

			//GLShader(const GLShader &other) { }
			GLShader & operator=(const GLShader &other) { return *this; }
			GLint getUniformLocation(const char * name);
			
		public:
			GLShader(QOpenGLFunctions_4_3_Core * gl);
			virtual ~GLShader(void);

			void   compile(const char * fileName) throw (GLShaderException);
			void   link(void) throw (GLShaderException);
			void   validate(void) throw(GLShaderException);
			void   use(void) throw (GLShaderException);

			GLuint getHandle(void);
			bool   isLinked(void);

			void   bindAttribLocation(GLuint location, const char * name);
			void   bindFragDataLocation(GLuint location, const char * name);

			void   setUniform(const char * name, float x, float y, float z);
			void   setUniform(const char * name, const glm::vec2 & v);
			void   setUniform(const char * name, const glm::vec3 & v);
			void   setUniform(const char * name, const glm::vec4 & v);
			void   setUniform(const char * name, const glm::mat4 & m);
			void   setUniform(const char * name, const glm::mat3 & m);
			void   setUniform(const char * name, float val);
			void   setUniform(const char * name, int val);
			void   setUniform(const char * name, bool val);
			void   setUniform(const char * name, GLuint val);

			void   printActiveUniforms(void);
			void   printActiveUniformBlocks(void);
			void   printActiveAttribs(void);

			const char * getTypeString(GLenum type);
	};

};

#endif
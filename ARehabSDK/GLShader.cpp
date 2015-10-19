#include "GLShader.h"

namespace ARehabGL
{

	GLShader::GLShader(QOpenGLFunctions_4_3_Core * gl) :
		GLFunctions(gl),
		handle(0), 
		linked(false)
	{
	}

	GLShader::~GLShader(void)
	{
		if (handle == 0) return;

		// Query the number of attached shaders
		GLint numShaders = 0;
		gl->glGetProgramiv(handle, GL_ATTACHED_SHADERS, &numShaders);

		// Get the shader names
		GLuint * shaderNames = new GLuint[numShaders];
		gl->glGetAttachedShaders(handle, numShaders, NULL, shaderNames);

		// Delete the shaders
		for (int i = 0; i < numShaders; i++)
			gl->glDeleteShader(shaderNames[i]);

		// Delete the program
		gl->glDeleteProgram(handle);

		delete [] shaderNames;
	}

	void GLShader::compile(const char * fileName)
		throw(GLShaderException)
	{
		int numExts = sizeof(ARehabGL::extensions) / sizeof(ShaderFileExtension);

		// Check the file name's extension to determine the shader type
		std::string ext = GLToolkit::getExtension(fileName);
		GLenum type;
		bool matchFound = false;
		for (int i = 0; i < numExts; ++i) {
			if (ext == ARehabGL::extensions[i].ext) {
				matchFound = true;
				type = ARehabGL::extensions[i].type;
				break;
			}
		}

		// If we didn't find a match, throw an exception
		if (!matchFound) {
			std::string msg = "Unrecognized extension: " + ext;
			throw GLShaderException(msg);
		}

		if (handle <= 0) {
			handle = gl->glCreateProgram();
			if (handle == 0) {
				throw GLShaderException("Unable to create shader program.");
			}
		}
		
		GLuint shaderHandle = gl->glCreateShader(type);
		
		// Get file contents
		std::ifstream inFile(fileName, std::ios::in);
		if (!inFile) {
			std::string message = std::string("Unable to open: ") + fileName;
			throw GLShaderException(message);
		}
		std::stringstream code;
		code << inFile.rdbuf();
		inFile.close();
		string str = code.str();
		const char * c_code = str.c_str();
		
		gl->glShaderSource(shaderHandle, 1, &c_code, NULL);

		// Compile the shader
		gl->glCompileShader(shaderHandle);

		// Check for errors
		int result = GL_FALSE;
		gl->glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
		if (GL_FALSE == result) {
			// Compile failed, get log
			int length = 0;
			std::string logString;
			gl->glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
			if (length > 0) {
				char * c_log = new char[length];
				int written = 0;
				gl->glGetShaderInfoLog(shaderHandle, length, &written, c_log);
				logString = c_log;
				delete[] c_log;
			}
			std::string msg;
			if (fileName) {
				msg = std::string(fileName) + ": shader compliation failed\n";
			} else {
				msg = "Shader compilation failed.\n";
			}
			msg += logString;

			throw GLShaderException(msg);

		} else {
			// Compile succeeded, attach shader
			gl->glAttachShader(handle, shaderHandle);
		}
	}

	void GLShader::link(void) throw(GLShaderException)
	{
		if (linked) return;
		if (handle <= 0)
			throw GLShaderException("Program has not been compiled.");

		gl->glLinkProgram(handle);

		int status = 0;
		gl->glGetProgramiv(handle, GL_LINK_STATUS, &status);
		if (GL_FALSE == status) {
			// Store log and return false
			int length = 0;
			std::string logString;

			gl->glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);

			if (length > 0) {
				char * c_log = new char[length];
				int written = 0;
				gl->glGetProgramInfoLog(handle, length, &written, c_log);
				logString = c_log;
				delete[] c_log;
			}

			throw GLShaderException(std::string("Program link failed:\n") + logString);
		}
		else {
			uniformLocations.clear();
			linked = true;
		}
		//gl->glDeleteShader(handle); // Delete the shader as they're linked into our program now and no longer necessary
	}

	void GLShader::use(void) throw(GLShaderException)
	{
		if (this->handle <= 0 || (!linked))
			throw GLShaderException("Shader has not been linked");
		else
		{
			gl->glUseProgram(this->handle);
		}
	}

	GLuint GLShader::getHandle(void)
	{
		return handle;
	}

	bool GLShader::isLinked(void)
	{
		return linked;
	}

	void GLShader::bindAttribLocation(GLuint location, const char * name)
	{
		gl->glBindAttribLocation(handle, location, name);
	}

	void GLShader::bindFragDataLocation(GLuint location, const char * name)
	{
		gl->glBindFragDataLocation(handle, location, name);
	}

	void GLShader::setUniform(const char *name, float x, float y, float z)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniform3f(loc, x, y, z);
	}

	void GLShader::setUniform(const char *name, const glm::vec3 & v)
	{
		this->setUniform(name, v.x, v.y, v.z);
	}

	void GLShader::setUniform(const char *name, const glm::vec4 & v)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniform4f(loc, v.x, v.y, v.z, v.w);
	}

	void GLShader::setUniform(const char *name, const glm::vec2 & v)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniform2f(loc, v.x, v.y);
	}

	void GLShader::setUniform(const char *name, const glm::mat4 & m)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	}

	void GLShader::setUniform(const char *name, const glm::mat3 & m)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	}

	void GLShader::setUniform(const char *name, float val)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniform1f(loc, val);
	}

	void GLShader::setUniform(const char *name, int val)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniform1i(loc, val);
	}

	void GLShader::setUniform(const char *name, GLuint val)
	{
		GLint loc = getUniformLocation(name);
		gl->glUniform1ui(loc, val);
	}

	void GLShader::setUniform(const char *name, bool val)
	{
		int loc = getUniformLocation(name);
		gl->glUniform1i(loc, val);
	}

	void GLShader::printActiveUniforms(void) {
		GLint numUniforms = 0;
		gl->glGetProgramInterfaceiv(handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

		printf("Active uniforms:\n");
		for (int i = 0; i < numUniforms; ++i) {
			GLint results[4];
			gl->glGetProgramResourceiv(handle, GL_UNIFORM, i, 4, properties, 4, NULL, results);

			if (results[3] != -1) continue;  // Skip uniforms in blocks 
			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			gl->glGetProgramResourceName(handle, GL_UNIFORM, i, nameBufSize, NULL, name);
			printf("%-5d %s (%s)\n", results[2], name, GLToolkit::getTypeString(results[1]));
			delete[] name;
		}
	}

	void GLShader::printActiveUniformBlocks(void) {
		GLint numBlocks = 0;

		gl->glGetProgramInterfaceiv(handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
		GLenum blockProps[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
		GLenum blockIndex[] = { GL_ACTIVE_VARIABLES };
		GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };

		for (int block = 0; block < numBlocks; ++block) {
			GLint blockInfo[2];
			gl->glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);
			GLint numUnis = blockInfo[0];

			char * blockName = new char[blockInfo[1] + 1];
			gl->glGetProgramResourceName(handle, GL_UNIFORM_BLOCK, block, blockInfo[1] + 1, NULL, blockName);
			printf("Uniform block \"%s\":\n", blockName);
			delete[] blockName;

			GLint * unifIndexes = new GLint[numUnis];
			gl->glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);

			for (int unif = 0; unif < numUnis; ++unif) {
				GLint uniIndex = unifIndexes[unif];
				GLint results[3];
				gl->glGetProgramResourceiv(handle, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results);

				GLint nameBufSize = results[0] + 1;
				char * name = new char[nameBufSize];
				gl->glGetProgramResourceName(handle, GL_UNIFORM, uniIndex, nameBufSize, NULL, name);
				printf("    %s (%s)\n", name, GLToolkit::getTypeString(results[1]));
				delete[] name;
			}

			delete[] unifIndexes;
		}
	}

	void GLShader::printActiveAttribs(void) {
		GLint numAttribs;
		gl->glGetProgramInterfaceiv(handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

		printf("Active attributes:\n");
		for (int i = 0; i < numAttribs; ++i) {
			GLint results[3];
			gl->glGetProgramResourceiv(handle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			gl->glGetProgramResourceName(handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);
			printf("%-5d %s (%s)\n", results[2], name, GLToolkit::getTypeString(results[1]));
			delete[] name;
		}
	}

	void GLShader::validate(void) throw(GLShaderException)
	{
		if (!isLinked())
			throw GLShaderException("Program is not linked");

		GLint status;
		gl->glValidateProgram(handle);
		gl->glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);

		if (GL_FALSE == status) {
			// Store log and return false
			int length = 0;
			std::string logString;

			gl->glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);

			if (length > 0) {
				char * c_log = new char[length];
				int written = 0;
				gl->glGetProgramInfoLog(handle, length, &written, c_log);
				logString = c_log;
				delete[] c_log;
			}
			throw GLShaderException(std::string("Program failed to validate\n") + logString);
		}
	}

	int GLShader::getUniformLocation(const char * name)
	{
		std::map<std::string, int>::iterator pos;
		pos = uniformLocations.find(name);

		if (pos == uniformLocations.end()) {
			uniformLocations[name] = gl->glGetUniformLocation(handle, name);
		}

		return uniformLocations[name];
	}

}; //namespace ARehabGL
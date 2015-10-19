#include "GLVBOVideo.h"

namespace ARehabGL
{
	
	GLVBOVideo::GLVBOVideo(QOpenGLFunctions_4_3_Core * gl) : GLVBO(gl)
	{		
		GLfloat geometry[] = {
			0.0f, 1080.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1920.0f, 0.0f, 0.0f,
			1920.0f, 1080.0f, 0.0f
		};

		GLfloat texCoords[] {
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};

		// Create and populate the buffer objects
		this->vboHandles = new unsigned int[3];
		gl->glGenBuffers(3, vboHandles);
		GLuint positionBufferHandle = vboHandles[0];
		GLuint texCoordsHandle = vboHandles[1];

		gl->glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		gl->glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), geometry, GL_STATIC_DRAW);

		gl->glBindBuffer(GL_ARRAY_BUFFER, texCoordsHandle);
		gl->glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), texCoords, GL_STATIC_DRAW);

		// Create and set-up the vertex array object
		gl->glGenVertexArrays(1, &(this->vaoHandle));
		gl->glBindVertexArray(this->vaoHandle);

		gl->glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		gl->glEnableVertexAttribArray(0);  // Vertex position

		gl->glBindBuffer(GL_ARRAY_BUFFER, texCoordsHandle);
		gl->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		gl->glEnableVertexAttribArray(1);  // Texture Coords

		gl->glBindVertexArray(0); // Unbind VAO
	}

	void GLVBOVideo::render(void)
	{
		if (this->vaoHandle)
		{
			gl->glBindVertexArray(this->vaoHandle);
			gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			gl->glBindVertexArray(0);
		}
	}

};

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

#include "GLVBOBody.h"

namespace ARehabGL
{
	const unsigned int GLVBOBody::maxJoints = 25;
	const GLuint GLVBOBody::bodyPartsIndexes[] = {
		0, 1, 20, 2, 3,	//LINE_STRIP_1 (SPINE_BASE -> SPINE_MID -> SPINE_SHOULDER -> NECK -> HEAD)
		20, 8, 9, 10, 11, // LINE_STRIP_RIGHT_ARM (SPINE_SHOULDER -> SHOULDER_RIGHT -> ELBOW_RIGHT -> WRIST_RIGHT -> HAND_RIGHT)
		20, 4, 5, 6, 7,	//LINE_STRIP_LEFT_ARM (SPINE_SHOULDER -> SHOULDER_LEFT -> ELBOW_LEFT -> WRIST_LEFT -> HAND_LEFT)
		0, 12, 13, 14, 15, //LINE_STRIP_LEFT_LEG (SPINE_BASE -> HIP_LEFT -> KNEE_LEFT -> ANKLE_LEFT ->FOOT_LEFT)
		0, 16, 17, 18, 19	//LINE_STRIP_RIGHT_LEG (SPINE_BASE -> HIP_RIGHT -> KNEE_RIGHT -> ANKLE_RIGHT -> FOOT_RIGHT)
	};

	GLVBOBody::GLVBOBody(QOpenGLFunctions_4_3_Core * gl) :
		GLVBO(gl),
		jointsTracked(new GLuint[GLVBOBody::maxJoints]),
		numJointsTracked(GLVBOBody::maxJoints)
	{
		GLfloat positionData[GLVBOBody::maxJoints * 3];
		memset(positionData, 0, GLVBOBody::maxJoints * 3 * sizeof(GLfloat));
		
		for (unsigned int i = 0; i < GLVBOBody::maxJoints; ++i)
			jointsTracked[i] = i;

		for (int i = 0; i < GLVBOBody::maxJoints * 3; i = i + 3)
		{			
			positionData[i] = -1000.0f;
			positionData[i + 1] = -1000.0f;
			positionData[i + 2] = -1000.0f;
		}

		GLfloat colorData[GLVBOBody::maxJoints * 3];
		memset(colorData, 0, GLVBOBody::maxJoints * 3 * sizeof(GLfloat));
		for (int i = 0; i < GLVBOBody::maxJoints * 3; i = i + 3)
		{
			colorData[i] = 0.0f;
			colorData[i + 1] = 1.0f;
			colorData[i + 2] = 1.0f;
		}

		GLfloat orientations[GLVBOBody::maxJoints * 4];
		memset(orientations, 0, GLVBOBody::maxJoints * 4 * sizeof(GLfloat));
		for (int i = 0; i < GLVBOBody::maxJoints * 3; i = i + 3)
		{
			orientations[i] = 1.0f;
			orientations[i + 1] = 1.0f;
			orientations[i + 2] = 0;
			orientations[i + 3] = 0;
		}

		// Create and populate the buffer objects
		this->numVBOHandles = 5;
		this->vboHandles = new unsigned int[this->numVBOHandles];
		memset(this->vboHandles, 0, this->numVBOHandles * sizeof(unsigned int));

		gl->glGenBuffers(this->numVBOHandles, this->vboHandles);
		GLuint positionBufferHandle = this->vboHandles[0];
		GLuint colorBufferHandle = this->vboHandles[1];
		GLuint orientationsHandle = this->vboHandles[2];
		GLuint indexesHandle = this->vboHandles[3];
		GLuint indexesHandleTracked = this->vboHandles[4]; //Indexes for GL_POINTS

		gl->glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		gl->glBufferData(GL_ARRAY_BUFFER, GLVBOBody::maxJoints * 3 * sizeof(GLfloat), positionData, GL_DYNAMIC_DRAW);

		gl->glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
		gl->glBufferData(GL_ARRAY_BUFFER, GLVBOBody::maxJoints * 3 * sizeof(GLfloat), colorData, GL_STATIC_DRAW);

		gl->glBindBuffer(GL_ARRAY_BUFFER, orientationsHandle);
		gl->glBufferData(GL_ARRAY_BUFFER, GLVBOBody::maxJoints * 4 * sizeof(GLfloat), orientations, GL_STATIC_DRAW);

		gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesHandle);
		gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 25 * sizeof(GLuint), GLVBOBody::bodyPartsIndexes, GL_STATIC_DRAW);

		gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesHandleTracked);
		gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numJointsTracked * sizeof(GLuint), this->jointsTracked, GL_STATIC_DRAW);

		// Create and set-up the vertex array object
		gl->glGenVertexArrays(1, &(this->vaoHandle));
		gl->glBindVertexArray(this->vaoHandle);

		gl->glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		gl->glEnableVertexAttribArray(0);  // Vertex position

		gl->glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
		gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		gl->glEnableVertexAttribArray(1);  // Vertex color

		gl->glBindBuffer(GL_ARRAY_BUFFER, orientationsHandle);
		gl->glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		gl->glEnableVertexAttribArray(2);

		gl->glBindVertexArray(0); // Unbind VAO
	}

	void GLVBOBody::updateJoints(glm::vec2 * joints, glm::vec4 * orient, unsigned int * kinectTrackingState, unsigned int numJoints)
	{
		if (!joints || !kinectTrackingState)
			return;

		gl->glBindVertexArray(this->vaoHandle);
		GLuint positionBufferHandle = this->vboHandles[0];
		GLuint orientationsHandle = this->vboHandles[2];
		int sizePos = numJoints * 3 * sizeof(float),
			sizeOrient = numJoints * 4 * sizeof(float);
		glm::vec3 * data = new glm::vec3[numJoints];
		for (int i = 0; i < numJoints; ++i)
		{
			if (kinectTrackingState[i])
			{
				data[i] = glm::vec3(joints[i], 0.0f);
			}
			else {
				data[i] = glm::vec3(-100.0f, -100.0f, -100.0f);
			}
		}
		gl->glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		gl->glBufferSubData(GL_ARRAY_BUFFER, 0, sizePos, reinterpret_cast<float*>(data));
		delete[] data;
		gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		if (orient)
		{
			gl->glBindBuffer(GL_ARRAY_BUFFER, orientationsHandle);
			gl->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOrient, reinterpret_cast<float*>(orient));
			gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		gl->glBindVertexArray(0); // Unbind VAO
	}

	void GLVBOBody::updateTrackedJoints(unsigned int * trackedJoints, unsigned int numTrackedJoints)
	{
		this->numJointsTracked = numTrackedJoints;
		memcpy(this->jointsTracked, trackedJoints, sizeof(unsigned int)*numTrackedJoints);

		gl->glBindVertexArray(this->vaoHandle);
		GLuint indexesHandleTracked = this->vboHandles[4]; //Indexes for GL_POINTS
		gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesHandleTracked);
		gl->glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->numJointsTracked * sizeof(GLuint), reinterpret_cast<float*>(this->jointsTracked));
		gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		gl->glBindVertexArray(0); // Unbind VAO
	}

	void GLVBOBody::render(void)
	{
		if (this->vaoHandle)
		{
			gl->glBindVertexArray(this->vaoHandle);
			if (!this->jointsTracked) //In this case, all the body will be rendered
			{				
				gl->glDrawArrays(GL_POINTS, 0, GLVBOBody::maxJoints);

				//GL_LINEs
				gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboHandles[3]);
				for (unsigned int i = 0; i < 5; ++i) //Foreach bodyPart (two nested Joints are one bodyPart)
				{
					for (unsigned int j = 5 * i; j < (5 * i + 4); ++j) //For each pair, draw GL_LINES
					{
						gl->glDrawRangeElements(GL_LINES, j, j + 1, 2, GL_UNSIGNED_INT, BUFFER_OFFSET_UINT(j));						
					}
				}
				gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);				
			} else
			{ //Only the selected joints will be rendered								
				//GL_POINTS
				gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboHandles[4]);
				gl->glDrawElements(GL_POINTS, this->numJointsTracked, GL_UNSIGNED_INT, BUFFER_OFFSET_UINT(0));
				gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				//GL_LINES
				gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboHandles[3]);
				for (unsigned int joint = 0; joint < this->numJointsTracked; ++joint)
				{
					for (unsigned int i = 0; i < 5; ++i) //Foreach bodyPart tracked (two nested Joints are one bodyPart)
					{
						for (unsigned int j = 5 * i; j < (5 * i + 4); ++j) //For each pair, draw GL_LINES
						{
							if ((this->jointsTracked[joint] == GLVBOBody::bodyPartsIndexes[j]) || (this->jointsTracked[joint] == GLVBOBody::bodyPartsIndexes[j + 1]))
							{
								gl->glDrawRangeElements(GL_LINES, j, j + 1, 2, GL_UNSIGNED_INT, BUFFER_OFFSET_UINT(j));
							}
						}
					}
				}
				gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);				
			}
			gl->glBindVertexArray(0);
		}
	}

	GLVBOBody::~GLVBOBody(void)
	{
		if (this->jointsTracked)
		{
			delete[] this->jointsTracked;
			this->jointsTracked = 0;
		}
	}

};
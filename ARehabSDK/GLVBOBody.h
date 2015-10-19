#ifndef GLVBOBODY_H_
#define GLVBOBODY_H_

#include <fstream>
#include <iostream>

#include "GLVBO.h"

namespace ARehabGL
{

	class GLVBOBody : public GLVBO
	{
		public:
			GLVBOBody(QOpenGLFunctions_4_3_Core * gl);
			//GLVBOBody(unsigned int * jointsTracked, unsigned int numJoints, QOpenGLFunctions_4_3_Core * gl);
			virtual void render(void);
			virtual void updateJoints(glm::vec2 * joints, glm::vec4 * orient, unsigned int * kinectTrackingState, unsigned int numJoints);
			virtual void updateTrackedJoints(unsigned int * trackedJoints, unsigned int numTrackedJoints);
			virtual ~GLVBOBody(void);

			const static GLuint GLVBOBody::bodyPartsIndexes[];
			unsigned int * jointsTracked;
			unsigned int numJointsTracked;
			const static unsigned int maxJoints;
	};

};

#endif
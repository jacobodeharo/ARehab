#ifndef BODY_FRAME_H
#define BODY_FRAME_H

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//Standard C/C++ includes
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>

// GLM
#define GLM_SWIZZLE //This enable swizzle operators, selecting components like foo.xyz
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

namespace ARehabControl
{
	class BodyFrame
	{
	public:
		glm::vec3 * joints3D;
		glm::vec2 * joints2D;
		glm::vec4 * jointOrientations; //Quaternion coded into a glm::vec4... We can use glm::quat if is needed...
		unsigned int * kinectTrackedState;
		unsigned int numJoints;

		BodyFrame(glm::vec3 * joints3D, glm::vec2 * joints2D, glm::vec4 * jointOrientations, unsigned int nJoints, unsigned int * tracked);
		BodyFrame(const BodyFrame &original);
		BodyFrame& operator=(const BodyFrame &original);
		virtual ~BodyFrame(void);

		friend std::ostream& operator<<(std::ostream &os, const BodyFrame &body);
	};
}

#endif
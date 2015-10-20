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
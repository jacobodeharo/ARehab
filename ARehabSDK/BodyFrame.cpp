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

#include "BodyFrame.h"

namespace ARehabControl
{
	BodyFrame::BodyFrame(glm::vec3 * joints3D, glm::vec2 * joints2D, glm::vec4 * jointOrientations, unsigned int nJoints, unsigned int * kinectTrackedState)
		:joints3D(joints3D), joints2D(joints2D), jointOrientations(jointOrientations), kinectTrackedState(kinectTrackedState), numJoints(nJoints)
	{
	}

	BodyFrame::BodyFrame(const BodyFrame &original)
		: joints2D(0), joints3D(0), kinectTrackedState(0), jointOrientations(0), numJoints(original.numJoints)
	{
		this->joints2D = new glm::vec2[numJoints];
		this->joints3D = new glm::vec3[numJoints];
		this->jointOrientations = new glm::vec4[numJoints];
		this->kinectTrackedState = new unsigned int[numJoints];

		memcpy(this->joints3D, original.joints3D, numJoints * sizeof(glm::vec3));
		memcpy(this->joints2D, original.joints2D, numJoints * sizeof(glm::vec2));
		memcpy(this->jointOrientations, original.jointOrientations, numJoints * sizeof(glm::vec4));
		memcpy(this->kinectTrackedState, original.kinectTrackedState, numJoints * sizeof(unsigned int));
	}

	BodyFrame& BodyFrame::operator=(const BodyFrame &original)
	{
		if (this != &original)
		{
			numJoints = original.numJoints;
			joints2D = new glm::vec2[numJoints];
			joints3D = new glm::vec3[numJoints];
			jointOrientations = new glm::vec4[numJoints];
			kinectTrackedState = new unsigned int[numJoints];

			memcpy(joints3D, original.joints3D, numJoints * sizeof(glm::vec3));
			memcpy(joints2D, original.joints2D, numJoints * sizeof(glm::vec2));
			memcpy(jointOrientations, original.jointOrientations, numJoints * sizeof(glm::vec4));
			memcpy(kinectTrackedState, original.kinectTrackedState, numJoints * sizeof(unsigned int));
		}
		return *this;
	}

	BodyFrame::~BodyFrame(void)
	{
		if (joints2D)
		{
			delete[] joints2D;
			joints2D = 0;
		}
		if (joints3D)
		{
			delete[] joints3D;
			joints3D = 0;
		}
		if (jointOrientations)
		{
			delete[] jointOrientations;
			jointOrientations = 0;
		}
		if (kinectTrackedState)
		{
			delete[] kinectTrackedState;
			kinectTrackedState = 0;
		}
	}
}
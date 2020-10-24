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

#include "DoubleExponentialFilter.h"

namespace ARehabControl
{
	/* COMMON VALUES:
	{ smoothing, correction, prediction, jitter radius, deviation radius }
	*/
	const float DoubleExponentialFilter::FILTER_LOW[5] = { 0.5f, 0.5f, 0.5f, 0.05f, 0.04f };
	const float DoubleExponentialFilter::FILTER_MEDIUM[5] = { 0.5f, 0.1f, 0.5f, 0.1f, 0.1f };
	const float DoubleExponentialFilter::FILTER_HIGH[5] = { 0.7f, 0.3f, 1.0f, 1.0f, 1.0f };

	DoubleExponentialFilter::DoubleExponentialFilter(unsigned int filter)
	{
		const float * ptr = 0;
		switch (filter)
		{
		case LOW:
			ptr = &(FILTER_LOW[0]);
			break;

		case MEDIUM:
			ptr = &(FILTER_MEDIUM[0]);
			break;

		case HIGH:
			ptr = &(FILTER_HIGH[0]);
			break;
		}
		if (ptr != 0)
		{
			Init(ptr[0], ptr[1], ptr[2], ptr[3], ptr[4]);
		}
	}

	DoubleExponentialFilter::~DoubleExponentialFilter(void)
	{
	}

	//--------------------------------------------------------------------------------------
	// if joint is 0 it is not valid.
	//--------------------------------------------------------------------------------------
	inline bool JointPositionIsValid(glm::vec3 vJointPosition)
	{
		return (vJointPosition.x != 0 || vJointPosition.y != 0 || vJointPosition.z != 0);
	}

	inline bool JointOrientationIsValid(glm::vec4 vJointOrientation)
	{
		return (vJointOrientation.x != 0 || vJointOrientation.y != 0 || vJointOrientation.z != 0 || vJointOrientation.w != 0);
	}

	void DoubleExponentialFilter::Init(float fSmoothing = 0.25f, float fCorrection = 0.25f, float fPrediction = 0.25f, float fJitterRadius = 0.03f, float fMaxDeviationRadius = 0.05f)
	{
		assert(filteredPosJoints);
		assert(jointPosHistory);

		m_fMaxDeviationRadius = fMaxDeviationRadius; // Size of the max prediction radius Can snap back to noisy data when too high
		m_fSmoothing = fSmoothing;                   // How much smothing will occur.  Will lag when too high
		m_fCorrection = fCorrection;                 // How much to correct back from prediction.  Can make things springy
		m_fPrediction = fPrediction;                 // Amount of prediction into the future to use. Can over shoot when too high
		m_fJitterRadius = fJitterRadius;             // Size of the radius where jitter is removed. Can do too much smoothing when too high

		memset(filteredPosJoints, 0, sizeof(glm::vec4) * JointType_Count);
		memset(jointPosHistory, 0, sizeof(DoubleExponentialFilterData) * JointType_Count);
	}

	//--------------------------------------------------------------------------------------
	// Implementation of a Holt Double Exponential Smoothing filter. The double exponential
	// smooths the curve and predicts.  There is also noise jitter removal. And maximum
	// prediction bounds.  The paramaters are commented in the init function.
	//--------------------------------------------------------------------------------------
	void DoubleExponentialFilter::Update(Joint joints[])
	{
		// Check for divide by zero. Use an epsilon of a 10th of a millimeter
		m_fJitterRadius = (glm::max)(0.0001f, m_fJitterRadius); //(glm::max) with parentheses around for preventing macros from <Windows.h>

		TRANSFORM_SMOOTH_PARAMETERS SmoothingParams;
		for (int i = 0; i < JointType_Count; i++)
		{
			SmoothingParams.fSmoothing = m_fSmoothing;
			SmoothingParams.fCorrection = m_fCorrection;
			SmoothingParams.fPrediction = m_fPrediction;
			SmoothingParams.fJitterRadius = m_fJitterRadius;
			SmoothingParams.fMaxDeviationRadius = m_fMaxDeviationRadius;

			// If inferred, we smooth a bit more by using a bigger jitter radius
			Joint joint = joints[i];
			if (joint.TrackingState == TrackingState::TrackingState_Inferred)
			{
				SmoothingParams.fJitterRadius *= 2.0f;
				SmoothingParams.fMaxDeviationRadius *= 2.0f;
			}
			Update(joints, i, SmoothingParams);
		}
	}

	void DoubleExponentialFilter::Update(Joint joints[], UINT JointID, TRANSFORM_SMOOTH_PARAMETERS smoothingParams)
	{
		//// JOINT POSITIONS
		const Joint joint = joints[JointID];

		glm::vec4 jointPrevRawPosition = jointPosHistory[JointID].m_vRawPosition;
		glm::vec4 jointPrevFilteredPosition = jointPosHistory[JointID].m_vFilteredPosition;
		glm::vec4 jointPrevTrend = jointPosHistory[JointID].m_vTrend;
		glm::vec4 jointRawPosition = glm::vec4(joint.Position.X, joint.Position.Y, joint.Position.Z, 0.0f);
		glm::vec4 jointFilteredPosition;
		glm::vec4 jointPredictedPosition;
		glm::vec4 jointDiff;
		glm::vec4 jointTrend;
		glm::vec4 jointLength;
		float jointfDiff;
		bool jointIsValid = JointPositionIsValid(jointRawPosition.xyz);

		// If joint is invalid, reset the filter
		if (!jointIsValid)
		{
			jointPosHistory[JointID].m_dwFrameCount = 0;
		}

		// Initial start values
		if (jointPosHistory[JointID].m_dwFrameCount == 0)
		{
			jointFilteredPosition = jointRawPosition;
			jointTrend = glm::vec4(0);
			jointPosHistory[JointID].m_dwFrameCount++;
		}
		else if (jointPosHistory[JointID].m_dwFrameCount == 1)
		{
			jointFilteredPosition = (jointRawPosition + jointPrevRawPosition) * 0.5f;			
			jointDiff = jointFilteredPosition - jointPrevFilteredPosition;
			jointTrend = (jointDiff * smoothingParams.fCorrection) + (jointPrevTrend * 1.0f - smoothingParams.fCorrection);			
			jointPosHistory[JointID].m_dwFrameCount++;
		}
		else
		{
			// First apply jitter filter
			jointDiff = jointRawPosition - jointPrevFilteredPosition;
			jointLength = glm::vec4(glm::length(jointDiff));
			jointfDiff = glm::abs(jointLength.x);
			if (jointfDiff <= smoothingParams.fJitterRadius)
			{
				jointFilteredPosition = (jointRawPosition * (jointfDiff / smoothingParams.fJitterRadius)) + jointPrevFilteredPosition * (1.0f - jointfDiff / smoothingParams.fJitterRadius);
			}
			else
			{
				jointFilteredPosition = jointRawPosition;
			}
			// Now the double exponential smoothing filter
			jointFilteredPosition = (jointFilteredPosition * (1.0f - smoothingParams.fSmoothing)) + ((jointPrevFilteredPosition + jointPrevTrend) * smoothingParams.fSmoothing);
			jointDiff = jointFilteredPosition - jointPrevFilteredPosition;
			jointTrend = (jointDiff * smoothingParams.fCorrection) + (jointPrevTrend * (1.0f - smoothingParams.fCorrection));
		}

		// Predict into the future to reduce latency
		jointPredictedPosition = (jointFilteredPosition + (jointTrend * smoothingParams.fPrediction));

		// Check that we are not too far away from raw data
		jointDiff = jointPredictedPosition - jointRawPosition;
		jointLength = glm::vec4(glm::length(jointDiff));
		jointfDiff = glm::abs(jointLength.x);
		if (jointfDiff > smoothingParams.fMaxDeviationRadius)
		{
			jointPredictedPosition = (jointPredictedPosition * (smoothingParams.fMaxDeviationRadius / jointfDiff)) + (jointRawPosition * (1.0f - smoothingParams.fMaxDeviationRadius / jointfDiff));
		}

		// Save the data from this frame
		jointPosHistory[JointID].m_vRawPosition = jointRawPosition;
		jointPosHistory[JointID].m_vFilteredPosition = jointFilteredPosition;
		jointPosHistory[JointID].m_vTrend = jointTrend;

		// Output the data
		filteredPosJoints[JointID] = jointPredictedPosition;
		filteredPosJoints[JointID].w = 1.0f;
	}

}

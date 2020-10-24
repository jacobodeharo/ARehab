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

#ifndef DOUBLE_EXPONENTIAL_FILTER_H
#define DOUBLE_EXPONENTIAL_FILTER_H


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// GLM
#define GLM_FORCE_SWIZZLE //This enable swizzle operators, selecting components like foo.xyz
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>

#include <Kinect.h>

namespace ARehabControl
{
	typedef struct _TRANSFORM_SMOOTH_PARAMETERS
	{
		float   fSmoothing;             // [0..1], lower values closer to raw data
		float   fCorrection;            // [0..1], lower values slower to correct towards the raw data
		float   fPrediction;            // [0..n], the number of frames to predict into the future
		float   fJitterRadius;          // The radius in meters for jitter reduction
		float   fMaxDeviationRadius;    // The maximum radius in meters that filtered positions are allowed to deviate from raw data
	} TRANSFORM_SMOOTH_PARAMETERS;

	// Holt Double Exponential Smoothing filter
	struct DoubleExponentialFilterData
	{
		glm::vec4 m_vRawPosition;
		glm::vec4 m_vFilteredPosition;
		glm::vec4 m_vTrend;
		long long unsigned int m_dwFrameCount;
	};

	class DoubleExponentialFilter
	{
		public:
			/* COMMON VALUES: (See definitions in the .cpp file)
			{ smoothing, correction, prediction, jitter radius, deviation radius }
			*/
			const static float FILTER_LOW[5];
			const static float FILTER_MEDIUM[5];
			const static float FILTER_HIGH[5];
			enum { LOW, MEDIUM, HIGH } FILTER_AMOUNT;

			DoubleExponentialFilter(unsigned int filter);
			virtual ~DoubleExponentialFilter(void);

			void Init(float fSmoothing, float fCorrection, float fPrediction, float fJitterRadius, float fMaxDeviationRadius);				
			
			//void Update(Joint joints[], JointOrientation orientations[]);			
			void Update(Joint joints[]);

			inline glm::vec4 GetJoint(unsigned int jointID)
			{
				return filteredPosJoints[jointID];
			}

			//inline glm::vec4 GetOrientation(unsigned int jointID)
			//{
			//	return filteredOrientations[jointID];
			//}

		private:
			glm::vec4 filteredPosJoints[25]; //Joint positions filtered
			DoubleExponentialFilterData jointPosHistory[25]; // Joint position history

			//glm::vec4 filteredOrientations[25]; // Joint orientations filteres
			//DoubleExponentialFilterData jointOrientationHistory[25]; // Joint orientations history

			float m_fSmoothing;
			float m_fCorrection;
			float m_fPrediction;
			float m_fJitterRadius;
			float m_fMaxDeviationRadius;

			//void Update(Joint joints[], JointOrientation orientations[], unsigned int JointID, TRANSFORM_SMOOTH_PARAMETERS smoothingParams);
			void Update(Joint joints[], unsigned int JointID, TRANSFORM_SMOOTH_PARAMETERS smoothingParams);
	};
};

#endif
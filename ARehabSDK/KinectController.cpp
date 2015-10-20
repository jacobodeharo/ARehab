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

#include "KinectController.h"

namespace ARehabControl
{
	KinectController::KinectController(void) :
		kinectSensor(NULL),		
		coordinateMapper(NULL),		
		multisourceFrameReader(NULL),
		jointFilterTracked(0),
		keyFrameCount(0)
	{
		jointFilterTracked = new DoubleExponentialFilter(DoubleExponentialFilter::LOW);
	}

	KinectController::KinectController(const KinectController& original) :
		kinectSensor(NULL),		
		coordinateMapper(NULL),		
		multisourceFrameReader(NULL),
		jointFilterTracked(0),
		keyFrameCount(0)
	{
		jointFilterTracked = new DoubleExponentialFilter(DoubleExponentialFilter::LOW);
	}

	KinectController& KinectController::operator=(const KinectController& c)
	{
		jointFilterTracked = new DoubleExponentialFilter(DoubleExponentialFilter::LOW);
		return *this;
	}

	KinectController::~KinectController(void)
	{
		Stop();

		if (jointFilterTracked) {
			delete jointFilterTracked;
			jointFilterTracked = NULL;
		}
	}

	void KinectController::Initialize(void)
	{
		HRESULT hr = GetDefaultKinectSensor(&kinectSensor);
		if (SUCCEEDED(hr) && kinectSensor)
		{
			// Initialize the Kinect and get the MultiSourceFrameReader
			hr = kinectSensor->Open();
			if (SUCCEEDED(hr))
			{
				//Open MultiSourceFrame reader
				hr = kinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Body | FrameSourceTypes::FrameSourceTypes_Color, &multisourceFrameReader);
				if (!SUCCEEDED(hr))
					std::cerr << "Error opening multiSourceFrameReader" << std::endl;

				//Get the coordinate mapper
				hr = kinectSensor->get_CoordinateMapper(&coordinateMapper);
				if (!SUCCEEDED(hr))
					std::cerr << "Error obtaining coordinate mapper" << std::endl;
			}
		}
		if (!kinectSensor || FAILED(hr))
		{
			std::cerr << "No ready Kinect found!" << std::endl;
			return;
		}

		//Instantiate a new Filter Double Exponential
		jointFilterTracked = new DoubleExponentialFilter(DoubleExponentialFilter::LOW);

		//Reset the keyframe count
		keyFrameCount = 0;
	}

	void KinectController::Stop(void)
	{		
		BOOLEAN opened = false;
		// done with coordinate mapper
		SafeRelease(coordinateMapper);

		//done with color frame reader
		SafeRelease(multisourceFrameReader);

		if (kinectSensor) {
			kinectSensor->get_IsOpen(&opened);
			if (opened>0)
			{
				kinectSensor->Close();
			}
		}
		SafeRelease(kinectSensor);

		keyFrameCount = 0;
	}

	ARehabFrame * KinectController::GetNextARehabFrame(void)
	{
		HRESULT hr = 1;
		//unsigned long long int timeElapsed = ARehabTools::Measures::milliseconds_now();
		ARehabFrame * frame = NULL;
		static const unsigned int nBytes = 1920 * 1080 * 2;

		/* ColorFrame variables */
		int nWidth = 0, nHeight = 0;
		IColorFrame * pColorFrame = NULL;		
		ColorFrame * colorFrame = NULL;
		
		/* BodyFrame variables */
		IBodyFrameReference * bodyFrameRef = NULL;
		IBodyFrame * pBodyFrame = NULL;
		IBody * pBody = NULL;
		IBody * ppBodies[BODY_COUNT] = { 0 };
		Joint joints[JointType_Count];
		JointOrientation orientations[JointType_Count];		
		BodyFrame * bodyFrame = NULL;

		if (!multisourceFrameReader)
			return NULL;

		//Get the latest MultiSourceFrame
		IMultiSourceFrame * multiSourceFrame = NULL;
		hr = multisourceFrameReader->AcquireLatestFrame(&multiSourceFrame);
		if (SUCCEEDED(hr))
		{
			//Get the ColorFrame
			IColorFrameReference * colorFrameRef = NULL;
			hr = multiSourceFrame->get_ColorFrameReference(&colorFrameRef);
			if (SUCCEEDED(hr))
			{
				hr = colorFrameRef->AcquireFrame(&pColorFrame);				
				if (SUCCEEDED(hr))
				{				
					/* RAW YUY2 color frame
					4 Bytes x 2 Pixels: Internal format -> [[Y0 U0] [Y1 V0]], [[Y2 U1] [Y3 V1]]..., sizes ->[[8bits 8bits][8bits 8bits]], [[8bits 8bits][8bits 8bits]]
					*/					
					colorFrame = new ColorFrame(new BYTE[nBytes], 1920, 1080);
					hr = pColorFrame->CopyRawFrameDataToArray(nBytes, colorFrame->colorBufferYUY2);
					SafeRelease(pColorFrame);
				}
			}

			//Get the BodyFrame		
			hr = multiSourceFrame->get_BodyFrameReference(&bodyFrameRef);
			SafeRelease(multiSourceFrame);
			if (SUCCEEDED(hr))
			{				
				hr = bodyFrameRef->AcquireFrame(&pBodyFrame);				
				if (SUCCEEDED(hr))
				{
					//BodyFrame					
					hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
					SafeRelease(pBodyFrame);
					if (SUCCEEDED(hr) && _countof(ppBodies))
					{
						for (int i = 0; i < BODY_COUNT; ++i)
						{
							SafeRelease(pBody);
							pBody = ppBodies[i];
							if (pBody)
							{
								BOOLEAN bTracked = false;
								pBody->get_IsTracked(&bTracked);
								if (bTracked)
								{
									bodyFrame = new BodyFrame(
										new glm::vec3[JointType_Count],
										new glm::vec2[JointType_Count],
										new glm::vec4[JointType_Count],
										JointType_Count,
										new unsigned int[JointType_Count]
									);

									pBody->GetJoints(_countof(joints), joints);
									hr = pBody->GetJointOrientations(_countof(joints), orientations); //Get the joint orientations										
									//TODO: UPDATE jointOrientations!!
									//jointFilterTracked->Update(joints, orientations); //Smooth filter... Slerp...
									jointFilterTracked->Update(joints);

									ColorSpacePoint colorPoint = { 0 };
									for (unsigned int i = 0; i < JointType_Count; ++i)
									{
										if (joints[i].TrackingState == TrackingState_NotTracked) continue;
										
										glm::vec4 jointglm = jointFilterTracked->GetJoint(i);
										CameraSpacePoint jointpos = { jointglm.x, jointglm.y, jointglm.z };
										hr = coordinateMapper->MapCameraPointToColorSpace(jointpos, &colorPoint);
										bodyFrame->kinectTrackedState[i] = static_cast<unsigned int>(joints[i].TrackingState);
										bodyFrame->joints3D[i] = jointglm.xyz;
										bodyFrame->joints2D[i].x = colorPoint.X;
										bodyFrame->joints2D[i].y = colorPoint.Y;
										bodyFrame->jointOrientations[i] = glm::vec4( orientations[i].Orientation.w,	orientations[i].Orientation.x, orientations[i].Orientation.y, orientations[i].Orientation.z	);
									} //For all joints
									//bodyFrame = new BodyFrame(points3D, points2D, jointOrientations, JointType_Count, tracked);
								} else continue; //else, continue checking the next body														
							}
						}//For all bodies
					}
					//SafeRelease(pBodyFrame);
				}
			}
//			SafeRelease(multiSourceFrame);
		}
		if (colorFrame)
		{
			frame = new ARehabFrame(keyFrameCount, ARehabTools::Measures::milliseconds_now(), colorFrame, bodyFrame);
			keyFrameCount++;
		}
		else
		{
			return NULL;
		}		
		return frame;
	}
};
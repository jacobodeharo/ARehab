#ifndef GLAREHABCONTROL_H_
#define GLAREHABCONTROL_H_

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <stdio.h>

//Standard C/C++ includes
#include <iostream>
#include <string>
#include <fstream>

#include "IARehabFrameReader.h"
#include "DoubleExponentialFilter.h"
#include "Toolkit.h"

namespace ARehabControl
{
	// Safe release for interfaces
	template<class Interface>
	inline void SafeRelease(Interface *& pInterfaceToRelease)
	{
		if (pInterfaceToRelease != NULL)
		{
			pInterfaceToRelease->Release();
			pInterfaceToRelease = NULL;
		}
	}

	class KinectController : public IARehabFrameReader
	{
		public:
			KinectController(void);
			virtual ~KinectController(void);
			void Initialize(void);
			void Stop(void);

			ARehabFrame * GetNextARehabFrame(void);

			IKinectSensor * kinectSensor;
			ICoordinateMapper *	coordinateMapper;
			IMultiSourceFrameReader * multisourceFrameReader; //Better for sync of multiple framereaders

		private:
			DoubleExponentialFilter * jointFilterTracked;
			unsigned long long int keyFrameCount; //Keyframe count

			KinectController(const KinectController&);
			KinectController& operator=(const KinectController&);
	};

};

#endif
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
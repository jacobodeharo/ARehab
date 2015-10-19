#ifndef COLOR_FRAME_H
#define COLOR_FRAME_H

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//Standard C/C++ includes
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>

#include <Kinect.h>

namespace ARehabControl {

	class ColorFrame
	{
		public:
			RGBQUAD * colorBufferRGBX;
			BYTE * colorBufferYUY2;
			unsigned int colorWidth;
			unsigned int colorHeight;
			//INT64 time; //Timestamp of the frame adquisition

			explicit ColorFrame(void);
			explicit ColorFrame(const ColorFrame&);
			explicit ColorFrame(BYTE * bufferYUY2, unsigned int width, unsigned int height);
			explicit ColorFrame(RGBQUAD * colorBufferRGBX, unsigned int width, unsigned int height, INT64 time);
			ColorFrame& operator=(const ColorFrame &assigned);
			void freeMem(void);
			~ColorFrame(void);
	};
}

#endif

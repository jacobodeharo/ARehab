#pragma once

#include "ColorFrame.h"
#include "BodyFrame.h"

namespace ARehabControl
{
	class ARehabFrame
	{
	public:
		const static unsigned long long int numBytesFrame = 2 * sizeof(unsigned long long int) + JointType_Count * (sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(unsigned int)) + 1920 * 1080 * 2 * sizeof(unsigned char);

		ColorFrame * color;
		BodyFrame * body;
		unsigned long long int keyFrame;
		unsigned long long int timespan;
		unsigned long long int refCount;

		ARehabFrame(void);
		ARehabFrame(const unsigned long long int &keyframe, const unsigned long long int timespan, ColorFrame * color, BodyFrame * body);
		ARehabFrame& ARehabFrame::operator=(const ARehabFrame &assigned);
		void freeMem(void);
		~ARehabFrame(void);
	};
}

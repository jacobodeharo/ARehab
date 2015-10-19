#include "ARehabFrame.h"

namespace ARehabControl
{
	ARehabFrame::ARehabFrame(void) :
		color(0),
		body(0),
		refCount(1),
		keyFrame(0),
		timespan(0)
	{
	}

	ARehabFrame::ARehabFrame(const unsigned long long int &keyframe, const unsigned long long int timespan, ColorFrame * color, BodyFrame * body) :
		color(color),
		body(body),
		refCount(1),
		keyFrame(keyframe),
		timespan(timespan)
	{
	}

	ARehabFrame& ARehabFrame::operator=(const ARehabFrame &assigned)
	{
		if (this != &assigned)
		{
			keyFrame = assigned.keyFrame;
			refCount = assigned.refCount;
			timespan = assigned.timespan;
			color = assigned.color;
			body = assigned.body;
		}
		return *this;
	}

	void ARehabFrame::freeMem(void)
	{
		refCount--;
		if (!refCount)
		{
			if (color)
			{
				delete color;
				color = 0;
			}
			if (body)
			{
				delete body;
				body = 0;
			}
		}
	}

	ARehabFrame::~ARehabFrame(void)
	{
	}
}
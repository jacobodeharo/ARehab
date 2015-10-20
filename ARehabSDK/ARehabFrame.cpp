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
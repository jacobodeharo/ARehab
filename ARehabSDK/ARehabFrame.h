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

#ifndef AREHAB_FRAME_H
#define AREHAB_FRAME_H

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

#endif
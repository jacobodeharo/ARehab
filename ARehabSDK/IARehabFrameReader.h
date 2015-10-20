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

#ifndef IAREHABFRAMEREADER_H_
#define IAREHABFRAMEREADER_H_

#include "ARehabFrame.h"

namespace ARehabControl
{
	//Interface for objects that want to retrieve ARehabFrame objects
	class IARehabFrameReader
	{
		public:
			virtual ARehabFrame * GetNextARehabFrame(void) = 0;
	};

}

#endif
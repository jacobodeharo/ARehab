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

#ifndef GLVBOVIDEO_H_
#define GLVBOVIDEO_H_

#include "GLVBO.h"

namespace ARehabGL
{
	class GLVBOVideo : public GLVBO
	{
		public:
			GLVBOVideo(QOpenGLFunctions_4_3_Core * gl);
			virtual void render(void);
	};
};

#endif
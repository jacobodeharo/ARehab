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

#include "GLFunctions.h"

namespace ARehabGL
{
	QOpenGLFunctions_4_3_Core * GLFunctions::setupOpenGLFunctionsPtr(QOpenGLContext * context)
	{
		QOpenGLFunctions_4_3_Core * fun = 0;
		if (context != 0) {
			fun = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
			if (!fun) {
				qWarning() << "Could not obtain required OpenGL context version";
				exit(1);
			}
			if (fun->initializeOpenGLFunctions())
			{
				qDebug() << "OpenGL functions initialized." << endl;
			}
			else
			{
				qWarning() << "Could not initialize the OpenGL Functions." << endl;
			}
		}
		return fun;
	}
};
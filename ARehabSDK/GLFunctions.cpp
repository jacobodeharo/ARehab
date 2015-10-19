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
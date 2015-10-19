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
#ifndef IAREHABFRAMEWRITER_H_
#define IAREHABFRAMEWRITER_H_

#include "ARehabFrame.h"

namespace ARehabControl
{
	//Interface for objects that want to store ARehabFrame objects
	class IARehabFrameWriter
	{
	public:
		virtual void StoreARehabFrame(ARehabFrame * frame) = 0;
	};
}

#endif
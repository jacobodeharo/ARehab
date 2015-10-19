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
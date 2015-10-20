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

#ifndef AREHABFILEREADERCONTROLLER_H_
#define AREHABFILEREADERCONTROLLER_H_

#include "IARehabFrameReader.h"

namespace ARehabControl
{
	class ARehabFileReaderController : public IARehabFrameReader
	{
		public:
			enum { BACKWARD = 0, FORWARD } PlayDirection;
			long long int currentFrame;
			unsigned long long int minKeyFrame;
			unsigned long long int maxKeyFrame;
			unsigned long long int numBytes;
			unsigned long long int numFramesImput;
			unsigned int playDirection;
			unsigned long long int firstTimeSpan;
			unsigned long long int previousTimeSpan;
			unsigned long long int currentTimeSpan;

			ARehabFileReaderController(void);
			virtual void ResetCurrentFrame(unsigned long long int minIntervalframe = 0);
			virtual void OpenImputFile(std::string filename = "default.arehab");
			virtual ARehabFrame * GetNextARehabFrame(void);
			virtual ARehabFrame * GetNextIntervalARehabFrame(unsigned long long int minIntervalFrame, unsigned long long int maxIntervalFrame);
			virtual ARehabFrame * GetiARehabFrame(unsigned long long int framei, bool updateCurrent = false);
			virtual bool CutIntervalToFile(unsigned long long int ini, unsigned long long int end, std::string outputFileURL = "");
			virtual void CloseImputFile(void);
			virtual ~ARehabFileReaderController(void);

	protected:
		std::string imputFileName;
		std::ifstream * imputFile;

		ARehabFileReaderController(const ARehabFileReaderController &){}
		ARehabFileReaderController& operator=(const ARehabFileReaderController &){}
	};

}

#endif
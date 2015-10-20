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

#ifndef AREHABFILEWRITERCONTROLLER_H_
#define AREHABFILEWRITERCONTROLLER_H_

#include "IARehabFrameWriter.h"
#include "Toolkit.h"

namespace ARehabControl
{
	class ARehabFileWriterController : public IARehabFrameWriter
	{
		public:
			ARehabFileWriterController(void);
			virtual void OpenOutputFile(std::string filename = "default.arehab.data");
			virtual void StoreARehabFrame(ARehabFrame * frame);
			virtual void CloseOutputFile(void);
			virtual ~ARehabFileWriterController(void);

		protected:
			std::string outputFileName;
			std::ofstream * outputFile;

			ARehabFileWriterController(const ARehabFileWriterController &){}
			ARehabFileWriterController& operator=(const ARehabFileWriterController &){}
	};
};

#endif
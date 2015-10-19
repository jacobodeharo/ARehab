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
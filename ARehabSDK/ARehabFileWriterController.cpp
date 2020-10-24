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

#include "ARehabFileWriterController.h"


namespace ARehabControl
{

	ARehabFileWriterController::ARehabFileWriterController(void) : outputFile(new std::ofstream)
	{
	}

	void ARehabFileWriterController::OpenOutputFile(std::string filename)
	{
		this->outputFileName = filename;
		if (outputFile)
		{
			outputFile->open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
			if (!(*outputFile))
			{
				std::cerr << "ARehabFileWriterController >> Error opening file!" << std::endl;
			}
		}
	}

	void ARehabFileWriterController::StoreARehabFrame(ARehabFrame * frame)
	{
		if (!frame || !frame->body || !frame->color || !(*outputFile))
		{
			std::cerr << "ARehabFileWriterController::StoreARehabFrame >> Error writing uncompleted frame!" << std::endl;
			return;
		}
		outputFile->write(reinterpret_cast<char const*>(&(frame->keyFrame)), sizeof(unsigned long long int)); //Keyframe							
		outputFile->write(reinterpret_cast<char const*>(frame->body->joints3D), JointType_Count * sizeof(glm::vec3)); //3D Joint positions
		outputFile->write(reinterpret_cast<char const*>(frame->body->joints2D), JointType_Count * sizeof(glm::vec2)); //2D Joint positions	
		outputFile->write(reinterpret_cast<char const*>(frame->body->jointOrientations), JointType_Count * sizeof(glm::vec4)); //Joint orientations
		outputFile->write(reinterpret_cast<char const*>(frame->body->kinectTrackedState), JointType_Count * sizeof(unsigned int)); //Joint state (tracked...)
		outputFile->write(reinterpret_cast<char const*>(frame->color->colorBufferYUY2), 1920 * 1080 * 2); //YUY2 color texture (2 Bytes per pixel)
		outputFile->write(reinterpret_cast<char const*>(&(frame->timespan)), sizeof(unsigned long long int)); //Timespan				
	}

	void ARehabFileWriterController::CloseOutputFile(void)
	{
		if (outputFile)
		{
			if (outputFile->is_open())
			{
				outputFile->close();
			}
		}
	}

	ARehabFileWriterController::~ARehabFileWriterController(void)
	{
		if (outputFile)
		{
			if (outputFile->is_open())
			{
				outputFile->close();
			}
			delete outputFile;
			outputFile = NULL;
		}
	}

};

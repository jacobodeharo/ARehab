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

#include "ARehabFileReaderController.h"

namespace ARehabControl
{
	ARehabFileReaderController::ARehabFileReaderController(void) :
		imputFile(new std::ifstream),
		minKeyFrame(0),
		maxKeyFrame(0),
		numBytes(0),
		numFramesImput(0),
		currentFrame(0),
		playDirection(FORWARD),
		firstTimeSpan(0),
		previousTimeSpan(MAXUINT64),
		currentTimeSpan(0)
	{
	}

	void ARehabFileReaderController::ResetCurrentFrame(unsigned long long int minIntervalframe)
	{
		if (this->imputFile->is_open())
		{
			if (minIntervalframe >= minKeyFrame && minIntervalframe <= maxKeyFrame)
				currentFrame = minIntervalframe - minKeyFrame;
			else
				currentFrame = 0;

			playDirection = FORWARD;
			firstTimeSpan = 0;
			previousTimeSpan = MAXUINT64;
			currentTimeSpan = 0;

			imputFile->seekg((currentFrame * ARehabFrame::numBytesFrame), std::ios::beg);
		}
		else{
			std::cerr << "ARehabFileReaderController::ResetCurrentFrame >> this->imputFile is not opened! OR minIntervalFrame is out of the interval [minKeyFrame, maxKeyFrame]!" << std::endl;
			return;
		}
	}

	void ARehabFileReaderController::OpenImputFile(std::string filename)
	{
		this->imputFileName = filename;
		if (imputFile && imputFile->is_open())
			imputFile->close();

		if (imputFile)
		{
			imputFile->open(filename, std::ios::binary | std::ios::ate);
			if (!(*imputFile))
			{
				std::cerr << "ARehabFileReaderController >> Error opening file for counting the number of frames!" << std::endl;
				return;
			}
			//Count number of frames inside the file
			numBytes = static_cast<unsigned long long int>(imputFile->tellg());
			this->numFramesImput = numBytes / ARehabFrame::numBytesFrame;
			imputFile->close();
			imputFile->open(filename, std::ios::in | std::ios::binary | std::ios::beg);
			imputFile->read(reinterpret_cast<char*>(&(this->minKeyFrame)), sizeof(unsigned long long int));
			this->maxKeyFrame = this->minKeyFrame + numFramesImput - 1;
			imputFile->seekg(ARehabFrame::numBytesFrame - sizeof(unsigned long long int), std::ios::beg);
			imputFile->read(reinterpret_cast<char*>(&(this->firstTimeSpan)), sizeof(unsigned long long int));
			imputFile->seekg(0, std::ios::beg);
			currentFrame = 0;
			currentTimeSpan = 0;
			previousTimeSpan = MAXUINT64;
		}
	}

	ARehabFrame * ARehabFileReaderController::GetNextARehabFrame(void)
	{
		ARehabFrame * frame = NULL;
		if (imputFile && imputFile->is_open())
		{
			frame = new ARehabFrame(
					0,
					0,
					new ColorFrame(
						new unsigned char[1920 * 1080 * 2],
						1920,
						1080
					),
					new BodyFrame(
						new glm::vec3[JointType_Count],
						new glm::vec2[JointType_Count],
						new glm::vec4[JointType_Count],
						JointType_Count,
						new unsigned int[JointType_Count]
					)
				);

			//It puts the file reader pointer at the correct position
			if (currentFrame < 0)
			{
				this->playDirection = FORWARD;
				currentFrame = 1;
			}else if (currentFrame > (maxKeyFrame - minKeyFrame - 1))
			{
				this->playDirection = BACKWARD;
				currentFrame = maxKeyFrame-minKeyFrame-2;
			}			
			imputFile->seekg((currentFrame * ARehabFrame::numBytesFrame), std::ios::beg);

			previousTimeSpan = currentTimeSpan;
			currentFrame += ((this->playDirection > 0) ? 1 : -1);					

			imputFile->read(reinterpret_cast<char*>(&(frame->keyFrame)), sizeof(unsigned long long int)); //Keyframe					
			imputFile->read(reinterpret_cast<char*>(frame->body->joints3D), JointType_Count * sizeof(glm::vec3)); //3D Joint positions
			imputFile->read(reinterpret_cast<char*>(frame->body->joints2D), JointType_Count * sizeof(glm::vec2)); //2D Joint positions
			imputFile->read(reinterpret_cast<char*>(frame->body->jointOrientations), JointType_Count * sizeof(glm::vec4)); //Joint orientations
			imputFile->read(reinterpret_cast<char*>(frame->body->kinectTrackedState), JointType_Count * sizeof(unsigned int)); //kinectTrackedState
			imputFile->read(reinterpret_cast<char*>(frame->color->colorBufferYUY2), 1920 * 1080 * 2); //YUY2 color texture					
			imputFile->read(reinterpret_cast<char*>(&(frame->timespan)), sizeof(unsigned long long int)); //Timespan					

			currentTimeSpan = frame->timespan;
			if (previousTimeSpan == 0 || (glm::max(currentTimeSpan, previousTimeSpan) - glm::min(currentTimeSpan, previousTimeSpan)) > 120)
			{
				previousTimeSpan = currentTimeSpan - 66; //66 ms per frame expected in the worst case
			}
		}
		return frame;
	}

	ARehabFrame * ARehabFileReaderController::GetNextIntervalARehabFrame(unsigned long long int minIntervalFrame, unsigned long long int maxIntervalFrame)
	{
		ARehabFrame * frame = NULL;
		if (imputFile && imputFile->is_open())
		{
			frame = new ARehabFrame(
					0,
					0,
					new ColorFrame(
						new unsigned char[1920 * 1080 * 2],
						1920,
						1080
					),
					new BodyFrame(
						new glm::vec3[JointType_Count],
						new glm::vec2[JointType_Count],
						new glm::vec4[JointType_Count],
						JointType_Count,
						new unsigned int[JointType_Count]
					)
				);

			//It puts the file reader pointer at the correct position
			long long int	minimum = minIntervalFrame - minKeyFrame,
							maximum = maxIntervalFrame - minKeyFrame;
			if (currentFrame < minimum)
			{
				this->playDirection = FORWARD;
				currentFrame = minimum + 1;
			}
			else if (currentFrame > maximum)
			{
				this->playDirection = BACKWARD;
				currentFrame = maximum - 1;
			}
			imputFile->seekg((currentFrame * ARehabFrame::numBytesFrame), std::ios::beg);

			previousTimeSpan = currentTimeSpan;
			currentFrame += ((this->playDirection > 0) ? 1 : -1);

			imputFile->read(reinterpret_cast<char*>(&(frame->keyFrame)), sizeof(unsigned long long int)); //Keyframe					
			imputFile->read(reinterpret_cast<char*>(frame->body->joints3D), JointType_Count * sizeof(glm::vec3)); //3D Joint positions
			imputFile->read(reinterpret_cast<char*>(frame->body->joints2D), JointType_Count * sizeof(glm::vec2)); //2D Joint positions
			imputFile->read(reinterpret_cast<char*>(frame->body->jointOrientations), JointType_Count * sizeof(glm::vec4)); //Joint orientations
			imputFile->read(reinterpret_cast<char*>(frame->body->kinectTrackedState), JointType_Count * sizeof(unsigned int)); //kinectTrackedState
			imputFile->read(reinterpret_cast<char*>(frame->color->colorBufferYUY2), 1920 * 1080 * 2); //YUY2 color texture
			imputFile->read(reinterpret_cast<char*>(&(frame->timespan)), sizeof(unsigned long long int)); //Timespan					

			currentTimeSpan = frame->timespan;

			unsigned long long int diferencia = glm::max(currentTimeSpan, previousTimeSpan) - glm::min(currentTimeSpan, previousTimeSpan);
			if (previousTimeSpan == 0 || diferencia > 120)
			{
				previousTimeSpan = currentTimeSpan - 66; //66 ms per frame expected in the worst case
			}
		}
		return frame;
	}

	ARehabFrame * ARehabFileReaderController::GetiARehabFrame(unsigned long long int framei, bool updateCurrent)
	{
		ARehabFrame * frame = NULL;
		if (imputFile && imputFile->is_open())
		{
			frame = new ARehabFrame(
				0,
				0,
				new ColorFrame(
				new unsigned char[1920 * 1080 * 2],
				1920,
				1080
				),
				new BodyFrame(
				new glm::vec3[JointType_Count],
				new glm::vec2[JointType_Count],
				new glm::vec4[JointType_Count],
				JointType_Count,
				new unsigned int[JointType_Count]
				)
				);

			previousTimeSpan = currentTimeSpan;

			this->imputFile->seekg(((framei - minKeyFrame) * ARehabFrame::numBytesFrame), std::ios::beg); //Move the ifstream to the i ArehabFrame position

			imputFile->read(reinterpret_cast<char*>(&(frame->keyFrame)), sizeof(unsigned long long int)); //Keyframe					
			imputFile->read(reinterpret_cast<char*>(frame->body->joints3D), JointType_Count * sizeof(glm::vec3)); //3D Joint positions
			imputFile->read(reinterpret_cast<char*>(frame->body->joints2D), JointType_Count * sizeof(glm::vec2)); //2D Joint positions
			imputFile->read(reinterpret_cast<char*>(frame->body->jointOrientations), JointType_Count * sizeof(glm::vec4)); //Joint orientations
			imputFile->read(reinterpret_cast<char*>(frame->body->kinectTrackedState), JointType_Count * sizeof(unsigned int)); //kinectTrackedState
			imputFile->read(reinterpret_cast<char*>(frame->color->colorBufferYUY2), 1920 * 1080 * 2); //YUY2 color texture
			imputFile->read(reinterpret_cast<char*>(&(frame->timespan)), sizeof(unsigned long long int)); //Timespan

			if (updateCurrent) {
				currentFrame = framei - minKeyFrame;
			}

			currentTimeSpan = frame->timespan;
			if (previousTimeSpan == 0 || (glm::max(currentTimeSpan, previousTimeSpan) - glm::min(currentTimeSpan, previousTimeSpan)) > 120)
			{
				previousTimeSpan = currentTimeSpan - 66; //66 ms per frame expected in the worst case
			}
		}
		return frame;
	}

	bool ARehabFileReaderController::CutIntervalToFile(unsigned long long int ini, unsigned long long int end, std::string outputFileURL)
	{
		bool cutOk = false;
		ARehabFrame * frame = NULL;
		std::ofstream outputFile;

		outputFile.open("./ARehabFiles/arehab.data.temp", std::ios::binary | std::ios::beg);
		if (this->imputFile->is_open() && outputFile.is_open())
		{
			if (ini < minKeyFrame || end > maxKeyFrame)
			{
				std::cerr << "Error (ARehabFileReader): The cutting interval exceeds the imput file frame count!" << std::endl;
				return false;
			}
			else if (ini > minKeyFrame || end < maxKeyFrame)
			{
				frame = new ARehabFrame(
					0,
					0,
					new ColorFrame(
					new unsigned char[1920 * 1080 * 2],
					1920,
					1080
					),
					new BodyFrame(
					new glm::vec3[JointType_Count],
					new glm::vec2[JointType_Count],
					new glm::vec4[JointType_Count],
					JointType_Count,
					new unsigned int[JointType_Count]
					)
					);

				for (unsigned long long int i = ini; i <= end; ++i)
				{
					this->imputFile->seekg(((i - minKeyFrame) * ARehabFrame::numBytesFrame), std::ios::beg); //Move the ifstream to the i ArehabFrame position
					imputFile->read(reinterpret_cast<char*>(&(frame->keyFrame)), sizeof(unsigned long long int)); //Keyframe
					outputFile.write(reinterpret_cast<char const*>(&(frame->keyFrame)), sizeof(unsigned long long int)); //Keyframe				

					imputFile->read(reinterpret_cast<char*>(frame->body->joints3D), JointType_Count * sizeof(glm::vec3)); //3D Joint positions
					outputFile.write(reinterpret_cast<char const*>(frame->body->joints3D), JointType_Count * sizeof(glm::vec3)); //3D Joint positions

					imputFile->read(reinterpret_cast<char*>(frame->body->joints2D), JointType_Count * sizeof(glm::vec2)); //2D Joint positions
					outputFile.write(reinterpret_cast<char const*>(frame->body->joints2D), JointType_Count * sizeof(glm::vec2)); //2D Joint positions	

					imputFile->read(reinterpret_cast<char*>(frame->body->jointOrientations), JointType_Count * sizeof(glm::vec4)); //Joint orientations
					outputFile.write(reinterpret_cast<char const*>(frame->body->jointOrientations), JointType_Count * sizeof(glm::vec4)); //Joint orientations

					imputFile->read(reinterpret_cast<char*>(frame->body->kinectTrackedState), JointType_Count * sizeof(unsigned int)); //Keyframe
					outputFile.write(reinterpret_cast<char const*>(frame->body->kinectTrackedState), JointType_Count * sizeof(unsigned int)); //Joint state (tracked...)

					imputFile->read(reinterpret_cast<char*>(frame->color->colorBufferYUY2), 1920 * 1080 * 2); //YUY2 color texture
					outputFile.write(reinterpret_cast<char const*>(frame->color->colorBufferYUY2), 1920 * 1080 * 2); //YUY2 color texture (2 Bytes per pixel)		

					imputFile->read(reinterpret_cast<char*>(&(frame->timespan)), sizeof(unsigned long long int)); //Timespan
					outputFile.write(reinterpret_cast<char const*>(&(frame->timespan)), sizeof(unsigned long long int)); //Timespan
				}
				frame->freeMem();
				outputFile.close();

				if (outputFileURL.empty() || outputFileURL == this->imputFileName.c_str())
				{
					imputFile->close();
					remove(this->imputFileName.c_str());
					rename("./ARehabFiles/arehab.data.temp", this->imputFileName.c_str());
					cutOk = true;
				}
				else{
					rename("./ARehabFiles/arehab.data.temp", outputFileURL.c_str());
					cutOk = true;
				}
			}
		}
		else{
			std::cerr << "CutIntervalToFile: ERROR imput or output file is not opened." << std::endl;
			return false;
		}
		return cutOk;
	}

	void ARehabFileReaderController::CloseImputFile(void)
	{
		if (imputFile)
		{
			if (imputFile->is_open())
			{
				imputFile->close();
			}
		}
	}

	ARehabFileReaderController::~ARehabFileReaderController(void)
	{
		if (imputFile)
		{
			if (imputFile->is_open())
			{
				imputFile->close();
			}
			delete imputFile;
			imputFile = NULL;
		}
	}

};
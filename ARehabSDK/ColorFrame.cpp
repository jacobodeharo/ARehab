#include "ColorFrame.h"

namespace ARehabControl
{
	ColorFrame::ColorFrame(void) :
		colorBufferRGBX(0),
		colorBufferYUY2(0),
		colorWidth(0),
		colorHeight(0)
	{
	}

	ColorFrame::ColorFrame(const ColorFrame& original) :
		colorBufferRGBX(0),
		colorBufferYUY2(0),
		colorWidth(0),
		colorHeight(0)
	{
		if (this != &original) {
			this->colorWidth = original.colorWidth;
			this->colorHeight = original.colorHeight;
			UINT64 size = colorWidth * colorHeight;
			if (original.colorBufferRGBX)
			{
				this->colorBufferRGBX = new RGBQUAD[size];
				memcpy(this->colorBufferRGBX, original.colorBufferRGBX, this->colorWidth * this->colorHeight * sizeof(RGBQUAD));
			}
			if (original.colorBufferYUY2)
			{
				this->colorBufferYUY2 = new BYTE[size * 2]; //2 BYTES per pixel => [ Byte 1: [Y0 U0] Byte 2: [Y1 V0] ]
				memcpy(this->colorBufferYUY2, original.colorBufferYUY2, this->colorWidth * this->colorHeight * 2);
			}
		}
	}

	ColorFrame::ColorFrame(BYTE * colorBufferYUY2, unsigned int width, unsigned int height) :
		colorBufferYUY2(colorBufferYUY2),
		colorBufferRGBX(0),
		colorWidth(width),
		colorHeight(height)
	{
	}

	ColorFrame& ColorFrame::operator=(const ColorFrame &assigned)
	{
		if (this != &assigned)
		{
			UINT64 size = assigned.colorWidth * assigned.colorHeight;
			if (assigned.colorBufferRGBX)
			{
				this->colorBufferRGBX = new RGBQUAD[size];
				memcpy(this->colorBufferRGBX, assigned.colorBufferRGBX, size * sizeof(RGBQUAD));
			}
			if (assigned.colorBufferYUY2)
			{
				this->colorBufferYUY2 = new BYTE[size * 2];
				memcpy(this->colorBufferYUY2, assigned.colorBufferYUY2, size * 2);
			}
		}
		return *this;
	}

	void ColorFrame::freeMem(void)
	{
		if (this->colorBufferRGBX) {
			delete[] this->colorBufferRGBX;
			this->colorBufferRGBX = 0;
		}
		if (this->colorBufferYUY2) {
			delete[] this->colorBufferYUY2;
			this->colorBufferYUY2 = 0;
		}
	}

	ColorFrame::~ColorFrame(void)
	{
		this->freeMem();
	}
}
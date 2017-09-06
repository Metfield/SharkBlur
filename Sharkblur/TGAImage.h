#pragma once

#include <vector>
#include "Pixel.h"

namespace TGA
{
	struct TGAImage
	{
		char header[18];

		int width;
		int height;
		int pixelDepth;

		ColorSpace::Type colorSpaceType;

		int bufferSize;
		unsigned char *buffer;
	};
}
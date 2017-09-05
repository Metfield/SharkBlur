#pragma once

#include <vector>

namespace TGA
{
	struct TGAImage
	{
		char header[18];

		int width;
		int height;
		int pixelDepth;

		int bufferSize;
		unsigned char *buffer;
	};
}
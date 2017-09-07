#pragma once

#include "TGAImage.h"


namespace VisualFX
{
	// Specific for this effect since it needs 
	// integer precision
	struct BlurBGRPixel
	{
		int B;
		int G;
		int R;
	};

	// Same but with alpha channel
	struct BlurBGRAPixel
	{
		int B;
		int G;
		int R;
		int A;
	};

	class Blur
	{
	public:
		// Run blur effect on reference to original TGA image
		static void Run(TGA::TGAImage &rawImage, float factor);

	private:
		static void BGRBlur();
		static void BGRABlur();
		static unsigned char *PixelAt(unsigned char *buffer, int x, int y);
	};
}
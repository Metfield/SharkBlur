#pragma once

#include "TGAImage.h"


namespace VisualFX
{
	class Blur
	{
	public:
		// Run blur effect on reference to original TGA image
		static void Run(TGA::TGAImage &rawImage, float factor);

	private:
		static void HorizontalBlur();
		static char *PixelAt(char *buffer, int x, int y);
	};
}
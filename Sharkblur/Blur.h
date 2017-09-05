#pragma once

#include "TGAImage.h"

namespace VisualFX
{
	class Blur
	{
	public:
		// Run blur effect on reference to original TGA image
		static void Run(TGA::TGAImage &rawImage);
	};
}
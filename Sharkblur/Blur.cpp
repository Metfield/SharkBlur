#include "Blur.h"
#include <iostream>
#include <bitset>

struct RGBPixel
{
	char R;
	char G;
	char B;
};

struct RGBAPixel
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};



void VisualFX::Blur::Run(TGA::TGAImage &rawImage)
{
	typedef RGBPixel Pixel;

	// Copy pointer so we don't have to access the struct multiple times
	unsigned char *rawBuffer = rawImage.buffer;

	Pixel *pixel = new Pixel();

	bool p = true;

	for (int i = 0; i < rawImage.bufferSize; i++)
	{
		rawImage.buffer[i] = (unsigned char)255;
		
	


		/*pixel = (Pixel*)(&rawImage.buffer[i]);
		pixel->B = 255;
		pixel->G = 255;
		pixel->R = 255;*/
	}

}


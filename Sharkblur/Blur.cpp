
#include <iostream>
#include "Blur.h"

namespace VisualFX
{
	// Static variables
	float m_radius;
	TGA::TGAImage *m_rawImage;
	char *m_blurBuffer;
	int m_bytesPerPixel;

	void Blur::Run(TGA::TGAImage &rawImage, float factor)
	{
		// Allocate memory for blur buffer
		m_blurBuffer = new char[rawImage.bufferSize];

		// Copy members
		m_radius = factor;
		m_rawImage = &rawImage;
		m_bytesPerPixel = rawImage.pixelDepth / 8;

		// Run horizontal pass
		HorizontalBlur();


		// Copy result to original image
		memcpy(rawImage.buffer, m_blurBuffer, rawImage.bufferSize);

		/*switch (rawImage.colorSpaceType)
		{
		case ColorSpace::ABGR:
			ColorSpace::Pixel<ColorSpace::ABGR> pixel;
			
			break;

			// Can add a bunch other types here..
			// Keeping it simple to maintain small project size

		default:
			ColorSpace::Pixel<ColorSpace::BGR> pixel;
			break;
		}*/
	}
	void Blur::HorizontalBlur()
	{
		int B, G, R;

		
		for (int y = 0; y < m_rawImage->height; y++)
		{
			for (int x = 0; x < m_rawImage->width; x++)
			{				
				ColorSpace::Pixel<ColorSpace::Type::BGR> *pixel = reinterpret_cast<ColorSpace::Pixel<ColorSpace::Type::BGR>*>(PixelAt((char*)m_blurBuffer, x, y));
				
				//Pixel *blah = reinterpret_cast<Pixel*>(PixelAt((char*)m_rawImage->buffer, x, y));
				
				

				char *origin = PixelAt((char*)m_rawImage->buffer, x, y);
				//char *p = PixelAt(m_blurBuffer, x, y);
				pixel->B = *origin;
				origin++;
				pixel->G = *origin;
				 origin++;
				 pixel->R = *origin;
				
			}
		}

	}
	
	char *Blur::PixelAt(char *buffer, int x, int y)
	{
		// Typical 2D buffer access x * y + width
		// In this case both x and y need to be multiplied by the number of bytes per pixel
		return &buffer[x * m_bytesPerPixel + y * m_rawImage->width * m_bytesPerPixel];
	}

	/*struct Pixel
	{
		char *B, *G, *R;
	};*/

	
	class Pixel
	{
	public:
		virtual Pixel &operator+(const Pixel &other) = 0;
		virtual Pixel &operator=(const Pixel &other) = 0;
	};

	class BGRPixel : Pixel
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	class ABGRPixel : Pixel
	{
		unsigned char A;
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

}
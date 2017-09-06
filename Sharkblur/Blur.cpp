
#include <iostream>
#include "Blur.h"
#include <cmath>

namespace VisualFX
{
	struct BlurBGRPixel
	{
		int B;
		int G;
		int R;
	};

	// Static variables
	float m_radius;
	TGA::TGAImage *m_rawImage;
	unsigned char *m_blurBuffer;
	int m_bytesPerPixel;

	void Blur::Run(TGA::TGAImage &rawImage, float factor)
	{
		// Allocate memory for blur buffer
		m_blurBuffer = new unsigned char[rawImage.bufferSize] { 0 };

		// Copy members
		m_radius = factor * 50;
		m_rawImage = &rawImage;
		m_bytesPerPixel = rawImage.pixelDepth / 8;

		// Run pixel type-specific blur
		switch (rawImage.colorSpaceType)
		{
			case ColorSpace::ABGR:
				ABGRBlur();
				break;

				// Can add a bunch other types here..
				// Keeping it simple to maintain small project size

			case ColorSpace::BGR:
				BGRBlur();
				break;
		}
		
		// Copy result to original image
		memcpy(rawImage.buffer, m_blurBuffer, rawImage.bufferSize);		
	}

	void Blur::BGRBlur()
	{
		unsigned char *rawColorComponent;
		BlurBGRPixel blur = { 0, 0, 0 };
				
		ColorSpace::BGRPixel *finalPixel;
		int count;

		// Horizontal pass
		for (int y = 0; y < m_rawImage->height; y++)
		{
			for (int x = 0; x < m_rawImage->width; x++)
			{
				blur = { 0, 0, 0 };
				count = 0;

				for (int horizontal = -m_radius; horizontal <= m_radius; horizontal++)
				{
					if (x + horizontal < 0)
						continue;

					if (x + horizontal > m_rawImage->width)
						break;

					count++;
					rawColorComponent = PixelAt(m_rawImage->buffer, x + horizontal, y);

					blur.B += *rawColorComponent; rawColorComponent++;
					blur.G += *rawColorComponent; rawColorComponent++;
					blur.R += *rawColorComponent;
				}

				finalPixel = reinterpret_cast<ColorSpace::BGRPixel*>(PixelAt(m_blurBuffer, x, y));
				finalPixel->B = (blur.B / count);
				finalPixel->G = (blur.G / count);
				finalPixel->R = (blur.R / count);
			}
		}

		// Vertical pass
		for (int x = 0; x < m_rawImage->width; x++)
		{
			for (int y = 0; y < m_rawImage->height; y++)
			{
				blur = { 0, 0, 0 };
				count = 0;

				for (int vertical = -m_radius; vertical <= m_radius; vertical++)
				{
					if (y + vertical < 0)
						continue;

					if (y + vertical > m_rawImage->height)
						break;

					count++;
					rawColorComponent = PixelAt(m_rawImage->buffer, x, y + vertical);

					blur.B += *rawColorComponent; rawColorComponent++;
					blur.G += *rawColorComponent; rawColorComponent++;
					blur.R += *rawColorComponent;
				}

				finalPixel = reinterpret_cast<ColorSpace::BGRPixel*>(PixelAt(m_blurBuffer, x, y));
				finalPixel->B = (blur.B / count);
				finalPixel->G = (blur.G / count);
				finalPixel->R = (blur.R / count);
			}
		}


		

	}

	void Blur::ABGRBlur()
	{
	}
	
	unsigned char *Blur::PixelAt(unsigned char *buffer, int x, int y)
	{
		// Typical 2D buffer access x * y + width
		// In this case both x and y need to be multiplied by the number of bytes per pixel
		return &buffer[x * m_bytesPerPixel + y * m_rawImage->width * m_bytesPerPixel];
	}
}
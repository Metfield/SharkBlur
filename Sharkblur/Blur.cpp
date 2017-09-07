
#include <iostream>
#include "Blur.h"
#include <cmath>
#include <exception>

namespace VisualFX
{
	// Static variables
	float m_radius;
	TGA::TGAImage *m_rawImage;
	unsigned char *m_blurBuffer;
	int m_bytesPerPixel;

	void Blur::Run(TGA::TGAImage &rawImage, float factor)
	{
		std::cout << "    Blurring image (factor " << factor << ")" << std::endl;

		// Allocate memory for blur buffer
		m_blurBuffer = new unsigned char[rawImage.bufferSize] { 0 };

		// Copy members
		m_radius = factor * 25;
		m_rawImage = &rawImage;
		m_bytesPerPixel = rawImage.pixelDepth / 8;

		try
		{
			// Run pixel type-specific blur
			switch (rawImage.colorSpaceType)
			{
			case ColorSpace::BGRA:
				BGRABlur();
				break;

				// Can add a bunch other types here..
				// Keeping it simple to maintain small project size

			case ColorSpace::BGR:
				BGRBlur();
				break;
			}
		}
		catch (std::exception e)
		{
			std::cout << "Blur::Exception: " << e.what() << std::endl;
		}
		
		// Copy result to original image
		memcpy(rawImage.buffer, m_blurBuffer, rawImage.bufferSize);		
	}

	// Three bytes-per-pixel implementation
	void Blur::BGRBlur()
	{
		// Accumulated blur per row/column cluster
		BlurBGRPixel blur = { 0, 0, 0 };

		// Holds individual color bytes from raw source
		// The BlurBGRPixel structure just refused to work here :( produced horrible artifacts
		unsigned char *rawColorComponent;
		
		// Final color per pixel
		ColorSpace::BGRPixel *finalPixel;

		// Amount of pixels in blur cluster
		int count;

		// Horizontal pass
		// For each row...
		for (int y = 0; y < m_rawImage->height; y++)
		{
			// ...calculate the blur along the width
			for (int x = 0; x < m_rawImage->width; x++)
			{
				// Clear variables
				blur = { 0, 0, 0 };
				count = 0;

				// Loop along radius * 2 pixels in the row
				for (int horizontal = -m_radius; horizontal <= m_radius; horizontal++)
				{
					// Check for borders, watch out for wrong memory access
					// Ignore pass if pixel is negative
					if (x + horizontal < 0)
						continue;

					// Bail if we reached the end
					if (x + horizontal >= m_rawImage->width)
						break;

					// Update blur pixel count
					count++;

					// Get pixel pointer from raw image
					rawColorComponent = PixelAt(m_rawImage->buffer, x + horizontal, y);

					// Add the pixel to the blur accumulator
					// Since we're using individual bytes to access a pixel (three bytes) 
					// we need to manually move the pointer to access the other color components
					blur.B += *rawColorComponent; rawColorComponent++;
					blur.G += *rawColorComponent; rawColorComponent++;
					blur.R += *rawColorComponent;
				}

				// Done with local blur cluster
				// Get a pointer to the pixel in the new blurred buffer...
				finalPixel = reinterpret_cast<ColorSpace::BGRPixel*>(PixelAt(m_blurBuffer, x, y));

				// ...and write to it
				// Use count-average of the blur accumulator
				finalPixel->B = (blur.B / count);
				finalPixel->G = (blur.G / count);
				finalPixel->R = (blur.R / count);
			}
		}

		// Vertical pass (same as before but, uh, vertically)
		// For each column...
		for (int x = 0; x < m_rawImage->width; x++)
		{
			// ...calculate the blur along the height
			for (int y = 0; y < m_rawImage->height; y++)
			{
				// Clear variables
				blur = { 0, 0, 0 };
				count = 0;

				// Loop along radius * 2 pixels in the column
				for (int vertical = -m_radius; vertical <= m_radius; vertical++)
				{
					// Check for borders, watch out for wrong memory access
					// Ignore pass if pixel is negative
					if (y + vertical < 0)
						continue;

					// Bail if we reached the end
					if (y + vertical >= m_rawImage->height)
						break;

					// Update blur pixel count
					count++;

					// Get pixel pointer from raw image
					rawColorComponent = PixelAt(m_rawImage->buffer, x, y + vertical);

					if (!rawColorComponent)
					{
						break;
					}

					// Add the pixel to the blur accumulator
					// Since we're using individual bytes to access a pixel (three bytes) 
					// we need to manually move the pointer to access the other color components
					blur.B += *rawColorComponent; rawColorComponent++;
					blur.G += *rawColorComponent; rawColorComponent++;
					blur.R += *rawColorComponent;
				}
				
				// Done with local blur cluster
				// Get a pointer to the pixel in the new blurred buffer...
				finalPixel = reinterpret_cast<ColorSpace::BGRPixel*>(PixelAt(m_blurBuffer, x, y));

				// ...and write to it
				// Use count-average of the blur accumulator
				finalPixel->B = (blur.B / count);
				finalPixel->G = (blur.G / count);
				finalPixel->R = (blur.R / count);
			}
		}

		// And we're done!
	}

	// Same as BGRBlur() but with added alpha
	void Blur::BGRABlur()
	{
		// Accumulated blur per row/column cluster
		BlurBGRAPixel blur = { 0, 0, 0, 0 };

		// Holds individual color bytes from raw source
		// The BlurBGRPixel structure just refused to work here :( produced horrible artifacts
		unsigned char *rawColorComponent;

		// Final color per pixel
		ColorSpace::BGRAPixel *finalPixel;

		// Amount of pixels in blur cluster
		int count;

		// Horizontal pass
		// For each row...
		for (int y = 0; y < m_rawImage->height; y++)
		{
			// ...calculate the blur along the width
			for (int x = 0; x < m_rawImage->width; x++)
			{
				// Clear variables
				blur = { 0, 0, 0, 0 };
				count = 0;

				// Loop along radius * 2 pixels in the row
				for (int horizontal = -m_radius; horizontal <= m_radius; horizontal++)
				{
					// Check for borders, watch out for wrong memory access
					// Ignore pass if pixel is negative
					if (x + horizontal < 0)
						continue;

					// Bail if we reached the end
					if (x + horizontal >= m_rawImage->width)
						break;

					// Update blur pixel count
					count++;

					// Get pixel pointer from raw image
					rawColorComponent = PixelAt(m_rawImage->buffer, x + horizontal, y);

					// Add the pixel to the blur accumulator
					// Since we're using individual bytes to access a pixel (three bytes) 
					// we need to manually move the pointer to access the other color components
					blur.B += *rawColorComponent; rawColorComponent++;
					blur.G += *rawColorComponent; rawColorComponent++;
					blur.R += *rawColorComponent; rawColorComponent++;
					blur.A += *rawColorComponent;
				}

				// Done with local blur cluster
				// Get a pointer to the pixel in the new blurred buffer...
				finalPixel = reinterpret_cast<ColorSpace::BGRAPixel*>(PixelAt(m_blurBuffer, x, y));

				// ...and write to it
				// Use count-average of the blur accumulator
				finalPixel->B = (blur.B / count);
				finalPixel->G = (blur.G / count);
				finalPixel->R = (blur.R / count);
				finalPixel->A = (blur.A / count);
			}
		}

		// Vertical pass (same as before but, uh, vertically)
		// For each column...
		for (int x = 0; x < m_rawImage->width; x++)
		{
			// ...calculate the blur along the height
			for (int y = 0; y < m_rawImage->height; y++)
			{
				// Clear variables
				blur = { 0, 0, 0, 0 };
				count = 0;

				// Loop along radius * 2 pixels in the column
				for (int vertical = -m_radius; vertical <= m_radius; vertical++)
				{
					// Check for borders, watch out for wrong memory access
					// Ignore pass if pixel is negative
					if (y + vertical < 0)
						continue;

					// Bail if we reached the end
					if (y + vertical >= m_rawImage->height)
						break;

					// Update blur pixel count
					count++;

					// Get pixel pointer from raw image
					rawColorComponent = PixelAt(m_rawImage->buffer, x, y + vertical);

					// Add the pixel to the blur accumulator
					// Since we're using individual bytes to access a pixel (three bytes) 
					// we need to manually move the pointer to access the other color components
					blur.B += *rawColorComponent; rawColorComponent++;
					blur.G += *rawColorComponent; rawColorComponent++;
					blur.R += *rawColorComponent; rawColorComponent++;
					blur.A += *rawColorComponent;
				}

				// Done with local blur cluster
				// Get a pointer to the pixel in the new blurred buffer...
				finalPixel = reinterpret_cast<ColorSpace::BGRAPixel*>(PixelAt(m_blurBuffer, x, y));

				// ...and write to it
				// Use count-average of the blur accumulator
				finalPixel->B = (blur.B / count);
				finalPixel->G = (blur.G / count);
				finalPixel->R = (blur.R / count);
				finalPixel->A = (blur.A / count);
			}
		}
		
		// And we're done!
	}
	
	// Get's pixel from buffer at coordinate
	unsigned char *Blur::PixelAt(unsigned char *buffer, int x, int y)
	{
		// Typical 2D buffer access x * y + width
		// In this case both x and y need to be multiplied by the number of bytes per pixel
		return &buffer[x * m_bytesPerPixel + y * m_rawImage->width * m_bytesPerPixel];
	}
}
#include "TGA_IO.h"
#include "ErrorConstants.h"

#include <exception>
#include <iostream>

namespace TGA
{
	// Reads file TGA file disk and parses it into TGAImage structure
	bool TGA_IO::Load(const char * filename, TGAImage &tgaImage)
	{
		try
		{
			// Load file into stream
			std::ifstream file(filename, /*std::ifstream::in | */std::ifstream::binary);

			// Check for bad file
			if (file.bad())
			{
				std::cout << ErrorConstants.INVALUD_INPUT_FILE << std::endl;
				return false;
			}

			// Read the whole image header
			char *imageHeader = new char[HEADER_SIZE];
			file.read(imageHeader, HEADER_SIZE);

			// Copy the header to the image file
			// This will come in handy when saving the file
			std::memcpy(&tgaImage.header, imageHeader, HEADER_SIZE);

			// Width is located in the 13th and 14th bytes
			// Height is located in the 15th and 16th bytes
			// Pixel depth is located in the 17th byte
			// Endianess is little: LSB
			tgaImage.width = (unsigned char)imageHeader[12] | ((__int16)imageHeader[13] << 8);
			tgaImage.height = (unsigned char)imageHeader[14] | ((__int16)imageHeader[15] << 8);
			tgaImage.pixelDepth = (unsigned char)imageHeader[16];

			// Calculate buffer size, allocate memory and read it 
			tgaImage.bufferSize = tgaImage.width * tgaImage.height * (tgaImage.pixelDepth / 8);
			tgaImage.buffer = new char[tgaImage.bufferSize];
			file.read(tgaImage.buffer, tgaImage.bufferSize);

			// Close file
			file.close();
		}
		catch (std::exception &e)
		{
			std::cout << "TGA_IO::SaveAs Exception: " << e.what() << std::endl;
			return false;
		}

		// Everything went A-okay
		return true;
	}

	bool TGA_IO::SaveAs(const char * filename, TGAImage *image)
	{
		try
		{
			// Open new binary stream 
			std::ofstream file(filename, std::ofstream::binary);

			// Write the original header to the file (blurring didn't change this)
			file.write(image->header, HEADER_SIZE);

			// Write actual pixel information
			file.write(image->buffer, image->bufferSize);

			// Close file
			file.close();
		}
		catch (std::exception &e)
		{
			std::cout << "TGA_IO::SaveAs Exception: " << e.what() << std::endl;
			return false;
		}

		// Everything is cool
		return true;
	}
}

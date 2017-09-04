#include "TGA_IO.h"
#include "IOManager.h"
#include <iostream>
#include "ErrorConstants.h"

TGAImage *TGA_IO::Load(const char * filename)
{
	// Load file into stream
	std::ifstream file(filename, std::ifstream::in | std::ifstream::binary);

	// Check for bad file
	if (file.bad())
	{
		std::cout << ErrorConstants.INVALUD_INPUT_FILE << std::endl;
		return nullptr;
	}

	// Declare structure to hold image information
	TGAImage tgaImage;
	
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

	return &tgaImage;
}
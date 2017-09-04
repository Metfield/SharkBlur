#include "TGA_IO.h"
#include "IOManager.h"
#include <iostream>
#include "ErrorConstants.h"

bool TGA_IO::Load(const char * filename)
{
	// Load file into stream
	std::ifstream file(filename, std::ifstream::in | std::ifstream::binary);

	// Check for bad file
	if (file.bad())
	{
		std::cout << ErrorConstants.INVALUD_INPUT_FILE << std::endl;
		return false;
	}

	// Parse TGA file
	Parse(file);
}

bool TGA_IO::Parse(std::ifstream &fileStream)
{
	// Read the image header
	char *imageHeader = new char[HEADER_SIZE];
	fileStream.read(imageHeader, HEADER_SIZE);



	return true;
}
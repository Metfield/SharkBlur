#include "Sharktest.h"
#include "ErrorConstants.h"
#include "TGA_IO.h"

#include <iostream>

int main(int argc, char **argv)
{
	// Command line arguments
	CL_Arguments *appParams;

	// Validate CL parameters
	if (!(appParams = ValidateParameters(argc, argv)))
		return -1;

	// No need to use dynamic storage duration due to program size
	TGA_IO sourceImage;
	
	// Load source image
	if(!sourceImage.Load(appParams->sourceFilename))
		return -1;

	return 0;
}

CL_Arguments *ValidateParameters(int count, char **args)
{
	std::cout << "count " << count;

	// Check if we didn't receive enough parameters
	if (count != 4)
	{
		std::cout << ErrorConstants.PARAMS_COUNT;
		return nullptr;
	}
	
	// Check if input file exists
	FILE *inputFile;
	errno_t err;

	if ((err = fopen_s(&inputFile, args[1], "r")) != 0)
	{
		std::cout << ErrorConstants.INPUT_FILE_NON_EXISTANT;
		return nullptr;
	}
	else 
	{
		fclose(inputFile);
	}

	// Output file validation? Nah, let's assume the user knows what he's doin'
	

	// Check if blur is in the [0, 1] range
	// let's do a simple check
	if (atof(args[3]) >= 0 && atof(args[3]) <= 1)
	{
		// Everything is alright, create structure and return it
		return new CL_Arguments{ args[1], args[2], (float)atof(args[3]) };
	}
	else
	{
		std::cout << ErrorConstants.INVALID_BLUR_RANGE;
		return nullptr;
	}
}


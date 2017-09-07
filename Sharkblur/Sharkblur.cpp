#include "Sharkblur.h"
#include "ErrorConstants.h"
#include "TGA_IO.h"
#include "Blur.h"

#include <iostream>

int main(int argc, char **argv)
{
	// Command line arguments
	Sharkblur::CL_Arguments *appParams;

	// Validate CL parameters
	if (!(appParams = Sharkblur::GetCLArguments(argc, argv)))
		return -1;

	// Create TGA reader/writer object
	// No need to use dynamic storage duration due to program size
	TGA::TGA_IO tga_IO;

	// Holds source image info
	TGA::TGAImage image;

	// Load source image
	if (!tga_IO.Load(appParams->sourceFilename, image))
		return -1;

	// Blur it!!
	VisualFX::Blur::Run(image, appParams->blurFactor);

	// Save new blurred image
	tga_IO.SaveAs(appParams->destinationFilename, &image);

	// Success! :D
	std::cout << std::endl << "    *DONE*" << std::endl;
	return 0;
}

namespace Sharkblur
{
	using namespace std;

	CL_Arguments *GetCLArguments(int count, char **args)
	{
		cout << endl << "    Validating arguments..." << endl;

		// Check if we didn't receive enough parameters
		if (count != 4)
		{
			cout << ErrorConstants.PARAMS_COUNT;
			return nullptr;
		}

		// Check if input file exists
		FILE *inputFile;
		errno_t err;

		if ((err = fopen_s(&inputFile, args[1], "r")) != 0)
		{
			cout << ErrorConstants.INPUT_FILE_NON_EXISTANT;
			return nullptr;
		}
		else
		{
			fclose(inputFile);
		}

		// Output file validation? Nah, let's assume the user knows what he's doin'
		// ...		

		// Check if blur is in the [0, 1] range
		// let's do a simple check
		if (atof(args[3]) >= 0 && atof(args[3]) <= 1)
		{
			// Everything is alright, create structure and return it
			return new CL_Arguments{ args[1], args[2], (float)atof(args[3]) };
		}
		else
		{
			cout << ErrorConstants.INVALID_BLUR_RANGE;
			return nullptr;
		}
	}
}

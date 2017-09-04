#pragma once

namespace Sharkblur
{
	// Used to hold command line arguments
	struct CL_Arguments
	{
		const char *sourceFilename;
		const char *destinationFilename;
		float blurFactor;
	};

	// Validates and copies command line arguments
	CL_Arguments *GetCLArguments(int count, char **args);
}
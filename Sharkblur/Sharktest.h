#pragma once

struct CL_Arguments
{
	const char *sourceFilename;
	const char *destinationFilename;
	float blurFactor;
};

CL_Arguments *ValidateParameters(int count, char **args);
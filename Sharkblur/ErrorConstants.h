#pragma once

static struct ErrorConstants
{
	const char *PARAMS_COUNT = "Error: Please provide three parameters in the following order: \n"  "<source-image> <destination-image> <blur-factor[0,1]> \n";
	const char *INPUT_FILE_NON_EXISTANT = "Error: Specified input file does not exist \n";
	const char *INVALID_BLUR_RANGE = "Error: Blur factor is not in the [0, 1] range \n";
	const char *INVALUD_INPUT_FILE = "Error: Please make sure the input file is valid \n";
} ErrorConstants;
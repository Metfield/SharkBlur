#pragma once

#include<fstream>
#include"TGAImage.h"

class TGA_IO
{
public:
	TGAImage *Load(const char *filename);

private:

	// Header size is 18 bytes according to TGA specification (Wikipedia FTW)
	const int HEADER_SIZE = 18;

};
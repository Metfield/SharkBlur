#pragma once

#include<fstream>

class TGA_IO
{
public:
	bool Load(const char *filename);

private:
	int m_width,
		m_height;

	// Header size is 18 bytes according to TGA specification (Wikipedia FTW)
	const int HEADER_SIZE = 18;

	bool Parse(std::ifstream &fileStream);

};
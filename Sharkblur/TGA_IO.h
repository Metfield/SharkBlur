#pragma once

#include<fstream>
#include"TGAImage.h"

namespace TGA
{
	class TGA_IO
	{
	public:
		bool Load(const char *filename, TGAImage &tgaImage);
		bool SaveAs(const char *filename, TGAImage *image);

	private:
		// Header size is 18 bytes according to TGA specification (Wikipedia FTW)
		const int HEADER_SIZE = 18;

		void Decompress(std::ifstream &file, TGAImage &tgaImage, bool hasAlphaChannel);
	};
}
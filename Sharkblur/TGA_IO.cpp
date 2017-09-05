#include "TGA_IO.h"
#include "ErrorConstants.h"

#include <exception>
#include <iostream>
#include <vector>

namespace TGA
{
	// Reads file TGA file disk and parses it into TGAImage structure
	bool TGA_IO::Load(const char * filename, TGAImage &tgaImage)
	{
		try
		{
			// Load file into stream
			std::ifstream file(filename, /*std::ifstream::in | */std::ifstream::binary);

			// Check for bad file
			if (file.bad())
			{
				std::cout << ErrorConstants.INVALUD_INPUT_FILE << std::endl;
				return false;
			}

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
			//tgaImage.buffer = new unsigned char[tgaImage.bufferSize];
			
			// Check if data is run-length encoded 
			// Only support true-color RLE
			if ((int)imageHeader[2] == 10)
			{
				// Decompress RLE data
				Decompress(file, tgaImage, tgaImage.pixelDepth == 32);
			}
			else
			{
				tgaImage.buffer = new unsigned char[tgaImage.bufferSize];
				file.read((char *)tgaImage.buffer, tgaImage.bufferSize);
			}

			// Close file
			file.close();
		}
		catch (std::exception &e)
		{
			std::cout << "TGA_IO::SaveAs Exception: " << e.what() << std::endl;
			return false;
		}

		// Everything went A-okay
		return true;
	}

	bool TGA_IO::SaveAs(const char *filename, TGAImage *image)
	{
		try
		{
			// Open new binary stream 
			std::ofstream file(filename, std::ofstream::binary);

			// This TGA file is now uncompressed, write this on the third byte of the header.
			// 2 is the code for uncompressed true-color image
			image->header[2] = 2;
			file.write(image->header, HEADER_SIZE);

			// Write actual pixel information
			//file.write((char*)image->vecBuffer.data(), image->vecBuffer.size());
			file.write((char*)image->buffer, image->bufferSize);

			// Close file
			file.close();
		}
		catch (std::exception &e)
		{
			std::cout << "TGA_IO::SaveAs Exception: " << e.what() << std::endl;
			return false;
		}

		// Everything is cool
		return true;
	}

	void TGA_IO::Decompress(std::ifstream &file, TGAImage &tgaImage, bool hasAlphaChannel)
	{
		// Number of pixels to read in a run
		int runLength = 0;

		// Ofc these hold color values
		char R, G, B, A;
		
		// RLE control byte
		char packet;

		// This will hold the uncompressed data
		// Use vector since it's easy to push bytes
		std::vector<unsigned char> decompressedBuffer;

		// Decompress!
		for (int i = 0; i <tgaImage.bufferSize; i++)
		{
			// Get RLE control byte
			file.read(&packet, 1);

			// Check if byte is RLE packet
			if ((unsigned char)packet & 128)
			{
				// Clear 8th bit and add one according to specification
				// Get number of times that next pixel will be written
				runLength = (unsigned char)packet - 127;

				// Read pixel that is to be repeated
				file.read(&B, 1);
				file.read(&G, 1);
				file.read(&R, 1);

				// Update buffer index
				i += 3;

				// Take care of alpha
				if (hasAlphaChannel)
				{
					file.read(&A, 1);
					i++;
				}

				// Write run-length to new buffer
				for (int j = 0; j < runLength; j++)
				{
					// Push values
					decompressedBuffer.push_back((unsigned char)B);
					decompressedBuffer.push_back((unsigned char)G);
					decompressedBuffer.push_back((unsigned char)R);

					if (hasAlphaChannel)
						decompressedBuffer.push_back((unsigned char)A);
				}
			}
			else
			{
				// Get number of raw pixels to fetch
				runLength = (unsigned char)packet + 1;

				// Get runLength-amount of raw pixels 
				for (int j = 0; j < runLength; j++)
				{
					// It's a normal pixel. Just copy the raw values
					file.read(&B, 1);
					file.read(&G, 1);
					file.read(&R, 1);

					// Update buffer index
					i += 3;

					// Push values
					decompressedBuffer.push_back((unsigned char)B);
					decompressedBuffer.push_back((unsigned char)G);
					decompressedBuffer.push_back((unsigned char)R);

					// Take care of alpha
					if (hasAlphaChannel)
					{
						file.read(&A, 1);
						i++;

						decompressedBuffer.push_back((unsigned char)A);
					}
				}
			}
		}

		// Finally copy uncompressed vector to image buffer
		tgaImage.bufferSize = decompressedBuffer.size();
		tgaImage.buffer = new unsigned char[tgaImage.bufferSize];
		memcpy(tgaImage.buffer, decompressedBuffer.data(), tgaImage.bufferSize);
	}
}
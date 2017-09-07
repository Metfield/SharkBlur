#pragma once

namespace ColorSpace
{
	enum Type
	{
		BGR,
		BGRA
	};

	struct BGRPixel
	{	
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	struct BGRAPixel
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
		unsigned char A;
	};
}
#pragma once

namespace ColorSpace
{
	enum Type
	{
		BGR,
		ABGR
	};

	struct BGRPixel
	{	
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	struct ABGRPixel
	{
		unsigned char A;
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};
}
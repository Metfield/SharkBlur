#pragma once

namespace ColorSpace
{
	enum Type
	{
		BGR,
		ABGR
	};

	template <Type pixelType>
	struct Pixel;
	
	template <>
	struct Pixel<Type::BGR>
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	template <>
	struct Pixel<Type::ABGR>
	{
		unsigned char A;
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};
}
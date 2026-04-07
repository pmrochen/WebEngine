/*
 *	Name: ColorFormat
 *	Author: Pawel Mrochen
 */

#pragma once

#include <functional>
#include <cstddef>
#include <cstdint>
#include <ColorSpace.hpp>
#include <PixelFormat.hpp>

namespace graphics {

using imaging::ColorSpace;
using imaging::PixelFormat;

struct ColorFormat
{
	constexpr ColorFormat() noexcept :
		pixelFormatAndColorSpace()
	{
	}

	//constexpr explicit ColorFormat(PixelFormat pixelFormat) noexcept : 
	//	pixelFormatAndColorSpace((std::uint8_t)((unsigned int)pixelFormat & 0x7F))
	//{
	//}
	
	constexpr ColorFormat(PixelFormat pixelFormat, ColorSpace colorSpace) noexcept : 
		pixelFormatAndColorSpace((std::uint8_t)(((unsigned int)pixelFormat & 0x7F) | ((unsigned int)colorSpace << 7)))
	{
	}

	bool operator==(const ColorFormat& colorFormat) const noexcept 
	{ 
		return (pixelFormatAndColorSpace == colorFormat.pixelFormatAndColorSpace); 
	}
	
	bool operator!=(const ColorFormat& colorFormat) const noexcept 
	{ 
		return (pixelFormatAndColorSpace != colorFormat.pixelFormatAndColorSpace); 
	}

	template<typename A> void serialize(A& ar) 
	{ 
		ar(pixelFormatAndColorSpace); 
	}

	static const ColorFormat& getEmpty() noexcept 
	{ 
		static const ColorFormat empty; 
		return empty; 
	}

	bool isEmpty() const noexcept 
	{ 
		return !pixelFormatAndColorSpace; 
	}

	void makeEmpty() noexcept 
	{ 
		pixelFormatAndColorSpace = (std::uint8_t)0; 
	}

	void set(PixelFormat pixelFormat, ColorSpace colorSpace) noexcept 
	{ 
		pixelFormatAndColorSpace = (std::uint8_t)(((unsigned int)pixelFormat & 0x7F) | ((unsigned int)colorSpace << 7));
	}

	PixelFormat getPixelFormat() const noexcept 
	{ 
		return (PixelFormat)((unsigned int)pixelFormatAndColorSpace & 0x7F);
	}

	void setPixelFormat(PixelFormat pixelFormat) noexcept 
	{ 
		pixelFormatAndColorSpace = (std::uint8_t)(((unsigned int)pixelFormatAndColorSpace & 0x80) | ((unsigned int)pixelFormat & 0x7F)); 
	}

	ColorSpace getColorSpace() const noexcept 
	{ 
		return (ColorSpace)((unsigned int)pixelFormatAndColorSpace >> 7); 
	}

	void setColorSpace(ColorSpace colorSpace) noexcept 
	{ 
		pixelFormatAndColorSpace = (std::uint8_t)(((unsigned int)pixelFormatAndColorSpace & 0x7F) | ((unsigned int)colorSpace << 7)); 
	}

	std::uint8_t/*unsigned int*/ pixelFormatAndColorSpace;	// 7+1 bits
};

} // namespace graphics

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::ColorFormat>
{
	size_t operator()(const ::graphics::ColorFormat& colorFormat) const noexcept
	{
		return hash<uint8_t/*unsigned int*/>()(colorFormat.pixelFormatAndColorSpace);
	}
};

} // namespace std

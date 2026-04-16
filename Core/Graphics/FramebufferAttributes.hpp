/*
 *	Name: FramebufferAttributes
 *	Author: Pawel Mrochen
 */

#pragma once

#include <stdexcept>
#include <functional>
#include <array>
#include <algorithm>
#include <cstddef>
#include <ColorSpace.hpp>
#include <PixelFormat.hpp>
#include "DepthStencilFormat.hpp"
#include "MultisampleMode.hpp"
#include "ColorFormat.hpp"

namespace graphics {

using imaging::ColorSpace;
using imaging::PixelFormat;

struct FramebufferAttributes
{
	static constexpr int MAX_COLOR_FORMAT_COUNT = 8;

	FramebufferAttributes() noexcept :
		//width(0),
		//height(0)
	{
	}
	
	FramebufferAttributes(/*int width, int height,*/ PixelFormat colorFormat, ColorSpace colorSpace,
		DepthStencilFormat depthStencilFormat, MultisampleMode multisampleMode) noexcept :
		//width(width),
		//height(height),
		depthStencilFormat(depthStencilFormat),
		multisampleMode(multisampleMode)
	{
		if (colorFormat != PixelFormat::UNSPECIFIED)
			colorFormats[0].set(colorFormat, colorSpace);
	}
	
	FramebufferAttributes(/*int width, int height,*/ int nColorBuffers, PixelFormat colorFormat, ColorSpace colorSpace,
		DepthStencilFormat depthStencilFormat, MultisampleMode multisampleMode) noexcept :
		//width(width),
		//height(height),
		depthStencilFormat(depthStencilFormat),
		multisampleMode(multisampleMode)
	{
		for (int i = 0, n = std::clamp(nColorBuffers, 0, MAX_COLOR_FORMAT_COUNT); i < n; i++)
			colorFormats[i].set(colorFormat, colorSpace);
	}

	bool operator==(const FramebufferAttributes& attributes) const noexcept
	{
		static_assert((sizeof(FramebufferAttributes) & (sizeof(std::size_t) - 1)) == 0);
		return std::equal((const std::size_t*)this, (const std::size_t*)this + sizeof(FramebufferAttributes)/sizeof(std::size_t), 
			(const std::size_t*)&attributes);
	}

	bool operator!=(const FramebufferAttributes& attributes) const noexcept
	{
		return !(*this == attributes);
	}

	template<typename A> void serialize(A& ar) 
	{
		for (int i = 0; i < MAX_COLOR_FORMAT_COUNT; i++)
			ar(colorFormats[i]);

		ar(depthStencilFormat, multisampleMode);
	}

	//int getWidth() const noexcept { return width; }
	//int getHeight() const noexcept { return height; }
	//IntVector2 getSize() const noexcept { return IntVector2(width, height); }

	PixelFormat getPixelFormat() const noexcept { return colorFormats[0].getPixelFormat(); }
	ColorSpace getColorSpace() const noexcept { return colorFormats[0].getColorSpace(); }

	const ColorFormat& getColorFormat(int index) const noexcept 
	{ 
		return ((std::size_t)index < (std::size_t)MAX_COLOR_FORMAT_COUNT) ? colorFormats[index] : ColorFormat::getEmpty();
	}

	bool hasColorBuffers() const noexcept 
	{
		for (int i = 0; i < MAX_COLOR_FORMAT_COUNT; i++)
		{
			if (!colorFormats[i].isEmpty())
				return true;
		}

		return false;
	}

	//int getNumberOfColorBuffers() const noexcept;
	//{
	//	int nBuffers = 0;
	//	for (int i = 0; i < MAX_COLOR_FORMAT_COUNT; i++)
	//	{
	//		if (!colorFormats[i].isEmpty())
	//			nBuffers++; //nBuffers = i + 1;
	//	}
	//	
	//	return nBuffers;
	//}

	DepthStencilFormat getDepthStencilFormat() const noexcept { return depthStencilFormat; }
	bool hasDepthBuffer() const noexcept { return depthStencilFormat.hasDepth(); }
	bool hasStencilBuffer() const noexcept { return depthStencilFormat.hasStencil(); }
	MultisampleMode getMultisampleMode() const noexcept { return multisampleMode; }

	//int width;
	//int height;
	std::array<ColorFormat, MAX_COLOR_FORMAT_COUNT> colorFormats;
	DepthStencilFormat depthStencilFormat;
	MultisampleMode multisampleMode;
};

} // namespace graphics

#include "Hash.inl"

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::FramebufferAttributes>
{
	size_t operator()(const ::graphics::FramebufferAttributes& attributes) const noexcept
	{
		static_assert((sizeof(::graphics::FramebufferAttributes) & (sizeof(size_t) - 1)) == 0);
		return ::graphics::hash::range/*boost::hash_range*/((const size_t*)&attributes,
			(const size_t*)&attributes + sizeof(::graphics::FramebufferAttributes)/sizeof(size_t));
	}
};

} // namespace std

/*
 *	Name: DepthStencilFormat
 *	Author: Pawel Mrochen
 */

#pragma once

#include <stdexcept>
#include <utility>
#include <string>

namespace graphics {

enum class DepthStencilFormat
{
	UNSPECIFIED = 0,
	DEPTH16 = 0x0001,
	DEPTH24 = 0x0002,
	DEPTH32 = 0x0003,
	DEPTH32F = 0x0004,
	STENCIL8 = 0x0100,
	DEPTH24_STENCIL8 = 0x0102,
	DEPTH32F_STENCIL8 = 0x0104
};

namespace detail {
constexpr int DEPTH_MASK = 0x00FF;
constexpr int STENCIL_MASK = 0xFF00;
} // namespace detail

inline std::string str(DepthStencilFormat format)
{
	using namespace std::string_literals;
	switch (format)
	{
		case DepthStencilFormat::UNSPECIFIED: return "Unspecified"s;
		case DepthStencilFormat::DEPTH16: return "Depth16"s;
		case DepthStencilFormat::DEPTH24: return "Depth24"s;
		case DepthStencilFormat::DEPTH32: return "Depth32"s;
		case DepthStencilFormat::DEPTH32F: return "Depth32f"s;
		case DepthStencilFormat::STENCIL8: return "Stencil8"s;
		case DepthStencilFormat::DEPTH24_STENCIL8: return "Depth24Stencil8"s;
		case DepthStencilFormat::DEPTH32F_STENCIL8: return "Depth32fStencil8"s;
		default: throw std::invalid_argument("format");
	}
}

inline DepthStencilFormat pack(DepthStencilFormat depth, DepthStencilFormat stencil) noexcept
{
	return (DepthStencilFormat)(((int)depth & detail::DEPTH_MASK) | ((int)stencil & detail::STENCIL_MASK));
}

inline std::pair<DepthStencilFormat, DepthStencilFormat> unpack(DepthStencilFormat format) noexcept
{
	return { (DepthStencilFormat)((int)format & detail::DEPTH_MASK), (DepthStencilFormat)((int)format & detail::STENCIL_MASK) };
}

inline bool isFloatingPoint(DepthStencilFormat format) noexcept 
{ 
	return (format == DepthStencilFormat::DEPTH32F) || (format == DepthStencilFormat::DEPTH32F_STENCIL8);
}

inline bool isPacked/*DepthStencil*/(DepthStencilFormat format) noexcept 
{ 
	return (format == DepthStencilFormat::DEPTH24_STENCIL8) || (format == DepthStencilFormat::DEPTH32F_STENCIL8);
}

inline bool hasDepth(DepthStencilFormat format) noexcept 
{ 
	return (format == DepthStencilFormat::DEPTH16) || (format == DepthStencilFormat::DEPTH24) || 
		(format == DepthStencilFormat::DEPTH32) || (format == DepthStencilFormat::DEPTH32F) || isPacked(format);
}

inline bool hasStencil(DepthStencilFormat format) noexcept 
{ 
	return (format == DepthStencilFormat::STENCIL8) || isPacked(format);
}

inline DepthStencilFormat getDepthOnly(DepthStencilFormat format) noexcept 
{ 
	return (DepthStencilFormat)((int)format & detail::DEPTH_MASK);
}

inline DepthStencilFormat getStencilOnly(DepthStencilFormat format) noexcept 
{ 
	return (DepthStencilFormat)((int)format & detail::STENCIL_MASK);
}

} // namespace graphics

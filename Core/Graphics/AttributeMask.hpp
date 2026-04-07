/*
 *	Name: AttributeMask
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>
#include <ColorMask.hpp>
#include "DepthMask.hpp"

namespace graphics {

using imaging::ColorMask;

enum class AttributeMask
{
	NONE = 0x0000,
	RED = ColorMask::RED,		// 0x0001,
	GREEN = ColorMask::GREEN,	// 0x0002,
	BLUE = ColorMask::BLUE,		// 0x0004,
	ALPHA = ColorMask::ALPHA,	// 0x0008,
	COLOR = ColorMask::ALL,		// 0x000F,
	DEPTH = DepthMask::ENABLED,	// 0x1000,
	STENCIL = 0x8000,
	ALL = 0x900F
};

ENABLE_ENUM_BITWISE_OPERATORS(AttributeMask);

inline ColorMask getColorMask(AttributeMask mask) noexcept { return (ColorMask)(mask & AttributeMask::COLOR); }
inline bool hasColor(AttributeMask mask) noexcept { return getColorMask(mask); }
inline bool getDepthMask(AttributeMask mask) noexcept { return (mask & AttributeMask::DEPTH); }
inline bool hasDepth(AttributeMask mask) noexcept { return getDepthMask(mask); }
inline bool getStencilMask(AttributeMask mask) noexcept { return (mask & AttributeMask::STENCIL); }
inline bool hasStencil(AttributeMask mask) noexcept { return getStencilMask(mask); }

} // namespace graphics

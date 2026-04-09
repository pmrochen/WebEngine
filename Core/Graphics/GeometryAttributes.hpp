/*
 *	Name: GeometryAttributes
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class GeometryAttributes
{
	NONE = 0,
	TEX_COORDS_1 = 0x0001,
	TEX_COORDS_2 = 0x0002,
	TEX_COORDS_3 = 0x0003,
	TEX_COORDS_4 = 0x0004,
	TEX_COORDS_5 = 0x0005,
	TEX_COORDS_6 = 0x0006,
	TEX_COORDS_7 = 0x0007,
	TEX_COORDS_8 = 0x0008,
	TEX_COORDS_MASK = 0x000F,		// 0..8
	BLEND_WEIGHTS_1 = 0x0010,
	BLEND_WEIGHTS_2 = 0x0020,
	BLEND_WEIGHTS_3 = 0x0030,
	BLEND_WEIGHTS_4 = 0x0040,
	BLEND_WEIGHTS_MASK = 0x0070,	// 0..4
	TANGENTS = 0x0080,
	BITANGENTS = 0x0100,
	NORMALS = 0x0200,
	COLORS = 0x0400,
	INSTANCED = 0x0800,
	ALL = 0x0FFF
};

ENABLE_ENUM_BITWISE_OPERATORS(GeometryAttributes);

namespace detail {
//constexpr int MAP_COORDS_SHIFT = 0;
constexpr int MAX_MAP_COORDS = 8;
constexpr int MAX_BLEND_WEIGHTS = 4;
constexpr int BLEND_WEIHTS_SHIFT = 4;
} // namespace detail

inline int getNumberOfMapCoords(GeometryAttributes attributes) noexcept 
{ 
	return ((int)attributes & detail::MAP_COORDS_MASK) /*>> detail::MAP_COORDS_SHIFT*/;
}

inline int getNumberOfBlendWeights(GeometryAttributes attributes) noexcept 
{ 
	return ((int)attributes & detail::BLEND_WEIGHTS_MASK) >> detail::BLEND_WEIGHTS_SHIFT;
}

} // namespace graphics

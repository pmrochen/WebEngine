/*
 *	Name: VertexAttributeTypes
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class VertexAttributeTypes
{
	NONE = 0,
	POSITION = 1 << 0,
	TANGENT = 1 << 1,
	BITANGENT = 1 << 2,
	NORMAL = 1 << 3,
	BLEND_WEIGHT = 1 << 4,
	BLEND_INDEX = 1 << 5,
	COLOR_0 = 1 << 6, COLOR = COLOR_0,
	COLOR_1 = 1 << 7,
	TEX_COORD_0 = 1 << 8, TEX_COORD = TEX_COORD_0,
	TEX_COORD_1 = 1 << 9,
	TEX_COORD_2 = 1 << 10,
	TEX_COORD_3 = 1 << 11,
	TEX_COORD_4 = 1 << 12,
	TEX_COORD_5 = 1 << 13,
	TEX_COORD_6 = 1 << 14,
	TEX_COORD_7 = 1 << 15
	//TEX_COORDS = 0xFF00,
	//ALL = 0xFFFF
};

ENABLE_ENUM_BITWISE_OPERATORS(VertexAttributeTypes);

namespace detail {
constexpr int TEX_COORD_MASK = 0xFF00;
} // namespace detail

} // namespace graphics

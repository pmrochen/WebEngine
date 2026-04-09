/*
 *	Name: VertexAttributeIndex
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class VertexAttributeIndex/*VertexAttributeType*/
{
	POSITION = 0,
	TANGENT = 1,
	BITANGENT = 2,
	NORMAL = 3,
	BLEND_WEIGHT = 4,
	BLEND_INDEX = 5,
	COLOR_0 = 6, COLOR = COLOR_0,
	COLOR_1 = 7,
	TEX_COORD_0 = 8, TEX_COORD = TEX_COORD_0,
	TEX_COORD_1 = 9,
	TEX_COORD_2 = 10,
	TEX_COORD_3 = 11,
	TEX_COORD_4 = 12,
	TEX_COORD_5 = 13,
	TEX_COORD_6 = 14,
	TEX_COORD_7 = 15
};

} // namespace graphics

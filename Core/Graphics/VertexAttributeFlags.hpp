/*
 *	Name: VertexAttributeFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class VertexAttributeFlags
{
	NONE = 0,
	STORE_LAST_BLEND_WEIGHT = 1,
	PACK_BLEND_WEIGHTS = 2,
	PACK_TANGENT_SPACE_VECTORS = 4,
	STORE_HANDEDNESS = 8,		// store determinant (1/-1) of tangent space matrix in tangent.w instead of storing bitangent
	//LINEARIZE_COLORS = 16,	// convert sRGB to linear color space
	//PACK_COLORS = 32,
	//CHANNEL_ORDER_BGRA = 64,
	DEFAULT = STORE_LAST_BLEND_WEIGHT
};

ENABLE_ENUM_BITWISE_OPERATORS(VertexAttributeFlags);

} // namespace graphics

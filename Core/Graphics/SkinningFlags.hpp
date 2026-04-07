/*
 *	Name: SkinningFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class SkinningFlags
{
	NONE = 0,
	WEIGHTED_SKINNING = 1,
	TANGENT_SPACE_WEIGHTING = 2,
	NORMALIZE_TANGENT_SPACE_VECTORS = 4,
	DEFAULT = WEIGHTED_SKINNING | TANGENT_SPACE_WEIGHTING | NORMALIZE_TANGENT_SPACE_VECTORS
};

ENABLE_ENUM_BITWISE_OPERATORS(SkinningFlags);

} // namespace graphics

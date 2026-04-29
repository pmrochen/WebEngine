/*
 *	Name: DepthRange
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class DepthRange
{
	//UNSPECIFIED,
	NEGATIVE_ONE_TO_ONE,	// OpenGL
	ZERO_TO_ONE,			// DirectX
	ONE_TO_ZERO				// Reverse Z
};

} // namespace graphics

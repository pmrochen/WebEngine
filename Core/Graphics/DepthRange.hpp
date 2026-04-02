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
	ZERO_TO_ONE				// DirectX
};

} // namespace graphics

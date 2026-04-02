/*
 *	Name: ClipOrigin
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class ClipOrigin
{
	UNSPECIFIED,
	LOWER_LEFT,		// OpenGL
	UPPER_LEFT		// DirectX
};

} // namespace graphics

/*
 *	Name: LightType
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class LightType
{
	NONE = 0,
	DIRECTIONAL = 0x0001,	// DISTANT, INFINITE
	POINT = 0x0002,			// POSITIONAL, OMNI
	SPOT = 0x0004
	//AREA = 0x0008
};

} // namespace graphics

/*
 *	Name: LightTypes
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class LightTypes
{
	NONE = 0,
	DIRECTIONAL = 0x01,	// DISTANT, INFINITE
	POINT = 0x02,		// POSITIONAL, OMNI
	SPOT = 0x04
	//AREA = 0x08
};

ENABLE_ENUM_BITWISE_OPERATORS(LightTypes);

} // namespace graphics

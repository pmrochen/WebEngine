/*
 *	Name: FCurveExtrapolationMode
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class FCurveExtrapolationMode
{
	//NONE,
	CONSTANT,
	REPEAT,				// CYCLIC
	MIRRORED_REPEAT,	// OSCILLATORY
	OFFSET_REPEAT,		// CYCLIC_RELATIVE
	LINEAR,
	RESET
};

} // namespace graphics

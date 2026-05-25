/*
 *	Name: AttenuationType
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class AttenuationType
{
	NONE,
	INVERSE_DISTANCE,
	INVERSE_DISTANCE_SQUARED,
	WEIGHTED
};

} // namespace graphics

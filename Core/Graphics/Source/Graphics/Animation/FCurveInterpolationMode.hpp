/*
 *	Name: FCurveInterpolationMode
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class FCurveInterpolationMode
{
	//NONE,
	STEPPED,		// CONSTANT, NEAREST_NEIGHBOR
	LINEAR,
	HERMITE,
	CATMULL_ROM,
	KOCHANEK_BARTELS,
	BEZIER,
	SPHERICAL_LINEAR
};

} // namespace graphics

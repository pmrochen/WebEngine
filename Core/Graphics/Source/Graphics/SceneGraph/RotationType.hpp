/*
 *	Name: RotationType
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class RotationType
{
	NONE,
	QUATERNION,
	EULER,
	YAW_PITCH_ROLL,
	TARGET_ROLL
	//AXIS_ANGLE
};

} // namespace graphics

/*
 *	Name: FCurveChannel
 *	Author: Pawel Mrochen
 */

#pragma once

#include <Axis.hpp>

namespace graphics {

using mathematics::Axis;

enum class FCurveChannel
{
	UNSPECIFIED = 0,
	ANY_SCALAR = 0x0100,
	ANY_OPACITY = 0x4100,
	TRANSLATION_X = 0x0110,
	TRANSLATION_Y = 0x0111,
	TRANSLATION_Z = 0x0112,
	QUATERNION_X = 0x1120,
	QUATERNION_Y = 0x1121,
	QUATERNION_Z = 0x1122,
	QUATERNION_W = 0x1123,
	ROTATION_X = 0x0130,
	ROTATION_Y = 0x0131,
	ROTATION_Z = 0x0132,
	ROTATION_YAW = 0x0140,
	ROTATION_PITCH = 0x0141,
	ROTATION_ROLL = 0x0142,
	TARGET_X = 0x0150,
	TARGET_Y = 0x0151,
	TARGET_Z = 0x0152,
	SCALE_X = 0x2160,
	SCALE_Y = 0x2161,
	SCALE_Z = 0x2162,
	ANY_VECTOR2 = 0x0200,
	ANY_VECTOR3 = 0x0300,
	ANY_COLOR3 = 0x4300,
	TRANSLATION_VECTOR3 = 0x0310,
	ROTATION_QUATERNION = 0x1420,
	ROTATION_EULER = 0x0330,
	ROTATION_YAW_PITCH_ROLL = 0x0340,
	TARGET_VECTOR3 = 0x0350,
	SCALE_VECTOR3 = 0x2360,
	ANY_VECTOR4 = 0x0400,
	ANY_QUATERNION = 0x1400,
	ANY_COLOR4 = 0x4400
};

namespace detail {

constexpr int TYPE_UNSPECIFIED = 0x0000;
constexpr int TYPE_TRANSLATION = 0x0010;
constexpr int TYPE_ROTATION_QUATERNION = 0x0020;
constexpr int TYPE_ROTATION_EULER = 0x0030;
constexpr int TYPE_ROTATION_YAW_PITCH_ROLL = 0x0040;
constexpr int TYPE_TARGET = 0x0050;
constexpr int TYPE_SCALE = 0x0060;
constexpr int COMPONENT_INDEX_MASK = 0x000F;
constexpr int TYPE_MASK = 0x00F0;
constexpr int N_COMPONENTS_MASK = 0x0F00;
constexpr int N_COMPONENTS_SHIFT = 8;
constexpr int QUATERNION_MASK = 0x1000;
constexpr int SCALE_MASK = 0x2000;
constexpr int COLOR_MASK = 0x4000;

} // namespace detail

inline int getNumberOfComponents(FCurveChannel channel) const noexcept
{ 
	return ((int)channel & detail::N_COMPONENTS_MASK) >> detail::N_COMPONENTS_SHIFT;
}

inline Axis getAxis(FCurveChannel channel) const noexcept 
{ 
	return (Axis)((int)channel & 3); 
}

inline bool isScaleChannel(FCurveChannel channel) const noexcept 
{ 
	return ((int)channel & detail::SCALE_MASK); 
}

inline bool isColorChannel(FCurveChannel channel) const noexcept 
{ 
	return ((int)channel & detail::COLOR_MASK); 
}

} // namespace graphics

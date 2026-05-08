/*
 *	Name: FCurveAttributes
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class FCurveAttributes
{
	NONE = 0,
	TIME = 0x0001,
	VALUE = 0x0002,
	INTERPOLATION_MODE = 0x0004,
	TCB = 0x0008,
	INCOMING_TIME = 0x0010,
	INCOMING_VECTOR = 0x0020,
	OUTGOING_TIME = 0x0040,
	OUTGOING_VECTOR = 0x0080
};

ENABLE_ENUM_BITWISE_OPERATORS(FCurveAttributes);

} // namespace graphics

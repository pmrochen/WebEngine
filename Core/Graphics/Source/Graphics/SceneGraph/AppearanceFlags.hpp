/*
 *	Name: AppearanceFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class AppearanceFlags
{
	NONE = 0,
	VISIBLE = 0x0001,
	TRACEABLE = 0x0002,
	PICKABLE = 0x0200
};

ENABLE_ENUM_BITWISE_OPERATORS(AppearanceFlags);

} // namespace graphics

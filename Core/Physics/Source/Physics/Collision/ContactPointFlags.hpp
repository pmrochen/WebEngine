/*
 *	Name: ContactPointFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace physics {

enum class ContactPointFlags
{
	NONE = 0,
	CUSTOM_FRICTION_DIRECTION = 0x01,
	CUSTOM_CONTACT_ERP = 0x02,
	CUSTOM_CONTACT_CFM = 0x04
};

ENABLE_ENUM_BITWISE_OPERATORS(ContactPointFlags);

} // namespace physics

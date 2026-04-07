/*
 *	Name: StorageFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class StorageFlags
{
	NONE = 0,
	DYNAMIC = 0x0001,
	SHARED = 0x0002,
	MUTABLE = 0x0004,
	//RETRIEVABLE = 0x0008,
	STREAMED = 0x0020
};

ENABLE_ENUM_BITWISE_OPERATORS(StorageFlags);

} // namespace graphics

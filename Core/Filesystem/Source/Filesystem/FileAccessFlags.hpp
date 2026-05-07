/*
 *	Name: FileAccessFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace filesystem {

enum class FileAccessFlags
{
	NONE = 0,
	SEQUENTIAL_SCAN	= 0x08000000,
	RANDOM_ACCESS = 0x10000000
};

ENABLE_ENUM_BITWISE_OPERATORS(FileAccessFlags);

} // namespace filesystem

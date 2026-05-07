/*
 *	Name: FileOpenMode
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>
#include "FileAccessFlags.hpp"

namespace filesystem {

enum class FileOpenMode
{
	NONE = 0,
	READ = 1,
	WRITE = 2,
	SEQUENTIAL_SCAN	= FileAccessFlags::SEQUENTIAL_SCAN,
	RANDOM_ACCESS = FileAccessFlags::RANDOM_ACCESS
};

ENABLE_ENUM_BITWISE_OPERATORS(FileOpenMode);

} // namespace filesystem

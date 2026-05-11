/*
 *	Name: FileDataException
 *	Author: Pawel Mrochen
 */

#pragma once

#include <filesystem>
#include "FilesystemException.hpp"

namespace filesystem {

class FileDataException : public FilesystemException
{
public:
	FileDataException() = default;

	FileDataException(const std::filesystem::path& path) noexcept :
		FilesystemException(path)
	{
	}
};

} // namespace filesystem

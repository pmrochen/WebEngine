/*
 *	Name: FilesystemError
 *	Author: Pawel Mrochen
 */

#pragma once

#include <string>
#include <filesystem>
#include <Common/Strings/LocalString.hpp>

namespace filesystem {

using common::LocalString;

enum class FilesystemError
{
	GENERIC,
	FILE_ALREADY_EXISTS,
	FILE_NOT_FOUND,
	TOO_MANY_OPEN_FILES,
	ACCESS_DENIED,
	INVALID_PATH,
	INVALID_HANDLE,
	INVALID_OFFSET,
	DIRECTORY_NOT_EMPTY,
	DISK_FULL,
	END_OF_FILE,
	NOT_SUPPORTED
};

int getSystemErrorCode(FilesystemError error) noexcept;
LocalString makeErrorMessage(const std::filesystem::path& path, FilesystemError cause = FilesystemError::GENERIC) noexcept;
LocalString makeErrorMessage(const std::filesystem::path& path, int systemErrorCode) noexcept;
LocalString makeErrorMessage(const std::filesystem::path& path, const LocalChar* message) noexcept;
LocalString makeErrorMessage(const std::filesystem::path& path, const LocalString& message) noexcept;

} // namespace filesystem

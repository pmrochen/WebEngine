/*
 *	Name: FilesystemError
 *	Author: Pawel Mrochen
 */

#pragma once

#include <string>
#include <filesystem>

namespace filesystem {

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
std::string makeErrorMessage(const std::filesystem::path& path, FilesystemError cause = FilesystemError::GENERIC) noexcept;
std::string makeErrorMessage(const std::filesystem::path& path, int systemErrorCode) noexcept;
//std::string makeErrorMessage(const std::filesystem::path& path, const char* message) noexcept;
//std::string makeErrorMessage(const std::filesystem::path& path, const std::string& message) noexcept;

} // namespace filesystem

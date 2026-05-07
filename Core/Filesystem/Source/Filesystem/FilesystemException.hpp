/*
 *	Name: FilesystemException
 *	Author: Pawel Mrochen
 */

#pragma once

//#include <string>
#include <system_error>
#include <filesystem>
#include "FilesystemError.hpp"

namespace filesystem {

class FilesystemException : public std::filesystem::filesystem_error
{
public:
	FileSystemException() = default;

	FileSystemException(const std::filesystem::path& path, FilesystemError cause = FilesystemError::GENERIC) noexcept :
		std::filesystem::filesystem_error(makeErrorMessage(path, cause), path, { getSystemErrorCode(cause), std::system_category{} }),
		cause_(cause)//,
		//message_()
	{
	}

	FileSystemException(const std::filesystem::path& path, int systemErrorCode) noexcept :
		std::filesystem::filesystem_error(makeErrorMessage(path, systemErrorCode), path, { systemErrorCode, std::system_category{} }),
		cause_(FilesystemError::GENERIC)//,
		//message_()
	{
	}

	// FileSystemException(const std::filesystem::path& path, const char* message) noexcept :
	// 	std::filesystem::filesystem_error(makeErrorMessage(path, message), path, {}),
	// 	cause_(),
	// 	message_(message ? message : "")
	// {
	// }

	// FileSystemException(const std::filesystem::path& path, const std::string& message) noexcept :
	// 	std::filesystem::filesystem_error(makeErrorMessage(path, message), path, {}),
	// 	cause_(),
	// 	message_(message)
	// {
	// }

	FilesystemError getCause() const noexcept { return cause_; }

private:
	FilesystemError cause_;
	//std::string message_;
};

} // namespace filesystem

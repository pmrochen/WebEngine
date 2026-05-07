/*
 *	Name: FilesystemError
 *	Author: Pawel Mrochen
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else /* !_WIN32 */
#include <cerrno>
#endif /* _WIN32 */
#include <cstring>
#include <string>
#include <filesystem>
#include "FilesystemError.hpp"

namespace filesystem {

int getSystemErrorCode(FilesystemError error)
{
	static const int systemErrorCodes[] =
	{
#ifdef _WIN32
		-1,							// GENERIC
		ERROR_ALREADY_EXISTS,		// FILE_ALREADY_EXISTS
		ERROR_FILE_NOT_FOUND,		// FILE_NOT_FOUND
		ERROR_TOO_MANY_OPEN_FILES,	// TOO_MANY_OPEN_FILES
		ERROR_ACCESS_DENIED,		// ACCESS_DENIED
		ERROR_PATH_NOT_FOUND,		// INVALID_PATH
		ERROR_INVALID_HANDLE,		// INVALID_HANDLE
		ERROR_SEEK,					// INVALID_OFFSET
		ERROR_DIR_NOT_EMPTY,		// DIRECTORY_NOT_EMPTY
		ERROR_HANDLE_DISK_FULL,		// DISK_FULL
		ERROR_HANDLE_EOF,			// END_OF_FILE
		ERROR_NOT_SUPPORTED			// NOT_SUPPORTED
#else /* !_WIN32 */
		-1,							// GENERIC
		EEXIST,						// FILE_ALREADY_EXISTS
		ENOENT,						// FILE_NOT_FOUND
		EMFILE,						// TOO_MANY_OPEN_FILES
		EACCES,						// ACCESS_DENIED
		ENOENT,						// INVALID_PATH
		EBADF,						// INVALID_HANDLE
		EINVAL,						// INVALID_OFFSET
		ENOTEMPTY,					// DIRECTORY_NOT_EMPTY
		ENOSPC,						// DISK_FULL
		-1,							// END_OF_FILE
		-1							// NOT_SUPPORTED
#endif /* _WIN32 */
	};

	return systemErrorCodes[error];
}

std::string makeErrorMessage(const std::filesystem::path& path, FilesystemError cause/* = FilesystemError::GENERIC*/)
{
	return makeErrorMessage(path, getSystemErrorCode(cause));
}

std::string makeErrorMessage(const std::filesystem::path& path, int systemErrorCode)
{
	auto getSystemMessage = [](int systemError) -> std::string
		{
#ifdef _WIN32
			char* msgBuffer = nullptr;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				0, systemError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgBuffer, 0, 0);
			if (!msgBuffer)
				return std::string("Unknown filesystem error");
			std::string message(msgBuffer);
			LocalFree(msgBuffer);
			while (!message.empty() && ((message[message.length() - 1] == '\n') || (message[message.length() - 1] == '\r')))
				message.erase(message.length() - 1);
			return message;
#else /* !_WIN32 */
			if (systemError == -1)
				return std::string("Unknown filesystem error");
			return std::string(std::strerror(systemError));
#endif /* _WIN32 */
		};

	return '\'' + path.string() + "': " + getSystemMessage(systemErrorCode);
}

// std::string makeErrorMessage(const std::filesystem::path& path, const char* message)
// {
// 	return '\'' + path.string() + "': " + std::string(message ? message : "");
// }

// std::string makeErrorMessage(const std::filesystem::path& path, const std::string& message)
// {
// 	return '\'' + path.string() + "': " + message;
// }

} // namespace filesystem

/*
 *	Name: LowLevelFilesystem
 *	Author: Pawel Mrochen
 */

#ifdef _WIN32
#include <io.h>
#else /* !_WIN32 */
#include <unistd.h>
#endif /* _WIN32 */
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits>
#include "FilesystemError.hpp"
#include "FilesystemException.hpp"
#include "LowLevelFilesystem.hpp"

#ifndef O_SEQUENTIAL
#define O_SEQUENTIAL 0
#endif

#ifndef O_RANDOM
#define O_RANDOM 0
#endif

#ifndef _WIN32

#define _open open
#define _close close
#define _filelengthi64 length
#define _chsize_s ftruncate
#define _telli64 tell
#define _lseeki64 lseek
#define _read read
#define _write write
#define _commit fsync

namespace {

inline off_t length(int fd)
{
	off_t pos = lseek(fd, 0, SEEK_CUR);
	if (pos == (off_t)-1)
		return (off_t)-1;

	off_t len = lseek(fd, 0, SEEK_END);
	if (len == (off_t)-1)
		return (off_t)-1;

	lseek(fd, pos, SEEK_SET);
	return len;
}

inline off_t tell(int fd) 
{ 
	return lseek(fd, 0, SEEK_CUR); 
}

} // anonymous namespace

#endif /* !_WIN32 */

namespace filesystem {

LowLevelFilesystem::LowLevelFilesystem(const PathString& protocol) :
	VirtualFileSystem(protocol)
{
	registerProtocol();
}

LowLevelFilesystem::LowLevelFilesystem(const PathString& protocol, const std::filesystem::path& directory) :
	LowLevelFilesystem(protocol),
	directory_(directory)
{
}

LowLevelFilesystem::LowLevelFilesystem(const PathString& protocol, const PathString& directory) :
	LowLevelFilesystem(protocol),
	directory_(directory)
{
}

const std::filesystem::path& LowLevelFilesystem::getWorkingDirectory() const
{
	return directory_;
}

bool LowLevelFilesystem::exists(const std::filesystem::path& path)
{
	std::filesystem::path fullPath = directory_.empty() ? path : directory_/path;
	int handle = ::_open(fullPath.string().c_str(), O_RDONLY | O_BINARY);
	if (handle == -1)
		return false;

	::_close(handle);
	return true;
}

VirtualFileSystem::IFile* LowLevelFilesystem::open(const std::filesystem::path& path, FileOpenMode mode)
{
	std::filesystem::path fullPath = directory_.empty() ? path : directory_/path;
	int handle = -1;

	if ((int)mode & FileOpenMode::READ)
	{
		handle = ::_open(fullPath.string().c_str(), O_RDONLY | O_BINARY |
			(((int)mode & FileOpenMode::SEQUENTIAL_SCAN) ? O_SEQUENTIAL : 0) |
			(((int)mode & FileOpenMode::RANDOM_ACCESS) ? O_RANDOM : 0));
	}
	else if ((int)mode & FileOpenMode::WRITE)
	{
		handle = ::_open(fullPath.string().c_str(), O_CREAT | O_TRUNC | O_WRONLY | O_BINARY |
			(((int)mode & FileOpenMode::SEQUENTIAL_SCAN) ? O_SEQUENTIAL : 0) |
			(((int)mode & FileOpenMode::RANDOM_ACCESS) ? O_RANDOM : 0), S_IREAD | S_IWRITE);
	}
	else
	{
		throw FilesystemException(fullPath, FilesystemError::NOT_SUPPORTED);
	}

	if (handle == -1)
	{
		switch (errno)
		{
			case EACCES: 
				throw FilesystemException(fullPath, FilesystemError::ACCESS_DENIED);
			case EEXIST: 
				throw FilesystemException(fullPath, FilesystemError::FILE_ALREADY_EXISTS);
			case EINVAL: 
				throw FilesystemException(fullPath, FilesystemError::NOT_SUPPORTED);
			case EMFILE: 
				throw FilesystemException(fullPath, FilesystemError::TOO_MANY_OPEN_FILES);
			case ENOENT: 
				throw FilesystemException(fullPath, ((int)mode & FileOpenMode::READ) ? FilesystemError::FILE_NOT_FOUND : FilesystemError::INVALID_PATH);
			default: 
				throw FilesystemException(fullPath);
		}
	}
	
	return new LowLevelFile { handle, fullPath };
}

void LowLevelFilesystem::close(VirtualFileSystem::IFile* file)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	::_close(lowLevelFile->handle);
	//if (::_close(lowLevelFile->handle) == -1)
	//{
	//	auto path = lowLevelFile->path;
	//	delete file;
	//	
	//	switch (errno)
	//	{
	//		case EBADF: throw FilesystemException(path, FilesystemError::INVALID_HANDLE);
	//		default: throw FilesystemException(path);
	//	}
	//}

	delete lowLevelFile;
}

long long LowLevelFilesystem::getSize(VirtualFileSystem::IFile* file)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	long long length = ::_filelengthi64(lowLevelFile->handle);
	if (length == -1ll)
		return 0ll;

	return length;
}

bool LowLevelFilesystem::canSetSize(VirtualFileSystem::IFile* file)
{
	return true;
}

void LowLevelFilesystem::setSize(VirtualFileSystem::IFile* file, long long size)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	if (::_chsize_s(lowLevelFile->handle, size) != 0)
	{
		switch (errno)
		{
			case EACCES: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::ACCESS_DENIED);
			case EBADF: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::INVALID_HANDLE);
			case ENOSPC: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::DISK_FULL);
			case EINVAL: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::INVALID_OFFSET);
			default: 
				throw FilesystemException(lowLevelFile->path);
		}
	}
}

long long LowLevelFilesystem::tell(VirtualFileSystem::IFile* file)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	long long pos = ::_telli64(lowLevelFile->handle);
	if (pos == -1ll)
		return 0ll;

	return pos;
}

bool LowLevelFilesystem::canSeek(VirtualFileSystem::IFile* file)
{
	return true;
}

long long LowLevelFilesystem::seek(VirtualFileSystem::IFile* file, long long offset, SeekOrigin origin)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	long long pos = ::_lseeki64(lowLevelFile->handle, offset, (int)origin);
	if (pos == -1ll)
	{
		switch (errno)
		{
			case EBADF: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::INVALID_HANDLE);
			case EINVAL: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::INVALID_OFFSET);
			default: 
				throw FilesystemException(lowLevelFile->path);
		}
	}
	
	return pos;
}

bool LowLevelFilesystem::canRead(VirtualFileSystem::IFile* file)
{
	return true;
}

std::size_t LowLevelFilesystem::tryRead(VirtualFileSystem::IFile* file, void* buffer, std::size_t size)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	if (size > (std::size_t)std::numeric_limits<unsigned int>::max()) 
		throw FilesystemException(lowLevelFile->path, FilesystemError::NOT_SUPPORTED);

	int count = ::_read(lowLevelFile->handle, buffer, (unsigned int)size);
	if (count == -1)
	{
		switch (errno)
		{
			case EBADF:
				throw FilesystemException(lowLevelFile->path, FilesystemError::INVALID_HANDLE);
			case EINVAL: 
				throw FilesystemException(lowLevelFile->path);
			default: 
				throw FilesystemException(lowLevelFile->path);
		}
	}
	
	return (std::size_t)(unsigned int)count;
}

void LowLevelFilesystem::read(VirtualFileSystem::IFile* file, void* buffer, std::size_t size)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	if (tryRead(file, buffer, size) != size)
		throw FilesystemException(lowLevelFile->path, FilesystemError::END_OF_FILE);
}

bool LowLevelFilesystem::canWrite(VirtualFileSystem::IFile* file)
{
	return true;
}

void LowLevelFilesystem::write(VirtualFileSystem::IFile* file, const void* buffer, std::size_t size)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	if (size > (std::size_t)std::numeric_limits<unsigned int>::max()) 
		throw FilesystemException(lowLevelFile->path, FilesystemError::NOT_SUPPORTED);
	
	int count = ::_write(lowLevelFile->handle, buffer, (unsigned int)size);
	if (count == -1)
	{
		switch (errno)
		{
			case EBADF: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::INVALID_HANDLE);
			case ENOSPC: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::DISK_FULL);
			case EINVAL: 
				throw FilesystemException(lowLevelFile->path);
			default: 
				throw FilesystemException(lowLevelFile->path);
		}
	}

	if ((std::size_t)(unsigned int)count != size)
		throw FilesystemException(lowLevelFile->path);
}

void LowLevelFilesystem::flush(VirtualFileSystem::IFile* file)
{
	LowLevelFile* lowLevelFile = static_cast<LowLevelFile*>(file);

	if (::_commit(lowLevelFile->handle) == -1)
	{
		switch (errno)
		{
			case EBADF: 
				throw FilesystemException(lowLevelFile->path, FilesystemError::INVALID_HANDLE);
			default: 
				throw FilesystemException(lowLevelFile->path);
		}
	}
}

} // namespace filesystem

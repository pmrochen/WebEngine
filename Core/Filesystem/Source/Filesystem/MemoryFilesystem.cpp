/*
 *	Name: MemoryFilesystem
 *	Author: Pawel Mrochen
 */

#include <algorithm>
#include <utility>
#include <optional>
#include <string>
#include <cstring>
#include "FilesystemError.hpp"
#include "FilesystemException.hpp"
#include "MemoryFilesystem.hpp"

namespace {

std::optional<std::pair<std::size_t, std::size_t>> fileNameToRange(const std::string& fileName)
{
	if (!fileName.empty())
	{
		std::ptrdiff_t commaIndex = fileName.find(',');
		if ((commaIndex > 0) && (commaIndex < ((std::ptrdiff_t)fileName.length() - 1)))
		{
			std::string strBegin(fileName, 0, commaIndex);
			std::string strSize(fileName, commaIndex + 1);
			std::size_t begin = (sizeof(void*) >= sizeof(long long)) ? std::stoull(strBegin, nullptr, 0) : std::stoul(strBegin, nullptr, 0);
			std::size_t end = begin + ((sizeof(void*) >= sizeof(long long)) ? std::stoull(strSize, nullptr, 0) : std::stoul(strSize, nullptr, 0));
			return {{ begin, end }};
		}
		
		std::ptrdiff_t dashIndex = fileName.find('-');
		if ((dashIndex > 0) && (dashIndex < ((std::ptrdiff_t)fileName.length() - 1)))
		{
			std::string strBegin(fileName, 0, dashIndex);
			std::string strEnd(fileName, dashIndex + 1);
			std::size_t begin = (sizeof(void*) >= sizeof(long long)) ? std::stoull(strBegin, nullptr, 0) : std::stoul(strBegin, nullptr, 0);
			std::size_t end = (sizeof(void*) >= sizeof(long long)) ? std::stoull(strEnd, nullptr, 0) : std::stoul(strEnd, nullptr, 0);
			return {{ begin, end }};
		}
	}

	return {};
}

} // anonymous namespace

namespace filesystem {

MemoryFileSystem::MemoryFileSystem(const PathString& protocol, void* data, std::size_t size) :
	VirtualFileSystem(protocol),
	data_((unsigned char*)data),
	size_(size),
	readOnly_(false)
{
	registerProtocol();
}

MemoryFileSystem::MemoryFileSystem(const PathString& protocol, const void* data, std::size_t size) :
	VirtualFileSystem(protocol),
	data_((unsigned char*)data),
	size_(size),
	readOnly_(true)
{
	registerProtocol();
}

bool MemoryFileSystem::exists(const std::filesystem::path& path)
{
	if (!path.empty())
	{
		auto result = fileNameToRange(path.string());
		if (!result.has_value())
			return false;

		auto [begin, end] = result.value();
		if ((begin > end) || (end > size_))
			return false;
	}

	return true;
}

VirtualFileSystem::IFile* MemoryFileSystem::open(const std::filesystem::path& path, FileOpenMode mode)
{
	std::size_t begin = 0;
	std::size_t end = size_;

	if (!path.empty())
	{
		auto result = fileNameToRange(path.string());
		if (!result.has_value())
			throw FilesystemException(path, FilesystemError::FILE_NOT_FOUND);
		
		std::tie(begin, end) = result.value();
		if (begin > end) 
			throw FilesystemException(path, FilesystemError::INVALID_OFFSET);
		if (end > size_)
			throw FilesystemException(path, FilesystemError::END_OF_FILE);
	}

	return new MemoryFile { data_ + begin, end - begin, 0ll, path };
}

void MemoryFileSystem::close(VirtualFileSystem::IFile* file)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);
	delete memoryFile;
}

long long MemoryFileSystem::getSize(VirtualFileSystem::IFile* file)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);
	return (long long)memoryFile->size;
}

long long MemoryFileSystem::tell(VirtualFileSystem::IFile* file)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);
	return memoryFile->pointer;
}

bool MemoryFileSystem::canSeek(VirtualFileSystem::IFile* file)
{
	return true;
}

long long MemoryFileSystem::seek(VirtualFileSystem::IFile* file, long long offset, SeekOrigin origin)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);

	long long newPointer = 0ll;
	switch (origin)
	{
		case SeekOrigin::BEGIN: 
			newPointer = offset; 
			break;
		case SeekOrigin::CURRENT: 
			newPointer = memoryFile->pointer + offset; 
			break;
		case SeekOrigin::END: 
			newPointer = (long long)memoryFile->size + offset; 
			break;
	}

	//newPointer = std::max(newPointer, 0ll);
	if (newPointer < 0ll) 
		throw FilesystemException(memoryFile->path, FilesystemError::INVALID_OFFSET);

	memoryFile->pointer = newPointer;
	return newPointer;
}

bool MemoryFileSystem::canRead(VirtualFileSystem::IFile* file)
{
	return true;
}

std::size_t MemoryFileSystem::tryRead(VirtualFileSystem::IFile* file, void* buffer, std::size_t size)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);
	
	if (memoryFile->pointer > (long long)memoryFile->size) 
		return 0;
	
	std::size_t count = std::min(size, memoryFile->size - (std::size_t)memoryFile->pointer);
	if (count) 
		std::memcpy(buffer, memoryFile->data + memoryFile->pointer, count);

	memoryFile->pointer += count;
	return count;
}

void MemoryFileSystem::read(VirtualFileSystem::IFile* file, void* buffer, std::size_t size)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);

	if (tryRead(file, buffer, size) != size)
		throw FilesystemException(memoryFile->path, FilesystemError::END_OF_FILE);
}

bool MemoryFileSystem::canWrite(VirtualFileSystem::IFile* file)
{
	return !readOnly_;
}

void MemoryFileSystem::write(VirtualFileSystem::IFile* file, const void* buffer, std::size_t size)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);

	if (readOnly_) 
		throw FilesystemException(memoryFile->path, FilesystemError::ACCESS_DENIED);

	if (size)
	{
		if ((memoryFile->pointer + (long long)size) > (long long)memoryFile->size) 
			throw FilesystemException(memoryFile->path, FilesystemError::DISK_FULL);
		
		std::memcpy(memoryFile->data + memoryFile->pointer, buffer, size);
		memoryFile->pointer += size;
	}
}

void MemoryFileSystem::flush(VirtualFileSystem::IFile* file)
{
}

bool MemoryFileSystem::canMap(VirtualFileSystem::IFile* file)
{
	return !readOnly_;
}

void* MemoryFileSystem::map(VirtualFileSystem::IFile* file, long long offset, std::size_t size)
{
	MemoryFile* memoryFile = static_cast<MemoryFile*>(file);
	
	if (readOnly_) 
		throw FilesystemException(memoryFile->path, FilesystemError::ACCESS_DENIED);
	if ((offset < 0ll) || ((offset + (long long)size) > (long long)memoryFile->size))
		throw FilesystemException(memoryFile->path, FilesystemError::INVALID_OFFSET);
	
	return memoryFile->data + (std::ptrdiff_t)offset;
}

void MemoryFileSystem::unmap(VirtualFileSystem::IFile* file, void* ptr)
{
}

void MemoryFileSystem::synchronize(VirtualFileSystem::IFile* file, void* ptr, std::size_t size)
{
}

} // namespace filesystem

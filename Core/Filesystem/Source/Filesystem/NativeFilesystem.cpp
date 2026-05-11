/*
 *	Name: NativeFilesystem
 *	Author: Pawel Mrochen
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
//#include <intsafe.h>
#endif /* _WIN32 */
#include <limits>
#include "FilesystemError.hpp"
#include "FilesystemException.hpp"
#include "NativeFilesystem.hpp"

#ifdef _WIN32
#undef SIZE_T_MAX
#ifdef _WIN64
#define SIZE_T_MAX 0xffffffffffffffffui64
#else
#define SIZE_T_MAX 0xffffffff
#endif /* _WIN64 */
#endif /* _WIN32 */

namespace filesystem {

NativeFilesystem::NativeFilesystem(const PathString& protocol) :
	VirtualFileSystem(protocol)
{
#ifdef _WIN32
	SYSTEM_INFO systemInfo = { 0 };
	GetSystemInfo(&systemInfo);
	allocationGranularity_ = systemInfo.dwAllocationGranularity;
#endif /* _WIN32 */

	registerProtocol();
}

NativeFilesystem::NativeFilesystem(const PathString& protocol, const std::filesystem::path& directory) :
	NativeFilesystem(protocol),
	directory_(directory)
{
}

NativeFilesystem::NativeFilesystem(const PathString& protocol, const PathString& directory) :
	NativeFilesystem(protocol),
	directory_(directory)
{
}

const std::filesystem::path& NativeFilesystem::getWorkingDirectory() const
{
	return directory_;
}

#ifdef _WIN32

bool NativeFilesystem::exists(const std::filesystem::path& path)
{
	std::filesystem::path fullPath = directory_.empty() ? path : directory_/path;
	DWORD attrib = GetFileAttributesW(fullPath.wstring().c_str());
	return ((attrib != INVALID_FILE_ATTRIBUTES) && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

VirtualFileSystem::IFile* NativeFilesystem::open(const std::filesystem::path& path, FileOpenMode mode)
{
	std::filesystem::path fullPath = directory_.empty() ? path : directory_/path;
	HANDLE handle = INVALID_HANDLE_VALUE;

	if ((int)mode & FileOpenMode::READ)
	{
        handle = CreateFileW(fullPath.wstring().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			(((int)mode & FileOpenMode::SEQUENTIAL_SCAN) ? FILE_FLAG_SEQUENTIAL_SCAN : 0) |
			(((int)mode & FileOpenMode::RANDOM_ACCESS) ? FILE_FLAG_RANDOM_ACCESS : 0), NULL);
	}
	else if ((int)mode & FileOpenMode::WRITE)
	{
		handle = CreateFileW(fullPath.wstring().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			(((int)mode & FileOpenMode::SEQUENTIAL_SCAN) ? FILE_FLAG_SEQUENTIAL_SCAN : 0) |
			(((int)mode & FileOpenMode::RANDOM_ACCESS) ? FILE_FLAG_RANDOM_ACCESS : 0), NULL);
	}
	else
	{
		throw FilesystemException(fullPath, FilesystemError::NOT_SUPPORTED);
	}

	if (handle == INVALID_HANDLE_VALUE)
		throw FilesystemException(fullPath, (int)GetLastError());
	
	return new NativeFile { handle, {}, fullPath, mode };
}

void NativeFilesystem::close(VirtualFileSystem::IFile* file)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);

	if (nativeFile->mappingHandle)
		CloseHandle((HANDLE)nativeFile->mappingHandle);

	CloseHandle((HANDLE)nativeFile->handle);
	//if (!CloseHandle((HANDLE)nativeFile->handle))
	//{
	//	auto path = nativeFile->path;
	//	delete file;
	//	throw FilesystemException(path, (int)GetLastError());
	//}

	delete nativeFile;
}

long long NativeFilesystem::getSize(VirtualFileSystem::IFile* file)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	LARGE_INTEGER size;
	size.QuadPart = 0ll;
	if (!GetFileSizeEx((HANDLE)nativeFile->handle, &size))
		return 0ll;

	return size.QuadPart;
}

bool NativeFilesystem::canSetSize(VirtualFileSystem::IFile* file)
{
	return true;
}

void NativeFilesystem::setSize(VirtualFileSystem::IFile* file, long long size)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	LARGE_INTEGER offset, pointer;
	offset.QuadPart = 0ll;
	pointer.QuadPart = 0ll;
	if (!SetFilePointerEx((HANDLE)nativeFile->handle, offset, &pointer, FILE_CURRENT))
		throw FilesystemException(nativeFile->path, (int)GetLastError());

	offset.QuadPart = size;
	if (!SetFilePointerEx((HANDLE)nativeFile->handle, offset, NULL, FILE_BEGIN))
		throw FilesystemException(nativeFile->path, (int)GetLastError());

	if (!SetEndOfFile((HANDLE)nativeFile->handle))
	{
		SetFilePointerEx((HANDLE)nativeFile->handle, pointer, NULL, FILE_BEGIN);
		throw FilesystemException(nativeFile->path, (int)GetLastError());
	}

	SetFilePointerEx((HANDLE)nativeFile->handle, pointer, NULL, FILE_BEGIN);
}

long long NativeFilesystem::tell(VirtualFileSystem::IFile* file)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	LARGE_INTEGER offset, pointer;
	offset.QuadPart = 0ll;
	pointer.QuadPart = 0ll;
	if (!SetFilePointerEx((HANDLE)nativeFile->handle, offset, &pointer, FILE_CURRENT))
		return 0ll;

	return pointer.QuadPart;
}

bool NativeFilesystem::canSeek(VirtualFileSystem::IFile* file)
{
	return true;
}

long long NativeFilesystem::seek(VirtualFileSystem::IFile* file, long long offset, SeekOrigin origin)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	LARGE_INTEGER offs, pointer;
	offs.QuadPart = offset;
	pointer.QuadPart = 0ll;
	if (!SetFilePointerEx((HANDLE)nativeFile->handle, offs, &pointer, (DWORD)origin))
		throw FilesystemException(nativeFile->path, (int)GetLastError());

	return pointer.QuadPart;
}

bool NativeFilesystem::canRead(VirtualFileSystem::IFile* file)
{
	return true;
}

std::size_t NativeFilesystem::tryRead(VirtualFileSystem::IFile* file, void* buffer, std::size_t size)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	if (size > (std::size_t)std::numeric_limits<DWORD>::max()) 
		throw FilesystemException(nativeFile->path, FilesystemError::NOT_SUPPORTED);
	
	DWORD count = 0;
	if (!ReadFile((HANDLE)nativeFile->handle, buffer, (DWORD)size, &count, NULL))
	{
		int error = GetLastError();
		if (error != ERROR_HANDLE_EOF)
			throw FilesystemException(nativeFile->path, error);
	}

	return (std::size_t)count;
}

void NativeFilesystem::read(VirtualFileSystem::IFile* file, void* buffer, std::size_t size)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);

	if (tryRead(file, buffer, size) != size)
		throw FilesystemException(nativeFile->path, FilesystemError::END_OF_FILE);
}

bool NativeFilesystem::canWrite(VirtualFileSystem::IFile* file)
{
	return true;
}

void NativeFilesystem::write(VirtualFileSystem::IFile* file, const void* buffer, std::size_t size)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	if (size > (std::size_t)std::numeric_limits<DWORD>::max()) 
		throw FilesystemException(nativeFile->path, FilesystemError::NOT_SUPPORTED);
	
	DWORD count = 0;
	if (!WriteFile((HANDLE)nativeFile->handle, buffer, (DWORD)size, &count, NULL))
		throw FilesystemException(nativeFile->path, (int)GetLastError());

	if ((std::size_t)count != size)
		throw FilesystemException(nativeFile->path);
}

void NativeFilesystem::flush(VirtualFileSystem::IFile* file)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	if (!FlushFileBuffers((HANDLE)nativeFile->handle))
		throw FilesystemException(nativeFile->path, (int)GetLastError());
}

bool NativeFilesystem::canMap(VirtualFileSystem::IFile* file)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	return ((int)nativeFile->mode & FileOpenMode::READ); //return true;
}

void* NativeFilesystem::map(VirtualFileSystem::IFile* file, long long offset, std::size_t size)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);

	if (offset < 0ll)
		throw FilesystemException(nativeFile->path, FilesystemError::INVALID_OFFSET);
	
	if (size > (std::size_t)SIZE_T_MAX)
		throw FilesystemException(nativeFile->path, FilesystemError::NOT_SUPPORTED);

	if (!size)
		return nullptr;

	if (!nativeFile->mappingHandle)
	{
		HANDLE handle = NULL;

		if ((int)nativeFile->mode & FileOpenMode::READ)
			handle = CreateFileMapping((HANDLE)nativeFile->handle, NULL, PAGE_READONLY, 0, 0, NULL);
		else // PAGE_READWRITE requires GENERIC_READ | GENERIC_WRITE set in CreateFile()
			throw FilesystemException(nativeFile->path, FilesystemError::NOT_SUPPORTED);

		if (!handle)
			throw FilesystemException(nativeFile->path, (int)GetLastError());

		nativeFile->mappingHandle = handle;
	}

	long long alignedOffset = offset & ~(long long)(allocationGranularity_ - 1);
	std::size_t alignedSize = size + (std::ptrdiff_t)(offset - alignedOffset);

	void* ptr = MapViewOfFile(nativeFile->mappingHandle, FILE_MAP_READ /*| FILE_MAP_WRITE*/,
		(DWORD)(alignedOffset >> 32), (DWORD)alignedOffset, alignedSize);
	if (!ptr)
		throw FilesystemException(nativeFile->path, (int)GetLastError());
	
	//assert(((std::size_t)ptr & (allocationGranularity_ - 1)) == 0);
	if (((std::size_t)ptr & (allocationGranularity_ - 1)) != 0)
	{
		UnmapViewOfFile(ptr);
		throw FilesystemException(nativeFile->path/*, "Unaligned pointer returned by MapViewOfFile()."*/);
	}

	return (char*)ptr + (std::ptrdiff_t)(offset - alignedOffset);
}

void NativeFilesystem::unmap(VirtualFileSystem::IFile* file, void* ptr)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);

	if (!ptr)
		return;

	if (!nativeFile->mappingHandle)
		throw FilesystemException(nativeFile->path, FilesystemError::NOT_SUPPORTED);

	void* alignedPtr = (void*)((std::size_t)ptr & ~(allocationGranularity_ - 1));
	UnmapViewOfFile(alignedPtr);
}

void NativeFilesystem::synchronize(VirtualFileSystem::IFile* file, void* ptr, std::size_t size)
{
	NativeFile* nativeFile = static_cast<NativeFile*>(file);
	
	if (size > (std::size_t)SIZE_T_MAX)
		throw FilesystemException(nativeFile->path, FilesystemError::NOT_SUPPORTED);

	if (!nativeFile->mappingHandle)
		throw FilesystemException(nativeFile->path, FilesystemError::NOT_SUPPORTED);
	
	if (!FlushViewOfFile(ptr, size))
		throw FilesystemException(nativeFile->path, (int)GetLastError());
}

#endif /* _WIN32 */

} // namespace filesystem

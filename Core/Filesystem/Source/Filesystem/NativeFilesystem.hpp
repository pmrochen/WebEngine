/*
 *	Name: NativeFilesystem
 *	Author: Pawel Mrochen
 */

#pragma once

#include <filesystem>
#include <cstddef>
#include "FileOpenMode.hpp"
#include "SeekOrigin.hpp"
#include "VirtualFilesystem.hpp"

namespace filesystem {

class NativeFilesystem : public VirtualFilesystem
{
public:
	NativeFilesystem(const PathString& protocol);
	NativeFilesystem(const PathString& protocol, const std::filesystem::path& directory);
	NativeFilesystem(const PathString& protocol, const PathString& directory);

	virtual const std::filesystem::path& getWorkingDirectory() const noexcept override;
#ifdef _WIN32
	virtual bool exists(const std::filesystem::path& path) override;
	virtual IFile* open(const std::filesystem::path& path, FileOpenMode mode) override; // throw (FilesystemException);
	virtual void close(IFile* file) override;
	virtual long long getSize(IFile* file) override;
	virtual bool canSetSize(IFile* file) override;
	virtual void setSize(IFile* file, long long size) override; // throw (FilesystemException);
	virtual long long tell(IFile* file) override;
	virtual bool canSeek(IFile* file) override;
	virtual long long seek(IFile* file, long long offset, SeekOrigin origin) override; // throw (FilesystemException);
	virtual bool canRead(IFile* file) override;
	virtual std::size_t tryRead(IFile* file, void* buffer, std::size_t size) override; // throw (FilesystemException);
	virtual void read(IFile* file, void* buffer, std::size_t size) override; // throw (FilesystemException);
	virtual bool canWrite(IFile* file) override;
	virtual void write(IFile* file, const void* buffer, std::size_t size) override; // throw (FilesystemException);
	virtual void flush(IFile* file) override; // throw (FilesystemException);
	virtual bool canMap(IFile* file) override;
	virtual void* map(IFile* file, long long offset, std::size_t size) override; // throw (FilesystemException);
	virtual void unmap(IFile* file, void* ptr) override;
	virtual void synchronize(IFile* file, void* ptr, std::size_t size) override; // throw (FilesystemException);
#endif /* _WIN32 */

private:
	struct NativeFile : public IFile
	{
		virtual const std::filesystem::path& getPath() const noexcept override { return path; }

#ifdef _WIN32
		/*HANDLE*/void* handle;
		/*HANDLE*/void* mappingHandle;
#endif /* _WIN32 */
		std::filesystem::path path;
		FileOpenMode mode;
	};

	std::filesystem::path directory_;
#ifdef _WIN32
	std::size_t allocationGranularity_ = 0;
#endif /* _WIN32 */
};

} // namespace filesystem

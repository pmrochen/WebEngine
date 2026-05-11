/*
 *	Name: MemoryFilesystem
 *	Author: Pawel Mrochen
 */

#pragma once

#include <filesystem>
#include <cstddef>
#include "FileOpenMode.hpp"
#include "SeekOrigin.hpp"
#include "VirtualFilesystem.hpp"

namespace filesystem {

class MemoryFilesystem : public VirtualFilesystem
{
public:
	MemoryFilesystem(const PathString& protocol, void* data, std::size_t size);
	MemoryFilesystem(const PathString& protocol, const void* data, std::size_t size);

	virtual bool exists(const std::filesystem::path& path) override;
	virtual IFile* open(const std::filesystem::path& path, FileOpenMode mode) override;
	virtual void close(IFile* file) override;
	virtual long long getSize(IFile* file) override;
	virtual long long tell(IFile* file) override;
	virtual bool canSeek(IFile* file) override;
	virtual long long seek(IFile* file, long long offset, SeekOrigin origin) override; // throw (FilesystemException);
	virtual bool canRead(IFile* file) override;
	virtual std::size_t tryRead(IFile* file, void* buffer, std::size_t size) override;
	virtual void read(IFile* file, void* buffer, std::size_t size) override;
	virtual bool canWrite(IFile* file) override;
	virtual void write(IFile* file, const void* buffer, std::size_t size) override; // throw (FilesystemException);
	virtual void flush(IFile* file) override; // throw (FilesystemException);
	virtual bool canMap(IFile* file) override;
	virtual void* map(IFile* file, long long offset, std::size_t size) override; // throw (FilesystemException);
	virtual void unmap(IFile* file, void* ptr) override;
	virtual void synchronize(IFile* file, void* ptr, std::size_t size) override; // throw (FilesystemException);

private:
	struct MemoryFile : public IFile
	{
		virtual const std::filesystem::path& getPath() const noexcept override { return path; }

		unsigned char* data;
		std::size_t size;
		long long pointer;
		std::filesystem::path path;
	};

	unsigned char* data_;
	std::size_t size_;
	bool readOnly_;
};

} // namespace filesystem

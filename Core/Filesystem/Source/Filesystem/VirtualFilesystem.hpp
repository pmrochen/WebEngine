/*
 *	Name: VirtualFilesystem
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <cstddef>
#include <Common/Strings/PathString.hpp>
#include "FileOpenMode.hpp"
#include "SeekOrigin.hpp"

namespace filesystem {

using common::PathString;

class VirtualFilesystem : public std::enable_shared_from_this<VirtualFilesystem>
{
public:
	using OffsetType = long long;
	using SizeType = std::size_t;

	class IFile
	{
	public:
		virtual const std::filesystem::path& getPath() const noexcept = 0;

	protected:
		~IFile() = default;
	};

	VirtualFilesystem(const PathString& protocol) : protocol_(protocol) {}
	virtual ~VirtualFilesystem();

	static std::pair<PathString, std::filesystem::path> decompose(const PathString& uri);
	static VirtualFilesystem* getDefault() noexcept { return defaultFilesystem_; }
	void makeDefault() noexcept { defaultFilesystem_ = this; }
	static VirtualFilesystem* find(const PathString& protocol);
	const PathString& getProtocolName() const noexcept { return protocol_; }
	
	virtual const std::filesystem::path& getWorkingDirectory() const noexcept;
	virtual bool exists(const std::filesystem::path& path);
	virtual IFile* open(const std::filesystem::path& path, FileOpenMode mode); // throw (FilesystemException);
	virtual void close(IFile* file);
	virtual long long getSize(IFile* file);
	virtual bool canSetSize(IFile* file);
	virtual void setSize(IFile* file, long long size); // throw (FilesystemException);
	virtual long long tell(IFile* file);
	virtual bool canSeek(IFile* file);
	virtual long long seek(IFile* file, long long offset, SeekOrigin origin); // throw (FilesystemException);
	virtual bool canRead(IFile* file);
	virtual std::size_t tryRead(IFile* file, void* buffer, std::size_t size); // throw (FilesystemException);
	virtual void read(IFile* file, void* buffer, std::size_t size); // throw (FilesystemException);
	virtual bool canWrite(IFile* file);
	virtual void write(IFile* file, const void* buffer, std::size_t size); // throw (FilesystemException);
	virtual void flush(IFile* file); // throw (FilesystemException);
	virtual bool canMap(IFile* file);
	virtual void* map(IFile* file, long long offset, std::size_t size); // throw (FilesystemException);
	virtual void unmap(IFile* file, void* ptr);
	virtual void synchronize(IFile* file, void* ptr, std::size_t size); // throw (FilesystemException);

protected:
	void registerProtocol();
	void unregisterProtocol();

private:
	VirtualFilesystem(const VirtualFilesystem& filesystem) = delete;
	VirtualFilesystem& operator=(const VirtualFilesystem& filesystem) = delete;

	static VirtualFilesystem* defaultFilesystem_;
	static std::unordered_map<PathString, VirtualFilesystem*> filesystemRegistry_;

	PathString protocol_;
};

} // namespace filesystem

/*
 *	Name: VirtualFilesystem
 *	Author: Pawel Mrochen
 */

#pragma once

//#include <type_traits>
//#include <concepts>
#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
//#include <filesystem>
#include <cstddef>
#include <Common/Strings/PathString.hpp>
#include "FileOpenMode.hpp"
#include "SeekOrigin.hpp"

namespace filesystem {

using common::PathString;

class VirtualFilesystem : public std::enable_shared_from_this<VirtualFilesystem>
{
public:
	using Handle = void*;
	using OffsetType = long long;
	using SizeType = std::size_t;

	VirtualFilesystem(const PathString& name) : name_(name) {}
	virtual ~VirtualFilesystem();

	static std::pair<PathString, PathString> decompose(const PathString& uri);
	static VirtualFilesystem* getDefault() { return defaultFilesystem_; }
	void makeDefault() { defaultFilesystem_ = this; }
	static VirtualFilesystem* find(const PathString& name);
	const PathString& getName() const { return name_; }
	virtual bool exists(const PathString& path);
	virtual Handle open(const PathString& path, FileOpenMode mode); // throw (FilesystemException);
	virtual void close(Handle handle);
	virtual long long getSize(Handle handle);
	virtual bool canSetSize(Handle handle);
	virtual void setSize(Handle handle, long long size); // throw (FilesystemException);
	virtual long long tell(Handle handle);
	virtual bool canSeek(Handle handle);
	virtual long long seek(Handle handle, long long offset, SeekOrigin origin); // throw (FilesystemException);
	virtual bool canRead(Handle handle);
	virtual std::size_t read(Handle handle, void* buffer, std::size_t size); // throw (FilesystemException);
	virtual bool canWrite(Handle handle);
	virtual void write(Handle handle, const void* buffer, std::size_t size); // throw (FilesystemException);
	virtual void flush(Handle handle); // throw (FilesystemException);
	virtual bool canMap(Handle handle);
	virtual void* map(Handle handle, long long offset, std::size_t size); // throw (FilesystemException);
	virtual void unmap(Handle handle, void* ptr);
	virtual void synchronize(Handle handle, void* ptr, std::size_t size); // throw (FilesystemException);

protected:
	void registerProtocol();
	void unregisterProtocol();

private:
	VirtualFilesystem(const VirtualFilesystem& filesystem) = delete;
	VirtualFilesystem& operator=(const VirtualFilesystem& filesystem) = delete;

	using PathStringVirtualFilesystemMap = std::unordered_map<PathString, VirtualFilesystem*>;

	static VirtualFilesystem* defaultFilesystem_;
	static PathStringVirtualFilesystemMap filesystemRegistry_;

	PathString name_;
};

} // namespace filesystem

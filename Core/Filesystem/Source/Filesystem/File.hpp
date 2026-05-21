/*
 *	Name: File
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <concepts>
#include <limits>
#include <algorithm>
#include <string>
#include <filesystem>
#include <cstddef>
#include <Common/Strings/PathString.hpp>
#include "FileOpenMode.hpp"
#include "SeekOrigin.hpp"
#include "VirtualFilesystem.hpp"

namespace filesystem {
namespace detail {

inline std::size_t clampSize(long long x)
{
	return (std::numeric_limits<long long>::max() > std::numeric_limits<std::size_t>::max()) ? 
		(std::size_t)std::min(x, (long long)std::numeric_limits<std::size_t>::max()) : 
		(std::size_t)x;
}

} // namespace detail

template<typename T>
concept AnyString = (std::same_as_v<T, std::string> || std::same_as_v<T, std::wstring> || std::same_as_v<T, std::u8string>);

using common::PathChar;
using common::PathString;

class File
{
public:
	File(const PathChar* uri, FileOpenMode mode); // throw (FilesystemException);
	File(const PathString& uri, FileOpenMode mode); // throw (FilesystemException);
	File(VirtualFilesystem* filesystem, const PathChar* path, FileOpenMode mode); // throw (FilesystemException);
	File(VirtualFilesystem* filesystem, const PathString& path, FileOpenMode mode); // throw (FilesystemException);
	virtual ~File();

	static bool exists(const PathChar* uri);
	static bool exists(const PathString& uri);
	static File* open(const PathChar* uri, FileOpenMode mode) { return new File(uri, mode); } // throw (FilesystemException);
	static File* open(const PathString& uri, FileOpenMode mode) { return new File(uri, mode); } // throw (FilesystemException);
	static File* openRead(const PathChar* uri) { return new File(uri, FileOpenMode::READ); } // throw (FilesystemException);
	static File* openRead(const PathString& uri) { return new File(uri, FileOpenMode::READ); } // throw (FilesystemException);
	static File* create(const PathChar* uri) { return new File(uri, FileOpenMode::WRITE); } // throw (FilesystemException);
	static File* create(const PathString& uri) { return new File(uri, FileOpenMode::WRITE); } // throw (FilesystemException);
	template<AnyString T> static T readAllText(const PathChar* uri);
	template<AnyString T> static T readAllText(const PathString& uri);
	template<AnyString T> static void writeAllText(const PathChar* uri, const T& text);
	template<AnyString T> static void writeAllText(const PathString& uri, const T& text);

	const PathString& getPath() const noexcept { return path_; }
	FileOpenMode getOpenMode() const noexcept { return mode_; }
	VirtualFilesystem* getFilesystem() const noexcept { return filesystem_; }

	bool eof() const { return (tell() >= getSize()); }
	virtual std::size_t getSize() const;
	virtual bool canSetSize();
	virtual void setSize(std::size_t size);
	virtual void reserve(std::size_t size, SeekOrigin origin = SeekOrigin::BEGIN);
	virtual std::size_t tell() const;
	virtual bool canSeek() const;
	virtual std::size_t seek(std::ptrdiff_t offset, SeekOrigin origin = SeekOrigin::BEGIN);
	virtual bool canRead() const;
	virtual std::size_t tryRead(void* buffer, std::size_t size);
	virtual void read(void* buffer, std::size_t size);
	virtual bool canWrite() const;
	virtual void write(const void* buffer, std::size_t size);
	virtual void flush();

protected:
	explicit File(FileOpenMode mode = FileOpenMode::NONE) noexcept : mode_(mode) {}

	PathString uri_;
	PathString protocol_;
	PathString path_;
	std::filesystem::path normalizedPath_;
	FileOpenMode mode_ = {};
	VirtualFilesystem* filesystem_ = nullptr;
	VirtualFilesystem::File* file_ = nullptr;

private:
	File(const File& file) = delete;
	File& operator=(const File& file) = delete;
};

} // namespace filesystem

#include "BufferedFile.hpp"

namespace filesystem {

template<AnyString T> 
/*static*/ inline T File::readAllText(const PathChar* uri)
{
	BufferedFile file(uri, FileOpenMode::READ);
	return file.getText<T>();
}

template<AnyString T> 
/*static*/ inline T File::readAllText(const PathString& uri)
{
	BufferedFile file(uri, FileOpenMode::READ);
	return file.getText<T>();
}

template<AnyString T> 
/*static*/ inline void File::writeAllText(const PathChar* uri, const T& text)
{
	BufferedFile file(uri, FileOpenMode::WRITE);
	file.setText(text);
}

template<AnyString T> 
/*static*/ inline void File::writeAllText(const PathString& uri, const T& text)
{
	BufferedFile file(uri, FileOpenMode::WRITE);
	file.setText(text);
}

} // namespace filesystem

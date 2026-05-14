/*
 *	Name: BufferedFile
 *	Author: Pawel Mrochen
 */

#pragma once

#include <optional>
#include <string>
#include <cstddef>
#include <cstdint>
#include <Common/Strings/PathString.hpp>
#include "FileOpenMode.hpp"
#include "SeekOrigin.hpp"
#include "FilesystemError.hpp"
#include "FilesystemException.hpp"
#include "File.hpp"

namespace filesystem {

template<typename T>
concept Blittable = (std::is_trivial_v<T> && std::is_standard_layout_v<T>);

using common::PathChar;
using common::PathString;

class BufferedFile : public File
{
public:
	explicit BufferedFile(FileOpenMode mode = FileOpenMode::READ | FileOpenMode::WRITE) noexcept : File(mode) {}
	BufferedFile(const PathChar* uri, FileOpenMode mode); // throw (FilesystemException);
	BufferedFile(const PathString& uri, FileOpenMode mode); // throw (FilesystemException);
	virtual ~BufferedFile();

	static BufferedFile* open(const PathChar* uri, FileOpenMode mode) { return new BufferedFile(uri, mode); } // throw (FilesystemException);
	static BufferedFile* open(const PathString& uri, FileOpenMode mode) { return new BufferedFile(uri, mode); } // throw (FilesystemException);
	static BufferedFile* openRead(const PathChar* uri) { return new BufferedFile(uri, FileOpenMode::READ); } // throw (FilesystemException);
	static BufferedFile* openRead(const PathString& uri) { return new BufferedFile(uri, FileOpenMode::READ); } // throw (FilesystemException);
	static BufferedFile* create(const PathChar* uri) { return new BufferedFile(uri, FileOpenMode::WRITE); } // throw (FilesystemException);
	static BufferedFile* create(const PathString& uri) { return new BufferedFile(uri, FileOpenMode::WRITE); } // throw (FilesystemException);

	void* getData() const noexcept { return buffer_; }
	template<typename T> T* getData() const noexcept { return (T*)buffer_; }
	void setData(void* data, std::size_t size);
	template<AnyString T> T getText() const;
	template<> std::string getText() const;
	template<> std::wstring getText() const;
	template<> std::u8string getText() const;
	template<AnyString T> void setText(const T& text);
	template<> void setText(const std::string& text) { setText(text, false); }
	template<> void setText(const std::wstring& text) { setText(text, true); }
	template<> void setText(const std::u8string& text) { setText(text, true); }
	template<AnyString T> void setText(const T& text, bool bom);
	template<> void setText(const std::string& text, bool bom);
	template<> void setText(const std::wstring& text, bool bom);
	template<> void setText(const std::u8string& text, bool bom);

	virtual std::size_t getSize() const override { return size_; }
	virtual bool canSetSize() override;
	virtual void setSize(std::size_t size) override;
	virtual void reserve(std::size_t size, SeekOrigin origin = SeekOrigin::BEGIN) override;
	virtual std::size_t tell() const override { return pointer_; }
	virtual bool canSeek() const override { return true; }
	virtual std::size_t seek(std::ptrdiff_t offset, SeekOrigin origin = SeekOrigin::BEGIN) override;
	virtual bool canRead() const override { return true; }
	virtual std::size_t tryRead(void* buffer, std::size_t size) override;
	virtual void read(void* buffer, std::size_t size) override;
	virtual bool canWrite() const override;
	virtual void write(const void* buffer, std::size_t size) override;
	virtual void flush() override;
	void align(std::size_t boundary);

	template<Blittable T> 
	std::optional<T> peek()
	{
		if ((pointer_ + sizeof(T)) > size_)
			return {};

		return { *(const T*)(buffer_ + pointer_) };
	}

	template<Blittable T> 
	std::optional<T> tryGet()
	{
		if ((pointer_ + sizeof(T)) > size_)
			return {};

		std::size_t base = pointer_;
		pointer_ += sizeof(T);
		return { *(const T*)(buffer_ + base) };
	}

	template<Blittable T> 
	T get()
	{
		if ((pointer_ + sizeof(T)) > size_)
			throw FilesystemException(normalizedPath_, FilesystemError::END_OF_FILE);

		std::size_t base = pointer_;
		pointer_ += sizeof(T);
		return *(const T*)(buffer_ + base);
	}

	template<Blittable T> 
	void put(T value)
	{
		if (!((int)mode_ & FileOpenMode::WRITE))
			throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

		std::size_t base = pointer_;
		std::size_t end = base + sizeof(T);
		if (end > size_) 
			setSize(end);
		pointer_ = end;
		*(T*)(buffer_ + base) = value;
	}

protected:
	static constexpr std::size_t BUFFER_ALIGNMENT = 16;
	static constexpr std::size_t MIN_BUFFER_SIZE = 4096;

	void load();
	void save();

private:
	std::uint8_t* buffer_ = nullptr;
	std::size_t bufferSize_ = 0;
	std::size_t pointer_ = 0;
	std::size_t size_ = 0;
	bool dirty_ = false;
	bool saved_ = false;
};

} // namespace filesystem

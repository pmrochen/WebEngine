/*
 *	Name: BufferedFile
 *	Author: Pawel Mrochen
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif /* _WIN32 */

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM64))
#include <intrin.h> // for _byteswap_xxx
#endif

#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <vector>
#include <algorithm>
#include <bit>
#include "Endianness.hpp"
#include "BufferedFile.hpp"

namespace {

#if defined(__clang__) || defined(__GNUC__)
inline std::uint16_t bswapw(std::uint16_t x) noexcept { return __builtin_bswap16(x); }
//inline std::uint32_t bswapd(std::uint32_t x) noexcept { return __builtin_bswap32(x); }
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM64))
inline std::uint16_t bswapw(std::uint16_t x) noexcept { return _byteswap_ushort(x); }
//inline std::uint32_t bswapd(std::uint32_t x) noexcept { return _byteswap_ulong(x); }
#else
inline std::uint16_t bswapw(std::uint16_t x) noexcept { return (x << 8) | (x >> 8); }
//inline std::uint32_t bswapd(std::uint32_t x) noexcept { return (x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24); }
#endif

inline wchar_t bswap(wchar_t x) noexcept { return (wchar_t)bswapw((std::uint16_t)x); }

} // anonymous namespace

namespace filesystem {

BufferedFile::BufferedFile(const PathChar* uri, FileOpenMode mode) :
File(uri, (mode | FileOpenMode::SEQUENTIAL_SCAN) & ~FileOpenMode::RANDOM_ACCESS)
{
	if ((int)mode_ & FileOpenMode::READ) 
		load();
}

BufferedFile::BufferedFile(const PathString& uri, FileOpenMode mode) :
File(uri, (mode | FileOpenMode::SEQUENTIAL_SCAN) & ~FileOpenMode::RANDOM_ACCESS)
{
	if ((int)mode_ & FileOpenMode::READ) 
		load();
}

BufferedFile::~BufferedFile()
{
	if (((int)mode_ & FileOpenMode::WRITE) && dirty_)
		save();

	if (buffer_) 
		std::_aligned_free(buffer_);
}

void BufferedFile::setData(void* data, std::size_t size)
{
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	setSize(size);

	if (size_) 
	{
		std::memcpy(buffer_, data, size_);
		dirty_ = true;
	}
}

template<>
std::string BufferedFile::getText() const
{
	if (!(buffer_ && size_))
		return {};

	const char* src = (const char*)buffer_;
	std::size_t count = size_;

	if ((count >= 3) && (src[0] == (char)0xEF) && (src[1] == (char)0xBB) && (src[2] == (char)0xBF))
	{
		src += 3;
		count -= 3;
	}

	if (!count)
		return {};

	std::string text;
	text.resize(count);
	char* dest = &text[0];
	
	do
	{
		char c = *src++;
		if (c != '\r') 
			*dest++ = c;
	} while (--count);

	text.resize(dest - &text[0]);
	return text;
}

template<>
std::wstring BufferedFile::getText() const
{
	if (!(buffer_ && size_))
		return {};

	const char* data = (const char*)buffer_;
	std::size_t count = size_;
	bool isUtf8 = false;
	bool isWchar = false;
	bool doSwap = false;

	if ((count >= 3) && (data[0] == (char)0xEF) && (data[1] == (char)0xBB) && (data[2] == (char)0xBF))
	{
		data += 3;
		count -= 3;
		isUtf8 = true;
	}
	else if ((count >= 2) && (data[0] == (char)0xFF) && (data[1] == (char)0xFE))
	{
		data += 2;
		count -= 2;
		isWchar = true;
		doSwap = (endianness() == Endianness::BIG_ENDIAN);
	}
	else if ((count >= 2) && (data[0] == (char)0xFE) && (data[1] == (char)0xFF))
	{
		data += 2;
		count -= 2;
		isWchar = true;
		doSwap = (endianness() == Endianness::LITTLE_ENDIAN);
	}

	if (!count)
		return {};

	std::unique_ptr<wchar_t[]> temp;
	if (!isWchar)
	{
		temp.reset(new wchar_t[count]);
#ifdef _WIN32
		if (!isUtf8)
		{
			count = MultiByteToWideChar(CP_ACP, 0, data, (int)count, temp.get(), (int)count);
			data = (const char*)temp.get();
		}
		else
#endif /* _WIN32 */
		{
			const unsigned char* utf8 = (const unsigned char*)data;
			const unsigned char* utf8End = (const unsigned char*)(data + count);
			wchar_t* out = temp.get();
			while (utf8 != utf8End)
			{
				unsigned char c = *utf8++;
				if ((c & 0x80) == 0)
				{
					*out++ = c;
				}
				else if ((c & 0xE0) == 0xE0)
				{
					*out = (c & 0x1F) << 12;
					if (utf8 != utf8End) 
						*out |= ((*utf8++) & 0x3F) << 6;
					if (utf8 != utf8End) 
						*out |= ((*utf8++) & 0x3F);
					out++;
				}
				else
				{
					*out = (c & 0x3F) << 6;
					if (utf8 != utf8End) 
						*out |= ((*utf8++) & 0x3F);
					out++;
				}
			}

			count = out - temp.get();
			data = (const char*)temp.get();
		}
	}
	else
	{
		count /= sizeof(wchar_t);
	}

	if (!count)
		return {};

	const wchar_t* src = (const wchar_t*)data;
	std::wstring text;
	text.resize(count);
	wchar_t* dest = &text[0];
	
	if (doSwap)
	{
		do
		{
			wchar_t c = bswap(*src++);
			if (c != L'\r') 
				*dest++ = c;
		} while (--count);
	}
	else
	{
		do
		{
			wchar_t c = *src++;
			if (c != L'\r') 
				*dest++ = c;
		} while (--count);
	}

	text.resize(dest - &text[0]);
	return text;
}

template<>
std::u8string BufferedFile::getText() const
{
	if (!(buffer_ && size_))
		return {};

	const char* src = (const char*)buffer_;
	std::size_t count = size_;

	if ((count >= 3) && (src[0] == (char)0xEF) && (src[1] == (char)0xBB) && (src[2] == (char)0xBF))
	{
		src += 3;
		count -= 3;
	}

	if (!count)
		return {};

	std::u8string text;
	text.resize(count);
	char8_t* dest = &text[0];
	
	do
	{
		char c = *src++;
		if (c != '\r') 
			*dest++ = (char8_t)c;
	} while (--count);

	text.resize(dest - &text[0]);
	return text;
}

template<>
void BufferedFile::setText(const std::string& text, bool bom)
{
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	const char* src = &text[0];
	std::size_t count = text.length();
	std::vector<char> chars;
	chars.reserve(3 + count*2);

	if (bom)
	{
		chars.push_back((char)0xEF);
		chars.push_back((char)0xBB);
		chars.push_back((char)0xBF);
	}

	do
	{
		char c = *src++;
		if (c == '\n') 
			chars.push_back('\r');
		chars.push_back(c);
	} while (--count);

	setSize(chars.size());

	if (size_)
	{ 
		std::memcpy(buffer_, &chars[0], size_);
		dirty_ = true;
	}
}

template<>
void BufferedFile::setText(const std::wstring& text, bool bom)
{
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	const wchar_t* src = &text[0];
	size_t count = text.length();
	std::vector<unsigned char> chars;
	chars.reserve(3 + count*6);

	if (bom)
	{
		chars.push_back((char)0xEF);
		chars.push_back((char)0xBB);
		chars.push_back((char)0xBF);
	}

	do
	{
		unsigned short c = (unsigned short)(*src++);

		if (c < 0x0080)
		{
			if (c == (unsigned short)L'\n') 
				chars.push_back((unsigned char)'\r');
			chars.push_back((unsigned char)c);
		}
		else if (c < 0x0800)
		{
			chars.push_back(0xC0 | (c >> 6));
			chars.push_back(0x80 | (c & 0x3F));
		}
		else
		{
			chars.push_back(0xE0 | (c >> 12));
			chars.push_back(0x80 | ((c >> 6) & 0x3F));
			chars.push_back(0x80 | (c & 0x3F));
		}
	} while (--count);

	setSize(chars.size());

	if (size_)
	{ 
		std::memcpy(buffer_, &chars[0], size_);
		dirty_ = true;
	}
}

template<>
void BufferedFile::setText(const std::u8string& text, bool bom)
{
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	const char8_t* src = &text[0];
	std::size_t count = text.length();
	std::vector<char> chars;
	chars.reserve(3 + count*2);

	if (bom)
	{
		chars.push_back((char)0xEF);
		chars.push_back((char)0xBB);
		chars.push_back((char)0xBF);
	}

	do
	{
		char c = (char)(*src++);
		if (c == '\n') 
			chars.push_back('\r');
		chars.push_back(c);
	} while (--count);

	setSize(chars.size());

	if (size_)
	{ 
		std::memcpy(buffer_, &chars[0], size_);
		dirty_ = true;
	}
}

bool BufferedFile::canSetSize()
{
	return ((int)mode_ & FileOpenMode::WRITE);
}

void BufferedFile::setSize(std::size_t size)
{
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (size == size_)
		return;

	std::size_t newSize = size;
	if (newSize > bufferSize_)
	{
		std::size_t newBufferSize = std::max(std::max(bufferSize_*2, newSize), MIN_BUFFER_SIZE);
		std::uint8_t* newBuffer = (std::uint8_t*)std::_aligned_malloc(newBufferSize, BUFFER_ALIGNMENT);
		if (!newBuffer) 
			throw std::bad_alloc();

		std::memset(newBuffer + bufferSize_, 0, newBufferSize - bufferSize_);
		if (buffer_ && bufferSize_) 
			std::memcpy(newBuffer, buffer_, bufferSize_);
		if (buffer_) 
			std::_aligned_free(buffer_);

		buffer_ = newBuffer;
		bufferSize_ = newBufferSize;
	}

	size_ = newSize;
	dirty_ = true;
}

void BufferedFile::reserve(std::size_t size, SeekOrigin origin)
{
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (!size)
		return;

	long long newEnd = 0ll;
	switch (origin)
	{
		case SeekOrigin::BEGIN: 
			newEnd = (long long)size; 
			break;
		case SeekOrigin::CURRENT: 
			newEnd = (long long)pointer_ + (long long)size; 
			break;
		case SeekOrigin::END: 
			newEnd = (long long)size_ + (long long)size; 
			break;
	}

	if (newEnd > 0ll)
	{
		std::size_t end = detail::clampFileSize(newEnd);
		if (end > size_) 
			setSize(end);
	}
}

std::size_t BufferedFile::seek(std::ptrdiff_t offset, SeekOrigin origin)
{
	long long newPointer = 0ll;
	switch (origin)
	{
		case SeekOrigin::BEGIN: 
			newPointer = (long long)offset; 
			break;
		case SeekOrigin::CURRENT: 
			newPointer = (long long)pointer_ + (long long)offset; 
			break;
		case SeekOrigin::END: 
			newPointer = (long long)size_ + (long long)offset; 
			break;
	}

	if (newPointer < 0ll)
		throw FilesystemException(normalizedPath_, FilesystemError::INVALID_OFFSET);

	pointer_ = detail::clampFileSize(newPointer);
	return pointer_;
}

std::size_t BufferedFile::tryRead(void* buffer, std::size_t size)
{
	if (!size || (pointer_ >= size_))
		return 0;

	std::size_t count = std::min(size, size_ - pointer_);
	std::memcpy(buffer, buffer_ + pointer_, count);
	pointer_ += count;
	return count;
}

void BufferedFile::read(void* buffer, std::size_t size)
{
	if (tryRead(buffer, size) != size)
		throw FilesystemException(normalizedPath_, FilesystemError::END_OF_FILE);
}

bool BufferedFile::canWrite() const
{
	return ((int)mode_ & FileOpenMode::WRITE);
}

void BufferedFile::write(const void* buffer, std::size_t size)
{
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (!size)
		return;

	std::size_t base = pointer_;
	std::size_t end = base + size;
	if (end > size_) 
		setSize(end);
	pointer_ = end;
	std::memcpy(buffer_ + base, buffer, size);
	dirty_ = true;
}

void BufferedFile::flush()
{
	if (((int)mode_ & FileOpenMode::WRITE) && dirty_)
	{
		save();
		dirty_ = false;
	}
}

void BufferedFile::align(std::size_t boundary)
{
	if (boundary <= 1)
		return;

	std::size_t bound = std::bit_ceil(boundary);
	std::size_t pos = tell();
	std::size_t alignedPos = ((pos + (bound - 1)) & ~(bound - 1));
	if (pos != alignedPos)
	{
		if (((int)mode_ & FileOpenMode::WRITE) && (alignedPos > getSize())) 
			setSize(alignedPos);
		seek(alignedPos);
	}
}

void BufferedFile::load()
{
	if (!(filesystem_ && file_))
		return;

	if (buffer_) 
		std::_aligned_free(buffer_);
	
	buffer_ = nullptr;
	bufferSize_ = 0;
	pointer_ = 0;
	size_ = 0;

	std::size_t size = detail::clampFileSize(fileSystem_->getSize(file_));
	if (!size)
		return;

	void* data = std::_aligned_malloc(size, BUFFER_ALIGNMENT);
	if (!data) 
		throw std::bad_alloc();

	try
	{
		filesystem_->read(file_, data, size);
	}
	catch (const FilesystemException& error)
	{
		std::_aligned_free(data);
		throw;
	}

	buffer_ = (std::uint8_t*)data;
	bufferSize_ = size;
	size_ = size;

	filesystem_->close(file_);
}

void BufferedFile::save()
{
	if (!(filesystem_ && file_))
		return;
	
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (saved_)
	{
		filesystem_->seek(0);
		filesystem_->setSize(0);
		saved_ = false;
	}

	if (buffer_ && size_)
		filesystem_->write(file_, buffer_, size_);

	saved_ = buffer_ && size_;
}

} // namespace filesystem

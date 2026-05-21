/*
 *	Name: File
 *	Author: Pawel Mrochen
 */

#include <tuple>
#include "FilesystemError.hpp"
#include "FilesystemException.hpp"
#include "Path.hpp"
#include "File.hpp"

namespace filesystem {

File::File(const PathChar* uri, FileOpenMode mode) :
	uri_(uri ? uri : PATH_CSTR("")),
	mode_(mode)
{
	std::tie(protocol_, normalizedPath_) = VirtualFilesystem::decompose(uri_);
	normalizedPath_ = normalizedPath_.lexically_normal();
	path_ = toPathString(normalizedPath_);
	filesystem_ = VirtualFilesystem::find(protocol_);
	file_ = filesystem_->open(normalizedPath_, mode);
}

File::File(const PathString& uri, FileOpenMode mode) :
	uri_(uri),
	mode_(mode)
{
	std::tie(protocol_, normalizedPath_) = VirtualFilesystem::decompose(uri_);
	normalizedPath_ = normalizedPath_.lexically_normal();
	path_ = toPathString(normalizedPath_);
	filesystem_ = VirtualFilesystem::find(protocol_);
	file_ = filesystem_->open(normalizedPath_, mode);
}

File::File(VirtualFilesystem* filesystem, const PathChar* path, FileOpenMode mode) :
	uri_(path ? path : PATH_CSTR("")),
	mode_(mode),
	filesystem_(filesystem)
{
	normalizedPath_ = std::filesystem::path(uri_).lexically_normal();
	path_ = toPathString(normalizedPath_);

	if (!filesystem)
		throw FilesystemException(normalizedPath_, FilesystemError::NOT_SUPPORTED);

	file_ = filesystem_->open(normalizedPath_, mode);
}

File::File(VirtualFilesystem* filesystem, const PathString& path, FileOpenMode mode) :
	uri_(path),
	mode_(mode),
	filesystem_(filesystem)
{
	normalizedPath_ = std::filesystem::path(uri_).lexically_normal();
	path_ = toPathString(normalizedPath_);

	if (!filesystem)
		throw FilesystemException(normalizedPath_, FilesystemError::NOT_SUPPORTED);

	file_ = filesystem_->open(normalizedPath_, mode);
}

File::~File()
{
	if (filesystem_ && file_) 
		filesystem_->close(file_);
}

/*static*/ bool File::exists(const PathChar* uri)
{
	auto [protocol, normalizedPath] = VirtualFilesystem::decompose(PathString(uri ? uri : PATH_CSTR("")));
	normalizedPath = normalizedPath.lexically_normal();
	auto filesystem = VirtualFilesystem::find(protocol);
	return fileSystem->exists(normalizedPath);
}

/*static*/ bool File::exists(const PathString& uri)
{
	auto [protocol, normalizedPath] = VirtualFilesystem::decompose(uri);
	normalizedPath = normalizedPath.lexically_normal();
	auto filesystem = VirtualFilesystem::find(protocol);
	return fileSystem->exists(normalizedPath);
}

std::size_t File::getSize() const
{
	return (filesystem_ && file_) ? detail::clampFileSize(filesystem_->getSize(file_)) : (std::size_t)0;
}

bool File::canSetSize()
{
	return filesystem_ && file_ && ((int)mode_ & FileOpenMode::WRITE) && filesystem_->canSetSize(file_);
}

void File::setSize(std::size_t size)
{
	if (!(filesystem_ && file_))
		return;

	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	filesystem_->setSize(file_, (long long)size);
}

void File::reserve(std::size_t size, SeekOrigin origin)
{
	if (!(filesystem_ && file_))
		return;
	
	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (!size)
		return;

	long long curSize = filesystem_->getSize(file_);
	long long end = 0ll;
	switch (origin)
	{
		case SeekOrigin::BEGIN: 
			end = (long long)size; 
			break;
		case SeekOrigin::CURRENT: 
			end = filesystem_->tell(file_) + (long long)size; 
			break;
		case SeekOrigin::END: 
			end = curSize + (long long)size; 
			break;
	}

	if (end > curSize)
		filesystem_->setSize(file_, end);
}

std::size_t File::tell() const
{
	return (filesystem_ && file_) ? detail::clampFileSize(filesystem_->tell(file_)) : (std::size_t)0;
}

bool File::canSeek() const
{
	return filesystem_ && file_ && filesystem_->canSeek(file_);
}

std::size_t File::seek(std::ptrdiff_t offset, SeekOrigin origin)
{
	if (!(filesystem_ && file_))
		return 0;

	return detail::clampFileSize(filesystem_->seek(file_, (long long)offset, origin));
}

bool File::canRead() const
{
	return filesystem_ && file_ && ((int)mode_ & FileOpenMode::READ) && filesystem_->canRead(file_);
}

std::size_t File::tryRead(void* buffer, std::size_t size)
{
	if (!(filesystem_ && file_))
		return 0;

	if (!((int)mode_ & FileOpenMode::READ))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (!size)
		return 0;

	return filesystem_->tryRead(file_, buffer, size);
}

void File::read(void* buffer, std::size_t size)
{
	if (!(filesystem_ && file_))
		return;

	if (!((int)mode_ & FileOpenMode::READ))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (!size)
		return;

	filesystem_->read(file_, buffer, size);
}

bool File::canWrite() const
{
	return filesystem_ && file_ && ((int)mode_ & FileOpenMode::WRITE) && filesystem_->canWrite(file_);
}

void File::write(const void* buffer, std::size_t size)
{
	if (!(filesystem_ && file_))
		return;

	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	if (!size)
		return;

	filesystem_->write(file_, buffer, size);
}

void File::flush()
{
	if (!(filesystem_ && file_))
		return;

	if (!((int)mode_ & FileOpenMode::WRITE))
		throw FilesystemException(normalizedPath_, FilesystemError::ACCESS_DENIED);

	filesystem_->flush(file_);
}

} // namespace filesystem

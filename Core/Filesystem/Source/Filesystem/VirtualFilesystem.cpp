/*
 *	Name: VirtualFilesystem
 *	Author: Pawel Mrochen
 */

#include "VirtualFilesystem.hpp"
#ifdef _WIN32
#include "NativeFilesystem.hpp"
#else
#include "LowLevelFilesystem.hpp"
#endif
#include "MemoryFilesystem.hpp"

namespace filesystem {
namespace {

#ifdef _WIN32
NativeFilesystem defaultFileVfs({});
#else
LowLevelFilesystem defaultFileVfs({});
#endif
MemoryFilesystem defaultMemoryVfs({}, (void*)nullptr, ~(std::size_t)0);

} // anonymous namespace

/*static*/ VirtualFilesystem* VirtualFilesystem::defaultFilesystem_ = &defaultFileVfs;
/*static*/ VirtualFilesystem::PathStringVirtualFilesystemMap VirtualFilesystem::filesystemRegistry_;

VirtualFilesystem::~VirtualFilesystem()
{
	unregisterProtocol();

	if (defaultFilesystem_ == this)
		defaultFilesystem_ = &defaultFileVfs;
}

/*static*/ std::pair<PathString, PathString> VirtualFilesystem::decompose(const PathString& uri)
{
	std::size_t colonPos = uri.find(PATH_CSTR(':'));
	if ((colonPos != std::string::npos) && (colonPos > 1))
	{
		std::string protocol(uri, 0, colonPos);
		std::string fileName(uri, colonPos + 1, uri.length() - colonPos - 1);
		if (!fileName.empty() && ((fileName[0] == '/') || (fileName[0] == '\\'))) 
			fileName.erase(0, 1);
		if (!fileName.empty() && ((fileName[0] == '/') || (fileName[0] == '\\'))) 
			fileName.erase(0, 1);
		return { protocol, fileName };
	}
	else
	{
		return { {}, uri };
	}
}

/*static*/ VirtualFilesystem* VirtualFilesystem::find(const PathString& name)
{
	if (name.length() > 1)
	{
		if (name == PATH_CSTR("file"))
		{
			return &defaultFileVfs;
		}
		else if (name == PATH_CSTR("mem"))
		{
			return &defaultMemoryVfs;
		}
		else
		{
			auto iVirtualFilesystem = filesystemRegistry_.find(name);
			if (iVirtualFilesystem != filesystemRegistry_.end())
				return iVirtualFilesystem->second;
			else
				return nullptr;
		}
	}

	return defaultFilesystem_;
}

bool VirtualFilesystem::exists(const PathString& path)
{
	return false;
}

VirtualFilesystem::Handle VirtualFilesystem::open(const PathString& path, FileOpenMode mode)
{
	throw FilesystemException(path, FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::close(Handle handle)
{
}

long long VirtualFilesystem::getSize(Handle handle)
{
	try
	{
		long long currentPos = tell(handle);
		long long endPos = seek(handle, 0ll, SeekOrigin::END);
		if (endPos != currentPos) 
			seek(handle, currentPos, SeekOrigin::BEGIN);
		return endPos;
	}
	catch (const FilesystemException&)
	{
		return 0ll;
	}
}

bool VirtualFilesystem::canSetSize(Handle handle)
{
	return false;
}

void VirtualFilesystem::setSize(Handle handle, long long size)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
}

long long VirtualFilesystem::tell(Handle handle)
{
	return 0ll;
}

bool VirtualFilesystem::canSeek(Handle handle)
{
	return false;
}

long long VirtualFilesystem::seek(Handle handle, long long offset, SeekOrigin origin)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
	//return 0ll;
}

bool VirtualFilesystem::canRead(Handle handle)
{
	return false;
}

std::size_t VirtualFilesystem::read(Handle handle, void* buffer, std::size_t size)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
	//return 0u;
}

bool VirtualFilesystem::canWrite(Handle handle)
{
	return false;
}

void VirtualFilesystem::write(Handle handle, const void* buffer, std::size_t size)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
}


void VirtualFilesystem::flush(Handle handle)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
}

bool VirtualFilesystem::canMap(Handle handle)
{
	return false;
}

void* VirtualFilesystem::map(Handle handle, long long offset, std::size_t size)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::unmap(Handle handle, void* ptr)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::synchronize(Handle handle, void* ptr, std::size_t size)
{
	throw FilesystemException({}, FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::registerProtocol()
{
	if ((this != &defaultFileVfs) && (this != &defaultMemoryVfs) && (name_.length() > 1) &&
		(name_ != PATH_CSTR("file")) && (name_ != PATH_CSTR("mem")))
	{
		auto iVirtualFilesystem = filesystemRegistry_.find(name_);
		if (iVirtualFilesystem != filesystemRegistry_.end())
			iVirtualFilesystem->second = this;
		else
			filesystemRegistry_.insert(PathStringVirtualFilesystemMap::value_type(name_, this));
	}
}

void VirtualFilesystem::unregisterProtocol()
{
	if ((this != &defaultFileVfs) && (this != &defaultMemoryVfs) && (name_.length() > 1) &&
		(name_ != PATH_CSTR("file")) && (name_ != PATH_CSTR("mem")))
	{
		for (auto iVirtualFilesystem = filesystemRegistry_.begin(); 
			iVirtualFilesystem != filesystemRegistry_.end(); ++iVirtualFilesystem)
		{
			if (iVirtualFilesystem->second == this)
			{
				filesystemRegistry_.erase(iVirtualFilesystem);
				break;
			}
		}
	}
}

} // namespace filesystem

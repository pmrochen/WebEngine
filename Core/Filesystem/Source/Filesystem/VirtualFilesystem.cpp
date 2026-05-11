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

namespace {

#ifdef _WIN32
NativeFilesystem defaultFileVfs({});
#else
LowLevelFilesystem defaultFileVfs({});
#endif
MemoryFilesystem defaultMemoryVfs({}, (void*)nullptr, ~(std::size_t)0);

} // anonymous namespace

namespace filesystem {

/*static*/ VirtualFilesystem* VirtualFilesystem::defaultFilesystem_ = &defaultFileVfs;
/*static*/ std::unordered_map<PathString, VirtualFilesystem*> VirtualFilesystem::filesystemRegistry_;

VirtualFilesystem::~VirtualFilesystem()
{
	unregisterProtocol();

	if (defaultFilesystem_ == this)
		defaultFilesystem_ = &defaultFileVfs;
}

/*static*/ std::pair<PathString, std::filesystem::path> VirtualFilesystem::decompose(const PathString& uri)
{
	std::size_t colonPos = uri.find(PATH_CHAR(':'));
	if ((colonPos != std::string::npos) && (colonPos > 1))
	{
		PathString protocol(uri, 0, colonPos);
		PathString fileName(uri, colonPos + 1, uri.length() - colonPos - 1);
		if (!fileName.empty() && ((fileName[0] == PATH_CHAR('/')) || (fileName[0] == PATH_CHAR('\\')))) 
			fileName.erase(0, 1);
		if (!fileName.empty() && ((fileName[0] == PATH_CHAR('/')) || (fileName[0] == PATH_CHAR('\\')))) 
			fileName.erase(0, 1);
		return { protocol, { fileName } };
	}
	else
	{
		return { {}, { uri } };
	}
}

/*static*/ VirtualFilesystem* VirtualFilesystem::find(const PathString& protocol)
{
	if (protocol.length() > 1)
	{
		if (protocol == PATH_CSTR("file"))
		{
			return &defaultFileVfs;
		}
		else if (protocol == PATH_CSTR("mem"))
		{
			return &defaultMemoryVfs;
		}
		else
		{
			auto iVirtualFilesystem = filesystemRegistry_.find(protocol);
			return (iVirtualFilesystem != filesystemRegistry_.end()) ?
				iVirtualFilesystem->second :
				nullptr;
		}
	}

	return defaultFilesystem_;
}

const std::filesystem::path& VirtualFilesystem::getWorkingDirectory() const
{
	static const std::filesystem::path empty;
	return empty;
}

bool VirtualFilesystem::exists(const std::filesystem::path& path)
{
	return false;
}

VirtualFilesystem::IFile* VirtualFilesystem::open(const std::filesystem::path& path, FileOpenMode mode)
{
	throw FilesystemException(path, FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::close(VirtualFilesystem::IFile* file)
{
}

long long VirtualFilesystem::getSize(VirtualFilesystem::IFile* file)
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

bool VirtualFilesystem::canSetSize(VirtualFilesystem::IFile* file)
{
	return false;
}

void VirtualFilesystem::setSize(VirtualFilesystem::IFile* file, long long size)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}

long long VirtualFilesystem::tell(VirtualFilesystem::IFile* file)
{
	return 0ll;
}

bool VirtualFilesystem::canSeek(VirtualFilesystem::IFile* file)
{
	return false;
}

long long VirtualFilesystem::seek(VirtualFilesystem::IFile* file, long long offset, SeekOrigin origin)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}

bool VirtualFilesystem::canRead(VirtualFilesystem::IFile* file)
{
	return false;
}

std::size_t VirtualFilesystem::tryRead(VirtualFilesystem::IFile* file, void* buffer, std::size_t size)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::read(VirtualFilesystem::IFile* file, void* buffer, std::size_t size)
{
	if (tryRead(file, buffer, size) != size)
		throw FilesystemException(file->getPath(), FilesystemError::END_OF_FILE);
}

bool VirtualFilesystem::canWrite(VirtualFilesystem::IFile* file)
{
	return false;
}

void VirtualFilesystem::write(VirtualFilesystem::IFile* file, const void* buffer, std::size_t size)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}


void VirtualFilesystem::flush(VirtualFilesystem::IFile* file)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}

bool VirtualFilesystem::canMap(VirtualFilesystem::IFile* file)
{
	return false;
}

void* VirtualFilesystem::map(VirtualFilesystem::IFile* file, long long offset, std::size_t size)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::unmap(VirtualFilesystem::IFile* file, void* ptr)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::synchronize(VirtualFilesystem::IFile* file, void* ptr, std::size_t size)
{
	throw FilesystemException(file->getPath(), FilesystemError::NOT_SUPPORTED);
}

void VirtualFilesystem::registerProtocol()
{
	if ((this != &defaultFileVfs) && (this != &defaultMemoryVfs) && (protocol_.length() > 1) &&
		(protocol_ != PATH_CSTR("file")) && (protocol_ != PATH_CSTR("mem")))
	{
		auto iVirtualFilesystem = filesystemRegistry_.find(protocol_);
		if (iVirtualFilesystem != filesystemRegistry_.end())
			iVirtualFilesystem->second = this;
		else
			filesystemRegistry_.insert({ protocol_, this });
	}
}

void VirtualFilesystem::unregisterProtocol()
{
	if ((this != &defaultFileVfs) && (this != &defaultMemoryVfs) && (protocol_.length() > 1) &&
		(protocol_ != PATH_CSTR("file")) && (protocol_ != PATH_CSTR("mem")))
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

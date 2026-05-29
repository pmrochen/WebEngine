/*
 *	Name: BufferCache
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <vector>
#include <cstddef>
#include "ResourceCache.hpp"

namespace graphics {

class Buffer;

class BufferCache : public ResourceCache<Buffer>
{
public:
	class IBufferResource : IMutableResource
	{
	public:
		using IMutableResource::update;
		virtual void update(Buffer* dataSource, std::ptrdiff_t offset, std::size_t size) = 0;
		virtual void retrieve(Buffer* dataSource/*, bool synchronize*/) = 0;
		//virtual void* map() = 0;
		//virtual void unmap() = 0;
		//virtual void* getPersistentPointer() = 0;
		virtual void swap() = 0;
	};

	// Buffer alignment
	virtual int getStorageBufferOffsetAlignment();
	virtual int getConstantBufferOffsetAlignment();

protected:
	explicit BufferCache(ResourceManager* resourceManager);

	// Get or create
	IBufferResource* getOrCreateResource(Buffer* dataSource, bool update = true);
	IBufferResource* getResource(Buffer* dataSource) const;

	// Create/release
	virtual IBufferResource* createResource(Buffer* dataSource) = 0;
	void releaseResource(Buffer* dataSource);

	// Upload/download
	virtual void upload(Buffer* dataSource);
	virtual void upload(Buffer* dataSource, std::ptrdiff_t offset, std::size_t size);
	virtual void download(Buffer* dataSource/*, bool synchronizeShaderDataStore*/);

private:
	std::vector<std::shared_ptr<IBufferResource>> resources_;
};

} // namespace graphics

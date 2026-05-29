/*
 *	Name: ResourceCache
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
//#include <oup/observable_unique_ptr.hpp>
#include <Common/Strings/NameString.hpp>

namespace graphics {

using common::NameString;

class ResourceManager;

template<typename T>
class ResourceCache //: public oup::enable_observer_from_this_unique<ResourceCache>
{
public:
	using DataSourceType = T;

	class IResource : public std::enable_shared_from_this<IResource>
	{
	public:
		virtual ~IResource() = default;
		virtual const NameString& getName() const = 0;
		virtual unsigned long long getTimestamp() const = 0;
		virtual void setTimestamp(unsigned long long timestamp) = 0;
	};

	class IMutableResource : public IResource
	{
	public:
		virtual bool needsRecreate(DataSourceType* dataSource) const = 0;
		virtual bool needsUpdate(DataSourceType* dataSource) const = 0;
		virtual void update(DataSourceType* dataSource) = 0;
		virtual void copy(IMutableResource<DataSourceType>* target/*, bool synchronize*/) const = 0;
	};

	virtual ~ResourceCache() {}

	virtual bool isEmpty() const;
	virtual void empty(long long longevityInFrames = 0ll);
	virtual void preload(DataSourceType* dataSource);
	virtual void unload(DataSourceType* dataSource);

protected:
	explicit ResourceCache(ResourceManager* resourceManager) noexcept : resourceManager_(resourceManager) {}

private:
	ResourceCache(const ResourceCache&) = delete;
	ResourceCache& operator=(const ResourceCache&) = delete;

	ResourceManager* resourceManager_;
};

} // namespace graphics

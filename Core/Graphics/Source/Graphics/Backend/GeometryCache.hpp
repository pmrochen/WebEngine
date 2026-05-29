/*
 *	Name: GeometryCache
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <vector>
#include <Common/Strings/PathString.hpp>
#include "ResourceCache.hpp"

namespace graphics {

using common::PathString;

class Geometry;

class GeometryCache : public ResourceCache<Geometry>
{
public:
	class IGeometryResource : IMutableResource
	{
	};

protected:
	explicit GeometryCache(ResourceManager* resourceManager);

	// Get or create
	IGeometryResource* getOrCreateResource(Geometry* dataSource, bool update = true);
	IGeometryResource* getResource(Geometry* dataSource) const;

	// Create/release
	virtual IGeometryResource* createResource(Geometry* dataSource) = 0;
	void releaseResource(Geometry* dataSource);

private:
	std::vector<std::shared_ptr<IGeometryResource>> resources_;
	robin_hood::unordered_flat_map<PathString, std::shared_ptr<IGeometryResource>> sharedResources_;
};

} // namespace graphics

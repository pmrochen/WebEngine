/*
 *	Name: TechniqueCache
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <utility>
#include <vector>
#include "../Geometry/GeometryAttributes.hpp"
#include "../Rasterization/FramebufferAttributes.hpp"
#include "../Geometry/VertexLayout.hpp"
#include "../Geometry/PrimitiveTopology.hpp"
#include "ResourceCache.hpp"

namespace graphics {

class Technique;
class ResourceManager;

class TechniqueCache : public ResourceCache<Technique>
{
public:
	class ITechniqueResource : IResource
	{
	};

	virtual void preload(Technique* dataSource) override { preloadAllPermutations(dataSource); }
	virtual void preload(Technique* dataSource, GeometryAttributes geometryAttributes); // throw (std::runtime_error);
	void preloadAllPermutations(Technique* dataSource, GeometryAttributes geometryAttributeMask = GeometryAttributes::ALL); // throw (std::runtime_error);

protected:
	explicit TechniqueCache(ResourceManager* resourceManager);
	TechniqueCache(ResourceManager* resourceManager, PrimitiveTopology primitiveTopology, const VertexLayout& vertexLayout,
		const FramebufferAttributes& framebufferAttributes);

	// Get or create
	ITechniqueResource* getOrCreateResource(Technique* dataSource, GeometryAttributes geometryAttributes); // throw (std::runtime_error);
	ITechniqueResource* getOrCreateResource(Technique* dataSource, GeometryAttributes geometryAttributes,
		PrimitiveTopology primitiveTopology, const VertexLayout& vertexLayout,
		const FramebufferAttributes& framebufferAttributes); // throw (std::runtime_error);
	ITechniqueResource* getResource(Technique* dataSource, GeometryAttributes geometryAttributes) const;
	ITechniqueResource* getResource(Technique* dataSource, GeometryAttributes geometryAttributes, 
		PrimitiveTopology primitiveTopology, const VertexLayout& vertexLayout,
		const FramebufferAttributes& framebufferAttributes) const;

	// Create/release
	virtual ITechniqueResource* createResource(Technique* dataSource); // throw (std::runtime_error);
	virtual ITechniqueResource* createResource(Technique* dataSource, PrimitiveTopology primitiveTopology, 
		const VertexLayout& vertexLayout, const FramebufferAttributes& framebufferAttributes); // throw (std::runtime_error);
	void releaseResource(Technique* dataSource);

	PrimitiveTopology primitiveTopology_;
	VertexLayout vertexLayout_;
	FramebufferAttributes framebufferAttributes_;

private:
	// Load
	ITechniqueResource* loadAndCreateResource(Technique* dataSource);

	std::vector<std::shared_ptr<ITechniqueResource>> resources_;
	//robin_hood::unordered_flat_map<PathString, std::shared_ptr<ITechniqueResource>> sharedResources_;
};

} // namespace graphics

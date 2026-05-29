/*
 *	Name: ResourceManager
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <array>
#include <oup/observable_unique_ptr.hpp>
#include <Common/Strings/LocalString.hpp>
#include "../Rasterization/FramebufferAttributes.hpp"
#include "../Geometry/VertexLayout.hpp"
#include "../Geometry/PrimitiveTopology.hpp"

namespace graphics {

using common::LocalString;

class BufferCache;
class GeometryCache;
class TextureCache;
class ShaderCache;
class TechniqueCache;

class ResourceManager : public oup::enable_observer_from_this_unique<ResourceManager>
{
public:
	virtual ~ResourceManager() {}

	// Current engine
	static ResourceManager* getCurrent() noexcept { return current_; }
	virtual void makeCurrent();

	// Renderer
	// /*virtual*/ Renderer* getRenderer() const noexcept { return renderer`_.get(); }

	// Device info
	virtual LocalString getDeviceName() const { return {}; }

	// Cache
	virtual BufferCache* getBufferCache() const noexcept { return {}; }
	virtual GeometryCache* getGeometryCache() const noexcept { return {}; }
	virtual TextureCache* getTextureCache() const noexcept { return {}; }

	// Texture operations
	virtual void clearPixels(Texture* texture, const Color4& color);
	virtual void copyPixels(Texture* srcTexture, Texture* destTexture, bool synchronizeShaderDataStore);

protected:
	// Get or create
	TechniqueCache* getOrCreateTechniqueCache(PrimitiveTopology primitiveTopology, const VertexLayout& vertexLayout,
		const FramebufferAttributes& framebufferAttributes);
	TechniqueCache* getTechniqueCache(PrimitiveTopology primitiveTopology, const VertexLayout& vertexLayout,
		const FramebufferAttributes& framebufferAttributes) const;

	// Create/release
	virtual TechniqueCache* createTechniqueCache(PrimitiveTopology primitiveTopology, const VertexLayout& vertexLayout,
		const FramebufferAttributes& framebufferAttributes);

private:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	thread_local ResourceManager* current_;

	std::unique_ptr<BufferCache> bufferCache_;
	std::unique_ptr<GeometryCache> geometryCache_;
	std::unique_ptr<TextureCache> textureCache_;
	std::unique_ptr<ShaderCache> shaderCache_;
	std::unique_ptr<TechniqueCache> techniqueCache_;
	robin_hood::unordered_flat_map<std::array<std::size_t, 1 + (sizeof(VertexLayout) + sizeof(FramebufferAttributes))/sizeof(std::size_t)>, std::unique_ptr<TechniqueCache>> techniqueCaches_;
};

} // namespace graphics

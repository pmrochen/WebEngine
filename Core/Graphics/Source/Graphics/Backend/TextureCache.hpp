/*
 *	Name: TextureCache
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <Common/Strings/PathString.hpp>
#include <Mathematics/Algebra/Vector3.hpp>
//#include <Imaging/Geometry/Size.hpp>
#include "ResourceCache.hpp"

namespace graphics {

using common::PathString;
using mathematics::IntVector3;
//using imaging::Size;

class Texture;

class TextureCache : public ResourceCache<Texture>
{
public:
	class ITextureResource : IMutableResource
	{
	public:
		virtual void retrieve(Texture* dataSource/*, bool synchronize*/) = 0;
		virtual IntVector3/*Size*/ getSize() const = 0;
		virtual int getNumberOfMipmapLevels() const = 0;
		virtual bool hasAlphaChannel() const = 0;
		virtual std::uint64_t getNativeHandle() const = 0;
	};

	// Texture limits
	/*virtual*/ int getTextureSizeLimit() const noexcept { return textureSizeLimit_; }
	/*virtual*/ void setTextureSizeLimit(int limit) noexcept { textureSizeLimit_ = std::max(limit, 0); }
	int getNumberOfMipmapLevelsToDiscard() const noexcept { return nMipmapsToDiscard_; }
	void setNumberOfMipmapLevelsToDiscard(int nLevels) noexcept { nMipmapsToDiscard_ = std::max(nLevels, 0); }

protected:
	explicit TextureCache(ResourceManager* resourceManager);

	// Get or create
	ITextureResource* getOrCreateResource(Texture* dataSource, bool update = true);
	ITextureResource* getResource(Texture* dataSource) const;

	// Create/release
	virtual ITextureResource* createResource(Texture* dataSource) = 0;
	void releaseResource(Texture* dataSource);

	// Upload/download
	virtual void upload(Texture* dataSource);
	virtual void download(Texture* dataSource/*, bool synchronizeShaderDataStore*/);

private:
	// Load
	ITextureResource* loadAndCreateResource(Texture* dataSource);

	std::vector<std::shared_ptr<ITextureResource>> resources_;
	robin_hood::unordered_flat_map<PathString, std::shared_ptr<ITextureResource>> sharedResources_;
	int textureSizeLimit_ = 0;
	int nMipmapsToDiscard_ = 0;
};

} // namespace graphics

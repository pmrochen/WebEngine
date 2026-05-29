/*
 *	Name: ShaderCache
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <vector>
#include <Common/Strings/PathString.hpp>
#include "ResourceCache.hpp"

namespace graphics {

using common::PathString;

class Shader;
class ShaderSet;

class ShaderCache : public ResourceCache<Shader>
{
public:
	class IShaderResource : IResource
	{
	};

protected:
	explicit ShaderCache(ResourceManager* resourceManager);

	// Get or create
	IShaderResource* getOrCreateResource(Shader* dataSource); // throw (std::runtime_error);
	IShaderResource* getResource(Shader* dataSource) const;

	// Create/release
	virtual IShaderResource* createResource(Shader* dataSource); // throw (std::runtime_error);

private:
	// Load
	IShaderResource* loadAndCreateResource(Shader* dataSource); 

	robin_hood::unordered_flat_map<PathString, std::unique_ptr<ShaderSet>> shaderSets_;
	robin_hood::unordered_flat_map<PathString, std::shared_ptr<IShaderResource>> sharedResources_;
};

} // namespace graphics

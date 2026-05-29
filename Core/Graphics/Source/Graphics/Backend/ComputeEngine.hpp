/*
 *	Name: ComputeEngine
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cstddef>
#include <oup/observable_unique_ptr.hpp>
#include <Common/Strings/LocalString.hpp>
#include "DispatchSizeMode.hpp"
#include "ResourceManager.hpp"

namespace graphics {

using common::LocalString;

class StorageBuffer;
class ShaderDataSource;
class ComputeTechnique;

class ComputeEngine : public oup::enable_observer_from_this_unique<ComputeEngine>
{
public:
	virtual ~ComputeEngine() {}

	// Current engine
	static ComputeEngine* getCurrent() noexcept { return current_; }
	virtual void makeCurrent();

	// Resource manager
	/*virtual*/ ResourceManager* getResourceManager() const noexcept { return resourceManager_.get(); }

	// Device info
	virtual LocalString getDeviceName() const { return resourceManager_->getDeviceName(); }
	
	// Compute
	virtual void dispatch(ComputeTechnique* technique, ShaderDataSource* dataSource, int sizeX, int sizeY/* = 1*/, int sizeZ/* = 1*/, 
		DispatchSizeMode sizeMode = DispatchSizeMode::INVOCATION_COUNT);
	virtual void dispatch(ComputeTechnique* technique, ShaderDataSource* dataSource, StorageBuffer* indirectBuffer, std::ptrdiff_t offset = 0);

	// Synchronization
	virtual void wait();

protected:
	explicit ComputeEngine(ResourceManager* resourceManager) : 
		resourceManager_(resourceManager->observer_from_this()) 
	{
	}

	oup::observer_ptr<ResourceManager> resourceManager_;

private:
	ComputeEngine(const ComputeEngine&) = delete;
	ComputeEngine& operator=(const ComputeEngine&) = delete;

	thread_local ComputeEngine* current_;
};

} // namespace graphics

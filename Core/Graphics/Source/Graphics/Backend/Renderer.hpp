/*
 *	Name: Renderer
 *	Author: Pawel Mrochen
 */

#pragma once

#include <vector>
#include <span>
#include <utility>
#include <algorithm>
#include <cstddef>
#include <oup/observable_unique_ptr.hpp>
#include <Common/Strings/NameString.hpp>
#include <Common/Strings/LocalString.hpp>
#include <Mathematics/Constants.hpp>
#include <Mathematics/Algebra/Vector2.hpp>
#include <Imaging/Color/ColorMask.hpp>
#include <Imaging/Color/ColorSpace.hpp>
#include <Imaging/Pixel/PixelFormat.hpp>
#include <Imaging/Geometry/Size.hpp>
#include <Imaging/Geometry/Rectangle.hpp>
#include <Imaging/Color/Color4.hpp>
#include "../Rasterization/DepthStencilFormat.hpp"
#include "../Rasterization/MultisampleMode.hpp"
#include "../Geometry/PrimitiveType.hpp"
#include "../Rasterization/ClipOrigin.hpp"
#include "../Rasterization/AttributeMask.hpp"
#include "../Rasterization/FramebufferAccessMode.hpp"
#include "DisplaySynchronizationMode.hpp"
#include "../Rasterization/RasterizerState.hpp"
#include "../Rasterization/ClearOptions.hpp"
#include "../Rasterization/FramebufferAttributes.hpp"
#include "../Rasterization/FramebufferOptions.hpp"
#include "../Rasterization/Viewport.hpp"
#include "../Rasterization/Framebuffer.hpp"
#include "ResourceManager.hpp"

namespace graphics {

using common::NameString;
using common::LocalString;
using mathematics::Constants;
using mathematics::Vector2;
using imaging::ColorMask;
using imaging::ColorSpace;
using imaging::PixelFormat;
using imaging::Size;
using imaging::Rectangle;
using imaging::Color4;

class SwapChain;
class StorageBuffer;
class Geometry;
class RenderTechnique;
class ShaderDataSource;

class Renderer : public oup::enable_observer_from_this_unique<Renderer>
{
public:
	using Real = Vector2::Real;
	using DepthComponentType = Viewport::DepthComponentType;

	class ISemanticDataProvider
	{
	public:
		virtual ~ISemanticDataProvider() {}
		virtual void* get(std::size_t itemIndex, int semantic, int semanticIndex = 0) = 0;
	};

	struct DrawItem
	{
		Geometry* geometry = {};
		RenderTechnique* technique = {};
		ShaderDataSource* dataSource = {};
		unsigned int meshIndex = {};
		unsigned int instanceCount = {};
		RasterizerState rasterizerState;
	};

	virtual ~Renderer() {}

	// Current renderer
	static Renderer* getCurrent() noexcept { return current_; }
	virtual void makeCurrent();

	// Resource manager
	/*virtual*/ ResourceManager* getResourceManager() const noexcept { return resourceManager_.get(); }

	// Device info
	virtual LocalString getDeviceName() const { return resourceManager_->getDeviceName(); }

	// Display properties
	int getDisplayWidth() const { return getDisplaySize().width; }
	int getDisplayHeight() const { return getDisplaySize().height; }
	virtual Size getDisplaySize() const;
	// #TODO setDisplaySize
	// #TODO is/setFullscreenModeEnabled
	Real getPixelAspectRatio() const noexcept { return pixelAspectRatio_; }
	void setPixelAspectRatio(Real aspectRatio) noexcept { pixelAspectRatio_ = std::max(aspectRatio, Constants<Real>::EPSILON); }
	virtual DisplaySynchronizationMode getDisplaySynchronizationMode() const;
	virtual void setDisplaySynchronizationMode(DisplaySynchronizationMode mode);

	// Swap chain 
	virtual SwapChain* getSwapChain() const;
	int getSwapChainWidth() const { return getSwapChainSize().width; }
	int getSwapChainHeight() const { return getSwapChainSize().height; }
	virtual Size getSwapChainSize() const;
	virtual FramebufferAttributes getSwapChainAttributes() const;
	virtual PixelFormat getPixelFormat() const;
	virtual ColorSpace getColorSpace() const;
	virtual DepthStencilFormat getDepthStencilFormat() const;
	virtual MultisampleMode getMultisampleMode() const;

	// Viewport
	virtual const Viewport& getViewport() const;
	virtual void setViewport(const Viewport& viewport);
	virtual void setDefaultViewport();

	// Scissor test
	virtual bool isScissorTestEnabled() const;
	virtual void setScissorTestEnabled(bool enabled);
	virtual const Rectangle& getScissorTestRectangle() const;
	virtual void setScissorTestRectangle(const Rectangle& rect);

	// Clip origin
	virtual ClipOrigin getClipOrigin() const;
	virtual void setClipOrigin(ClipOrigin clipOrigin);

	// Linear-sRGB conversion
	virtual bool isSrgbEncodingEnabled() const;
	virtual void setSrgbEncodingEnabled(bool enabled);

	// Back buffer operations
	void clear(ColorMask colorMask, const Color4& color, bool depthMask = true, 
		DepthComponentType depthValue = DepthComponentType(1), bool stencilMask = true, int stencilValue = 0);
	virtual void clear(const ClearOptions& clearOptions);
	virtual void present();

	// Framebuffer
	/*virtual*/ Framebuffer* getFramebuffer() const noexcept { return framebuffer_.get(); }
	void setFramebuffer(Framebuffer* framebuffer); // throw (std::invalid_argument);
	void setFramebuffer(Framebuffer* framebuffer, FramebufferAccessMode accessMode, AttributeMask attributeMask = AttributeMask::ALL); // throw (std::invalid_argument);
	void setFramebuffer(Framebuffer* framebuffer, const ClearOptions& clearOptions); // throw (std::invalid_argument);
	virtual void setFramebuffer(Framebuffer* framebuffer, const FramebufferOptions& framebufferOptions); // throw (std::invalid_argument);
	void setDefaultFramebuffer() { setFramebuffer(nullptr); }
	int getFramebufferWidth() const { return getFramebufferSize().width; }
	int getFramebufferHeight() const { return getFramebufferSize().height; }
	Size getFramebufferSize() const;
	FramebufferAttributes getFramebufferAttributes() const;
	const Viewport& getFramebufferViewport() const;
	/*virtual*/ const FramebufferOptions& getFramebufferOptions() const { return framebufferOptions_; }
	virtual void discardFramebuffer(Framebuffer* framebuffer, AttributeMask attributeMask);
	virtual void clearFramebuffer(Framebuffer* framebuffer, const ClearOptions& clearOptions);
	void beginPass(Framebuffer* framebuffer /*= nullptr*/); // throw (std::invalid_argument);
	void beginPass(Framebuffer* framebuffer, FramebufferAccessMode accessMode, AttributeMask attributeMask = AttributeMask::ALL); // throw (std::invalid_argument);
	void beginPass(Framebuffer* framebuffer, const ClearOptions& clearOptions); // throw (std::invalid_argument);
	/*virtual*/ void beginPass(Framebuffer* framebuffer, const FramebufferOptions& framebufferOptions); // throw (std::invalid_argument);
	/*virtual*/ void endPass();

	// Drawing
	virtual void draw(std::span<DrawItem> drawItems, ShaderDataSource* dataSource = nullptr, 
		ISemanticDataProvider* dataProvider = nullptr);
	virtual void draw(PrimitiveType primitiveType, unsigned int startIndex, unsigned int nIndices, RenderTechnique* technique, 
		ShaderDataSource* dataSource = nullptr);
	virtual void draw(PrimitiveType primitiveType, StorageBuffer* indirectBuffer, std::ptrdiff_t offset, std::size_t count, 
		RenderTechnique* technique, ShaderDataSource* dataSource = nullptr);

	// Synchronization
	virtual void wait();
	virtual bool isSuspended() const;
	virtual void suspend();
	virtual void resume();

	// Global texture filtering options
	/*virtual*/ int getMaxAnisotropy() const noexcept { return maxAnisotropy_; }
	virtual void setMaxAnisotropy(int maxAnisotropy);
	/*virtual*/ float getMipmapLodBias() const noexcept { return mipLodBias_; }
	virtual void setMipmapLodBias(float mipLodBias);

protected:
	explicit Renderer(ResourceManager* resourceManager) : 
		resourceManager_(resourceManager->observer_from_this()) 
	{
	}

	oup::observer_ptr<ResourceManager> resourceManager_;
	Size displaySize_;
	Real pixelAspectRatio_ = {};
	FramebufferAttributes swapChainAttributes_;
	Viewport viewport_;
	bool scissorTestEnabled_ = false;
	Rectangle scissorTestRectangle_;
	ClipOrigin clipOrigin_;
	bool srgbEncodingEnabled_ = false;
	Framebuffer::weak_ptr framebuffer_;
	FramebufferOptions framebufferOptions_;
	std::vector<std::pair<Framebuffer::weak_ptr, FramebufferOptions>> framebufferStack_;
	int	suspendCount_ = 0;
	int maxAnisotropy_ = 1;
	float mipLodBias_ = 0.0f;
	DisplaySynchronizationMode synchronizationMode_ = {};

private:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	thread_local Renderer* current_;
};

} // namespace graphics

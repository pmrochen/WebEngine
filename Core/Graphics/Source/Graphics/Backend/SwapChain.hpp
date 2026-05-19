/*
 *	Name: SwapChain
 *	Author: Pawel Mrochen
 */

#pragma once

#include <oup/observable_unique_ptr.hpp>
#include <Imaging/Color/ColorSpace.hpp>
#include <Imaging/Pixel/PixelFormat.hpp>
#include <Imaging/Geometry/Size.hpp>
//#include <Imaging/Geometry/Rectangle.hpp>
#include "DepthStencilFormat.hpp"
#include "MultisampleMode.hpp"
#include "AttributeMask.hpp"
#include "FramebufferAttributes.hpp"
//#include "Viewport.hpp"
#include "Renderer.hpp"

namespace graphics {

using imaging::ColorSpace;
using imaging::PixelFormat;
using imaging::Size;
//using imaging::Rectangle;

class RenderBuffer;

class SwapChain : public oup::enable_observer_from_this_unique<SwapChain>
{
public:
	virtual ~SwapChain() {}

	// Properties
	int getWidth() const { return getSize().x; }
	int getHeight() const { return getSize().y; }
	virtual Size getSize() const { return size_; }
	virtual FramebufferAttributes getAttributes() const { return attributes_; }
	virtual PixelFormat getPixelFormat() const { return attributes_.getPixelFormat(); }
	virtual ColorSpace getColorSpace() const { return attributes_.getColorSpace(); }
	virtual DepthStencilFormat getDepthStencilFormat() const { return attributes_.depthStencilFormat; }
	virtual MultisampleMode getMultisampleMode() const { return attributes_.multisampleMode; }

	// Buffers
	virtual std::size_t getColorBufferCount() const { return 0; }
	virtual RenderBuffer* getCurrentColorBuffer() const { return nullptr; }
	//virtual RenderBuffer* getColorBuffer(std::size_t index) const; // #TODO
	virtual RenderBuffer* getDepthStencilBuffer() const { return nullptr; }

	// Aspect ratio
	//float getPixelAspectRatio() const noexcept { return pixelAspectRatio_; }
	//void setPixelAspectRatio(float aspectRatio) noexcept { pixelAspectRatio_ = std::max(aspectRatio, Constants<float>::EPSILON); }

	//// Viewport
	//virtual const Viewport& getViewport() const { return viewport_; }
	//virtual void setViewport(const Viewport& viewport) { viewport_ = viewport; }
	//virtual void setDefaultViewport() {}

	//// Scissor test
	//virtual bool isScissorTestEnabled() const { return scissorTestEnabled_; }
	//virtual void setScissorTestEnabled(bool enabled) { scissorTestEnabled_ = enabled; }
	//virtual const Rectangle& getScissorTestRectangle() const { return scissorTestRectangle_; }
	//virtual void setScissorTestRectangle(const Rectangle& rect) { scissorTestRectangle_ = rect; }

	//// Linear-sRGB conversion
	//virtual bool isSrgbEncodingEnabled() const { return srgbEncodingEnabled_; }
	//virtual void setSrgbEncodingEnabled(bool enabled) { srgbEncodingEnabled_ = enabled; }

	// Synchronization
	//virtual DisplaySynchronizationMode getSynchronizationMode() const { return synchronizationMode_; }
	//virtual void setSynchronizationMode(DisplaySynchronizationMode mode) { synchronizationMode_ = mode; }

protected:
	explicit SwapChain(Renderer* renderer) : 
		renderer_(renderer->observer_from_this()) 
	{
	}
	
	SwapChain(Renderer* renderer, int width, int height, int nColorBuffers, PixelFormat colorFormat, ColorSpace colorSpace,
		DepthStencilFormat depthStencilFormat, MultisampleMode multisampleMode) :
		renderer_(renderer->observer_from_this()),
		size_(width, height),
		attributes_(/*width, height,*/ colorFormat, colorSpace, depthStencilFormat, multisampleMode)//,
		//viewport_(width, height),
		//scissorTestRectangle_(0, 0, width, height),
		//srgbEncodingEnabled_(colorSpace == ColorSpace::SRGB)
	{
	}

	oup::observer_ptr<Renderer>/*Renderer* */ renderer_;
	Size size_;
	FramebufferAttributes attributes_;
	//float pixelAspectRatio_ = 1.0f;
	//Viewport viewport_;
	//bool scissorTestEnabled_ = false;
	//Rectangle scissorTestRectangle_;
	//bool srgbEncodingEnabled_ = false;
	//DisplaySynchronizationMode synchronizationMode_ = DisplaySynchronizationMode::DISABLED;

private:
	SwapChain(const SwapChain&) = delete;
	SwapChain& operator=(const SwapChain&) = delete;
};

} // namespace graphics

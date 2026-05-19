/*
 *	Name: Framebuffer
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <algorithm>
#include <vector>
#include <cstddef>
#include <oup/observable_unique_ptr.hpp>
#include <Common/Collections/ObservableVector.hpp>
#include <Common/Strings/NameString.hpp>
#include <Mathematics/Constants.hpp>
#include <Mathematics/Algebra/Vector2.hpp>
#include <Imaging/ColorSpace.hpp>
#include <Imaging/PixelFormat.hpp>
//#include <Imaging/ImageInterpolationMode.hpp>
#include <Imaging/Size.hpp>
#include <Imaging/Rectangle.hpp>
#include "DepthStencilFormat.hpp"
#include "MultisampleMode.hpp"
#include "ClipOrigin.hpp"
#include "AttributeMask.hpp"
#include "ClearOptions.hpp"
#include "FramebufferAttributes.hpp"
#include "Viewport.hpp"

namespace graphics {

using common::ObservableVector;
using common::NameChar;
using common::NameString;
using mathematics::Constants;
using mathematics::Vector2;
using imaging::ColorSpace;
using imaging::PixelFormat;
//using imaging::ImageInterpolationMode;
using imaging::Size;
using imaging::Rectangle;

class Framebuffer final : public std::enable_shared_from_this<Framebuffer>
{
	friend class RenderBuffer;
	friend class RenderTargetTexture;
	friend class Renderer;

public:
	using RenderBufferVector = ObservableVector<std::shared_ptr<RenderBuffer>>;

	Framebuffer() noexcept {}
	explicit Framebuffer(const NameChar* name) : name(name ? name : NAME_CSTR("")) {}
	explicit Framebuffer(const NameString& name) : name(name) {}
	Framebuffer(const NameString& name, int width, int height, MultisampleMode multisampleMode = MultisampleMode::NONE); 
	Framebuffer(const NameString& name, float widthRatio, float heightRatio, MultisampleMode multisampleMode = MultisampleMode::NONE); 
	~Framebuffer();

	// Create
	static Framebuffer* createRenderBuffers(const NameString& name, int width, int height, std::size_t nColorBuffers, PixelFormat colorFormat,
		ColorSpace colorSpace = ColorSpace::SRGB, DepthStencilFormat depthFormat = DepthStencilFormat::UNSPECIFIED, 
		MultisampleMode multisampleMode = MultisampleMode::NONE);
	static Framebuffer* createRenderBuffers(const NameString& name, float widthRatio, float heightRatio, std::size_t nColorBuffers, 
		PixelFormat colorFormat, ColorSpace colorSpace = ColorSpace::SRGB, DepthStencilFormat depthFormat = DepthStencilFormat::UNSPECIFIED, 
		MultisampleMode multisampleMode = MultisampleMode::NONE);
	static Framebuffer* createRenderTargets(const NameString& name, int width, int height, std::size_t nColorBuffers, PixelFormat colorFormat,
		ColorSpace colorSpace = ColorSpace::SRGB, DepthStencilFormat depthFormat = DepthStencilFormat::UNSPECIFIED, 
		MultisampleMode multisampleMode = MultisampleMode::NONE, int nMipLevels = 1);
	static Framebuffer* createRenderTargets(const NameString& name, float widthRatio, float heightRatio, std::size_t nColorBuffers, 
		PixelFormat colorFormat, ColorSpace colorSpace = ColorSpace::SRGB, DepthStencilFormat depthFormat = DepthStencilFormat::UNSPECIFIED, 
		MultisampleMode multisampleMode = MultisampleMode::NONE, int nMipLevels = 1);

	// Name
	const NameString& getName() const noexcept { return name_; }
	void setName(const NameChar* name) { name_ = name ? name : NAME_CSTR(""); }
	void setName(const NameString& name) { name_ = name; }

	// Associatiom
	bool isBound() const noexcept { return !renderers_.empty(); }
	// #TODO bool isBoundTo(Renderer*)
	bool isMutable() const noexcept { return mutable_; }
	void makeImmutable();

	// Properties
	bool isEmpty() const noexcept { return colorBuffers_.empty() && !depthBuffer_; }
	bool isValid() const;
	int getWidth() const;
	int getWidth(const Viewport& viewport) const;
	int getHeight() const;
	int getHeight(const Viewport& viewport) const;
	Size getSize() const;
	Size getSize(const Viewport& viewport) const;
	float getViewportWidthRatio() const;
	float getViewportHeightRatio() const;
	//Vector2 getViewportSizeRatio() const;
	const FramebufferAttributes& getAttributes() const noexcept { return attributes_; }
	PixelFormat getPixelFormat() const;
	ColorSpace getColorSpace() const;
	DepthStencilFormat getDepthStencilFormat() const noexcept { return attributes_.depthStencilFormat; }
	MultisampleMode getMultisampleMode() const noexcept { return attributes_.multisampleMode; }

	// Buffers
	//RenderBufferIteratorRange getColorBuffers() const noexcept { return RenderBufferIteratorRange(colorBuffers_.begin(), colorBuffers_.end()); }
	std::size_t getColorBufferCount() const noexcept { return colorBuffers_.size(); }
	std::ptrdiff_t getIndexOfColorBuffer(RenderBuffer* buffer) const;
	RenderBuffer* getColorBuffer() const noexcept { return colorBuffers_.empty() ? nullptr : colorBuffers_[0].get(); }
	RenderBuffer* getColorBuffer(std::size_t index) const;
	void setColorBuffer(RenderBuffer* buffer); // throw (std::runtime_error, std::invalid_argument);
	void setColorBuffer(std::size_t index, RenderBuffer* buffer); // throw (std::runtime_error, std::out_of_range, std::invalid_argument);
	void addColorBuffer(RenderBuffer* buffer); // throw (std::runtime_error, std::invalid_argument);
	void insertColorBuffer(std::size_t index, RenderBuffer* buffer); // throw (std::runtime_error, std::out_of_range, std::invalid_argument);
	bool removeColorBuffer(RenderBuffer* buffer); // throw (std::runtime_error);
	bool removeColorBufferAt(std::size_t index); // throw (std::runtime_error);
	void deleteAllColorBuffers(); // throw (std::runtime_error);
	RenderBuffer* getDepthStencilBuffer() const noexcept { return depthBuffer_.get(); }
	void setDepthStencilBuffer(RenderBuffer* buffer); // throw (std::runtime_error, std::invalid_argument);
	void makeUnique();

	// Aspect ratio
	float getPixelAspectRatio() const noexcept { return pixelAspectRatio_; }
	void setPixelAspectRatio(float aspectRatio) noexcept { pixelAspectRatio_ = std::max(aspectRatio, Constants<float>::EPSILON); }

	// Viewport
	const Viewport& getViewport();
	const Viewport& getViewport(const Viewport& mainViewport);
	void setViewport(const Viewport& viewport); // throw (std::runtime_error);
	void setDefaultViewport(); // throw (std::runtime_error);

	// Scissor test
	bool isScissorTestEnabled() const noexcept { return scissorTestEnabled_; }
	void setScissorTestEnabled(bool enabled); // throw (std::runtime_error);
	const Rectangle& getScissorTestRectangle() const noexcept { return scissorTestRectangle_; }
	void setScissorTestRectangle(const Rectangle& rect); // throw (std::runtime_error);

	// Clip origin
	ClipOrigin getClipOrigin() const noexcept { return clipOrigin_; }
	void setClipOrigin(ClipOrigin clipOrigin); // throw (std::runtime_error);

	// Linear-sRGB conversion
	bool isSrgbEncodingEnabled() const noexcept { return srgbEncodingEnabled_; }
	void setSrgbEncodingEnabled(bool enabled); // throw (std::runtime_error);

	// Multiview
	bool isMultiviewRenderingEnabled() const noexcept { return multiviewRenderingEnabled_; }
	void setMultiviewRenderingEnabled(bool enabled);

	// Resource
	// void discard(Renderer* renderer, AttributeMask attributeMask);
	// void clear(Renderer* renderer, const ClearOptions& clearOptions);
	// static void blit(Renderer* renderer, Framebuffer* srcFramebuffer, Framebuffer* destFramebuffer, bool colorMask, bool depthMask,
	// 	bool stencilMask, ImageInterpolationMode interpolation, bool synchronize = false);
	// static void blit(Renderer* renderer, Framebuffer* srcFramebuffer, const Rectangle& srcRectangle, Framebuffer* destFramebuffer, 
	// 	const Rectangle& destRectangle, bool colorMask, bool depthMask, bool stencilMask, ImageInterpolationMode interpolation,
	// 	bool synchronize = false);

private:
	Framebuffer(const Framebuffer& framebuffer);
	Framebuffer& operator=(const Framebuffer&) = delete;

	// Associated renderers
	void addRenderer(Renderer* renderer) { renderers_.push_back(renderer); }
	void removeRenderer(Renderer* renderer);

	// Attributes
	void updateAttributes();

	NameString name_;
	RenderBufferVector colorBuffers_;
	std::shared_ptr<RenderBuffer> depthBuffer_;
	Size size_;
	Vector2 viewportSizeRatio_;
	FramebufferAttributes attributes_;
	float pixelAspectRatio_ = 1.0f;
	Viewport viewport_;
	bool defaultViewport_ = true;
	bool scissorTestEnabled_ = false;
	Rectangle scissorTestRectangle_;
	ClipOrigin clipOrigin_ = ClipOrigin::UNSPECIFIED;
	bool srgbEncodingEnabled_ = true;
	bool multiviewRenderingEnabled_ = false;
	std::vector<oup::observer_ptr<Renderer>/*Renderer* */> renderers_;
	bool mutable_ = true;
};

} // namespace graphics

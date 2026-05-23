/*
 *	Name: Framebuffer
 *	Author: Pawel Mrochen
 */

#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <cstddef>
#include <oup/observable_unique_ptr.hpp>
#include <intrusive_shared_ptr/ref_counted.h>
#include <intrusive_shared_ptr/refcnt_ptr.h>
#include <sigslot/signal.hpp>
#include <Common/Collections/ObservableVector.hpp>
#include <Common/Strings/NameString.hpp>
#include <Mathematics/Constants.hpp>
#include <Mathematics/Algebra/Vector2.hpp>
#include <Imaging/Color/ColorSpace.hpp>
#include <Imaging/Pixel/PixelFormat.hpp>
#include <Imaging/Geometry/Size.hpp>
#include <Imaging/Geometry/Rectangle.hpp>
#include "DepthStencilFormat.hpp"
#include "MultisampleMode.hpp"
#include "ClipOrigin.hpp"
#include "AttributeMask.hpp"
#include "ClearOptions.hpp"
#include "FramebufferAttributes.hpp"
#include "Viewport.hpp"

namespace graphics {

using common::ObservableVector;
using common::NameString;
using mathematics::Constants;
using mathematics::Vector2;
using imaging::ColorSpace;
using imaging::PixelFormat;
using imaging::Size;
using imaging::Rectangle;

class Framebuffer final : public isptr::weak_ref_counted<Framebuffer>
{
public:
	using Real = typename Vector2::Real;
	using RenderBufferVector = ObservableVector<isptr::refcnt_ptr<RenderBuffer>>;

	Framebuffer();
	explicit Framebuffer(const NameString& name);
	explicit Framebuffer(NameString&& name);
	Framebuffer(const NameString& name, int width, int height, MultisampleMode multisampleMode = MultisampleMode::NONE); 
	Framebuffer(const NameString& name, Real widthRatio, Real heightRatio, MultisampleMode multisampleMode = MultisampleMode::NONE); 
	Framebuffer(const Framebuffer& framebuffer);
	Framebuffer(Framebuffer&& framebuffer);
	~Framebuffer();

	Framebuffer& operator=(const Framebuffer& framebuffer); // throw (std::runtime_error);
	Framebuffer& operator=(Framebuffer&& framebuffer); // throw (std::runtime_error);

	// Create
	// template<typename T> // #TODO
	// static Framebuffer* create(const NameString& name, int width, int height, std::size_t nColorBuffers, PixelFormat colorFormat,
	// 	ColorSpace colorSpace = ColorSpace::SRGB, DepthStencilFormat depthFormat = DepthStencilFormat::UNSPECIFIED, 
	// 	MultisampleMode multisampleMode = MultisampleMode::NONE);
	// template<typename T>
	// static Framebuffer* create(const NameString& name, Real widthRatio, Real heightRatio, std::size_t nColorBuffers, 
	// 	PixelFormat colorFormat, ColorSpace colorSpace = ColorSpace::SRGB, DepthStencilFormat depthFormat = DepthStencilFormat::UNSPECIFIED, 
	// 	MultisampleMode multisampleMode = MultisampleMode::NONE);

	// Clone
	Framebuffer* clone() const;

	// Name
	const NameString& getName() const noexcept { return name_; }
	void setName(const NameString& name) { name_ = name; }
	void setName(NameString&& name) { name_ = std::move(name); }

	// Attributes
	bool isEmpty() const noexcept { return colorBuffers_.empty() && !depthBuffer_; }
	bool isValid() const noexcept;
	int getWidth() const noexcept { return getSize().width; }
	int getWidth(const Viewport& viewport) const noexcept { return getSize(viewport).width; }
	int getHeight() const noexcept { return getSize().height; }
	int getHeight(const Viewport& viewport) const noexcept { return getSize(viewport).height; }
	Size getSize() const noexcept;
	Size getSize(const Viewport& viewport) const noexcept;
	Real getViewportWidthRatio() const noexcept { return getViewportSizeRatio().x; }
	Real getViewportHeightRatio() const noexcept { return getViewportSizeRatio().y; }
	Vector2 getViewportSizeRatio() const noexcept;
	const FramebufferAttributes& getAttributes() const noexcept { return attributes_; }
	PixelFormat getPixelFormat() const noexcept;
	ColorSpace getColorSpace() const noexcept;
	DepthStencilFormat getDepthStencilFormat() const noexcept { return attributes_.depthStencilFormat; }
	MultisampleMode getMultisampleMode() const noexcept { return attributes_.multisampleMode; }

	// Buffers
	RenderBufferVector& getColorBuffers() const noexcept { return colorBuffers_; }
	std::size_t getColorBufferCount() const noexcept { return colorBuffers_.size(); }
	std::ptrdiff_t getIndexOfColorBuffer(RenderBuffer* buffer) const noexcept;
	RenderBuffer* getColorBuffer() const noexcept { return colorBuffers_.empty() ? nullptr : colorBuffers_[0].get(); }
	RenderBuffer* getColorBuffer(std::size_t index) const noexcept { return (index < colorBuffers_.size()) ? colorBuffers_[index].get() : nullptr; }
	void setColorBuffer(RenderBuffer* buffer); // throw (std::runtime_error, std::invalid_argument);
	void setColorBuffer(std::size_t index, RenderBuffer* buffer); // throw (std::runtime_error, std::out_of_range, std::invalid_argument);
	void addColorBuffer(RenderBuffer* buffer) { colorBuffers_.emplace_back(buffer); } // throw (std::runtime_error, std::invalid_argument);
	void insertColorBuffer(std::size_t index, RenderBuffer* buffer); // throw (std::runtime_error, std::out_of_range, std::invalid_argument);
	bool removeColorBuffer(RenderBuffer* buffer); // throw (std::runtime_error);
	bool removeColorBufferAt(std::size_t index); // throw (std::runtime_error);
	void deleteAllColorBuffers() { colorBuffers_.resize(0); } // throw (std::runtime_error);
	RenderBuffer* getDepthStencilBuffer() const noexcept { return depthBuffer_.get(); }
	void setDepthStencilBuffer(RenderBuffer* buffer); // throw (std::runtime_error, std::invalid_argument);
	
	// Aspect ratio
	Real getPixelAspectRatio() const noexcept { return pixelAspectRatio_; }
	void setPixelAspectRatio(Real aspectRatio) noexcept { pixelAspectRatio_ = std::max(aspectRatio, Constants<Real>::EPSILON); }

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
	//bool isMultiviewRenderingEnabled() const noexcept { return multiviewRenderingEnabled_; }
	//void setMultiviewRenderingEnabled(bool enabled) noexcept { multiviewRenderingEnabled_ = enabled; }

	// Mutability
	bool isMutable() const noexcept { return mutable_; }
	void makeImmutable() noexcept { mutable_ = false; }

	// Signals
	sigslot::signal<> onViewportChanged;
	sigslot::signal<> onScissorTestEnabledChanged;
	sigslot::signal<> onScissorTestRectangleChanged;
	sigslot::signal<> onClipOriginChanged;
	sigslot::signal<> onSrgbEncodingEnabledChanged;

private:
	// Attributes
	void updateAttributes();

	NameString name_;
	RenderBufferVector colorBuffers_;
	isptr::refcnt_ptr<RenderBuffer> depthBuffer_;
	Size size_;
	Vector2 viewportSizeRatio_;
	FramebufferAttributes attributes_;
	Real pixelAspectRatio_ = Real(1);
	Viewport viewport_;
	bool defaultViewport_ = true;
	bool scissorTestEnabled_ = false;
	Rectangle scissorTestRectangle_;
	ClipOrigin clipOrigin_ = ClipOrigin::UNSPECIFIED;
	bool srgbEncodingEnabled_ = true;
	//bool multiviewRenderingEnabled_ = false;
	//std::vector<oup::observer_ptr<Renderer>/*Renderer* */> renderers_; // #TODO Store weak_ptr<Framebuffer> in Renderer instead
	bool mutable_ = true;
};

} // namespace graphics

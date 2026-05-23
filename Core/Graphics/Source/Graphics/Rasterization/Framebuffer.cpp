/*
 *	Name: Framebuffer
 *	Author: Pawel Mrochen
 */

#include <stdexcept>
#include <limits>
#include "Framebuffer.hpp"
#include <Common/Cloneable.inl>

namespace graphics {

Framebuffer::Framebuffer()
{
	colorBuffers_.onElementAdding.connect([this](const auto& buffer)
		{
			if (!mutable_)
				throw std::runtime_error("Immutable framebuffer");
			if (!buffer)
				throw std::invalid_argument("buffer");
		});

	colorBuffers_.onElementAdded.connect([this](const auto& buffer)
		{
			updateAttributes();
		});

	colorBuffers_.onElementRemoving.connect([this](const auto& buffer)
		{
			if (!mutable_)
				throw std::runtime_error("Immutable framebuffer");
		});

	colorBuffers_.onElementRemoved.connect([this](const auto& buffer)
		{
			updateAttributes();
		});
}

Framebuffer::Framebuffer(const NameString& name) : 
	Framebuffer()
{
	name_ = name;
}

Framebuffer::Framebuffer(NameString&& name) : 
	Framebuffer()
{
	name_ = std::move(name);
}

Framebuffer::Framebuffer(const NameString& name, int width, int height, MultisampleMode multisampleMode) :
	Framebuffer()
{
	name_ = name;
	size_.set(width, height);
	attributes_.multisampleMode = multisampleMode;
}

Framebuffer::Framebuffer(const NameString& name, float widthRatio, float heightRatio, MultisampleMode multisampleMode) :
	Framebuffer()
{
	name_ = name;
	viewportSizeRatio_.set(widthRatio, heightRatio);
	attributes_.multisampleMode = multisampleMode;
}

Framebuffer::Framebuffer(const Framebuffer& framebuffer) :
	Framebuffer()
{
	name_ = framebuffer.name_;
	colorBuffers_ = framebuffer.colorBuffers_;
	depthBuffer_ = framebuffer.depthBuffer_;
	size_ = framebuffer.size_;
	viewportSizeRatio_ = framebuffer.viewportSizeRatio_;
	attributes_ = framebuffer.attributes_;
	pixelAspectRatio_ = framebuffer.pixelAspectRatio_;
	viewport_ = framebuffer.viewport_;
	defaultViewport_ = framebuffer.defaultViewport_;
	scissorTestEnabled_ = framebuffer.scissorTestEnabled_;
	scissorTestRectangle_ = framebuffer.scissorTestRectangle_;
	clipOrigin_ = framebuffer.clipOrigin_;
	srgbEncodingEnabled_ = framebuffer.srgbEncodingEnabled_;
	multiviewRenderingEnabled_ = framebuffer.multiviewRenderingEnabled_;
}

Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
	Framebuffer()
{
	name_ = std::move(framebuffer.name_);
	colorBuffers_ = std::move(framebuffer.colorBuffers_);
	depthBuffer_ = std::move(framebuffer.depthBuffer_);
	size_ = framebuffer.size_;
	viewportSizeRatio_ = framebuffer.viewportSizeRatio_;
	attributes_ = framebuffer.attributes_;
	pixelAspectRatio_ = framebuffer.pixelAspectRatio_;
	viewport_ = framebuffer.viewport_;
	defaultViewport_ = framebuffer.defaultViewport_;
	scissorTestEnabled_ = framebuffer.scissorTestEnabled_;
	scissorTestRectangle_ = framebuffer.scissorTestRectangle_;
	clipOrigin_ = framebuffer.clipOrigin_;
	srgbEncodingEnabled_ = framebuffer.srgbEncodingEnabled_;
	multiviewRenderingEnabled_ = framebuffer.multiviewRenderingEnabled_;
}

Framebuffer::~Framebuffer()
{
	colorBuffers_.onElementAdding.disconnect_all();
	colorBuffers_.onElementAdded.disconnect_all();
	colorBuffers_.onElementRemoving.disconnect_all();
	colorBuffers_.onElementRemoved.disconnect_all();
}

Framebuffer& Framebuffer::operator=(const Framebuffer& framebuffer)
{
	if (!mutable_)
		throw std::runtime_error("Immutable framebuffer");

	colorBuffers_.onElementAdded.block();
	colorBuffers_.onElementRemoved.block();

	try
	{
		name_ = framebuffer.name_;
		colorBuffers_ = framebuffer.colorBuffers_;
		depthBuffer_ = framebuffer.depthBuffer_;
		size_ = framebuffer.size_;
		viewportSizeRatio_ = framebuffer.viewportSizeRatio_;
		attributes_ = framebuffer.attributes_;
		pixelAspectRatio_ = framebuffer.pixelAspectRatio_;
		viewport_ = framebuffer.viewport_;
		defaultViewport_ = framebuffer.defaultViewport_;
		scissorTestEnabled_ = framebuffer.scissorTestEnabled_;
		scissorTestRectangle_ = framebuffer.scissorTestRectangle_;
		clipOrigin_ = framebuffer.clipOrigin_;
		srgbEncodingEnabled_ = framebuffer.srgbEncodingEnabled_;
		multiviewRenderingEnabled_ = framebuffer.multiviewRenderingEnabled_;
	}
	finally
	{
		colorBuffers_.onElementAdded.unblock();
		colorBuffers_.onElementRemoved.unblock();
	}

	updateAttributes();
	return *this;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer)
{
	if (!mutable_)
		throw std::runtime_error("Immutable framebuffer");

	colorBuffers_.onElementAdded.block();
	colorBuffers_.onElementRemoved.block();

	try
	{
		name_ = std::move(framebuffer.name_);
		colorBuffers_ = std::move(framebuffer.colorBuffers_);
		depthBuffer_ = std::move(framebuffer.depthBuffer_);
		size_ = framebuffer.size_;
		viewportSizeRatio_ = framebuffer.viewportSizeRatio_;
		attributes_ = framebuffer.attributes_;
		pixelAspectRatio_ = framebuffer.pixelAspectRatio_;
		viewport_ = framebuffer.viewport_;
		defaultViewport_ = framebuffer.defaultViewport_;
		scissorTestEnabled_ = framebuffer.scissorTestEnabled_;
		scissorTestRectangle_ = framebuffer.scissorTestRectangle_;
		clipOrigin_ = framebuffer.clipOrigin_;
		srgbEncodingEnabled_ = framebuffer.srgbEncodingEnabled_;
		multiviewRenderingEnabled_ = framebuffer.multiviewRenderingEnabled_;
	}
	finally
	{
		colorBuffers_.onElementAdded.unblock();
		colorBuffers_.onElementRemoved.unblock();
	}

	updateAttributes();
	return *this;
}

Framebuffer* Framebuffer::clone() const
{
	Framebuffer* framebuffer = new Framebuffer(*this);

	framebuffer->colorBuffers_.onElementAdded.block();
	framebuffer->colorBuffers_.onElementRemoved.block();

	try
	{
		framebuffer->colorBuffers_ = common::cloneable::clone(framebuffer->colorBuffers_);
		framebuffer->depthBuffer_ = common::cloneable::clone(framebuffer->depthBuffer_);
	}
	finally
	{
		framebuffer->colorBuffers_.onElementAdded.unblock();
		framebuffer->colorBuffers_.onElementRemoved.unblock();
	}

	//framebuffer->updateAttributes();
	return framebuffer;
}

bool Framebuffer::isValid() const
{
	//auto size = getSize();
	MultisampleMode multisampleMode = getMultisampleMode();

	for (const auto& colorBuffer : colorBuffers_)
	{
		if (/*(colorBuffer->getSize() != size) ||*/ (colorBuffer->getMultisampleMode() != multisampleMode))
			return false;
		if ((colorBuffer->getPixelFormat() == PixelFormat::UNSPECIFIED) || (colorBuffer->getDepthStencilFormat() != PixelFormat::UNSPECIFIED))
			return false;
	}

	if (depthBuffer_)
	{
		if (/*(depthBuffer_->getSize() != size) ||*/ (depthBuffer_->getMultisampleMode() != multisampleMode))
			return false;
		if ((depthBuffer_->getPixelFormat() != PixelFormat::UNSPECIFIED) || !depthBuffer_->getDepthStencilFormat().hasDepth())
			return false;
	}

	return true;
}

Size Framebuffer::getSize() const
{
	if (!size_.isZero())
		return size_;
	if (colorBuffers_.empty() && !depthBuffer_)
		return {};

	int w = std::numeric_limits<int>::max();
	int h = std::numeric_limits<int>::max();
	for (const auto& colorBuffer : colorBuffers_)
	{
		w = std::min(w, colorBuffer->getWidth());
		h = std::min(h, colorBuffer->getHeight());
	}
	
	if (depthBuffer_)
	{
		w = std::min(w, depthBuffer_->getWidth());
		h = std::min(h, depthBuffer_->getHeight());
	}

	return { w, h };
}

Size Framebuffer::getSize(const Viewport& viewport) const
{
	//if (!size_.isZero()) // #TODO
	//	...
	if (colorBuffers_.empty() && !depthBuffer_)
		return {};

	int w = std::numeric_limits<int>::max();
	int h = std::numeric_limits<int>::max();
	for (const auto& colorBuffer : colorBuffers_)
	{
		w = std::min(w, colorBuffer->getWidth(viewport));
		h = std::min(h, colorBuffer->getHeight(viewport));
	}

	if (depthBuffer_)
	{
		w = std::min(w, depthBuffer_->getWidth(viewport));
		h = std::min(h, depthBuffer_->getHeight(viewport));
	}

	return { w, h };
}

Vector2 Framebuffer::getViewportSizeRatio() const
{
	if (!viewportSizeRatio_.isZero())
		return viewportSizeRatio_;
	if (colorBuffers_.empty() && !depthBuffer_)
		return {};

	Vector2 ratio(std::numeric_limits<Real>::max());
	for (const auto& colorBuffer : colorBuffers_)
		ratio = min(ratio, colorBuffer->getViewportSizeRatio());
	if (depthBuffer_)
		ratio = min(ratio, depthBuffer_->getViewportSizeRatio());

	return ratio;
}

PixelFormat Framebuffer::getPixelFormat() const 
{ 
	return colorBuffers_.empty() ? PixelFormat::UNSPECIFIED : colorBuffers_[0]->getPixelFormat(); 
}

ColorSpace Framebuffer::getColorSpace() const
{
	return colorBuffers_.empty() ? ColorSpace() : colorBuffers_[0]->getColorSpace();
}

std::ptrdiff_t Framebuffer::getIndexOfColorBuffer(RenderBuffer* buffer) const 
{
	if (!buffer)
		return (std::ptrdiff_t)-1;
	
	auto iRenderBuffer = std::find(colorBuffers_.begin(), colorBuffers_.end(), buffer);
	return (iRenderBuffer != colorBuffers_.end()) ? std::distance(colorBuffers_.begin(), iRenderBuffer) : (std::ptrdiff_t)-1;
}

void Framebuffer::setColorBuffer(RenderBuffer* buffer)
{
	if (!buffer)
	{
		colorBuffers_.resize(0);
	}
	else if (colorBuffers_.empty())
	{
		colorBuffers_.emplace_back(buffer);
	}
	else
	{
		if (colorBuffers_.size() > 1)
			colorBuffers_.resize(1);
		if (colorBuffers_[0].get() != buffer) 
			colorBuffers_.set(0, buffer);
	}
}

void Framebuffer::setColorBuffer(std::size_t index, RenderBuffer* buffer)
{
	if (index > colorBuffers_.size()) 
		throw std::out_of_range("index");

	if (colorBuffers_[index].get() != buffer)
		colorBuffers_.set(index, buffer);
}

void Framebuffer::insertColorBuffer(std::size_t index, RenderBuffer* buffer)
{
	if (index > colorBuffers_.size()) 
		throw std::out_of_range("index");

	colorBuffers_.emplace(colorBuffers_.begin() + index, buffer);
}

bool Framebuffer::removeColorBuffer(RenderBuffer* buffer)
{
	if (!buffer)
		return false;

	auto iRenderBuffer = std::find(colorBuffers_.rbegin(), colorBuffers_.rend(), buffer);
	if (iRenderBuffer == colorBuffers_.rend()) 
		return false;

	buffer->add_ref();
	try
	{
		colorBuffers_.erase((++iRenderBuffer).base());
	}
	finally
	{
		buffer->sub_ref();
	}

	return true;
}

bool Framebuffer::removeColorBufferAt(std::size_t index)
{
	if (index >= colorBuffers_.size()) 
		return false;

	RenderBuffer* buffer = colorBuffers_[index].get();
	buffer->add_ref();
	try
	{
		colorBuffers_.erase(colorBuffers_.begin() + index);
	}
	finally
	{
		buffer->sub_ref();
	}

	return true;
}

void Framebuffer::setDepthStencilBuffer(RenderBuffer* buffer)
{
	if (depthBuffer_.get() == buffer)
		return;
	
	if (!mutable_)
		throw std::runtime_error("Immutable framebuffer");

	depthBuffer_.reset(buffer);
	updateAttributes();
}

const Viewport& Framebuffer::getViewport()
{
	if (defaultViewport_)
		viewport_.bounds.size = getSize();

	return viewport_;
}

const Viewport& Framebuffer::getViewport(const Viewport& mainViewport)
{
	if (defaultViewport_)
		viewport_.bounds.size = getSize(mainViewport);

	return viewport_;
}

void Framebuffer::setViewport(const Viewport& viewport) 
{ 
	if (defaultViewport_ || (viewport_ != viewport))
	{
		viewport_ = viewport;
		defaultViewport_ = false;
		onViewportChanged();
		// #TODO Restore the previous value if an exception was thrown
	}
}

void Framebuffer::setDefaultViewport()
{
	viewport_.set({ { 0, 0 }, getSize() }, { 0, 1 });
	defaultViewport_ = true;
	onViewportChanged();
	// #TODO Restore the previous value if an exception was thrown
}

void Framebuffer::setScissorTestEnabled(bool enabled)
{ 
	if (scissorTestEnabled_ != enabled)
	{
		scissorTestEnabled_ = enabled;
		onScissorTestEnabledChanged();
		// #TODO Restore the previous value if an exception was thrown
	}
}

void Framebuffer::setScissorTestRectangle(const Rectangle& rect)
{
	if (scissorTestRectangle_ != rect)
	{
		scissorTestRectangle_ = rect;
		onScissorTestRectangleChanged();
		// #TODO Restore the previous value if an exception was thrown
	}
}

void Framebuffer::setClipOrigin(ClipOrigin clipOrigin)
{
	if (clipOrigin_ != clipOrigin)
	{
		clipOrigin_ = clipOrigin;
		onClipOriginChanged();
		// #TODO Restore the previous value if an exception was thrown
	}
}

void Framebuffer::setSrgbEncodingEnabled(bool enabled)
{
	if (srgbEncodingEnabled_ != enabled)
	{
		srgbEncodingEnabled_ = enabled;
		onSrgbEncodingEnabledChanged();
		// #TODO Restore the previous value if an exception was thrown
	}
}

void Framebuffer::updateAttributes()
{
	for (int i = 0; i < FramebufferAttributes::MAX_COLOR_FORMAT_COUNT; i++)
	{
		attributes_.colorFormats[i] = ((std::size_t)i < colorBuffers_.size()) ?
			colorBuffers_[i]->getColorFormat() :
			{};
	}

	attributes_.depthStencilFormat = depthBuffer_ ? depthBuffer_->getDepthStencilFormat() : DepthStencilFormat::UNSPECIFIED;

	int multisampleMode = (int)MultisampleMode::NONE;
	for (const auto& colorBuffer : colorBuffers_)
		multisampleMode = std::max(multisampleMode, (int)colorBuffer->getMultisampleMode());
	if (depthBuffer_)
		multisampleMode = std::max(multisampleMode, (int)depthBuffer_->getMultisampleMode());

	attributes_.multisampleMode = (MultisampleMode)multisampleMode;
}

} // namespace graphics

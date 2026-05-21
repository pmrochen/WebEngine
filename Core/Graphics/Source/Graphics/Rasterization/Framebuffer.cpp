/*
 *	Name: Framebuffer
 *	Author: Pawel Mrochen
 */

#include <stdexcept>
#include "Framebuffer.hpp"
#include <Common/Clonable.inl>

namespace graphics {

Framebuffer::Framebuffer()
{
	colorBuffers_.onElementAdding.connect([](const auto& buffer)
		{
			if (!mutable_)
				throw std::runtime_error("Immutable framebuffer");
			if (!renderers_.empty())
				throw std::runtime_error("Bound framebuffer");
			if (!buffer)
				throw std::invalid_argument("Null buffer");
		});

	colorBuffers_.onElementAdded.connect([this](const auto& buffer)
		{
			updateAttributes();
		});

	colorBuffers_.onElementRemoving.connect([this](const auto& buffer)
		{
			if (!mutable_)
				throw std::runtime_error("Immutable framebuffer");
			if (!renderers_.empty())
				throw std::runtime_error("Bound framebuffer");
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

Framebuffer& Framebuffer::operator=(const Framebuffer& framebuffer)
{
	if (!mutable_)
		throw std::runtime_error("Immutable framebuffer");
	if (!renderers_.empty())
		throw std::runtime_error("Bound framebuffer");

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

	return *this;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer)
{
	if (!mutable_)
		throw std::runtime_error("Immutable framebuffer");
	if (!renderers_.empty())
		throw std::runtime_error("Bound framebuffer");

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

	return *this;
}

Framebuffer* Framebuffer::clone() const
{
	Framebuffer* framebuffer = new Framebuffer(*this);
	framebuffer->colorBuffers_ = common::clonable::clone(framebuffer->colorBuffers_);
	framebuffer->depthBuffer_ = common::clonable::clone(framebuffer->depthBuffer_);
	return framebuffer;
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

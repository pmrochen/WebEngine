/*
 *	Name: Framebuffer
 *	Author: Pawel Mrochen
 */

#include "Framebuffer.hpp"

namespace graphics {

Framebuffer::Framebuffer(const NameString& name, int width, int height, MultisampleMode multisampleMode) :
	name_(name),
	size_(width, height),
	attributes_(multisampleMode)
{
}

Framebuffer::Framebuffer(const NameString& name, float widthRatio, float heightRatio, MultisampleMode multisampleMode) :
	name_(name),
	viewportSizeRatio_(widthRatio, heightRatio),
	attributes_(multisampleMode)
{
}

} // namespace graphics

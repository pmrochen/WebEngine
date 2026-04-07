/*
 *	Name: FramebufferOptions
 *	Author: Pawel Mrochen
 */

#pragma once

#include "FramebufferAccessMode.hpp"
#include "ClearOptions.hpp"

namespace graphics {

//enum class FramebufferFlags
//{
//	NONE = 0x00,
//	ENABLE_SCISSOR_TEST = 0x01,
//	ENABLE_SRGB_ENCODING = 0x02,
//	//ENABLE_DEPTH_CLAMP = 0x04,		// == !ENABLE_DEPTH_CLIP
//	DEFAULT = ENABLE_SRGB_ENCODING
//};

struct FramebufferOptions
{
	bool operator==(const FramebufferOptions& options) const noexcept 
	{
		return (clearOptions == options.clearOptions) && /*(viewport == options.viewport) && 
			(scissorTestRectangle == options.scissorTestRectangle) && (clipOrigin == options.clipOrigin) &&
			(targetMipmapLevel == options.targetMipmapLevel) && (targetLayerIndex == options.targetLayerIndex) &&
			(targetCubemapFace == options.targetCubemapFace) &&*/ (accessMode == options.accessMode) /*&&
			(framebufferFlags == options.framebufferFlags)*/;
	}

	bool operator!=(const FramebufferOptions& options) const noexcept { return !(*this == options); }

	// Properties
	const ClearOptions& getClearOptions() const noexcept { return clearOptions; }
	void setClearOptions(const ClearOptions& options) noexcept { clearOptions = options; }
	//const Viewport& getViewport() const noexcept { return viewport; }
	//void setViewport(const Viewport& viewport) noexcept { this->viewport = viewport; }
	//const Rectangle& getScissorTestRectangle() const noexcept { return scissorTestRectangle; }
	//void setScissorTestRectangle(const Rectangle& rectangle) noexcept { scissorTestRectangle = rectangle; }
	//ClipOrigin getClipOrigin() const noexcept { return clipOrigin; }
	//void setClipOrigin(ClipOrigin clipOrigin) noexcept { this->clipOrigin = clipOrigin; }
	//int getTargetMipmapLevel() const noexcept { return targetMipmapLevel; }
	//void setTargetMipmapLevel(int level) noexcept { targetMipmapLevel = level; }
	//int getTargetLayerIndex() const noexcept { return targetLayerIndex; }
	//void setTargetLayerIndex(int index) noexcept { targetLayerIndex = index; }
	//int getTargetCubemapFace() const noexcept { return targetCubemapFace; }
	//void setTargetCubemapFace(int cubemapFace) noexcept { targetCubemapFace = cubemapFace; }
	FramebufferAccessMode getAccessMode() const noexcept { return accessMode; }
	void setAccessMode(FramebufferAccessMode mode) noexcept { accessMode = mode; }
	//FramebufferFlags getFramebufferFlags() const noexcept { return framebufferFlags; }
	//void setFramebufferFlags(FramebufferFlags flags) noexcept { framebufferFlags = flags; }

	ClearOptions clearOptions;
	//Viewport viewport;
	//Rectangle scissorTestRectangle;
	//ClipOrigin clipOrigin = ClipOrigin::UNSPECIFIED;
	//int targetMipmapLevel = 0;
	//int targetLayerIndex = 0;
	//int targetCubemapFace = 0;
	FramebufferAccessMode accessMode = FramebufferAccessMode::DISCARD;
	//FramebufferFlags framebufferFlags = FramebufferFlags::DEFAULT;
};

} // namespace graphics

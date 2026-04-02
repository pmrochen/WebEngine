/*
 *	Name: RasterizerFlags
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>
#include <ColorMask.hpp>
#include "FrontFace.hpp"
#include "CullFaceMode.hpp"
#include "PolygonFillMode.hpp"
#include "DepthMask.hpp"

namespace graphics {

using ::imaging::ColorMask;

enum class RasterizerFlags
{
	NONE = 0,
	COLOR_MASK_RED = ColorMask::RED,					// 0x0001
	COLOR_MASK_GREEN = ColorMask::GREEN,				// 0x0002
	COLOR_MASK_BLUE = ColorMask::BLUE,					// 0x0004
	COLOR_MASK_ALPHA = ColorMask::ALPHA,				// 0x0008
	COLOR_MASK_ALL = ColorMask::ALL,					// 0x000F
	FRONT_FACE_CCW = FrontFace::CCW,					// 0x0010
	CULL_BACK = CullFaceMode::BACK,						// 0x0020
	CULL_FRONT = CullFaceMode::FRONT,					// 0x0040
	FILL_MODE_WIREFRAME = PolygonFillMode::WIREFRAME,	// 0x0100
	ENABLE_DEPTH_TEST = 0x0800,
	ENABLE_DEPTH_WRITE = DepthMask::ENABLED,			// 0x1000
	//ENABLE_DEPTH_CLAMP = 0x4000,						// == !ENABLE_DEPTH_CLIP
	//ENABLE_MULTISAMPLE = 0x8000,
	DEFAULT = COLOR_MASK_ALL | CULL_BACK | ENABLE_DEPTH_TEST | ENABLE_DEPTH_WRITE /*| ENABLE_MULTISAMPLE*/
};

ENABLE_ENUM_BITWISE_OPERATORS(RasterizerFlags);

inline ColorMask extractColorWriteMask(RasterizerFlags flags) noexcept 
{ 
	return (ColorMask)(flags & RasterizerFlags::COLOR_MASK_ALL);
}

inline RasterizerFlags applyColorWriteMask(RasterizerFlags flags, ColorMask colorWriteMask) noexcept 
{ 
	return (flags & ~RasterizerFlags::COLOR_MASK_ALL) | ((RasterizerFlags)colorWriteMask & RasterizerFlags::COLOR_MASK_ALL);
}

inline FrontFace extractFrontFace(RasterizerFlags flags) noexcept 
{ 
	return (FrontFace)(flags & RasterizerFlags::FRONT_FACE_CCW);
}

inline RasterizerFlags applyFrontFace(RasterizerFlags flags, FrontFace frontFace) noexcept 
{ 
	return (flags & ~RasterizerFlags::FRONT_FACE_CCW) | ((RasterizerFlags)frontFace & RasterizerFlags::FRONT_FACE_CCW);
}

inline CullFaceMode extractCullMode(RasterizerFlags flags) noexcept 
{ 
	return (CullFaceMode)(flags & (RasterizerFlags::CULL_BACK | RasterizerFlags::CULL_FRONT));
}

inline RasterizerFlags applyCullMode(RasterizerFlags flags, CullFaceMode cullMode) noexcept 
{ 
	return (flags & ~(RasterizerFlags::CULL_BACK | RasterizerFlags::CULL_FRONT)) | 
		((RasterizerFlags)cullMode & (RasterizerFlags::CULL_BACK | RasterizerFlags::CULL_FRONT));
}

inline PolygonFillMode extractFillMode(RasterizerFlags flags) noexcept 
{ 
	return (PolygonFillMode)(flags & RasterizerFlags::FILL_MODE_WIREFRAME);
}

inline RasterizerFlags applyFillMode(RasterizerFlags flags, PolygonFillMode fillMode) noexcept 
{ 
	return (flags & ~RasterizerFlags::FILL_MODE_WIREFRAME) | ((RasterizerFlags)fillMode & RasterizerFlags::FILL_MODE_WIREFRAME);
}

inline DepthMask extractDepthWriteMask(RasterizerFlags flags) noexcept 
{ 
	return (DepthMask)(flags & RasterizerFlags::ENABLE_DEPTH_WRITE);
}

inline RasterizerFlags applyDepthWriteMask(RasterizerFlags flags, DepthMask depthWriteMask) noexcept 
{ 
	return (flags & ~RasterizerFlags::ENABLE_DEPTH_WRITE) | ((RasterizerFlags)depthWriteMask & RasterizerFlags::ENABLE_DEPTH_WRITE);
}

} // namespace graphics

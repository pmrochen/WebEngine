/*
 *	Name: ChannelOrder
 *	Author: Pawel Mrochen
 */

#pragma once

#ifndef IMAGING_NATIVE_ORDER_RGBA
#if defined(_WIN32) && !defined(_XBOX) && !defined(_XBOX_ONE)
#define IMAGING_NATIVE_ORDER_RGBA 0
#else
#define IMAGING_NATIVE_ORDER_RGBA 1
#endif
#endif

#ifndef IMAGING_NATIVE_ORDER_BGRA
#define IMAGING_NATIVE_ORDER_BGRA (1 - IMAGING_NATIVE_ORDER_RGBA)
#endif

namespace core::imaging {

enum class ChannelOrder
{
    UNSPECIFIED,
	RGBA,
	BGRA,
#if IMAGING_NATIVE_ORDER_RGBA
	NATIVE = RGBA
#else
	NATIVE = BGRA
#endif
};

} // namespace core::imaging

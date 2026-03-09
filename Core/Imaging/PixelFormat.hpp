/*
 *	Name: PixelFormat
 *	Author: Pawel Mrochen
 */

#pragma once

#include <stdexcept>
#include <string>
#include "ChannelOrder.hpp"
#include "Pixel.inl"

namespace core::imaging {

enum class PixelFormat
{
    UNSPECIFIED,
	LUMINANCE8, // !DXGI
	ALPHA8,
	/*PACKED_*/LUMINANCE8_ALPHA8, // !DXGI	// Packed
	/*PACKED_*/BGR4, // !DXGI
	/*PACKED_*/BGRA4,
	/*PACKED_*/BGR5,
	/*PACKED_*/BGR5_A1,
	/*PACKED_*/B5_G6_R5,
	R8,
	RG8,
	/*PACKED_*/RGB8,				// Packed
	/*PACKED_*/RGBA8,
	/*PACKED_*/BGR8,
	/*PACKED_*/BGRA8,
	/*PACKED_*/RGB10_A2,
	R16,
	RG16,
	RGBA16,
	R32,
	RG32,
	RGB32,
	RGBA32,
	R8I,							// Signed integer
	RG8I,
	/*PACKED_*/RGBA8I,
	R16I,
	RG16I,
	RGBA16I,
	R32I,
	RG32I,
	RGB32I,
	RGBA32I,
	/*PACKED_*/R11F_G11F_B10F,		// Floating point packed
	R16F,							// Floating point
	RG16F,
	RGBA16F,
	R32F,
	RG32F,
	RGB32F,
	RGBA32F,
	/*PACKED_*/RGB9_E5,				// Shared exponent packed
	/*COMPRESSED_*/BC1, // DXT1,    // Compressed
	/*COMPRESSED_*/BC2, // DXT3,
	/*COMPRESSED_*/BC3, // DXT5,
	/*COMPRESSED_*/BC4, // RGTC1,
	/*COMPRESSED_*/BC5, // RGTC2,
	/*COMPRESSED_*/BC6H, // BPTC_FLOAT,
	/*COMPRESSED_*/BC7, // BPTC,
};
  
inline std::string str(PixelFormat format)
{
	using namespace std::string_literals;
	switch (format)
	{
		case PixelFormat::UNSPECIFIED: return "Unspecified"s;
		case PixelFormat::LUMINANCE8: return "Luminance8"s;
		case PixelFormat::ALPHA8: return "Alpha8"s;
		case PixelFormat::LUMINANCE8_ALPHA8: return "Luminance8Alpha8"s;
		case PixelFormat::BGR4: return "Bgr4"s;
		case PixelFormat::BGRA4: return "Bgra4"s;
		case PixelFormat::BGR5: return "Bgr5"s;
		case PixelFormat::BGR5_A1: return "Bgr5A1"s;
		case PixelFormat::B5_G6_R5: return "B5G6R5"s;
		case PixelFormat::R8: return "R8"s;
		case PixelFormat::RG8: return "Rg8"s;
		case PixelFormat::RGB8: return "Rgb8"s;
		case PixelFormat::RGBA8: return "Rgba8"s;
		case PixelFormat::BGR8: return "Bgr8"s;
		case PixelFormat::BGRA8: return "Bgra8"s;
		case PixelFormat::RGB10_A2: return "Rgb10A2"s;
		case PixelFormat::R16: return "R16"s;
		case PixelFormat::RG16: return "Rg16"s;
		case PixelFormat::RGBA16: return "Rgba16"s;
		case PixelFormat::R32: return "R32"s;
		case PixelFormat::RG32: return "Rg32"s;
		case PixelFormat::RGB32: return "Rgb32"s;
		case PixelFormat::RGBA32: return "Rgba32"s;
		case PixelFormat::R8I: return "R8i"s;
		case PixelFormat::RG8I: return "Rg8i"s;
		case PixelFormat::RGBA8I: return "Rgba8i"s;
		case PixelFormat::R16I: return "R16i"s;
		case PixelFormat::RG16I: return "Rg16i"s;
		case PixelFormat::RGBA16I: return "Rgba16i"s;
		case PixelFormat::R32I: return "R32i"s;
		case PixelFormat::RG32I: return "Rg32i"s;
		case PixelFormat::RGB32I: return "Rgb32i"s;
		case PixelFormat::RGBA32I: return "Rgba32i"s;
		case PixelFormat::R11F_G11F_B10F: return "R11fG11fB10f"s;
		case PixelFormat::R16F: return "R16f"s;
		case PixelFormat::RG16F: return "Rg16f"s;
		case PixelFormat::RGBA16F: return "Rgba16f"s;
		case PixelFormat::R32F: return "R32f"s;
		case PixelFormat::RG32F: return "Rg32f"s;
		case PixelFormat::RGB32F: return "Rgb32f"s;
		case PixelFormat::RGBA32F: return "Rgba32f"s;
		case PixelFormat::RGB9_E5: return "Rgb9E5"s;
		case PixelFormat::BC1: return "BC1"s;
		case PixelFormat::BC2: return "BC2"s;
		case PixelFormat::BC3: return "BC3"s;
		case PixelFormat::BC4: return "BC4"s;
		case PixelFormat::BC5: return "BC5"s;
		case PixelFormat::BC6H: return "BC6H"s;
		case PixelFormat::BC7: return "BC7"s;
		default: throw std::invalid_argument("format");
    }
}

template<typename Func>
inline auto dispatch(PixelFormat format, Func&& f)
{
	switch (format)
	{
		//case PixelFormat::UNSPECIFIED:
		case PixelFormat::LUMINANCE8: return f.template operator()<pixel::Luminance8>();
		case PixelFormat::ALPHA8: return f.template operator()<pixel::Alpha8>();
		case PixelFormat::LUMINANCE8_ALPHA8: return f.template operator()<pixel::Luminance8Alpha8>();
		case PixelFormat::BGR4: return f.template operator()<pixel::Bgr4>();
		case PixelFormat::BGRA4: return f.template operator()<pixel::Bgra4>();
		case PixelFormat::BGR5: return f.template operator()<pixel::Bgr5>();
		case PixelFormat::BGR5_A1: return f.template operator()<pixel::Bgr5A1>();
		case PixelFormat::B5_G6_R5: return f.template operator()<pixel::B5G6R5>();
		case PixelFormat::R8: return f.template operator()<pixel::R8>();
		case PixelFormat::RG8: return f.template operator()<pixel::Rg8>();
		case PixelFormat::RGB8: return f.template operator()<pixel::Rgb8>();
		case PixelFormat::RGBA8: return f.template operator()<pixel::Rgba8>();
		case PixelFormat::BGR8: return f.template operator()<pixel::Bgr8>();
		case PixelFormat::BGRA8: return f.template operator()<pixel::Bgra8>();
		case PixelFormat::RGB10_A2: return f.template operator()<pixel::Rgb10A2>();
		case PixelFormat::R16: return f.template operator()<pixel::R16>();
		case PixelFormat::RG16: return f.template operator()<pixel::Rg16>();
		case PixelFormat::RGBA16: return f.template operator()<pixel::Rgba16>();
		case PixelFormat::R32: return f.template operator()<pixel::R32>();
		case PixelFormat::RG32: return f.template operator()<pixel::Rg32>();
		case PixelFormat::RGB32: return f.template operator()<pixel::Rgb32>();
		case PixelFormat::RGBA32: return f.template operator()<pixel::Rgba32>();
		case PixelFormat::R8I: return f.template operator()<pixel::R8i>();
		case PixelFormat::RG8I: return f.template operator()<pixel::Rg8i>();
		case PixelFormat::RGBA8I: return f.template operator()<pixel::Rgba8i>();
		case PixelFormat::R16I: return f.template operator()<pixel::R16i>();
		case PixelFormat::RG16I: return f.template operator()<pixel::Rg16i>();
		case PixelFormat::RGBA16I: return f.template operator()<pixel::Rgba16i>();
		case PixelFormat::R32I: return f.template operator()<pixel::R32i>();
		case PixelFormat::RG32I: return f.template operator()<pixel::Rg32i>();
		case PixelFormat::RGB32I: return f.template operator()<pixel::Rgb32i>();
		case PixelFormat::RGBA32I: return f.template operator()<pixel::Rgba32i>();
		case PixelFormat::R11F_G11F_B10F: return f.template operator()<pixel::R11fG11fB10f>();
		case PixelFormat::R16F: return f.template operator()<pixel::R16f>();
		case PixelFormat::RG16F: return f.template operator()<pixel::Rg16f>();
		case PixelFormat::RGBA16F: return f.template operator()<pixel::Rgba16f>();
		case PixelFormat::R32F: return f.template operator()<pixel::R32f>();
		case PixelFormat::RG32F: return f.template operator()<pixel::Rg32f>();
		case PixelFormat::RGB32F: return f.template operator()<pixel::Rgb32f>();
		case PixelFormat::RGBA32F: return f.template operator()<pixel::Rgba32f>();
		case PixelFormat::RGB9_E5: return f.template operator()<pixel::Rgb9E5>();
		case PixelFormat::BC1: return f.template operator()<pixel::BC1>();
		case PixelFormat::BC2: return f.template operator()<pixel::BC2>();
		case PixelFormat::BC3: return f.template operator()<pixel::BC3>();
		case PixelFormat::BC4: return f.template operator()<pixel::BC4>();
		case PixelFormat::BC5: return f.template operator()<pixel::BC5>();
		case PixelFormat::BC6H: return f.template operator()<pixel::BC6H>();
		case PixelFormat::BC7: return f.template operator()<pixel::BC7>();
		default: throw std::invalid_argument("format");
    }
}

inline int bits(PixelFormat format)
{
	return dispatch(format, []<typename T>() { return T::BIT_DEPTH; });
}

inline int size(PixelFormat format)
{
	return dispatch(format, []<typename T>() { return T::SIZE; });
}

template<typename TResult, typename TComponent>
inline TResult makePackedRgb/*packRgb*/(TComponent r, TComponent g, TComponent b) noexcept
{
    return pixel::pack<pixel::PackedRgb<TResult>>(r, g, b);
}

template<typename TResult, typename TComponent>
inline TResult makePackedBgr/*packBgr*/(TComponent r, TComponent g, TComponent b) noexcept
{
    return pixel::pack<pixel::PackedBgr<TResult>>(r, g, b);
}

template<typename TResult, typename TComponent>
inline TResult makePackedRgba/*packRgba*/(TComponent r, TComponent g, TComponent b, TComponent a) noexcept
{ 
    return pixel::pack<pixel::PackedRgba<TResult>>(r, g, b, a);
}

template<typename TResult, typename TComponent>
inline TResult makePackedBgra/*packBgra*/(TComponent r, TComponent g, TComponent b, TComponent a) noexcept
{ 
    return pixel::pack<pixel::PackedBgra<TResult>>(r, g, b, a);
}

template<typename TResult, typename TComponent>
inline TResult makePacked/*Native*/(TComponent r, TComponent g, TComponent b) noexcept
{
#if IMAGING_NATIVE_ORDER_RGBA
    return makePackedRgb<TResult, TComponent>(r, g, b);
#else
    return makePackedBgr<TResult, TComponent>(r, g, b);
#endif
}

template<typename TResult, typename TComponent>
inline TResult makePacked/*Native*/(TComponent r, TComponent g, TComponent b, TComponent a) noexcept
{ 
#if IMAGING_NATIVE_ORDER_RGBA
    return makePackedRgba<TResult, TComponent>(r, g, b, a);
#else
    return makePackedBgra<TResult, TComponent>(r, g, b, a);
#endif
}

template<typename TResult, typename TPacked>
inline TResult unpackRgb(TPacked c) noexcept
{
    return pixel::unpack<TResult, typename TResult::ComponentType, TResult::NUM_COMPONENTS, pixel::PackedRgb<TPacked>>(c);
}

template<typename TResult, typename TPacked>
inline TResult unpackBgr(TPacked c) noexcept
{
    return pixel::unpack<TResult, typename TResult::ComponentType, TResult::NUM_COMPONENTS, pixel::PackedBgr<TPacked>>(c);
}

template<typename TResult, typename TPacked>
inline TResult unpackRgba(TPacked c) noexcept
{ 
    return pixel::unpack<TResult, typename TResult::ComponentType, TResult::NUM_COMPONENTS, pixel::PackedRgba<TPacked>>(c);
}

template<typename TResult, typename TPacked>
inline TResult unpackBgra(TPacked c) noexcept
{ 
    return pixel::unpack<TResult, typename TResult::ComponentType, TResult::NUM_COMPONENTS, pixel::PackedBgra<TPacked>>(c);
}
/*
template<typename TResult, typename TPacked>
inline TResult unpackNative(TPacked c) noexcept
{
#if IMAGING_NATIVE_ORDER_RGBA
    return unpackRgb<TResult, TPacked>(c);
#else
    return unpackBgr<TResult, TPacked>(c);
#endif
}

template<typename TResult, typename TPacked>
inline TResult unpackNative(TPacked c) noexcept
{ 
#if IMAGING_NATIVE_ORDER_RGBA
    return unpackRgba<TResult, TPacked>(c);
#else
    return unpackBgra<TResult, TPacked>(c);
#endif
}
*/
} // namespace core::imaging

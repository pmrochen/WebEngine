/*
 *	Name: PixelTypes
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cstdint>
#include <type_traits>
#include <limits>
#include <algorithm>

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

namespace detail/*pixel*/ {

template<typename T, int S, int RB, int GB, int BB, int AB = 0, bool C = false>
struct Pixel
{
    using DataType = T;

    static constexpr bool COMPRESSED = C;
    static constexpr bool FLOATING_POINT = std::is_float_v<T>;
    static constexpr bool HAS_ALPHA = (AB > 0);
	static constexpr int BIT_DEPTH = RB + GB + BB + AB;
	static constexpr int NUM_COMPONENTS = (int)(RB > 0) + (int)(GB > 0) + (int)(BB > 0) + (int)(AB > 0);
    static constexpr int SIZE = S;
    static constexpr int R_BITS = RB;
	static constexpr int G_BITS = GB;
	static constexpr int B_BITS = BB;
	static constexpr int A_BITS = AB;
	static constexpr int R_MAX = (1 << RB) - 1;
	static constexpr int G_MAX = (1 << GB) - 1;
	static constexpr int B_MAX = (1 << BB) - 1;
	static constexpr int A_MAX = (1 << AB) - 1;
};

template<typename T, int N>
struct SequentialPixel : public Pixel<T, sizeof(T)*N, (N > 0) ? sizeof(T)*8 : 0,
    (N > 1) ? sizeof(T)*8 : 0, (N > 2) ? sizeof(T)*8 : 0, (N > 3) ? sizeof(T)*8 : 0>
{
    using ComponentType = T;
};

template<typename T, int RB, int GB, int BB, int AB, int RS, int GS, int BS, int AS = 0>
struct PackedPixel : public Pixel<T, sizeof(T), RB, GB, BB, AB>
{
    using PackedType = T;

    //static constexpr bool PACKED_PIXEL = !std::is_void_v<T>;
    static constexpr bool PACKED_PIXEL = (RS > 0) || (GS > 0) || (BS > 0) || (AS > 0);
	static constexpr int R_SHIFT = RS;
	static constexpr int G_SHIFT = GS;
	static constexpr int B_SHIFT = BS;
	static constexpr int A_SHIFT = AS;
	static constexpr int R_MASK = ((1 << RB) - 1) << RS;
	static constexpr int G_MASK = ((1 << GB) - 1) << GS;
	static constexpr int B_MASK = ((1 << BB) - 1) << BS;
	static constexpr int A_MASK = ((1 << AB) - 1) << AS;
};

template<typename T, int RB = sizeof(T)*2, int GB = sizeof(T)*2, int BB = sizeof(T)*2>
struct Rgb : public PackedPixel<T, RB, GB, BB, 0, 0, RB, RB + GB> {};

template<typename T, int RB = sizeof(T)*2, int GB = sizeof(T)*2, int BB = sizeof(T)*2, int AB = sizeof(T)*2>
struct Rgba : public PackedPixel<T, RB, GB, BB, AB, 0, RB, RB + GB, RB + GB + BB> {};

template<typename T, int RB = sizeof(T)*2, int GB = sizeof(T)*2, int BB = sizeof(T)*2>
struct Bgr : public PackedPixel<T, RB, GB, BB, 0, BB + GB, BB, 0> {};

template<typename T, int RB = sizeof(T)*2, int GB = sizeof(T)*2, int BB = sizeof(T)*2, int AB = sizeof(T)*2>
struct Bgra : public PackedPixel<T, RB, GB, BB, AB, BB + GB, BB, 0, BB + GB + RB> {};

struct Rgb8 : public Rgb<std::uint32_t, 8, 8, 8> {};
struct Rgba8 : public Rgba<std::uint32_t, 8, 8, 8, 8> {};
struct Bgr8 : public Bgr<std::uint32_t, 8, 8, 8> {};
struct Bgra8 : public Bgra<std::uint32_t, 8, 8, 8, 8> {};

struct R32f : public SequentialPixel<float, 1> {};
struct Rg32f : public SequentialPixel<float, 2> {};
struct Rgb32f : public SequentialPixel<float, 3> {};
struct Rgba32f : public SequentialPixel<float, 4> {};

struct R32i : public SequentialPixel<int, 1> {};
struct Rg32i : public SequentialPixel<int, 2> {};
struct Rgb32i : public SequentialPixel<int, 3> {};
struct Rgba32i : public SequentialPixel<int, 4> {};

template<typename T, bool SCALE, int MAX, typename U>
inline T convert(U x)
{
    static_assert(std::is_integral_v<T> && (std::is_integral_v<U> || std::is_float_v<U>));
    if constexpr (!SCALE || (!std::is_float_v<U> && std::is_unsigned_v<U> && ((int)std::numeric_limits<U>::max() == MAX)))
        return T(x);
    else if constexpr (std::is_float_v<U>)
        return T(std::clamp(int(U(0.5) + x*U(MAX)), 0, MAX));
    else if constexpr (std::is_unsigned_v<U>)
        return std::min(T(x), T(MAX));
    else    
        return T(std::clamp(x, U(0), U(MAX))); // #FIXME
}

template<typename T, bool SCALE = false, typename U>
inline typename T::PackedType pack(U r, U g, U b) noexcept
{
    static_assert(T::R_BITS > 0);
    typename T::PackedType v;
    if constexpr (T::R_SHIFT > 0)
        v = convert<typename T::PackedType, SCALE, T::R_MAX>(r) << T::R_SHIFT;
    else
        v = convert<typename T::PackedType, SCALE, T::R_MAX>(r); 
    if constexpr (T::G_BITS > 0)
        v |= convert<typename T::PackedType, SCALE, T::G_MAX>(g) << T::G_SHIFT;
    if constexpr (T::B_BITS > 0) && (T::B_SHIFT > 0)
        v |= convert<typename T::PackedType, SCALE, T::B_MAX>(b) << T::B_SHIFT;
    else if constexpr (T::B_BITS > 0)
        v |= convert<typename T::PackedType, SCALE, T::B_MAX>(b);
    if constexpr (T::A_BITS > 0)
        v |= T::A_MASK;
    return v;
}

template<typename T, bool SCALE = false, typename U>
inline typename T::PackedType pack(U r, U g, U b, U a) noexcept
{
    static_assert(T::R_BITS > 0);
    typename T::PackedType v;
    if constexpr (T::R_SHIFT > 0)
        v = convert<typename T::PackedType, SCALE, T::R_MAX>(r) << T::R_SHIFT;
    else
        v = convert<typename T::PackedType, SCALE, T::R_MAX>(r); 
    if constexpr (T::G_BITS > 0)
        v |= convert<typename T::PackedType, SCALE, T::G_MAX>(g) << T::G_SHIFT;
    if constexpr (T::B_BITS > 0) && (T::B_SHIFT > 0)
        v |= convert<typename T::PackedType, SCALE, T::B_MAX>(b) << T::B_SHIFT;
    else if constexpr (T::B_BITS > 0)
        v |= convert<typename T::PackedType, SCALE, T::B_MAX>(b);
    if constexpr (T::A_BITS > 0)
        v |= convert<typename T::PackedType, SCALE, T::A_MAX>(a) << T::A_SHIFT;
    return v;
}

} // namespace detail

template<typename T, typename U>
inline T makePackedRgb(U r, U g, U b) noexcept 
{
    return detail::pack<detail::Rgb<T>>(r, g, b); 
}

template<typename T, typename U>
inline T makePackedBgr(U r, U g, U b) noexcept 
{
    return detail::pack<detail::Bgr<T>>(r, g, b); 
}

template<typename T, typename U>
inline T makePackedRgba(U r, U g, U b, U a) noexcept 
{ 
    return detail::pack<detail::Rgba<T>>(r, g, b, a); 
}

template<typename T, typename U>
inline T makePackedBgra(U r, U g, U b, U a) noexcept 
{ 
    return detail::pack<detail::Bgra<T>>(r, g, b, a); 
}

template<typename T, typename U>
inline T makePackedNative(U r, U g, U b) noexcept 
{
#if IMAGING_NATIVE_ORDER_RGBA
    return makePackedRgb<T, U>(r, g, b);
#else
    return makePackedBgr<T, U>(r, g, b);
#endif
}

template<typename T, typename U>
inline T makePackedNative(U r, U g, U b, U a) noexcept 
{ 
#if IMAGING_NATIVE_ORDER_RGBA
    return makePackedRgba<T, U>(r, g, b, a);
#else
    return makePackedBgra<T, U>(r, g, b, a);
#endif
}

} // namespace core::imaging

/*
 *	Name: ColorSpaces
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <limits>
#include <algorithm>
#include <cmath>
#include <Simd/Intrinsics.hpp>

namespace core::imaging {

namespace detail {

template<typename T>
inline T branchlessIntMin(T x, T y)
{
    if constexpr (std::is_signed_v<T>)
        return y + ((x - y) & ((x - y) >> std::numeric_limits<T>::digits));
    else
        static_assert(false);
}

template<typename T>
inline T branchlessIntMax(T x, T y)
{
    if constexpr (std::is_signed_v<T>)
        return x - ((x - y) & ((x - y) >> std::numeric_limits<T>::digits));
    else
        static_assert(false);
}

template<int Y, typename T>
inline T branchlessIntMax(T x)
{
    if constexpr (Y == 0)
    {
        if constexpr (std::is_signed_v<T>)
            return x - (x & (x >> std::numeric_limits<T>::digits));
        else    
            return x;
    }
    else
    {
        if constexpr (std::is_signed_v<T>)
            return x - ((x - y) & ((x - y) >> std::numeric_limits<T>::digits));
        else
            static_assert(false);
    }
}

} // namespace detail

inline float saturate(float x)
{
    return std::clamp(x, 0.f, 1.f);
}

inline double saturate(double x)
{
    return std::clamp(x, 0.0, 1.0);
}

template<typename T, typename U>
inline T saturate(U x) 
{
    if constexpr (std::is_integral_v<U>)
    {
        if constexpr (std::is_same_v<T, U>)
            return x;
        else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<U>)
            return T(std::min(x, U(std::numeric_limits<T>::max()))); // #TODO branchless
        else if constexpr (std::is_integral_v<T>)
            return T(std::clamp(x, U(0), U(std::numeric_limits<T>::max()))); // #TODO branchless
        else if constexpr (std::is_floating_point_v<T> && std::is_unsigned_v<U>)
            return T(x)/T(std::numeric_limits<U>::max());
        else if constexpr (std::is_floating_point_v<T>)
            return T(detail::branchlessIntMax<0>(x)/T(std::numeric_limits<U>::max());
    }
    else if constexpr (std::is_floating_point_v<U>)
    {
        if constexpr (std::is_integral_v<T>)                        // #TODO branchless
            return T(std::clamp(int(U(0.5) + x*U(std::numeric_limits<T>::max())), 0, int(std::numeric_limits<T>::max())));
        else if constexpr (std::is_floating_point_v<T>)
            return T(std::clamp(x, U(0), U(1)));
    }

    static_assert(false);
    //return T();
}

template<typename T>
inline T makeLinear(T x) 
{ 
    return (x <= T(0.04045)) ? x/T(12.92) : std::pow((x + T(0.055))/T(1.055), T(2.4)); 
}

template<typename T>
inline T makeSrgb(T x) 
{ 
    if (!(x == x)) 
        return T(0); 
    else if (x > T(1)) 
        return T(1); 
    else if (x < T(0)) 
        return T(0); 
    else if (x < T(0.0031308)) 
        return T(12.92)*x; 
    else 
        return T(1.055)*std::pow(x, T(0.41666)) - T(0.055); 
}

template<typename T, typename U>
inline T rgbToHsv(U r, U g, U b)
{
	U hi = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
	U lo = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
	U range = hi - lo;
	U h = U(0);
	U s = U(0);

	if (hi != U(0))
		s = range/hi;

	if (range/*s*/ != U(0)) 
	{
		if (r == hi)
			h = (g - b)/range;
		else if (g == hi)
			h = U(2) + (b - r)/range;
		else
			h = U(4) + (r - g)/range;

		h = h/U(6);
		if (h < U(0)) 
			h += U(1);
	}

    return T(h, s, hi);
}

template<typename T, typename U>
inline T hsvToRgb(U h, U s, U v)
{
	U hue = (h - std::floor(h))*U(6);
	U i = std::floor(hue);
	U f = hue - i;
	U p = v*(U(1) - s);
	U q = v*(U(1) - (s*f));
	U t = v*(U(1) - (s*(U(1) - f)));

	switch ((int)i) 
	{
		case 0: return T(v, t, p);
		case 1: return T(q, v, p);
		case 2: return T(p, v, t);
		case 3: return T(p, q, v);
		case 4: return T(t, p, v);
		case 5: return T(v, p, q);
		default: return T(U(0), U(0), U(0));
	}
}

template<typename T, typename U>
inline T rgbToXyz(U r, U g, U b)
{
    return T(U(0.412453)*r + U(0.357580)*g + U(0.180423)*b,
	    U(0.212671)*r + U(0.715160)*g + U(0.072169)*b,
	    U(0.019334)*r + U(0.119193)*g + U(0.950227)*b);
}

template<typename T, typename U>
inline T xyzToRgb(U x, U y, U z)
{
    return T(U(3.240479)*x - U(1.537150)*y - U(0.498535)*z,
	    U(-0.969256)*x + U(1.875992)*y + U(0.041556)*z,
	    U(0.055648)*x - U(0.204043)*y + U(1.057311)*z);
}

#if SIMD_HAS_FLOAT4

template<typename T>
inline T rgbToXyz<T, float>(float r, float g, float b)
{
    static const simd::float4 row0 = simd::set3(0.412453f, 0.212671f, 0.019334f);
    static const simd::float4 row1 = simd::set3(0.357580f, 0.715160f, 0.119193f);
    static const simd::float4 row2 = simd::set3(0.180423f, 0.072169f, 0.950227f);
    return T(row0*r + row1*g + row2*b);
}

template<typename T>
inline T rgbToXyz(simd::float4 rgb)
{
    static const simd::float4 row0 = simd::set3(0.412453f, 0.212671f, 0.019334f);
    static const simd::float4 row1 = simd::set3(0.357580f, 0.715160f, 0.119193f);
    static const simd::float4 row2 = simd::set3(0.180423f, 0.072169f, 0.950227f);
    return T(row0*simd::xxxx(rgb) + row1*simd::yyyy(rgb) + row2*simd::zzzz(rgb));
}

template<typename T>
inline T xyzToRgb<T, float>(float x, float y, float z)
{
    static const simd::float4 row0 = simd::set3(3.240479f, -0.969256f, 0.055648f);
    static const simd::float4 row1 = simd::set3(-1.537150f, 1.875992f, -0.204043f);
    static const simd::float4 row2 = simd::set3(-0.498535f, 0.041556f, 1.057311f);
    return T(row0*x + row1*y + row2*z);
}

template<typename T>
inline T xyzToRgb(simd::float4 xyz)
{
    static const simd::float4 row0 = simd::set3(3.240479f, -0.969256f, 0.055648f);
    static const simd::float4 row1 = simd::set3(-1.537150f, 1.875992f, -0.204043f);
    static const simd::float4 row2 = simd::set3(-0.498535f, 0.041556f, 1.057311f);
    return T(row0*simd::xxxx(xyz) + row1*simd::yyyy(xyz) + row2*simd::zzzz(xyz));
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace core::imaging

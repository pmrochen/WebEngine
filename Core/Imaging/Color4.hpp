/*
 *	Name: Color4
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <tuple>
#include <cstddef>
#include <cmath>
#include <Simd/Intrinsics.hpp>
#include <Tuples/Tuple4.hpp>
#include "ColorSpaces.hpp"
#include "PixelTypes.hpp"
#include "Color3.hpp"

namespace core::imaging {
namespace templates {

//template<typename T>
//struct IntColor4;

template<typename T>
struct Color4
{
	using Real = T;
	using ComponentType = T;
	using ConstArg = const Color4&;
	using ConstResult = const Color4&;

	static constexpr int NUM_COMPONENTS = 4;

	constexpr Color4() noexcept : r(), g(), b(), a() {}
	explicit Color4(Uninitialized) noexcept {}
	constexpr explicit Color4(T scalar) noexcept : r(scalar), g(scalar), b(scalar), a(scalar) {}
	constexpr Color4(T r, T g, T b, T a) noexcept : r(r), g(g), b(b), a(a) {}
	constexpr Color4(Color3<T>::ConstArg c) noexcept : r(c.r), g(c.g), b(c.b), a(T(1)) {}
	constexpr Color4(Color3<T>::ConstArg c, T a) noexcept : r(c.r), g(c.g), b(c.b), a(a) {}
	//template<typename U> explicit Color4(const IntColor4<U>&/*IntColor4<U>::ConstArg*/ c) noexcept;
	explicit Color4(const tuples::templates::Tuple4<T>& t) noexcept : r(t.x), g(t.y), b(t.z), a(t.w) {}
	template<typename U> explicit Color4(const tuples::templates::Tuple4<U>& t) noexcept : r(T(t.x)), g(T(t.y)), b(T(t.z)), a(T(t.w)) {}
	explicit Color4(const std::tuple<T, T, T, T>& t) noexcept : r(std::get<0>(t)), g(std::get<1>(t)), b(std::get<2>(t)), a(std::get<3>(t)) {}
	template<typename U> explicit Color4(const std::tuple<U, U, U, U>& t) noexcept : r(T(std::get<0>(t))), g(T(std::get<1>(t))), b(T(std::get<2>(t))), a(T(std::get<3>(t))) {}
	explicit Color4(const T* c) noexcept : r(c[0]), g(c[1]), b(c[2]), a(c[3]) {}

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(r, g, b, a); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(r), U(g), U(b), U(a)); }
	//explicit operator std::tuple<T, T, T, T>() { return std::tuple<T, T, T, T>(r, g, b, a); }
	//template<typename U> explicit operator std::tuple<U, U, U, U>() { return std::tuple<U, U, U, U>(U(r), U(g), U(b), U(a)); }
	explicit operator T*() noexcept { return &r; }
	explicit operator const T*() const noexcept { return &r; }
	T& operator[](int i) noexcept { return (&r)[i]; }
	const T& operator[](int i) const noexcept { return (&r)[i]; }

	Color4 operator+() const noexcept { return *this; }
	Color4 operator-() const noexcept { return Color4(-r, -g, -b, -a); }
	Color4& operator+=(const Color4& c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
	Color4& operator-=(const Color4& c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
	Color4& operator*=(const Color4& c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
	Color4& operator*=(T f) noexcept { r *= f; g *= f; b *= f; a *= f; return *this; }
	Color4& operator/=(const Color4& c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	Color4& operator/=(T f) noexcept { return operator*=(T(1)/f); }
	bool operator==(const Color4& c) const noexcept { return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a); }
	bool operator!=(const Color4& c) const noexcept { return !(*this == c); }
	
	template<typename A> void load(A& ar) { ar(r, g, b, a); }
	template<typename A> void save(A& ar) const { ar(r, g, b, a); }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	template<typename U> static Color4 fromPackedRgba(U c) noexcept;
	std::uint32_t toPackedRgba() const noexcept { return toPackedRgba<std::uint32_t>(); }
	template<typename U> U toPackedRgba() const noexcept;
	template<typename U> static Color4 fromPackedBgra(U c) noexcept;
	std::uint32_t toPackedBgra() const noexcept { return toPackedBgra<std::uint32_t>(); }
	template<typename U> U toPackedBgra() const noexcept;
#if IMAGING_NATIVE_ORDER_RGBA
	template<typename U> static Color4 fromPacked/*Native*/(U c) noexcept { return fromPackedRgba(c); }
	std::uint32_t toPacked/*Native*/() const noexcept { return toPackedRgba(); }
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedRgba<U>(); }
#else
	template<typename U> static Color4 fromPacked/*Native*/(U c) noexcept { return fromPackedBgra(c); }
	std::uint32_t toPacked/*Native*/() const noexcept { return toPackedBgra(); }
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedBgra<U>(); }
#endif

	Color3<T>::ConstResult rgb/*getRgb*/() const noexcept { return reinterpret_cast<const Color3&>(*this); }
	//void setRgb(Color3<T>::ConstArg c) noexcept { r = c.r; g = c.g; b = c.b; }
	bool isZero() const noexcept { return (r == T()) && (g == T()) && (b == T()) && (a == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(const Color4& c) const noexcept;
	bool isApproxEqual(const Color4& c, T tolerance) const noexcept;
	bool allLessThan(const Color4& c) const noexcept { return (r < c.r) && (g < c.g) && (b < c.b) && (a < c.a); }
	bool allLessThanEqual(const Color4& c) const noexcept { return (r <= c.r) && (g <= c.g) && (b <= c.b) && (a <= c.a); }
	bool allGreaterThan(const Color4& c) const noexcept { return (r > c.r) && (g > c.g) && (b > c.b) && (a > c.a); }
	bool allGreaterThanEqual(const Color4& c) const noexcept { return (r >= c.r) && (g >= c.g) && (b >= c.b) && (a >= c.a); }
	bool anyLessThan(const Color4& c) const noexcept { return (r < c.r) || (g < c.g) || (b < c.b) || (a < c.a); }
	bool anyLessThanEqual(const Color4& c) const noexcept { return (r <= c.r) || (g <= c.g) || (b <= c.b) || (a <= c.a); }
	bool anyGreaterThan(const Color4& c) const noexcept { return (r > c.r) || (g > c.g) || (b > c.b) || (a > c.a); }
	bool anyGreaterThanEqual(const Color4& c) const noexcept { return (r >= c.r) || (g >= c.g) || (b >= c.b) || (a >= c.a); }
	bool isFinite() const noexcept { return std::isfinite(r) && std::isfinite(g) && std::isfinite(b) && std::isfinite(a); }
	T getLuminance() const noexcept { return r*T(0.2126) + g*T(0.7152) + b*T(0.0722); }
 	T getMinComponent() const { return std::min(std::min(std::min(r, g), b), a); }
	T getMaxComponent() const { return std::max(std::max(std::max(r, g), b), a); }
	Color4& setZero() noexcept { r = T(); g = T(); b = T(); a = T(); return *this; }
	Color4& set(T r, T g, T b, T a) noexcept { this->r = r; this->g = g; this->b = b; this->a = a; return *this; }
	Color4& setMinimum(const Color4& c1, const Color4& c2);
	Color4& setMaximum(const Color4& c1, const Color4& c2);
	Color4& saturate();
	Color4& makeLinear() noexcept;
	Color4& makeSrgb() noexcept;

	//static const Color4& getZero() noexcept { return ZERO; }
	//static const Color4& getUnitR() noexcept { return UNIT_R; }
	//static const Color4& getUnitG() noexcept { return UNIT_G; }
	//static const Color4& getUnitB() noexcept { return UNIT_B; }
	//static const Color4& getUnitA() noexcept { return UNIT_A; }

	static const Color4 ZERO;
	static const Color4 UNIT_R;
	static const Color4 UNIT_G;
	static const Color4 UNIT_B; 
	static const Color4 UNIT_A; 
	static const Color4 ONE;
	static const Color4 HALF;
	static const Color4 TOLERANCE;
	static const Color4 INF;
	static const Color4 MINUS_INF;
	static const Color4 LUMINANCE;

	T r, g, b, a;
};

template<typename T> const Color4<T> Color4<T>::ZERO{};
template<typename T> const Color4<T> Color4<T>::UNIT_R{ T(1), T(0), T(0), T(0) };
template<typename T> const Color4<T> Color4<T>::UNIT_G{ T(0), T(1), T(0), T(0) };
template<typename T> const Color4<T> Color4<T>::UNIT_B{ T(0), T(0), T(1), T(0) };
template<typename T> const Color4<T> Color4<T>::UNIT_A{ T(0), T(0), T(0), T(1) };
template<typename T> const Color4<T> Color4<T>::ONE{ T(1), T(1), T(1), T(1) };
template<typename T> const Color4<T> Color4<T>::HALF{ T(0.5), T(0.5), T(0.5), T(0.5) };
template<typename T> const Color4<T> Color4<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };
template<typename T> const Color4<T> Color4<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() };
template<typename T> const Color4<T> Color4<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity() };
template<typename T> const Color4<T> Color4<T>::LUMINANCE{ T(0.2126), T(0.7152), T(0.0722), T(0) };

#if SIMD_HAS_FLOAT4

template<>
struct Color4<float>
{
	using Real = float;
	using ComponentType = float;
	using ConstArg = const Color4;
	using ConstResult = const Color4;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Color4() noexcept : rgba(simd::zero<simd::float4>()) {}
	explicit Color4(Uninitialized) noexcept {}
	/*constexpr*/ explicit Color4(float scalar) noexcept : rgba(simd::set4(scalar)) {}
	/*constexpr*/ Color4(float r, float g, float b, float a) noexcept : rgba(simd::set4(r, g, b, a)) {}
	/*constexpr*/ Color4(Color3<float>::ConstArg c) noexcept : rgba(simd::insert3(c, UNIT_A)) {}
	/*constexpr*/ Color4(Color3<float>::ConstArg c, float a) noexcept : rgba(simd::insert<3>(a, c)) {}
	//template<typename U> explicit Color4(const IntColor4<U>&/*IntColor4<U>::ConstArg*/ c) noexcept;
	explicit Color4(const tuples::templates::Tuple4<float>& t) noexcept : rgba(simd::set4(t.x, t.y, t.z, t.w)) {}
	template<typename U> explicit Color4(const tuples::templates::Tuple4<U>& t) noexcept : rgba(simd::set4((float)t.x, (float)t.y, (float)t.z, (float)t.w)) {}
	explicit Color4(const std::tuple<float, float, float, float>& t) noexcept : rgba(simd::set4(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t))) {}
	template<typename U> explicit Color4(const std::tuple<U, U, U, U>& t) noexcept : rgba(simd::set4((float)std::get<0>(t), (float)std::get<1>(t), (float)std::get<2>(t), (float)std::get<3>(t))) {}
	explicit Color4(const float* c) noexcept : rgba(simd::load4(c)) {}
	explicit Color4(simd::float4 c) noexcept : rgba(c) {}
	Color4(const Color4& c) noexcept : rgba(c.rgba) {}
	Color4& operator=(const Color4& c) noexcept { rgba = c.rgba; return *this; }

	operator simd::float4() const noexcept { return rgba; }
	explicit operator tuples::templates::Tuple4<float>() noexcept { return tuples::templates::Tuple4<float>(r, g, b, a); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(r), U(g), U(b), U(a)); }
	//explicit operator std::tuple<float, float, float, float>() { return std::tuple<float, float, float, float>(r, g, b, a); }
	//template<typename U> explicit operator std::tuple<U, U, U, U>() { return std::tuple<U, U, U, U>(U(r), U(g), U(b), U(a)); }
	explicit operator float* () noexcept { return &r; }
	explicit operator const float* () const noexcept { return &r; }
	float& operator[](int i) noexcept { return (&r)[i]; }
	const float& operator[](int i) const noexcept { return (&r)[i]; }

	Color4 operator+() const noexcept { return *this; }
	Color4 operator-() const noexcept { return Color4(simd::neg4(rgba)); }
	Color4& operator+=(const Color4& c) noexcept { rgba = simd::add4(rgba, c); return *this; }
	Color4& operator-=(const Color4& c) noexcept { rgba = simd::sub4(rgba, c); return *this; }
	Color4& operator*=(const Color4& c) noexcept { rgba = simd::mul4(rgba, c); return *this; }
	Color4& operator*=(float f) noexcept { rgba = simd::mul4(rgba, simd::set4(f)); return *this; }
	Color4& operator/=(const Color4& c) noexcept { rgba = simd::div4(rgba, c); return *this; }
	Color4& operator/=(float f) noexcept { rgba = simd::div4(rgba, simd::set4(f)); return *this; }
	bool operator==(const Color4& c) const noexcept { return simd::all4(simd::equal(rgba, c)); }
	bool operator!=(const Color4& c) const noexcept { return !(*this == c); }

	template<typename A> void load(A& ar);
	template<typename A> void save(A& ar) const { ar(r, g, b, a); }

	template<std::size_t I> float& get() noexcept;
	template<std::size_t I> const float& get() const noexcept;
	template<typename U> U& get() noexcept;				// intentionally undefined
	template<typename U> const U& get() const noexcept;	// intentionally undefined
	template<> simd::float4& get() noexcept { return rgba; }
	template<> const simd::float4& get() const noexcept { return rgba; }

	template<typename U> static Color4 fromPackedRgba(U c) noexcept;
	std::uint32_t toPackedRgba() const noexcept { return toPackedRgba<std::uint32_t>(); }
	template<typename U> U toPackedRgba() const noexcept;
	template<typename U> static Color4 fromPackedBgra(U c) noexcept;
	std::uint32_t toPackedBgra() const noexcept { return toPackedBgra<std::uint32_t>(); }
	template<typename U> U toPackedBgra() const noexcept;
#if IMAGING_NATIVE_ORDER_RGBA
	template<typename U> static Color4 fromPacked/*Native*/(U c) noexcept { return fromPackedRgba(c); }
	std::uint32_t toPacked/*Native*/() const noexcept { return toPackedRgba(); }
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedRgba<U>(); }
#else
	template<typename U> static Color4 fromPacked/*Native*/(U c) noexcept { return fromPackedBgra(c); }
	std::uint32_t toPacked/*Native*/() const noexcept { return toPackedBgra(); }
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedBgra<U>(); }
#endif

#if IMAGING_SIMD_EXPAND_LAST
	Color3<float> rgb/*getRgb*/() const noexcept { return Color3<float>(simd::xyzz(rgba)); }
#else
	Color3<float> rgb/*getRgb*/() const noexcept { return Color3<float>(simd::cutoff3(rgba)); }
#endif
	//void setRgb(Color3<float>::ConstArg c) noexcept { rgba = simd::insert3(c, rgba); }
	bool isZero() const noexcept { return simd::all4(simd::equal(rgba, simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept { simd::all4(simd::lessThan(simd::abs4(rgba), TOLERANCE)); }
	bool isApproxEqual(const Color4& c) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(rgba, c)), TOLERANCE)); }
	bool isApproxEqual(const Color4& c, float tolerance) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(rgba, c)), simd::set4(tolerance))); }
	bool allLessThan(const Color4& c) const noexcept { return simd::all4(simd::lessThan(rgba, c)); }
	bool allLessThanEqual(const Color4& c) const noexcept { return simd::all4(simd::lessThanEqual(rgba, c)); }
	bool allGreaterThan(const Color4& c) const noexcept { return simd::all4(simd::greaterThan(rgba, c)); }
	bool allGreaterThanEqual(const Color4& c) const noexcept { return simd::all4(simd::greaterThanEqual(rgba, c)); }
	bool anyLessThan(const Color4& c) const noexcept { return simd::any4(simd::lessThan(rgba, c)); }
	bool anyLessThanEqual(const Color4& c) const noexcept { return simd::any4(simd::lessThanEqual(rgba, c)); }
	bool anyGreaterThan(const Color4& c) const noexcept { return simd::any4(simd::greaterThan(rgba, c)); }
	bool anyGreaterThanEqual(const Color4& c) const noexcept { return simd::any4(simd::greaterThanEqual(rgba, c)); }
	bool isFinite() const noexcept { return simd::all4(simd::isFinite(rgba)); }
	float getLuminance() const noexcept { return simd::toFloat(simd::dot3(rgba, LUMINANCE)); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin4(rgba)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax4(rgba)); }
	Color4& setZero() noexcept { rgba = simd::zero<simd::float4>(); return *this; }
	Color4& set(float r, float g, float b, float a) noexcept { rgba = simd::set4(r, g, b, a); return *this; }
	Color4& setMinimum(const Color4& c1, const Color4& c2) noexcept { rgba = simd::min4(c1, c2); return *this; }
	Color4& setMaximum(const Color4& c1, const Color4& c2) noexcept { rgba = simd::max4(c1, c2); return *this; }
	Color4& saturate() noexcept { rgba = simd::min4(simd::max4(rgba, simd::zero<simd::float4>()), ONE); return *this; }
	Color4& makeLinear() noexcept { rgba = simd::insert3(simd::set3(makeLinear(r), makeLinear(g), makeLinear(b)), rgba); }
	Color4& makeSrgb() noexcept { rgba = simd::insert3(simd::set3(makeSrgb(r), makeSrgb(g), makeSrgb(b)), rgba); }

	//static const Color4& getZero() noexcept { return ZERO; }
	//static const Color4& getUnitR() noexcept { return UNIT_R; }
	//static const Color4& getUnitG() noexcept { return UNIT_G; }
	//static const Color4& getUnitB() noexcept { return UNIT_B; }
	//static const Color4& getUnitA() noexcept { return UNIT_A; }

	static const Color4 ZERO;
	static const Color4 UNIT_R;
	static const Color4 UNIT_G;
	static const Color4 UNIT_B;
	static const Color4 UNIT_A;
	static const Color4 ONE;
	static const Color4 HALF;
	static const Color4 TOLERANCE;
	static const Color4 INF;
	static const Color4 MINUS_INF;
	static const Color4 LUMINANCE;

	union
	{
		simd::float4 rgba;
		struct { float r, g, b, a; };
	};
};

const Color4<float> Color4<float>::ZERO{};
const Color4<float> Color4<float>::UNIT_R{ 1.f, 0.f, 0.f, 0.f };
const Color4<float> Color4<float>::UNIT_G{ 0.f, 1.f, 0.f, 0.f };
const Color4<float> Color4<float>::UNIT_B{ 0.f, 0.f, 1.f, 0.f };
const Color4<float> Color4<float>::UNIT_A{ 0.f, 0.f, 0.f, 1.f };
const Color4<float> Color4<float>::ONE{ 1.f, 1.f, 1.f, 1.f };
const Color4<float> Color4<float>::HALF{ 0.5f, 0.5f, 0.5f, 0.5f };
const Color4<float> Color4<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };
const Color4<float> Color4<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
const Color4<float> Color4<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
const Color4<float> Color4<float>::LUMINANCE{ 0.2126f, 0.7152f, 0.0722f, 0.f };

#endif /* SIMD_HAS_FLOAT4 */

template<std::size_t I, typename T>
inline T& get(Color4<T>& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	else if constexpr (I == 3)
		return c.a;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const Color4<T>& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	else if constexpr (I == 3)
		return c.a;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(Color4<T>&& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	else if constexpr (I == 3)
		return c.a;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const Color4<T>&& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	else if constexpr (I == 3)
		return c.a;
	static_assert(false);
}

//template<typename T, typename U>
//inline T& get(Color4<U>& c) noexcept;
//
//template<typename T, typename U>
//inline const T& get(const Color4<U>& c) noexcept;

template<typename T>
inline T luminance(const Color4<T>& c) noexcept
{
	return c.r*T(0.2126) + c.g*T(0.7152) + c.b*T(0.0722);
}

template<typename T>
inline Color4<T> minimum(const Color4<T>& c1, const Color4<T>& c2)
{
	return Color4<T>(std::min(c1.r, c2.r), std::min(c1.g, c2.g), std::min(c1.b, c2.b), std::min(c1.a, c2.a));
}

template<typename T>
inline Color4<T> maximum(const Color4<T>& c1, const Color4<T>& c2)
{
	return Color4<T>(std::max(c1.r, c2.r), std::max(c1.g, c2.g), std::max(c1.b, c2.b), std::max(c1.a, c2.a));
}

template<typename T>
inline Color4<T> saturate(const Color4<T>& c)
{
	return Color4<T>(std::clamp(c.r, T(0), T(1)), std::clamp(c.g, T(0), T(1)), std::clamp(c.b, T(0), T(1)), std::clamp(c.a, T(0), T(1)));
}

template<typename T>
inline Color4<T> lerp(const Color4<T>& c1, const Color4<T>& c2, T t) noexcept
{
	return Color4<T>(c1.r + t*(c2.r - c1.r), c1.g + t*(c2.g - c1.g), c1.b + t*(c2.b - c1.b), c1.a + t*(c2.a - c1.a));
}

template<typename T>
inline Color4<T> makeLinear(const Color4<T>& c) noexcept
{
	return Color4<T>(makeLinear(c.r), makeLinear(c.g), makeLinear(c.b), c.a);
}

template<typename T>
inline Color4<T> makeSrgb(const Color4<T>& c) noexcept
{
	return Color4<T>(makeSrgb(c.r), makeSrgb(c.g), makeSrgb(c.b), c.a);
}

#if SIMD_HAS_FLOAT4

//template<>
//inline core::simd::float4& get(Color4<float>& c) noexcept
//{
//	return c.rgba;
//}
//
//template<>
//inline const core::simd::float4& get(const Color4<float>& c) noexcept
//{
//	return c.rgba;
//}

template<>
inline float luminance(const Color4<float>& c) noexcept
{
	//static const simd::float4 coeff = simd::set4(0.2126f, 0.7152f, 0.0722f, 0.f);
	return simd::toFloat(simd::dot3(c, Color4<float>::LUMINANCE/*coeff*/));
}

template<>
inline Color4<float> minimum(const Color4<float>& c1, const Color4<float>& c2)
{
	return Color4<float>(simd::min4(c1, c2));
}

template<>
inline Color4<float> maximum(const Color4<float>& c1, const Color4<float>& c2)
{
	return Color4<float>(simd::max4(c1, c2));
}

template<>
inline Color4<float> saturate(const Color4<float>& c)
{
	//static const simd::float4 one = simd::set4(1.f);
	return Color4<float>(simd::min4(simd::max4(c, simd::zero<simd::float4>()), Color4<float>::ONE/*one*/));
}

template<>
inline Color4<float> lerp(const Color4<float>& c1, const Color4<float>& c2, float t) noexcept
{
	return Color4<float>(simd::mulAdd4(simd::set4(t), simd::sub4(c2, c1), c1));
}

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Color4<T> operator+(const Color4<T>& c1, const Color4<T>& c2) noexcept 
{ 
	return Color4<T>(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); 
}

template<typename T>
inline Color4<T> operator-(const Color4<T>& c1, const Color4<T>& c2) noexcept 
{ 
	return Color4<T>(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
}

template<typename T>
inline Color4<T> operator*(const Color4<T>& c1, const Color4<T>& c2) noexcept 
{ 
	return Color4<T>(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b, c1.a*c2.a); 
}

template<typename T>
inline Color4<T> operator*(T f, const Color4<T>& c) noexcept 
{ 
	return Color4<T>(f*c.r, f*c.g, f*c.b, f*c.a);
}

template<typename T>
inline Color4<T> operator*(const Color4<T>& c, T f) noexcept 
{ 
	return Color4<T>(c.r*f, c.g*f, c.b*f, c.a*f); 
}

template<typename T>
inline Color4<T> operator/(const Color4<T>& c1, const Color4<T>& c2) noexcept 
{ 
	return Color4<T>(c1.r/c2.r, c1.g/c2.g, c1.b/c2.b, c1.a/c2.a);
}

template<typename T>
inline Color4<T> operator/(T f, const Color4<T>& c) noexcept 
{ 
	return Color4<T>(f/c.r, f/c.g, f/c.b, f/c.a); 
}

template<typename T>
inline Color4<T> operator/(const Color4<T>& c, T f) noexcept 
{ 
	return operator*(c, T(1)/f); 
}

template<typename C, typename T, typename U>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, Color4<U>& c)
{ 
	return s >> c.r >> std::ws >> c.g >> std::ws >> c.b >> std::ws >> c.a; 
}

template<typename C, typename T, typename U>
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const Color4<U>& c)
{ 
	constexpr C WS(0x20);
	return s << c.r << WS << c.g << WS << c.b << WS << c.a;
}

template<typename T>
template<std::size_t I>
inline T& Color4<T>::get()
{
	if constexpr (I == 0)
		return r;
	else if constexpr (I == 1)
		return g;
	else if constexpr (I == 2)
		return b;
	else if constexpr (I == 3)
		return a;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& Color4<T>::get() const
{
	if constexpr (I == 0)
		return r;
	else if constexpr (I == 1)
		return g;
	else if constexpr (I == 2)
		return b;
	else if constexpr (I == 3)
		return a;
	static_assert(false);
}

template<typename T>
template<typename U> 
inline Color4<T> Color4<T>::fromPackedRgba(U c)
{
	Color4<T> result(unpackRgba<Color4<T>>(c));
	result *= T(1)/(T)detail::Rgba<U>::R_MAX;
	return result;
}

template<typename T>
template<typename U> 
inline U Color4<T>::toPackedRgba() const
{
	return makePackedRgba<U>(saturate<int, detail::Rgba<U>::R_MAX>(c.r),
		saturate<int, detail::Rgba<U>::G_MAX>(c.g),
		saturate<int, detail::Rgba<U>::B_MAX>(c.b),
		saturate<int, detail::Rgba<U>::A_MAX>(c.a));
}

template<typename T>
template<typename U> 
inline Color4<T> Color4<T>::fromPackedBgra(U c)
{
	Color4<T> result(unpackBgra<Color4<T>>(c));
	result *= T(1)/(T)detail::Bgra<U>::R_MAX;
	return result;
}

template<typename T>
template<typename U> 
inline U Color4<T>::toPackedBgra() const
{
	return makePackedBgra<U>(saturate<int, detail::Bgra<U>::R_MAX>(c.r),
		saturate<int, detail::Bgra<U>::G_MAX>(c.g),
		saturate<int, detail::Bgra<U>::B_MAX>(c.b),
		saturate<int, detail::Bgra<U>::A_MAX>(c.a));
}

template<typename T>
inline bool Color4<T>::isApproxZero() const
{ 
	return (std::fabs(r) < Constants<T>::TOLERANCE) && (std::fabs(g) < Constants<T>::TOLERANCE) && 
		(std::fabs(b) < Constants<T>::TOLERANCE) && (std::fabs(a) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Color4<T>::isApproxEqual(const Color4<T>& c) const
{ 
	return (std::fabs(c.r - r) < Constants<T>::TOLERANCE) && (std::fabs(c.g - g) < Constants<T>::TOLERANCE) && 
		(std::fabs(c.b - b) < Constants<T>::TOLERANCE) && (std::fabs(c.a - a) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Color4<T>::isApproxEqual(const Color4<T>& c, T tolerance) const
{ 
	return (std::fabs(c.r - r) < tolerance) && (std::fabs(c.g - g) < tolerance) && 
		(std::fabs(c.b - b) < tolerance) && (std::fabs(c.a - a) < tolerance); 
}

template<typename T>
inline Color4<T>& Color4<T>::setMinimum(const Color4<T>& c1, const Color4<T>& c2)
{ 
	r = std::min(c1.r, c2.r); 
	g = std::min(c1.g, c2.g);
	b = std::min(c1.b, c2.b); 
	a = std::min(c1.a, c2.a); 
	return *this;
}

template<typename T>
inline Color4<T>& Color4<T>::setMaximum(const Color4<T>& c1, const Color4<T>& c2)
{ 
	r = std::max(c1.r, c2.r); 
	g = std::max(c1.g, c2.g);
	b = std::max(c1.b, c2.b); 
	a = std::max(c1.a, c2.a); 
	return *this;
}

template<typename T>
inline Color4<T>& Color4<T>::saturate()
{ 
	r = std::clamp(r, T(0), T(1)); 
	g = std::clamp(g, T(0), T(1));
	b = std::clamp(b, T(0), T(1));
	a = std::clamp(a, T(0), T(1));
	return *this;
}

template<typename T>
inline Color4<T>& Color4<T>::makeLinear()
{ 
	r = makeLinear(r);
	g = makeLinear(g);
	b = makeLinear(b);
	return *this;
}

template<typename T>
inline Color4<T>& Color4<T>::makeSrgb()
{ 
	r = makeSrgb(r);
	g = makeSrgb(g);
	b = makeSrgb(b);
	return *this;
}

#if SIMD_HAS_FLOAT4

template<>
inline Color4<float> operator+(const Color4<float>& c1, const Color4<float>& c2) noexcept 
{ 
	return Color4<float>(simd::add4(c1, c2)); 
}

template<>
inline Color4<float> operator-(const Color4<float>& c1, const Color4<float>& c2) noexcept 
{ 
	return Color4<float>(simd::sub4(c1, c2)); 
}

template<>
inline Color4<float> operator*(const Color4<float>& c1, const Color4<float>& c2) noexcept 
{ 
	return Color4<float>(simd::mul4(c1, c2)); 
}

template<>
inline Color4<float> operator*(float f, const Color4<float>& c) noexcept 
{ 
	return Color4<float>(simd::mul4(simd::set4(f), c)); 
}

template<>
inline Color4<float> operator*(const Color4<float>& c, float f) noexcept 
{ 
	return Color4<float>(simd::mul4(c, simd::set4(f))); 
}

template<>
inline Color4<float> operator/(const Color4<float>& c1, const Color4<float>& c2) noexcept 
{ 
	return Color4<float>(simd::div4(c1, c2)); 
}

template<>
inline Color4<float> operator/(float f, const Color4<float>& c) noexcept 
{ 
	return Color4<float>(simd::div4(simd::set4(f), c)); 
}

template<>
inline Color4<float> operator/(const Color4<float>& c, float f) noexcept 
{ 
	return Color4<float>(simd::div4(c, simd::set4(f))); 
}

template<typename C, typename T>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, Color4<float>& c)
{ 
	float r, g, b, a; 
	s >> r >> std::ws >> g >> std::ws >> b >> std::ws >> a; 
	c.set(r, g, b, a); 
	return s;
}

template<typename A>
inline void Color4<float>::load(A& ar)
{
	float t0, t1, t2, t3;
	ar(t0, t1, t2, t3);
	set(t0, t1, t2, t3);
}

template<std::size_t I>
inline float& Color4<float>::get()
{
	if constexpr (I == 0)
		return r;
	else if constexpr (I == 1)
		return g;
	else if constexpr (I == 2)
		return b;
	else if constexpr (I == 3)
		return a;
	static_assert(false);
}

template<std::size_t I>
inline const float& Color4<float>::get() const
{
	if constexpr (I == 0)
		return r;
	else if constexpr (I == 1)
		return g;
	else if constexpr (I == 2)
		return b;
	else if constexpr (I == 3)
		return a;
	static_assert(false);
}

template<typename U> 
inline Color4<float> Color4<float>::fromPackedRgba(U c)
{
	static const simd::float4 s = simd::set4(1.f/(float)detail::Rgba<U>::R_MAX);
	return Color4<float>(simd::mul4(unpackRgba<Color4<float>>(c), s));
}

template<typename U> 
inline U Color4<float>::toPackedRgba() const
{
	static const simd::float4 s = simd::set4((float)detail::Rgba<U>::R_MAX);
	//static const simd::float4 half = simd::set4(0.5f);
	Color4<float> c(simd::mulAdd4(simd::min4(simd::max4(c, simd::zero<simd::float4>()), Color4<float>::ONE),
		s, /*half*/Color4<float>::HALF));
	return makePackedRgba<U>(c.r, c.g, c.b, c.a);
}

template<typename U> 
inline Color4<float> Color4<float>::fromPackedBgra(U c)
{
	static const simd::float4 s = simd::set4(1.f/(float)detail::Bgra<U>::R_MAX);
	return Color4<float>(simd::mul4(unpackBgra<Color4<float>>(c), s));
}

template<typename U> 
inline U Color4<float>::toPackedBgra() const
{
	static const simd::float4 s = simd::set4((float)detail::Bgra<U>::R_MAX);
	//static const simd::float4 half = simd::set4(0.5f);
	Color4<float> c(simd::mulAdd4(simd::min4(simd::max4(c, simd::zero<simd::float4>()), Color4<float>::ONE),
		s, /*half*/Color4<float>::HALF));
	return makePackedBgra<U>(c.r, c.g, c.b, c.a);
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Color4 = templates::Color4<float>;
using Color4Arg = templates::Color4<float>::ConstArg;
using Color4Result = templates::Color4<float>::ConstResult;

} // namespace core::imaging

namespace std {

template<size_t I, typename T>
struct tuple_element;

template<size_t I, typename T>
struct tuple_element<I, ::core::imaging::templates::Color4<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size;

template<typename T>
struct tuple_size<::core::imaging::templates::Color4<T>> : integral_constant<size_t, 4> 
{
};

template<typename T>
struct hash;

template<typename T>
struct hash<::core::imaging::templates::Color4<T>>
{
	std::size_t operator()(const ::core::imaging::templates::Color4<T>& c) const noexcept
	{
		std::hash<T> hasher;
		std::size_t seed = hasher(c.r) + 0x9e3779b9;
		seed ^= hasher(c.g) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(c.b) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(c.a) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

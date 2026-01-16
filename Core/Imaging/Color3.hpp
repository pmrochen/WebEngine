/*
 *	Name: Color3
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <Simd/Intrinsics.hpp>
#include <Tuples/Tuple3.hpp>
#include "ColorSpaces.hpp"
#include "PackedColor.hpp"

namespace core {
namespace imaging {
namespace templates {

//template<typename T>
//struct IntColor3;

template<typename T>
struct Color3
{
	using Real = T;
	using ConstArg = const Color3&;
	using ConstResult = const Color3&;

	static constexpr int NUM_COMPONENTS = 3;

	constexpr Color3() noexcept : r(), g(), b() {}
	constexpr explicit Color3(T scalar) noexcept : r(scalar), g(scalar), b(scalar) {}
	constexpr Color3(T r, T g, T b) noexcept : r(r), g(g), b(b) {}
	//template<typename U> explicit Color3(const IntColor3<U>&/*IntColor3<U>::ConstArg*/ c) noexcept;
	explicit Color3(const tuples::templates::Tuple3<T>& t) noexcept : r(t.x), g(t.y), b(t.z) {}
	template<typename U> explicit Color3(const tuples::templates::Tuple3<U>& t) noexcept : r(T(t.x)), g(T(t.y)), b(T(t.z)) {}
	explicit Color3(const std::tuple<T, T, T>& t) noexcept : r(std::get<0>(t)), g(std::get<1>(t)), b(std::get<2>(t)) {}
	template<typename U> explicit Color3(const std::tuple<U, U, U>& t) noexcept : r(T(std::get<0>(t))), g(T(std::get<1>(t))), b(T(std::get<2>(t))) {}
	explicit Color3(const T* c) noexcept { r = c[0]; g = c[1]; b = c[2]; }

	explicit operator tuples::templates::Tuple3<T>() noexcept { return tuples::templates::Tuple3<T>(r, g, b); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(r), U(g), U(b)); }
	explicit operator std::tuple<T, T, T>() noexcept { return std::tuple<T, T, T>(r, g, b); }
	template<typename U> explicit operator std::tuple<U, U, U>() noexcept { return std::tuple<U, U, U>(U(r), U(g), U(b)); }
	explicit operator T*() noexcept { return &r; }
	explicit operator const T*() const noexcept { return &r; }
	T& operator[](int i) noexcept { return (&r)[i]; }
	const T& operator[](int i) const noexcept { return (&r)[i]; }

	Color3 operator+() const noexcept { return *this; }
	Color3 operator-() const noexcept { return Color3(-r, -g, -b); }
	Color3& operator+=(ConstArg c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
	Color3& operator-=(ConstArg c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
	Color3& operator*=(ConstArg c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
	Color3& operator*=(T f) noexcept { r *= f; g *= f; b *= f; return *this; }
	Color3& operator/=(ConstArg c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	Color3& operator/=(T f) noexcept { T s = T(1)/f; r *= s; g *= s; b *= s; return *this; }
	friend Color3 operator+(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	friend Color3 operator-(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	friend Color3 operator*(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b); }
	friend Color3 operator*(T f, ConstArg c) noexcept { return Color3(f*c.r, f*c.g, f*c.b); }
	friend Color3 operator*(ConstArg c, T f) noexcept { return Color3(c.r*f, c.g*f, c.b*f); }
	friend Color3 operator/(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r/c2.r, c1.g/c2.g, c1.b/c2.b); }
	friend Color3 operator/(T f, ConstArg c) noexcept { return Color3(f/c.r, f/c.g, f/c.b); }
	friend Color3 operator/(ConstArg c, T f) noexcept { T s = T(1)/f; return Color3(c.r*s, c.g*s, c.b*s); }
	bool operator==(const Color3& c) const noexcept { return (r == c.r) && (g == c.g) && (b == c.b); }
	bool operator!=(const Color3& c) const noexcept { return !(*this == c); }
	friend std::istream& operator>>(std::istream& s, Color3& c);
	friend std::ostream& operator<<(std::ostream& s, const Color3& c) { return s << c.r << ' ' << c.g << ' ' << c.b; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & r & g & b; }

	bool isZero() const noexcept { return (r == T()) && (g == T()) && (b == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(const Color3& c) const noexcept;
	bool isApproxEqual(const Color3& c, T tolerance) const noexcept;
	bool allLessThan(const Color3& c) const noexcept { return (r < c.r) && (g < c.g) && (b < c.b); }
	bool allLessThanEqual(const Color3& c) const noexcept { return (r <= c.r) && (g <= c.g) && (b <= c.b); }
	bool allGreaterThan(const Color3& c) const noexcept { return (r > c.r) && (g > c.g) && (b > c.b); }
	bool allGreaterThanEqual(const Color3& c) const noexcept { return (r >= c.r) && (g >= c.g) && (b >= c.b); }
	bool anyLessThan(const Color3& c) const noexcept { return (r < c.r) || (g < c.g) || (b < c.b); }
	bool anyLessThanEqual(const Color3& c) const noexcept { return (r <= c.r) || (g <= c.g) || (b <= c.b); }
	bool anyGreaterThan(const Color3& c) const noexcept { return (r > c.r) || (g > c.g) || (b > c.b); }
	bool anyGreaterThanEqual(const Color3& c) const noexcept { return (r >= c.r) || (g >= c.g) || (b >= c.b); }
	bool isFinite() const { return std::isfinite(r) && std::isfinite(g) && std::isfinite(b); }
	T getLuminance() const noexcept { return r*T(0.2126) + g*T(0.7152) + b*T(0.0722); }
	T getMinComponent() const { return std::min(std::min(r, g), b); }
	T getMaxComponent() const { return std::max(std::max(r, g), b); }
	Color3& setZero() noexcept { r = T(); g = T(); b = T(); return *this; }
	Color3& set(T r, T g, T b) noexcept { this->r = r; this->g = g; this->b = b; return *this; }
	Color3& setMinimum(const Color3& c1, const Color3& c2);
	Color3& setMaximum(const Color3& c1, const Color3& c2);
	Color3& saturate();
	Color3& makeLinear();
	Color3& makeSrgb();
	template<typename U> U toHsv() const { return rgbToHsv<U>(r, g, b); }
	static Color3 fromHsv(T h, T s, T v) { return hsvToRgb<Color3>(h, s, v); }
	template<typename U> static Color3 fromHsv(U c) { return hsvToRgb<Color3>(std::get<0>(c), std::get<1>(c), std::get<2>(c)); }
	template<typename U> U toXyz() const { return rgbToXyz<U>(r, g, b); }
	static Color3 fromXyz(T x, T y, T z) { return xyzToRgb<Color3>(x, y, z); }
	template<typename U> static Color3 fromXyz(U c) { return xyzToRgb<Color3>(std::get<0>(c), std::get<1>(c), std::get<2>(c)); }
	template<typename U> U toPackedRgb() const noexcept; // #TODO
	template<typename U> static Color3 fromPackedRgb(U c) noexcept; // #TODO
	template<typename U> U toPackedBgr() const noexcept; // #TODO
	template<typename U> static Color3 fromPackedBgr(U c) noexcept; // #TODO
#if IMAGING_NATIVE_ORDER_RGBA
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedRgb<U>(); }
	template<typename U> static Color3 fromPacked/*Native*/(U c) noexcept { return fromPackedRgb(c); }
#else
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedBgr<U>(); }
	template<typename U> static Color3 fromPacked/*Native*/(U c) noexcept { return fromPackedBgr(c); }
#endif

	//static const Color3& getZero() noexcept { return ZERO; }
	//static const Color3& getUnitR() noexcept { return UNIT_R; }
	//static const Color3& getUnitG() noexcept { return UNIT_G; }
	//static const Color3& getUnitB() noexcept { return UNIT_B; }

	static const Color3 ZERO;
	static const Color3 UNIT_R;
	static const Color3 UNIT_G;
	static const Color3 UNIT_B; 
	static const Color3 ONE;
	static const Color3 TOLERANCE;
	static const Color3 INF;
	static const Color3 MINUS_INF;
	static const Color3 LUMINANCE;

	T r, g, b;
};

template<typename T>
inline T luminance(T r, T g, T b) noexcept
{
	return r*T(0.2126) + g*T(0.7152) + b*T(0.0722);
}

template<typename T>
inline T luminance(const Color3<T>& c) noexcept
{
	return c.r*T(0.2126) + c.g*T(0.7152) + c.b*T(0.0722);
}

template<typename T>
inline Color3<T> minimum(const Color3<T>& c1, const Color3<T>& c2)
{
	return Color3<T>(std::min(c1.r, c2.r), std::min(c1.g, c2.g), std::min(c1.b, c2.b));
}

template<typename T>
inline Color3<T> maximum(const Color3<T>& c1, const Color3<T>& c2)
{
	return Color3<T>(std::max(c1.r, c2.r), std::max(c1.g, c2.g), std::max(c1.b, c2.b));
}

template<typename T>
inline Color3<T> saturate(const Color3<T>& c)
{
	return Color3<T>(std::clamp(c.r, T(0), T(1)), std::clamp(c.g, T(0), T(1)), std::clamp(c.b, T(0), T(1)));
}

template<typename T>
inline Color3<T> lerp(const Color3<T>& c1, const Color3<T>& c2, T t) noexcept
{
	return Color3<T>(c1.r + t*(c2.r - c1.r), c1.g + t*(c2.g - c1.g), c1.b + t*(c2.b - c1.b));
}

template<typename T>
inline Color3<T> makeLinear(const Color3<T>& c)
{
	return Color3<T>(makeLinear(c.r), makeLinear(c.g), makeLinear(c.b)); 
}

template<typename T>
inline Color3<T> makeSrgb(const Color3<T>& c)
{
	return Color3<T>(makeSrgb(c.r), makeSrgb(c.g), make Srgb(c.b));
}

template<typename T>
inline std::istream& operator>>(std::istream& s, Color3<T>& c) 
{ 
	return s >> c.r >> std::skipws >> c.g >> std::skipws >> c.b; 
}

template<typename T>
inline bool Color3<T>::isApproxZero() const
{ 
	return (std::fabs(r) < Constants<T>::TOLERANCE) && (std::fabs(g) < Constants<T>::TOLERANCE) &&
		(std::fabs(b) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Color3<T>::isApproxEqual(const Color3<T>& c) const
{ 
	return (std::fabs(c.r - r) < Constants<T>::TOLERANCE) && (std::fabs(c.g - g) < Constants<T>::TOLERANCE) &&
		(std::fabs(c.b - b) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Color3<T>::isApproxEqual(const Color3<T>& c, T tolerance) const
{ 
	return (std::fabs(c.r - r) < tolerance) && (std::fabs(c.g - g) < tolerance) && 
		(std::fabs(c.b - b) < tolerance); 
}

template<typename T>
inline Color3<T>& Color3<T>::setMinimum(const Color3<T>& c1, const Color3<T>& c2)
{ 
	r = std::min(c1.r, c2.r); 
	g = std::min(c1.g, c2.g);
	b = std::min(c1.b, c2.b); 
	return *this;
}

template<typename T>
inline Color3<T>& Color3<T>::setMaximum(const Color3<T>& c1, const Color3<T>& c2)
{ 
	r = std::max(c1.r, c2.r); 
	g = std::max(c1.g, c2.g);
	b = std::max(c1.b, c2.b); 
	return *this; 
}

template<typename T>
inline Color3<T>& Color3<T>::saturate()
{ 
	r = std::clamp(r, T(0), T(1)); 
	g = std::clamp(g, T(0), T(1));
	b = std::clamp(b, T(0), T(1));
	return *this;
}

template<typename T>
inline Color3<T>& Color3<T>::makeLinear()
{ 
	r = makeLinear(r);
	g = makeLinear(g);
	b = makeLinear(b);
	return *this;
}

template<typename T>
inline Color3<T>& Color3<T>::makeSrgb()
{ 
	r = makeSrgb(r);
	g = makeSrgb(g);
	b = makeSrgb(b);
	return *this;
}

template<typename T> const Color3<T> Color3<T>::ZERO{};
template<typename T> const Color3<T> Color3<T>::UNIT_R{ T(1), T(0), T(0) };
template<typename T> const Color3<T> Color3<T>::UNIT_G{ T(0), T(1), T(0) };
template<typename T> const Color3<T> Color3<T>::UNIT_B{ T(0), T(0), T(1) };
template<typename T> const Color3<T> Color3<T>::ONE{ T(1), T(1), T(1) };
template<typename T> const Color3<T> Color3<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };
template<typename T> const Color3<T> Color3<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() };
template<typename T> const Color3<T> Color3<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity() };
template<typename T> const Color3<T> Color3<T>::LUMINANCE{ T(0.2126), T(0.7152), T(0.0722) };

#if SIMD_HAS_FLOAT4

template<>
struct Color3<float>
{
	using Real = float;
	using ConstArg = const Color3;
	using ConstResult = const Color3;

	static constexpr int NUM_COMPONENTS = 3;

	/*constexpr*/ Color3() noexcept { rgb = simd::zero<simd::float4>(); }
#if IMAGING_SIMD_EXPAND_LAST
	/*constexpr*/ explicit Color3(float scalar) noexcept { rgb = simd::set4(scalar); }
	/*constexpr*/ Color3(float r, float g, float b) noexcept { rgb = simd::set4(r, g, b, b); }
	//template<typename U> explicit Color3(const IntColor3<U>&/*IntColor3<U>::ConstArg*/ c) noexcept;
	explicit Color3(const tuples::templates::Tuple3<float>& t) noexcept { rgb = simd::set4(t.x, t.y, t.z, t.z); }
	template<typename U> explicit Color3(const tuples::templates::Tuple3<U>& t) noexcept { float z = (float)t.z; rgb = simd::set4((float)t.x, (float)t.y, z, z); }
	explicit Color3(const std::tuple<float, float, float>& t) noexcept { float z = std::get<2>(t); rgb = simd::set4(std::get<0>(t), std::get<1>(t), z, z); }
	template<typename U> explicit Color3(const std::tuple<U, U, U>& t) noexcept { float z = (float)std::get<2>(t); rgb = simd::set4((float)std::get<0>(t), (float)std::get<1>(t), z, z); }
	explicit Color3(const float* c) noexcept { set(c[0], c[1], c[2]); }
#else
	/*constexpr*/ explicit Color3(float scalar) noexcept { rgb = simd::set3(scalar); }
	/*constexpr*/ Color3(float r, float g, float b) noexcept { rgb = simd::set3(r, g, b); }
	//template<typename U> explicit Color3(const IntColor3<U>&/*IntColor3<U>::ConstArg*/ c) noexcept;
	explicit Color3(const tuples::templates::Tuple3<float>& t) noexcept { rgb = simd::set3(t.x, t.y, t.z); }
	template<typename U> explicit Color3(const tuples::templates::Tuple3<U>& t) noexcept { rgb = simd::set3((float)t.x, (float)t.y, (float)t.z); }
	explicit Color3(const std::tuple<float, float, float>& t) noexcept { rgb = simd::set3(std::get<0>(t), std::get<1>(t), std::get<2>(t)); }
	template<typename U> explicit Color3(const std::tuple<U, U, U>& t) noexcept { rgb = simd::set3((float)std::get<0>(t), (float)std::get<1>(t), (float)std::get<2>(t)); }
	explicit Color3(const float* c) noexcept { rgb = simd::load3(c); }
#endif
	explicit Color3(simd::float4 c) noexcept : rgb(c) {}

	operator simd::float4() const noexcept { return rgb; }
	explicit operator tuples::templates::Tuple3<float>() noexcept { return tuples::templates::Tuple3<float>(r, g, b); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(r), U(g), U(b)); }
	explicit operator std::tuple<float, float, float>() noexcept { return std::tuple<float, float, float>(r, g, b); }
	template<typename U> explicit operator std::tuple<U, U, U>() noexcept { return std::tuple<U, U, U>(U(r), U(g), U(b)); }
	explicit operator float*() noexcept { return &r; }
	explicit operator const float*() const noexcept { return &r; }
	float& operator[](int i) noexcept { return (&r)[i]; }
	const float& operator[](int i) const noexcept { return (&r)[i]; }

	Color3 operator+() const noexcept { return *this; }
#if IMAGING_SIMD_EXPAND_LAST
	Color3 operator-() const noexcept { return Color3(simd::neg4(rgb)); }
#else
	Color3 operator-() const noexcept { return Color3(simd::neg3(rgb)); }
#endif
	Color3& operator+=(ConstArg c) noexcept { rgb = simd::add4(rgb, c); return *this; }
	Color3& operator-=(ConstArg c) noexcept { rgb = simd::sub4(rgb, c); return *this; }
	Color3& operator*=(ConstArg c) noexcept { rgb = simd::mul4(rgb, c); return *this; }
	Color3& operator*=(float f) noexcept { rgb = simd::mul4(rgb, simd::set4(f)); return *this; }
#if IMAGING_SIMD_EXPAND_LAST
	Color3& operator/=(ConstArg c) noexcept { rgb = simd::div4(rgb, c); return *this; }
#else
	Color3& operator/=(ConstArg c) noexcept { rgb = simd::div3(rgb, c); return *this; }
#endif
	Color3& operator/=(float f) noexcept { rgb = simd::div4(rgb, simd::set4(f)); return *this; }
	friend Color3 operator+(ConstArg c1, ConstArg c2) noexcept { return Color3(simd::add4(c1, c2)); }
	friend Color3 operator-(ConstArg c1, ConstArg c2) noexcept { return Color3(simd::sub4(c1, c2)); }
	friend Color3 operator*(ConstArg c1, ConstArg c2) noexcept { return Color3(simd::mul4(c1, c2)); }
	friend Color3 operator*(float f, ConstArg c) noexcept { return Color3(simd::mul4(simd::set4(f), c)); }
	friend Color3 operator*(ConstArg c, float f) noexcept { return Color3(simd::mul4(c, simd::set4(f))); }
#if IMAGING_SIMD_EXPAND_LAST
	friend Color3 operator/(ConstArg c1, ConstArg c2) noexcept { return Color3(simd::div4(c1, c2)); }
	friend Color3 operator/(float f, ConstArg c) noexcept { return Color3(simd::div4(simd::set4(f), c)); }
#else
	friend Color3 operator/(ConstArg c1, ConstArg c2) noexcept { return Color3(simd::div3(c1, c2)); }
	friend Color3 operator/(float f, ConstArg c) noexcept { return Color3(simd::div3(simd::set3(f), c)); }
#endif
	friend Color3 operator/(ConstArg c, float f) noexcept { return Color3(simd::div4(c, simd::set4(f))); }
	bool operator==(const Color3& c) const noexcept { return simd::all3(simd::equal(rgb, c)); }
	bool operator!=(const Color3& c) const noexcept { return !(*this == c); }
	friend std::istream& operator>>(std::istream& s, Color3& c);
	friend std::ostream& operator<<(std::ostream& s, const Color3& c) { return s << c.r << ' ' << c.g << ' ' << c.b; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & r & g & b; } // #FIXME use simd::set(r, g, b, b)

	bool isZero() const noexcept { return simd::all3(simd::equal(rgb, simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept { simd::all3(simd::lessThan(simd::abs4(rgb), TOLERANCE)); }
	bool isApproxEqual(const Color3& c) const noexcept { simd::all3(simd::lessThan(simd::abs4(simd::sub4(rgb, c)), TOLERANCE)); }
	bool isApproxEqual(const Color3& c, float tolerance) const noexcept { simd::all3(simd::lessThan(simd::abs4(simd::sub4(rgb, c)), simd::set4(tolerance))); }
	bool allLessThan(const Color3& c) const noexcept { return simd::all3(simd::lessThan(rgb, c)); }
	bool allLessThanEqual(const Color3& c) const noexcept { return simd::all3(simd::lessThanEqual(rgb, c)); }
	bool allGreaterThan(const Color3& c) const noexcept { return simd::all3(simd::greaterThan(rgb, c)); }
	bool allGreaterThanEqual(const Color3& c) const noexcept { return simd::all3(simd::greaterThanEqual(rgb, c)); }
	bool anyLessThan(const Color3& c) const noexcept { return simd::any3(simd::lessThan(rgb, c)); }
	bool anyLessThanEqual(const Color3& c) const noexcept { return simd::any3(simd::lessThanEqual(rgb, c)); }
	bool anyGreaterThan(const Color3& c) const noexcept { return simd::any3(simd::greaterThan(rgb, c)); }
	bool anyGreaterThanEqual(const Color3& c) const noexcept { return simd::any3(simd::greaterThanEqual(rgb, c)); }
	bool isFinite() const { return simd::all3(simd::isFinite(rgb)); }
	float getLuminance() const noexcept { return simd::toFloat(simd::dot3(rgb, LUMINANCE)); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin3(rgb)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax3(rgb)); }
	Color3& setZero() noexcept { rgb = simd::zero<simd::float4>(); return *this; }
#if IMAGING_SIMD_EXPAND_LAST
	Color3& set(float r, float g, float b) noexcept { rgb = simd::set4(r, g, b, b); return *this; }
#else
	Color3& set(float r, float g, float b) noexcept { rgb = simd::set3(r, g, b); return *this; }
#endif
	Color3& setMinimum(const Color3& c1, const Color3& c2) noexcept { rgb = simd::min4(c1, c2); return *this; }
	Color3& setMaximum(const Color3& c1, const Color3& c2) noexcept { rgb = simd::max4(c1, c2); return *this; }
	Color3& saturate() noexcept { rgb = simd::min4(simd::max4(rgb, simd::zero<simd::float4>()), ONE); return *this; }
	Color3& makeLinear() { set(makeLinear(r), makeLinear(g), makeLinear(b)); }
	Color3& makeSrgb() { set(makeSrgb(r), makeSrgb(g), makeSrgb(b)); }
	template<typename U> U toHsv() const { return rgbToHsv<U>(r, g, b); }
	static Color3 fromHsv(float h, float s, float v) { return hsvToRgb<Color3>(h, s, v); }
	template<typename U> static Color3 fromHsv(U c) { return hsvToRgb<Color3>(std::get<0>(c), std::get<1>(c), std::get<2>(c)); }
	template<typename U> U toXyz() const { return rgbToXyz<U>(rgb); }
	static Color3 fromXyz(float x, float y, float z) { return xyzToRgb<Color3>(x, y, z); }
	template<typename U> static Color3 fromXyz(U c) { return xyzToRgb<Color3>((simd::float4)c/*std::get<simd::float4>(c)*/); }
	template<typename U> U toPackedRgb() const noexcept; // #TODO
	template<typename U> static Color3 fromPackedRgb(U c) noexcept; // #TODO
	template<typename U> U toPackedBgr() const noexcept; // #TODO
	template<typename U> static Color3 fromPackedBgr(U c) noexcept; // #TODO
#if IMAGING_NATIVE_ORDER_RGBA
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedRgb<U>(); }
	template<typename U> static Color3 fromPacked/*Native*/(U c) noexcept { return fromPackedRgb(c); }
#else
	template<typename U> U toPacked/*Native*/() const noexcept { return toPackedBgr<U>(); }
	template<typename U> static Color3 fromPacked/*Native*/(U c) noexcept { return fromPackedBgr(c); }
#endif

	//static const Color3& getZero() noexcept { return ZERO; }
	//static const Color3& getUnitR() noexcept { return UNIT_R; }
	//static const Color3& getUnitG() noexcept { return UNIT_G; }
	//static const Color3& getUnitB() noexcept { return UNIT_B; }

	static const Color3 ZERO;
	static const Color3 UNIT_R;
	static const Color3 UNIT_G;
	static const Color3 UNIT_B;
	static const Color3 ONE;
	static const Color3 TOLERANCE;
	static const Color3 INF;
	static const Color3 MINUS_INF;
	static const Color3 LUMINANCE;

	union
	{
		simd::float4 rgb;
		struct { float r, g, b; };
	};
};

template<>
inline float luminance(float r, float g, float b) noexcept
{
	//static const simd::float4 coeff = simd::set4(0.2126f, 0.7152f, 0.0722f, 0.f);
	return simd::toFloat(simd::dot3(simd::set3(r, g, b), Color3<float>::LUMINANCE/*coeff*/));
}

template<>
inline float luminance(const Color3<float>& c) noexcept
{
	//static const simd::float4 coeff = simd::set4(0.2126f, 0.7152f, 0.0722f, 0.f);
	return simd::toFloat(simd::dot3(c, Color3<float>::LUMINANCE/*coeff*/));
}

template<>
inline Color3<float> minimum(const Color3<float>& c1, const Color3<float>& c2)
{
	return Color3<float>(simd::min4(c1, c2));
}

template<>
inline Color3<float> maximum(const Color3<float>& c1, const Color3<float>& c2)
{
	return Color3<float>(simd::max4(c1, c2));
}

template<>
inline Color3<float> saturate(const Color3<float>& c)
{
	//static const simd::float4 one = simd::set4(1.f);
	return Color3<float>(simd::min4(simd::max4(c, simd::zero<simd::float4>()), Color3<float>::ONE/*one*/));
}

template<>
inline Color3<float> lerp(const Color3<float>& c1, const Color3<float>& c2, float t) noexcept
{
	return Color3<float>(simd::mulAdd4(simd::set4(t), simd::sub4(c2, c1), c1));
}

template<>
inline std::istream& operator>>(std::istream& s, Color3<float>& c) 
{ 
	float r, g, b; 
	s >> r >> std::skipws >> g >> std::skipws >> b; 
	c.set(r, g, b); 
	return s; 
}

const Color3<float> Color3<float>::ZERO{};
const Color3<float> Color3<float>::UNIT_R{ 1.f, 0.f, 0.f };
const Color3<float> Color3<float>::UNIT_G{ 0.f, 1.f, 0.f };
const Color3<float> Color3<float>::UNIT_B{ 0.f, 0.f, 1.f };
const Color3<float> Color3<float>::ONE{ 1.f, 1.f, 1.f };
const Color3<float> Color3<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };
const Color3<float> Color3<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
const Color3<float> Color3<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
const Color3<float> Color3<float>::LUMINANCE{ 0.2126f, 0.7152f, 0.0722f };

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Color3 = templates::Color3<float>;
using Color3Arg = templates::Color3<float>::ConstArg;
using Color3Result = templates::Color3<float>::ConstResult;

} // namespace imaging
} // namespace core

//#include "IntColor3.hpp"

namespace core::imaging::templates {

//template<typename T>
//template<typename U> 
//inline Color3<T>::Color3(const IntColor3<U>& c) : r(T(c.r)), g(T(c.g)), b(T(c.b))
//{
//}

#if SIMD_HAS_FLOAT4

//template<typename U> 
//inline Color3<float>::Color3(const IntColor3<U>& c)
//{
//	float t = (float)c.b;
//	rgb = simd::set4((float)c.r, (float)c.g, t, t); 
//}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace core::imaging::templates

namespace std
{

template<typename T>
struct tuple_size<core::imaging::templates::Color3<T>> : std::integral_constant<std::size_t, 3> {};

template<std::size_t I, typename T>
inline T& get(core::imaging::templates::Color3<T>& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const core::imaging::templates::Color3<T>& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(core::imaging::templates::Color3<T>&& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const core::imaging::templates::Color3<T>&& c) noexcept
{
	if constexpr (I == 0)
		return c.r;
	else if constexpr (I == 1)
		return c.g;
	else if constexpr (I == 2)
		return c.b;
	static_assert(false);
}

#if SIMD_HAS_FLOAT4

//template<typename T, typename U>
//inline T& get(core::imaging::templates::Color3<U>& c) noexcept
//{
//}
//
//template<typename T, typename U>
//inline const T& get(const core::imaging::templates::Color3<U>& c) noexcept
//{
//}
//
//template<>
//inline core::simd::float4& get(core::imaging::templates::Color3<float>& c) noexcept
//{
//	return c.rgb;
//}
//
//template<>
//inline const core::simd::float4& get(const core::imaging::templates::Color3<float>& c) noexcept
//{
//	return c.rgb;
//}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace std

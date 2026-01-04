/*
 *	Name: Color3
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_IMAGING_COLOR3_HPP
#define CORE_IMAGING_COLOR3_HPP

#include <cmath>
#include <istream>
#include <ostream>
#include <limits>
#include <algorithm>
#include <Simd.hpp>
#include <Tuples.hpp>

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
	constexpr explicit Color3(const T scalar) noexcept : r(scalar), g(scalar), b(scalar) {}
	constexpr Color3(const T r, const T g, const T b) noexcept : r(r), g(g), b(b) {}
	//template<typename U> explicit Color3(const IntColor3<U>&/*IntColor3<U>::ConstArg*/ c) noexcept;
	explicit Color3(const tuples::templates::Tuple3<T>& t) noexcept : r(t.x), g(t.y), b(t.z) {}
	template<typename U> explicit Color3(const tuples::templates::Tuple3<U>& t) noexcept : r(T(t.x)), g(T(t.y)), b(T(t.z)) {}
	explicit Color3(const T* const c) noexcept { r = c[0]; g = c[1]; b = c[2]; }

	explicit operator tuples::templates::Tuple3<T>() noexcept { return tuples::templates::Tuple3<T>(r, g, b); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(r), U(g), U(b)); }
	explicit operator T*() noexcept { return &r; }
	explicit operator const T*() const noexcept { return &r; }
	T& operator[](int i) noexcept { return (&r)[i]; }
	const T& operator[](int i) const noexcept { return (&r)[i]; }

	Color3 operator+() const noexcept { return *this; }
	Color3 operator-() const noexcept { return Color3(-r, -g, -b); }
	Color3& operator+=(ConstArg c) noexcept { r += c.r; g += c.g; b += c.b; return *this; }
	Color3& operator-=(ConstArg c) noexcept { r -= c.r; g -= c.g; b -= c.b; return *this; }
	Color3& operator*=(ConstArg c) noexcept { r *= c.r; g *= c.g; b *= c.b; return *this; }
	Color3& operator*=(const T f) noexcept { r *= f; g *= f; b *= f; return *this; }
	Color3& operator/=(ConstArg c) noexcept { r /= c.r; g /= c.g; b /= c.b; return *this; }
	Color3& operator/=(const T f) noexcept { const T s = T(1)/f; r *= s; g *= s; b *= s; return *this; }
	friend Color3 operator+(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b); }
	friend Color3 operator-(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b); }
	friend Color3 operator*(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b); }
	friend Color3 operator*(const T f, ConstArg c) noexcept { return Color3(f*c.r, f*c.g, f*c.b); }
	friend Color3 operator*(ConstArg c, const T f) noexcept { return Color3(c.r*f, c.g*f, c.b*f); }
	friend Color3 operator/(ConstArg c1, ConstArg c2) noexcept { return Color3(c1.r/c2.r, c1.g/c2.g, c1.b/c2.b); }
	friend Color3 operator/(const T f, ConstArg c) noexcept { return Color3(f/c.r, f/c.g, f/c.b); }
	friend Color3 operator/(ConstArg c, const T f) noexcept { const T s = T(1)/f; return Color3(c.r*s, c.g*s, c.b*s); }
	bool operator==(const Color3& c) const noexcept { return (r == c.r) && (g == c.g) && (b == c.b); }
	bool operator!=(const Color3& c) const noexcept { return !(*this == c); }
	friend std::istream& operator>>(std::istream& s, Color3& c) { return s >> c.r >> std::skipws >> c.g >> std::skipws >> c.b; }
	friend std::ostream& operator<<(std::ostream& s, const Color3& c) { return s << c.r << ' ' << c.g << ' ' << c.b; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & r & g & b; }

	bool isZero() const noexcept { return (r == T()) && (g == T()) && (b == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(ConstArg c) const noexcept;
	bool isApproxEqual(ConstArg c, const T tolerance) const noexcept;
	bool allLessThan(ConstArg c) const noexcept { return (r < c.r) && (g < c.g) && (b < c.b); }
	bool allLessThanEqual(ConstArg c) const noexcept { return (r <= c.r) && (g <= c.g) && (b <= c.b); }
	bool allGreaterThan(ConstArg c) const noexcept { return (r > c.r) && (g > c.g) && (b > c.b); }
	bool allGreaterThanEqual(ConstArg c) const noexcept { return (r >= c.r) && (g >= c.g) && (b >= c.b); }
	bool anyLessThan(ConstArg c) const noexcept { return (r < c.r) || (g < c.g) || (b < c.b); }
	bool anyLessThanEqual(ConstArg c) const noexcept { return (r <= c.r) || (g <= c.g) || (b <= c.b); }
	bool anyGreaterThan(ConstArg c) const noexcept { return (r > c.r) || (g > c.g) || (b > c.b); }
	bool anyGreaterThanEqual(ConstArg c) const noexcept { return (r >= c.r) || (g >= c.g) || (b >= c.b); }
	bool isFinite() const { return std::isfinite(r) && std::isfinite(g) && std::isfinite(b); }
	T getMinComponent() const noexcept { return std::min(std::min(r, g), b); }
	T getMaxComponent() const noexcept { return std::max(std::max(r, g), b); }
	Color3& setZero() noexcept { r = T(); g = T(); b = T(); return *this; }
	Color3& set(const T r, const T g, const T b) noexcept { this->r = r; this->g = g; this->b = b; return *this; }
	Color3& setMinimum(ConstArg c1, ConstArg c2) noexcept;
	Color3& setMaximum(ConstArg c1, ConstArg c2) noexcept;

	static const Color3& getZero() noexcept { return ZERO; }
	static const Color3& getUnitR() noexcept { return UNIT_R; }
	static const Color3& getUnitG() noexcept { return UNIT_G; }
	static const Color3& getUnitB() noexcept { return UNIT_B; }

	static const Color3 ZERO;
	static const Color3 UNIT_R;
	static const Color3 UNIT_G;
	static const Color3 UNIT_B; 
	static const Color3 ONE;
	static const Color3 TOLERANCE;
	static const Color3 INF;
	static const Color3 MINUS_INF;

	T r, g, b;
};

template<typename T>
inline bool Color3<T>::isApproxZero() const
{ 
	return (std::fabs(r) < Constants<T>::TOLERANCE) && (std::fabs(g) < Constants<T>::TOLERANCE) &&
		(std::fabs(b) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Color3<T>::isApproxEqual(ConstArg c) const
{ 
	return (std::fabs(c.r - r) < Constants<T>::TOLERANCE) && (std::fabs(c.g - g) < Constants<T>::TOLERANCE) &&
		(std::fabs(c.b - b) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Color3<T>::isApproxEqual(ConstArg c, const T tolerance) const
{ 
	return (std::fabs(c.r - r) < tolerance) && (std::fabs(c.g - g) < tolerance) && 
		(std::fabs(c.b - b) < tolerance); 
}

template<typename T>
inline Color3<T>& Color3<T>::setMinimum(ConstArg c1, ConstArg c2)
{ 
	r = std::min(c1.r, c2.r); 
	g = std::min(c1.g, c2.g);
	b = std::min(c1.b, c2.b); 
	return *this;
}

template<typename T>
inline Color3<T>& Color3<T>::setMaximum(ConstArg c1, ConstArg c2)
{ 
	r = std::max(c1.r, c2.r); 
	g = std::max(c1.g, c2.g);
	b = std::max(c1.b, c2.b); 
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

#if SIMD_HAS_FLOAT4

namespace float4 = simd::float4;

template<>
struct Color3<float>
{
	using Real = float;
	using ConstArg = const Color3;
	using ConstResult = const Color3;

	static constexpr int NUM_COMPONENTS = 3;

	/*constexpr*/ Color3() noexcept { rgb = float4::zero(); }
#if IMAGING_SIMD_EXPAND_LAST
	/*constexpr*/ explicit Color3(const float scalar) noexcept { rgb = float4::set4(scalar); }
	/*constexpr*/ Color3(const float r, const float g, const float b) noexcept { rgb = float4::set4(r, g, b, b); }
	//template<typename U> explicit Color3(const IntColor3<U>&/*IntColor3<U>::ConstArg*/ c) noexcept;
	explicit Color3(const tuples::templates::Tuple3<float>& t) noexcept { rgb = float4::set4(t.x, t.y, t.z, t.z); }
	template<typename U> explicit Color3(const tuples::templates::Tuple3<U>& t) noexcept { float z = (float)t.z; rgb = float4::set4((float)t.x, (float)t.y, z, z); }
	explicit Color3(const float* const c) noexcept { set(c[0], c[1], c[2]); }
#else
	/*constexpr*/ explicit Color3(const float scalar) noexcept { rgb = float4::set3(scalar); }
	/*constexpr*/ Color3(const float r, const float g, const float b) noexcept { rgb = float4::set3(r, g, b); }
	//template<typename U> explicit Color3(const IntColor3<U>&/*IntColor3<U>::ConstArg*/ c) noexcept;
	explicit Color3(const tuples::templates::Tuple3<float>& t) noexcept { rgb = float4::set3(t.x, t.y, t.z); }
	template<typename U> explicit Color3(const tuples::templates::Tuple3<U>& t) noexcept { rgb = float4::set3((float)t.x, (float)t.y, (float)t.z); }
	explicit Color3(const float* const c) noexcept { rgb = float4::load3(c); }
#endif
	explicit Color3(const float4::Type c) noexcept : rgb(c) {}

	operator float4::Type() const noexcept { return rgb; }
	explicit operator tuples::templates::Tuple3<float>() noexcept { return tuples::templates::Tuple3<float>(r, g, b); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(r), U(g), U(b)); }
	explicit operator float*() noexcept { return &r; }
	explicit operator const float*() const noexcept { return &r; }
	float& operator[](int i) noexcept { return (&r)[i]; }
	const float& operator[](int i) const noexcept { return (&r)[i]; }

	Color3 operator+() const noexcept { return *this; }
#if IMAGING_SIMD_EXPAND_LAST
	Color3 operator-() const noexcept { return Color3(float4::neg4(rgb)); }
#else
	Color3 operator-() const noexcept { return Color3(float4::neg3(rgb)); }
#endif
	Color3& operator+=(ConstArg c) noexcept { rgb = float4::add4(rgb, c); return *this; }
	Color3& operator-=(ConstArg c) noexcept { rgb = float4::sub4(rgb, c); return *this; }
	Color3& operator*=(ConstArg c) noexcept { rgb = float4::mul4(rgb, c); return *this; }
	Color3& operator*=(const float f) noexcept { rgb = float4::mul4(rgb, float4::set4(f)); return *this; }
#if IMAGING_SIMD_EXPAND_LAST
	Color3& operator/=(ConstArg c) noexcept { rgb = float4::div4(rgb, c); return *this; }
#else
	Color3& operator/=(ConstArg c) noexcept { rgb = float4::div3(rgb, c); return *this; }
#endif
	Color3& operator/=(const float f) noexcept { rgb = float4::mul4(rgb, float4::set4(1.f/f)); return *this; }
	friend Color3 operator+(ConstArg c1, ConstArg c2) noexcept { return Color3(float4::add4(c1, c2)); }
	friend Color3 operator-(ConstArg c1, ConstArg c2) noexcept { return Color3(float4::sub4(c1, c2)); }
	friend Color3 operator*(ConstArg c1, ConstArg c2) noexcept { return Color3(float4::mul4(c1, c2)); }
	friend Color3 operator*(const float f, ConstArg c) noexcept { return Color3(float4::mul4(float4::set4(f), c)); }
	friend Color3 operator*(ConstArg c, const float f) noexcept { return Color3(float4::mul4(c, float4::set4(f))); }
#if IMAGING_SIMD_EXPAND_LAST
	friend Color3 operator/(ConstArg c1, ConstArg c2) noexcept { return Color3(float4::div4(c1, c2)); }
	friend Color3 operator/(const float f, ConstArg c) noexcept { return Color3(float4::div4(float4::set4(f), c)); }
#else
	friend Color3 operator/(ConstArg c1, ConstArg c2) noexcept { return Color3(float4::div3(c1, c2)); }
	friend Color3 operator/(const float f, ConstArg c) noexcept { return Color3(float4::div3(float4::set3(f), c)); }
#endif
	friend Color3 operator/(ConstArg c, const float f) noexcept { return Color3(float4::mul4(c, float4::set4(1.f/f))); }
	bool operator==(const Color3& c) const noexcept { return float4::all3(float4::equal(rgb, c)); }
	bool operator!=(const Color3& c) const noexcept { return !(*this == c); }
	friend std::istream& operator>>(std::istream& s, Color3& c);
	friend std::ostream& operator<<(std::ostream& s, const Color3& c) { return s << c.r << ' ' << c.g << ' ' << c.b; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & r & g & b; } // #FIXME use float4::set(r, g, b, b)

	bool isZero() const noexcept { return float4::all3(float4::equal(rgb, float4::zero())); }
	bool isApproxZero() const noexcept { float4::all3(float4::lessThan(float4::abs4(rgb), TOLERANCE)); }
	bool isApproxEqual(ConstArg c) const noexcept { float4::all3(float4::lessThan(float4::abs4(float4::sub4(rgb, c)), TOLERANCE)); }
	bool isApproxEqual(ConstArg c, const float tolerance) const noexcept { float4::all3(float4::lessThan(float4::abs4(float4::sub4(rgb, c)), float4::set4(tolerance))); }
	bool allLessThan(ConstArg c) const noexcept { return float4::all3(float4::lessThan(rgb, c)); }
	bool allLessThanEqual(ConstArg c) const noexcept { return float4::all3(float4::lessThanEqual(rgb, c)); }
	bool allGreaterThan(ConstArg c) const noexcept { return float4::all3(float4::greaterThan(rgb, c)); }
	bool allGreaterThanEqual(ConstArg c) const noexcept { return float4::all3(float4::greaterThanEqual(rgb, c)); }
	bool anyLessThan(ConstArg c) const noexcept { return float4::any3(float4::lessThan(rgb, c)); }
	bool anyLessThanEqual(ConstArg c) const noexcept { return float4::any3(float4::lessThanEqual(rgb, c)); }
	bool anyGreaterThan(ConstArg c) const noexcept { return float4::any3(float4::greaterThan(rgb, c)); }
	bool anyGreaterThanEqual(ConstArg c) const noexcept { return float4::any3(float4::greaterThanEqual(rgb, c)); }
	bool isFinite() const { return float4::all3(float4::isFinite(rgb)); }
	float getMinComponent() const noexcept { return float4::toFloat(float4::hMin3(rgb)); }
	float getMaxComponent() const noexcept { return float4::toFloat(float4::hMax3(rgb)); }
	Color3& setZero() noexcept { rgb = float4::zero(); return *this; }
#if IMAGING_SIMD_EXPAND_LAST
	Color3& set(const float r, const float g, const float b) noexcept { rgb = float4::set4(r, g, b, b); return *this; }
#else
	Color3& set(const float r, const float g, const float b) noexcept { rgb = float4::set3(r, g, b); return *this; }
#endif
	Color3& setMinimum(ConstArg c1, ConstArg c2) noexcept { rgb = float4::min4(c1, c2); return *this; }
	Color3& setMaximum(ConstArg c1, ConstArg c2) noexcept { rgb = float4::max4(c1, c2); return *this; }

	static const Color3& getZero() noexcept { return ZERO; }
	static const Color3& getUnitR() noexcept { return UNIT_R; }
	static const Color3& getUnitG() noexcept { return UNIT_G; }
	static const Color3& getUnitB() noexcept { return UNIT_B; }

	static const Color3 ZERO;
	static const Color3 UNIT_R;
	static const Color3 UNIT_G;
	static const Color3 UNIT_B;
	static const Color3 ONE;
	static const Color3 TOLERANCE;
	static const Color3 INF;
	static const Color3 MINUS_INF;

	union
	{
		float4::Type rgb;
		struct { float r, g, b; };
	};
};

inline std::istream& Color3<float>::operator>>(std::istream& s, Color3<float>& c) 
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

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Color3 = templates::Color3<float>;
using Color3Arg = templates::Color3<float>::ConstArg;
using Color3Result = templates::Color3<float>::ConstResult;

} // namespace imaging
} // namespace core

//#include "IntColor3.hpp"

namespace core {
namespace imaging {
namespace templates {

//template<typename T>
//template<typename U> 
//inline Color3<T>::Color3(const IntColor3<U>& c) : r(T(c.r)), g(T(c.g)), b(T(c.b))
//{
//}

#if SIMD_HAS_FLOAT4

//template<typename U> 
//inline Color3<float>::Color3(const IntColor3<U>& c)
//{
//	const float t = (float)c.b;
//	rgb = simd::float4::set4((float)c.r, (float)c.g, t, t); 
//}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace imaging
} // namespace core

#endif /* CORE_IMAGING_COLOR3_HPP */

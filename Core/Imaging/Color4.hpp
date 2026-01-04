/*
 *	Name: Color4
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_IMAGING_COLOR4_HPP
#define CORE_IMAGING_COLOR4_HPP

#include <cmath>
#include <istream>
#include <ostream>
#include <limits>
#include <algorithm>
#include <Simd.hpp>
#include <Tuples.hpp>
#include "Color3.hpp"

namespace core {
namespace imaging {
namespace templates {

//template<typename T>
//struct IntColor4;

template<typename T>
struct Color4
{
	using Real = T;
	using ConstArg = const Color4&;
	using ConstResult = const Color4&;

	static constexpr int NUM_COMPONENTS = 4;

	constexpr Color4() noexcept : r(), g(), b(), a() {}
	constexpr explicit Color4(const T scalar) noexcept : r(scalar), g(scalar), b(scalar), a(scalar) {}
	constexpr Color4(const T r, const T g, const T b, const T a) noexcept : r(r), g(g), b(b), a(a) {}
	constexpr Color4(Color3<T>::ConstArg c) noexcept : r(c.r), g(c.g), b(c.b), a(T(1)) {}
	constexpr Color4(Color3<T>::ConstArg c, const T a) noexcept : r(c.r), g(c.g), b(c.b), a(a) {}
	//template<typename U> explicit Color4(const IntColor4<U>&/*IntColor4<U>::ConstArg*/ c) noexcept;
	explicit Color4(const tuples::templates::Tuple4<T>& t) noexcept : r(t.x), g(t.y), b(t.z), a(t.w) {}
	template<typename U> explicit Color4(const tuples::templates::Tuple4<U>& t) noexcept : r(T(t.x)), g(T(t.y)), b(T(t.z)), a(T(t.w)) {}
	explicit Color4(const T* const c) noexcept { r = c[0]; g = c[1]; b = c[2]; a = c[3]; }

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(r, g, b, a); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(r), U(g), U(b), U(a)); }
	explicit operator T*() noexcept { return &r; }
	explicit operator const T*() const noexcept { return &r; }
	T& operator[](int i) noexcept { return (&r)[i]; }
	const T& operator[](int i) const noexcept { return (&r)[i]; }

	Color4 operator+() const noexcept { return *this; }
	Color4 operator-() const noexcept { return Color4(-r, -g, -b, -a); }
	Color4& operator+=(ConstArg c) noexcept { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
	Color4& operator-=(ConstArg c) noexcept { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
	Color4& operator*=(ConstArg c) noexcept { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
	Color4& operator*=(const T f) noexcept { r *= f; g *= f; b *= f; a *= f; return *this; }
	Color4& operator/=(ConstArg c) noexcept { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }
	Color4& operator/=(const T f) noexcept { const T s = T(1)/f; r *= s; g *= s; b *= s; a *= s; return *this; }
	friend Color4 operator+(ConstArg c1, ConstArg c2) noexcept { return Color4(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	friend Color4 operator-(ConstArg c1, ConstArg c2) noexcept { return Color4(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a); }
	friend Color4 operator*(ConstArg c1, ConstArg c2) noexcept { return Color4(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b, c1.a*c2.a); }
	friend Color4 operator*(const T f, ConstArg c) noexcept { return Color4(f*c.r, f*c.g, f*c.b, f*c.a); }
	friend Color4 operator*(ConstArg c, const T f) noexcept { return Color4(c.r*f, c.g*f, c.b*f, c.a*f); }
	friend Color4 operator/(ConstArg c1, ConstArg c2) noexcept { return Color4(c1.r/c2.r, c1.g/c2.g, c1.b/c2.b, c1.a/c2.a); }
	friend Color4 operator/(const T f, ConstArg c) noexcept { return Color4(f/c.r, f/c.g, f/c.b, f/c.a); }
	friend Color4 operator/(ConstArg c, const T f) noexcept { const T s = T(1)/f; return Color4(c.r*s, c.g*s, c.b*s, c.a*s); }
	bool operator==(const Color4& c) const noexcept { return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a); }
	bool operator!=(const Color4& c) const noexcept { return !(*this == c); }
	friend std::istream& operator>>(std::istream& s, Color4& c) { return s >> c.r >> std::skipws >> c.g >> std::skipws >> c.b >> std::skipws >> c.a; }
	friend std::ostream& operator<<(std::ostream& s, const Color4& c) { return s << c.r << ' ' << c.g << ' ' << c.b << ' ' << c.a; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & r & g & b & a; }

	Color3<T>::ConstResult getRgb() const noexcept { return *reinterpret_cast<const Color3*>(this); }
	void setRgb(Color3<T>::ConstArg c) noexcept { r = c.r; g = c.g; b = c.b; }
	bool isZero() const noexcept { return (r == T()) && (g == T()) && (b == T()) && (a == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(ConstArg c) const noexcept;
	bool isApproxEqual(ConstArg c, const T tolerance) const noexcept;
	bool allLessThan(ConstArg c) const noexcept { return (r < c.r) && (g < c.g) && (b < c.b) && (a < c.a); }
	bool allLessThanEqual(ConstArg c) const noexcept { return (r <= c.r) && (g <= c.g) && (b <= c.b) && (a <= c.a); }
	bool allGreaterThan(ConstArg c) const noexcept { return (r > c.r) && (g > c.g) && (b > c.b) && (a > c.a); }
	bool allGreaterThanEqual(ConstArg c) const noexcept { return (r >= c.r) && (g >= c.g) && (b >= c.b) && (a >= c.a); }
	bool anyLessThan(ConstArg c) const noexcept { return (r < c.r) || (g < c.g) || (b < c.b) || (a < c.a); }
	bool anyLessThanEqual(ConstArg c) const noexcept { return (r <= c.r) || (g <= c.g) || (b <= c.b) || (a <= c.a); }
	bool anyGreaterThan(ConstArg c) const noexcept { return (r > c.r) || (g > c.g) || (b > c.b) || (a > c.a); }
	bool anyGreaterThanEqual(ConstArg c) const noexcept { return (r >= c.r) || (g >= c.g) || (b >= c.b) || (a >= c.a); }
	bool isFinite() const { return std::isfinite(r) && std::isfinite(g) && std::isfinite(b) && std::isfinite(a); }
	T getMinComponent() const noexcept { return std::min(std::min(std::min(r, g), b), a); }
	T getMaxComponent() const noexcept { return std::max(std::max(std::max(r, g), b), a); }
	Color4& setZero() noexcept { r = T(); g = T(); b = T(); a = T(); return *this; }
	Color4& set(const T r, const T g, const T b, const T a) noexcept { this->r = r; this->g = g; this->b = b; this->a = a; return *this; }
	Color4& setMinimum(ConstArg c1, ConstArg c2) noexcept;
	Color4& setMaximum(ConstArg c1, ConstArg c2) noexcept;

	static const Color4& getZero() noexcept { return ZERO; }
	static const Color4& getUnitR() noexcept { return UNIT_R; }
	static const Color4& getUnitG() noexcept { return UNIT_G; }
	static const Color4& getUnitB() noexcept { return UNIT_B; }
	static const Color4& getUnitA() noexcept { return UNIT_A; }

	static const Color4 ZERO;
	static const Color4 UNIT_R;
	static const Color4 UNIT_G;
	static const Color4 UNIT_B; 
	static const Color4 UNIT_A; 
	static const Color4 ONE;
	static const Color4 TOLERANCE;
	static const Color4 INF;
	static const Color4 MINUS_INF;

	T r, g, b, a;
};

template<typename T>
inline bool Color4<T>::isApproxZero() const
{ 
	return (std::fabs(r) < Constants<T>::TOLERANCE) && (std::fabs(g) < Constants<T>::TOLERANCE) && 
		(std::fabs(b) < Constants<T>::TOLERANCE) && (std::fabs(a) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Color4<T>::isApproxEqual(ConstArg c) const
{ 
	return (std::fabs(c.r - r) < Constants<T>::TOLERANCE) && (std::fabs(c.g - g) < Constants<T>::TOLERANCE) && 
		(std::fabs(c.b - b) < Constants<T>::TOLERANCE) && (std::fabs(c.a - a) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Color4<T>::isApproxEqual(ConstArg c, const T tolerance) const
{ 
	return (std::fabs(c.r - r) < tolerance) && (std::fabs(c.g - g) < tolerance) && 
		(std::fabs(c.b - b) < tolerance) && (std::fabs(c.a - a) < tolerance); 
}

template<typename T>
inline Color4<T>& Color4<T>::setMinimum(ConstArg c1, ConstArg c2)
{ 
	r = std::min(c1.r, c2.r); 
	g = std::min(c1.g, c2.g);
	b = std::min(c1.b, c2.b); 
	a = std::min(c1.a, c2.a); 
	return *this;
}

template<typename T>
inline Color4<T>& Color4<T>::setMaximum(ConstArg c1, ConstArg c2)
{ 
	r = std::max(c1.r, c2.r); 
	g = std::max(c1.g, c2.g);
	b = std::max(c1.b, c2.b); 
	a = std::max(c1.a, c2.a); 
	return *this;
}

template<typename T> const Color4<T> Color4<T>::ZERO{};
template<typename T> const Color4<T> Color4<T>::UNIT_R{ T(1), T(0), T(0), T(0) };
template<typename T> const Color4<T> Color4<T>::UNIT_G{ T(0), T(1), T(0), T(0) };
template<typename T> const Color4<T> Color4<T>::UNIT_B{ T(0), T(0), T(1), T(0) };
template<typename T> const Color4<T> Color4<T>::UNIT_A{ T(0), T(0), T(0), T(1) };
template<typename T> const Color4<T> Color4<T>::ONE{ T(1), T(1), T(1), T(1) };
template<typename T> const Color4<T> Color4<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };
template<typename T> const Color4<T> Color4<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() };
template<typename T> const Color4<T> Color4<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity() };

#if SIMD_HAS_FLOAT4

namespace float4 = simd::float4;

template<>
struct Color4<float>
{
	using Real = float;
	using ConstArg = const Color4;
	using ConstResult = const Color4;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Color4() noexcept { rgba = float4::zero(); }
	/*constexpr*/ explicit Color4(const float scalar) noexcept { rgba = float4::set4(scalar); }
	/*constexpr*/ Color4(const float r, const float g, const float b, const float a) noexcept { rgba = float4::set4(r, g, b, a); }
	/*constexpr*/ Color4(Color3<float>::ConstArg c) noexcept { rgba = float4::insert3(c, UNIT_A); }
	/*constexpr*/ Color4(Color3<float>::ConstArg c, const float a) noexcept { rgba = float4::insert<3>(a, c); }
	//template<typename U> explicit Color4(const IntColor4<U>&/*IntColor4<U>::ConstArg*/ c) noexcept;
	explicit Color4(const tuples::templates::Tuple4<float>& t) noexcept { rgba = float4::set4(t.x, t.y, t.z, t.w); }
	template<typename U> explicit Color4(const tuples::templates::Tuple4<U>& t) noexcept { rgba = float4::set4((float)t.x, (float)t.y, (float)t.z, (float)t.w); }
	explicit Color4(const float* const c) noexcept { rgba = float4::load4(c); }
	explicit Color4(const float4::Type c) noexcept : rgba(c) {}

	operator float4::Type() const noexcept { return rgba; }
	explicit operator tuples::templates::Tuple4<float>() noexcept { return tuples::templates::Tuple4<float>(r, g, b, a); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(r), U(g), U(b), U(a)); }
	explicit operator float*() noexcept { return &r; }
	explicit operator const float*() const noexcept { return &r; }
	float& operator[](int i) noexcept { return (&r)[i]; }
	const float& operator[](int i) const noexcept { return (&r)[i]; }

	Color4 operator+() const noexcept { return *this; }
	Color4 operator-() const noexcept { return Color4(float4::neg4(rgba)); }
	Color4& operator+=(ConstArg c) noexcept { rgba = float4::add4(rgba, c); return *this; }
	Color4& operator-=(ConstArg c) noexcept { rgba = float4::sub4(rgba, c); return *this; }
	Color4& operator*=(ConstArg c) noexcept { rgba = float4::mul4(rgba, c); return *this; }
	Color4& operator*=(const float f) noexcept { rgba = float4::mul4(rgba, float4::set4(f)); return *this; }
	Color4& operator/=(ConstArg c) noexcept { rgba = float4::div4(rgba, c); return *this; }
	Color4& operator/=(const float f) noexcept { rgba = float4::mul4(rgba, float4::set4(1.f/f)); return *this; }
	friend Color4 operator+(ConstArg c1, ConstArg c2) noexcept { return Color4(float4::add4(c1, c2)); }
	friend Color4 operator-(ConstArg c1, ConstArg c2) noexcept { return Color4(float4::sub4(c1, c2)); }
	friend Color4 operator*(ConstArg c1, ConstArg c2) noexcept { return Color4(float4::mul4(c1, c2)); }
	friend Color4 operator*(const float f, ConstArg c) noexcept { return Color4(float4::mul4(float4::set4(f), c)); }
	friend Color4 operator*(ConstArg c, const float f) noexcept { return Color4(float4::mul4(c, float4::set4(f))); }
	friend Color4 operator/(ConstArg c1, ConstArg c2) noexcept { return Color4(float4::div4(c1, c2)); }
	friend Color4 operator/(const float f, ConstArg c) noexcept { return Color4(float4::div4(float4::set4(f), c)); }
	friend Color4 operator/(ConstArg c, const float f) noexcept { return Color4(float4::mul4(c, float4::set4(1.f/f))); }
	bool operator==(const Color4& c) const noexcept { return float4::all4(float4::equal(rgba, c)); }
	bool operator!=(const Color4& c) const noexcept { return !(*this == c); }
	friend std::istream& operator>>(std::istream& s, Color4& c);
	friend std::ostream& operator<<(std::ostream& s, const Color4& c) { return s << c.r << ' ' << c.g << ' ' << c.b << ' ' << c.a; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & r & g & b & a; } // #FIXME use float4::set(r, g, b, a)

#if IMAGING_SIMD_EXPAND_LAST
	Color3<float> getRgb() const noexcept { return Color3<float>(float4::swizzle<float4::XYZZ>(rgba)); }
#else
	Color3<float> getRgb() const noexcept { return Color3<float>(float4::cutoff3(rgba)); }
#endif
	void setRgb(Color3<float>::ConstArg c) noexcept { rgba = float4::insert3(c, rgba); }
	bool isZero() const noexcept { return float4::all4(float4::equal(rgba, float4::zero())); }
	bool isApproxZero() const noexcept { float4::all4(float4::lessThan(float4::abs4(rgba), TOLERANCE)); }
	bool isApproxEqual(ConstArg c) const noexcept { float4::all4(float4::lessThan(float4::abs4(float4::sub4(rgba, c)), TOLERANCE)); }
	bool isApproxEqual(ConstArg c, const float tolerance) const noexcept { float4::all4(float4::lessThan(float4::abs4(float4::sub4(rgba, c)), float4::set4(tolerance))); }
	bool allLessThan(ConstArg c) const noexcept { return float4::all4(float4::lessThan(rgba, c)); }
	bool allLessThanEqual(ConstArg c) const noexcept { return float4::all4(float4::lessThanEqual(rgba, c)); }
	bool allGreaterThan(ConstArg c) const noexcept { return float4::all4(float4::greaterThan(rgba, c)); }
	bool allGreaterThanEqual(ConstArg c) const noexcept { return float4::all4(float4::greaterThanEqual(rgba, c)); }
	bool anyLessThan(ConstArg c) const noexcept { return float4::any4(float4::lessThan(rgba, c)); }
	bool anyLessThanEqual(ConstArg c) const noexcept { return float4::any4(float4::lessThanEqual(rgba, c)); }
	bool anyGreaterThan(ConstArg c) const noexcept { return float4::any4(float4::greaterThan(rgba, c)); }
	bool anyGreaterThanEqual(ConstArg c) const noexcept { return float4::any4(float4::greaterThanEqual(rgba, c)); }
	bool isFinite() const { return float4::all4(float4::isFinite(rgba)); }
	float getMinComponent() const noexcept { return float4::toFloat(float4::hMin4(rgba)); }
	float getMaxComponent() const noexcept { return float4::toFloat(float4::hMax4(rgba)); }
	Color4& setZero() noexcept { rgba = float4::zero(); return *this; }
	Color4& set(const float r, const float g, const float b, const float a) noexcept { rgba = float4::set4(r, g, b, a); return *this; }
	Color4& setMinimum(ConstArg c1, ConstArg c2) noexcept { rgba = float4::min4(c1, c2); return *this; }
	Color4& setMaximum(ConstArg c1, ConstArg c2) noexcept { rgba = float4::max4(c1, c2); return *this; }

	static const Color4& getZero() noexcept { return ZERO; }
	static const Color4& getUnitR() noexcept { return UNIT_R; }
	static const Color4& getUnitG() noexcept { return UNIT_G; }
	static const Color4& getUnitB() noexcept { return UNIT_B; }
	static const Color4& getUnitA() noexcept { return UNIT_A; }

	static const Color4 ZERO;
	static const Color4 UNIT_R;
	static const Color4 UNIT_G;
	static const Color4 UNIT_B;
	static const Color4 UNIT_A;
	static const Color4 ONE;
	static const Color4 TOLERANCE;
	static const Color4 INF;
	static const Color4 MINUS_INF;

	union
	{
		float4::Type rgba;
		struct { float r, g, b, a; };
	};
};

inline std::istream& Color4<float>::operator>>(std::istream& s, Color4<float>& c) 
{ 
	float r, g, b, a; 
	s >> r >> std::skipws >> g >> std::skipws >> b >> std::skipws >> a; 
	c.set(r, g, b, a); 
	return s;
}

const Color4<float> Color4<float>::ZERO{};
const Color4<float> Color4<float>::UNIT_R{ 1.f, 0.f, 0.f, 0.f };
const Color4<float> Color4<float>::UNIT_G{ 0.f, 1.f, 0.f, 0.f };
const Color4<float> Color4<float>::UNIT_B{ 0.f, 0.f, 1.f, 0.f };
const Color4<float> Color4<float>::UNIT_A{ 0.f, 0.f, 0.f, 1.f };
const Color4<float> Color4<float>::ONE{ 1.f, 1.f, 1.f, 1.f };
const Color4<float> Color4<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };
const Color4<float> Color4<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
const Color4<float> Color4<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Color4 = templates::Color4<float>;
using Color4Arg = templates::Color4<float>::ConstArg;
using Color4Result = templates::Color4<float>::ConstResult;

} // namespace imaging
} // namespace core

//#include "IntColor4.hpp"

namespace core {
namespace imaging {
namespace templates {

//template<typename T>
//template<typename U> 
//inline Color4<T>::Color4(const IntColor4<U>& c) : r(T(c.r)), g(T(c.g)), b(T(c.b)), a(T(c.a)) 
//{
//}

#if SIMD_HAS_FLOAT4

//template<typename U> 
//inline Color4<float>::Color4(const IntColor4<U>& c)
//{ 
//	rgba = simd::float4::set4((float)c.r, (float)c.g, (float)c.b, (float)c.a); 
//}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace imaging
} // namespace core

#endif /* CORE_IMAGING_COLOR4_HPP */

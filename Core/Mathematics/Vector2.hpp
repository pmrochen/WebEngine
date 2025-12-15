/*
 *	Name: Vector2
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_VECTOR2_HPP
#define CORE_MATHEMATICS_VECTOR2_HPP

#include <cmath>
#include <istream>
#include <ostream>
#include <limits>
#include <algorithm>
#include <Simd.hpp>
#include "Constants.hpp"
#include "Axis.hpp"
#include "Scalar.hpp"

namespace core {
namespace mathematics {

namespace templates {

template<typename T>
struct Vector2
{
	using Real = T;
//#if SIMD_HAS_FLOAT4
//	using Arg = const Vector2;
//#else
	using Arg = const Vector2&;
//#endif

	static constexpr int NUM_COMPONENTS = 2;

	constexpr Vector2() noexcept : x(), y() {}
	constexpr explicit Vector2(const T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr Vector2(const T x, const T y) noexcept : x(x), y(y) {}
	//explicit Vector2(const IntVector2<T>& v) noexcept; // #TODO
	explicit Vector2(const Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)) {}
	explicit Vector2(const T* const v) noexcept { /*if (v) {*/ x = v[0]; y = v[1]; /*} else zero();*/ }
	/*explicit*/ operator T* () noexcept { return &x; }
	/*explicit*/ operator const T* () const noexcept { return &x; }

	Vector2 operator+() const noexcept { return *this; }
	Vector2 operator-() const noexcept { return Vector2(-x, -y); }
	Vector2& operator+=(Arg v) noexcept { x += v.x; y += v.y; return *this; }
	Vector2& operator-=(Arg v) noexcept { x -= v.x; y -= v.y; return *this; }
	Vector2& operator*=(const T f) noexcept { x *= f; y *= f; return *this; }
	Vector2& operator/=(const T f) noexcept { T s = T(1)/f; x *= s; y *= s; return *this; }
	//Vector2& operator*=(const Matrix2<T>& m) noexcept; // #TODO
	friend Vector2 operator+(Arg v1, Arg v2) noexcept { return Vector2(v1.x + v2.x, v1.y + v2.y); }
	friend Vector2 operator-(Arg v1, Arg v2) noexcept { return Vector2(v1.x - v2.x, v1.y - v2.y); }
	friend Vector2 operator*(const T f, Arg v) noexcept { return Vector2(f*v.x, f*v.y); }
	friend Vector2 operator*(Arg v, const T f) noexcept { return Vector2(v.x*f, v.y*f); }
	friend Vector2 operator/(const T f, Arg v) noexcept { return Vector2(f/v.x, f/v.y); }
	friend Vector2 operator/(Arg v, const T f) noexcept { T s = T(1)/f; return Vector2(v.x*s, v.y*s); }
	//friend Vector2 operator*(Arg v, const Matrix2<T>& m) noexcept; // #TODO
	////friend Vector2 operator*(const Matrix2<T>& m, Arg v) noexcept; // valid for column vectors only
	bool operator==(Arg v) const noexcept { return (x == v.x) && (y == v.y); }
	bool operator!=(Arg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector2& v) { return s >> v.x >> std::skipws >> v.y; }
	friend std::ostream& operator<<(std::ostream& s, const Vector2& v) { return s << v.x << ' ' << v.y; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	bool isZero() const noexcept { return (x == T()) && (y == T()); }
	bool isApproxZero() const noexcept { return (std::fabs(x) < Constants<T>::TOLERANCE) &&
		(std::fabs(y) < Constants<T>::TOLERANCE); }
	bool isApproxEqualTo(Arg v) const noexcept { return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && 
		(std::fabs(v.y - y) < Constants<T>::TOLERANCE); }
	bool isApproxEqualTo(Arg v, const T tolerance) const noexcept { return (std::fabs(v.x - x) < tolerance) &&
		(std::fabs(v.y - y) < tolerance); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y); }
	T getMagnitude() const { return std::sqrt(x*x + y*y); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y); }
	void setMagnitude(const T magnitude) { T m = getMagnitude(); if (m > T(0)) *this *= magnitude/m; }
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const T length) { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { return (std::fabs(y) > std::fabs(x)) ? Axis::Y : Axis::X; }
	T getMinComponent() const noexcept { return std::min(x, y); }
	T getMaxComponent() const noexcept { return std::max(x, y); }
	Vector2& zero() noexcept { x = T(); y = T(); return *this; } // setZero()
	Vector2& set(const T x, const T y) noexcept { this->x = x; this->y = y; return *this; }
	Vector2& minimumOf(Arg v1, Arg v2) noexcept { x = std::min(v1.x, v2.x); y = std::min(v1.y, v2.y);
		return *this; }
	Vector2& maximumOf(Arg v1, Arg v2) noexcept { x = std::max(v1.x, v2.x); y = std::max(v1.y, v2.y);
		return *this; }
	Vector2& negate() noexcept { x = -x; y = -y; return *this; }
#if MATHEMATICS_FAST_NORMALIZE
	Vector2& normalize() noexcept { T m = rcpSqrtApprox(getMagnitudeSquared()); if (m <= std::numeric_limits<T>::max()) *this *= m; return *this; }
#else
	Vector2& normalize() { T m = getMagnitude(); if (m > T(0)) *this /= m; return *this; }
#endif
	//Vector2& rotate(const T angle); // #TODO
	Vector2& scale(Arg v) noexcept { x *= v.x; y *= v.y; return *this; }
	//Vector2& transform(const Matrix2<T>& m); // #TODO
	static const Vector2& getZero() noexcept { return ZERO; }
	static const Vector2& getUnitX() noexcept { return UNIT_X; }
	static const Vector2& getUnitY() noexcept { return UNIT_Y; }

	static const Vector2 ZERO;
	static const Vector2 UNIT_X;
	static const Vector2 UNIT_Y;

	float x, y;
};

#if SIMD_HAS_FLOAT4
template<>
/*_MM_ALIGN16*/ struct Vector2<float>
{
	using Real = float;
	using Arg = const Vector2;

	static constexpr int NUM_COMPONENTS = 2;

	/*constexpr*/ Vector2() noexcept { xy = simd::setZero(); }
	/*constexpr*/ explicit Vector2(const float scalar) noexcept { xy = simd::set4(scalar); }
	/*constexpr*/ Vector2(const float x, const float y) noexcept { xy = simd::set(x, y, y, y); }
	//explicit Vector2(const IntVector2<float>& v) noexcept; // #TODO
	explicit Vector2(const Axis axis) noexcept { xy = simd::set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f,
		(axis == Axis::Y) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f); }
	explicit Vector2(const float* const v) noexcept { xy = simd::load2Broadcast(v)/*simd::set(v[0], v[1], v[1], v[1])*/; }

	explicit Vector2(const simd::Float4 v) noexcept : xy(v) {}
	operator simd::Float4() const noexcept { return xy; }
	/*explicit*/ operator float* () noexcept { return &x; }
	/*explicit*/ operator const float* () const noexcept { return &x; }

	Vector2 operator+() const noexcept { return *this; }
	Vector2 operator-() const noexcept { return Vector2(simd::neg4(xy)); }
	Vector2& operator+=(Arg v) noexcept { xy = simd::add4(xy, v); return *this; }
	Vector2& operator-=(Arg v) noexcept { xy = simd::sub4(xy, v); return *this; }
	Vector2& operator*=(const float f) noexcept { xy = simd::mul4(xy, simd::set4(f)); return *this; }
	Vector2& operator/=(const float f) noexcept { xy = simd::mul4(xy, simd::set4(1.f/f)); return *this; }
	//Vector2& operator*=(const Matrix2<T>& m) noexcept; // #TODO
	friend Vector2 operator+(Arg v1, Arg v2) noexcept { return Vector2(simd::add4(v1, v2)); }
	friend Vector2 operator-(Arg v1, Arg v2) noexcept { return Vector2(simd::sub4(v1, v2)); }
	friend Vector2 operator*(const float f, Arg v) noexcept { return Vector2(simd::mul4(simd::set4(f), v)); }
	friend Vector2 operator*(Arg v, const float f) noexcept { return Vector2(simd::mul4(v, simd::set4(f))); }
	friend Vector2 operator/(const float f, Arg v) noexcept { return Vector2(simd::div4(simd::set4(f), v)); }
	friend Vector2 operator/(Arg v, const float f) noexcept { return Vector2(simd::mul4(v, simd::set4(1.f/f))); }
	//friend Vector2 operator*(Arg v, const Matrix2<T>& m) noexcept; // #TODO
	////friend Vector2 operator*(const Matrix2<T>& m, Arg v) noexcept; // valid for column vectors only
	bool operator==(Arg v) const noexcept { return simd::all2(simd::equal4(xy, v)); }
	bool operator!=(Arg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector2& v) { float x, y; s >> x >> std::skipws >> y;
		v.set(x, y); return s; }
	friend std::ostream& operator<<(std::ostream& s, const Vector2& v) { return s << v.x << ' ' << v.y; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; } // #FIXME use simd::set(x, y, y, y)

	bool isZero() const noexcept { return simd::all2(simd::equal4(xy, ZERO)); }
	bool isApproxZero() const noexcept { simd::all2(simd::lessThan4(simd::abs4(xy), TOLERANCE)); }
	bool isApproxEqualTo(Arg v) const noexcept { simd::all2(simd::lessThan4(simd::abs4(simd::sub4(xy, v)), TOLERANCE)); }
	bool isApproxEqualTo(Arg v, const float tolerance) const noexcept { simd::all2(simd::lessThan4(simd::abs4(simd::sub4(xy, v)), 
		simd::set4(tolerance))); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y); } // #TODO SSE
	float getMagnitude() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot2(xy, xy))); }
	float getMagnitudeSquared() const noexcept { return simd::toFloat(simd::dot2(xy, xy)); }
	void setMagnitude(const float magnitude) noexcept { float m = getMagnitude(); if (m > 0.f) *this *= magnitude/m; }
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const float length) noexcept { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { (Axis)simd::toIndex(simd::equal4(xy, simd::hMax2(xy))); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin2(xy)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax2(xy)); }
	Vector2& zero() noexcept { xy = simd::setZero(); return *this; } // setZero()
	Vector2& set(const float x, const float y) noexcept { xy = simd::set(x, y, y, y); return *this; }
	Vector2& minimumOf(Arg v1, Arg v2) noexcept { xy = simd::min4(v1, v2); return *this; }
	Vector2& maximumOf(Arg v1, Arg v2) noexcept { xy = simd::max4(v1, v2); return *this; }
	Vector2& negate() noexcept { xy = simd::neg4(xy); return *this; }
#if MATHEMATICS_FAST_NORMALIZE
	Vector2& normalize() noexcept { float m = rcpSqrtApprox(getMagnitudeSquared()); if (m <= std::numeric_limits<float>::max()) *this *= m; return *this; }
#else
	Vector2& normalize() noexcept { float m = getMagnitude(); if (m > 0.f) *this /= m; return *this; }
#endif
	//Vector2& rotate(const float angle); // #TODO
	Vector2& scale(Arg v) noexcept { xy = simd::mul4(xy, v); return *this; }
	//Vector2& transform(const Matrix2<float>& m); // #TODO
	static const Vector2& getZero() noexcept { return ZERO; }
	static const Vector2& getUnitX() noexcept { return UNIT_X; }
	static const Vector2& getUnitY() noexcept { return UNIT_Y; }

	static const Vector2 ZERO;
	static const Vector2 UNIT_X;
	static const Vector2 UNIT_Y;
	static const Vector2 TOLERANCE;

	union
	{
		simd::Float4 xy;
		struct { float x, y; };
	};
};
#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Vector2f = templates::Vector2<float>;
using Vector2d = templates::Vector2<double>;

#if MATHEMATICS_DOUBLE
using Vector2 = templates::Vector2<double>;
using Vector2Arg = templates::Vector2<double>::Arg;
#else
using Vector2 = templates::Vector2<float>;
using Vector2Arg = templates::Vector2<float>::Arg;
#endif

//template<typename T>
//inline T length(templates::Vector2<T>::Arg v) { return v.getMagnitude(); }
//
//template<typename T>
//inline T lengthSquared(templates::Vector2<T>::Arg v) { return v.getMagnitudeSquared(); }

} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_VECTOR2_HPP */

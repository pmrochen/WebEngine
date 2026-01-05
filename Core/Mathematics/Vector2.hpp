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
#include <Tuples.hpp>
#include "Constants.hpp"
#include "Axis.hpp"
#include "Scalar.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
struct IntVector2;

template<typename T>
struct Matrix2;

template<typename T>
struct Vector2
{
	using Real = T;
	using ConstArg = const Vector2&;
	using ConstResult = const Vector2&;

	static constexpr int NUM_COMPONENTS = 2;

	constexpr Vector2() noexcept : x(), y() {}
	constexpr explicit Vector2(const T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr Vector2(const T x, const T y) noexcept : x(x), y(y) {}
	template<typename U> explicit Vector2(const IntVector2<U>& v) noexcept;
	explicit Vector2(const tuples::templates::Tuple2<T>& t) noexcept : x(t.x), y(t.y) {}
	template<typename U> explicit Vector2(const tuples::templates::Tuple2<U>& t) noexcept : x(T(t.x)), y(T(t.y)) {}
	explicit Vector2(const T* const v) noexcept { x = v[0]; y = v[1]; }
	explicit Vector2(const Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)) {}

	explicit operator tuples::templates::Tuple2<T>() noexcept { return tuples::templates::Tuple2<T>(x, y); }
	template<typename U> explicit operator tuples::templates::Tuple2<U>() noexcept { return tuples::templates::Tuple2<U>(U(x), U(y)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	Vector2 operator+() const noexcept { return *this; }
	Vector2 operator-() const noexcept { return Vector2(-x, -y); }
	Vector2& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; return *this; }
	Vector2& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; return *this; }
	Vector2& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; return *this; }
	Vector2& operator*=(const T f) noexcept { x *= f; y *= f; return *this; }
	Vector2& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; return *this; }
	Vector2& operator/=(const T f) noexcept { const T s = T(1)/f; x *= s; y *= s; return *this; }
	Vector2& operator*=(const Matrix2<T>& m) noexcept; // #TODO
	friend Vector2 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x + v2.x, v1.y + v2.y); }
	friend Vector2 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x - v2.x, v1.y - v2.y); }
	friend Vector2 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x*v2.x, v1.y*v2.y); }
	friend Vector2 operator*(const T f, ConstArg v) noexcept { return Vector2(f*v.x, f*v.y); }
	friend Vector2 operator*(ConstArg v, const T f) noexcept { return Vector2(v.x*f, v.y*f); }
	friend Vector2 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x/v2.x, v1.y/v2.y); }
	friend Vector2 operator/(const T f, ConstArg v) noexcept { return Vector2(f/v.x, f/v.y); }
	friend Vector2 operator/(ConstArg v, const T f) noexcept { const T s = T(1)/f; return Vector2(v.x*s, v.y*s); }
	friend Vector2 operator*(ConstArg v, const Matrix2<T>& m) noexcept; // #TODO
	//friend Vector2 operator*(const Matrix2<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector2& v) const noexcept { return (x == v.x) && (y == v.y); }
	bool operator!=(const Vector2& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector2& v) { return s >> v.x >> std::skipws >> v.y; }
	friend std::ostream& operator<<(std::ostream& s, const Vector2& v) { return s << v.x << ' ' << v.y; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	bool isZero() const noexcept { return (x == T()) && (y == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(ConstArg v) const noexcept;
	bool isApproxEqual(ConstArg v, const T tolerance) const noexcept;
	bool allLessThan(ConstArg v) const noexcept { return (x < v.x) && (y < v.y); }
	bool allLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) && (y <= v.y); }
	bool allGreaterThan(ConstArg v) const noexcept { return (x > v.x) && (y > v.y); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) && (y >= v.y); }
	bool anyLessThan(ConstArg v) const noexcept { return (x < v.x) || (y < v.y); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) || (y <= v.y); }
	bool anyGreaterThan(ConstArg v) const noexcept { return (x > v.x) || (y > v.y); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) || (y >= v.y); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y); }
	T getMagnitude() const { return std::sqrt(x*x + y*y); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y); }
	void setMagnitude(const T magnitude);
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const T length) { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { return (std::fabs(y) > std::fabs(x)) ? Axis::Y : Axis::X; }
	T getMinComponent() const noexcept { return std::min(x, y); }
	T getMaxComponent() const noexcept { return std::max(x, y); }
	Vector2& setZero() noexcept { x = T(); y = T(); return *this; }
	Vector2& set(const T x, const T y) noexcept { this->x = x; this->y = y; return *this; }
	Vector2& setMinimum(ConstArg v1, ConstArg v2) noexcept;
	Vector2& setMaximum(ConstArg v1, ConstArg v2) noexcept;
	Vector2& negate() noexcept { x = -x; y = -y; return *this; }
	//template<std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
	Vector2& normalize();
	Vector2& rotate(const T angle);
	Vector2& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; return *this; }
	Vector2& transform(const Matrix2<T>& m); // #TODO

	static const Vector2& getZero() noexcept { return ZERO; }
	static const Vector2& getUnitX() noexcept { return UNIT_X; }
	static const Vector2& getUnitY() noexcept { return UNIT_Y; }

	static const Vector2 ZERO;
	static const Vector2 UNIT_X;
	static const Vector2 UNIT_Y;
	static const Vector2 ONE;
	static const Vector2 TOLERANCE;
	static const Vector2 INF;
	static const Vector2 MINUS_INF;

	T x, y;
};

template<typename T>
inline bool Vector2<T>::isApproxZero() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Vector2<T>::isApproxEqual(ConstArg v) const
{ 
	return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && (std::fabs(v.y - y) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Vector2<T>::isApproxEqual(ConstArg v, const T tolerance) const
{ 
	return (std::fabs(v.x - x) < tolerance) && (std::fabs(v.y - y) < tolerance); 
}

template<typename T>
inline void Vector2<T>::setMagnitude(const T magnitude) 
{ 
	const T m = getMagnitude(); 
	if (m > T(0)) 
		*this *= magnitude/m;
}

template<typename T>
inline Vector2<T>& Vector2<T>::setMinimum(ConstArg v1, ConstArg v2)
{ 
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y); 
	return *this; 
}

template<typename T>
inline Vector2<T>& Vector2<T>::setMaximum(ConstArg v1, ConstArg v2)
{ 
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y); 
	return *this; 
}

template<typename T>
inline Vector3<T>& Vector3<T>::normalize()
{
#if MATHEMATICS_FAST_NORMALIZE
	const T m = rcpSqrtApprox(getMagnitudeSquared()); 
	if (m <= std::numeric_limits<T>::max()) 
		*this *= m;
#else
	const T m = getMagnitude(); 
	if (m > T(0)) 
		*this /= m;
#endif
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::rotate(const T angle)
{
	if (angle != T(0))
	{
		const T sinAngle = std::sin(angle);
		const T cosAngle = std::cos(angle);
		set(x*cosAngle - y*sinAngle, y*cosAngle + x*sinAngle);
	}

	return *this;
}

template<typename T> const Vector2<T> Vector2<T>::ZERO{};
template<typename T> const Vector2<T> Vector2<T>::UNIT_X{ T(1), T(0) };
template<typename T> const Vector2<T> Vector2<T>::UNIT_Y{ T(0), T(1) };
template<typename T> const Vector2<T> Vector2<T>::ONE{ T(1), T(1) };
template<typename T> const Vector2<T> Vector2<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };
template<typename T> const Vector2<T> Vector2<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() };
template<typename T> const Vector2<T> Vector2<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity() };

#if SIMD_HAS_FLOAT4

namespace float4 = simd::float4;

template<>
struct Vector2<float>
{
	using Real = float;
	using ConstArg = const Vector2;
	using ConstResult = const Vector2;

	static constexpr int NUM_COMPONENTS = 2;

	/*constexpr*/ Vector2() noexcept { xy = float4::zero(); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	/*constexpr*/ explicit Vector2(const float scalar) noexcept { xy = float4::set4(scalar); }
	/*constexpr*/ Vector2(const float x, const float y) noexcept { xy = float4::set4(x, y, y, y); }
	template<typename U> explicit Vector2(const IntVector2<U>& v) noexcept;
	explicit Vector2(const tuples::templates::Tuple2<float>& t) noexcept { xy = float4::set4(t.x, t.y, t.y, t.y); }
	template<typename U> explicit Vector2(const tuples::templates::Tuple2<U>& t) noexcept { float y = (float)t.y; xy = float4::set4((float)t.x, y, y, y); }
	explicit Vector2(const float* const v) noexcept { set(v[0], v[1]); }
#else
	/*constexpr*/ explicit Vector2(const float scalar) noexcept { xy = float4::set2(scalar); }
	/*constexpr*/ Vector2(const float x, const float y) noexcept { xy = float4::set2(x, y); }
	template<typename U> explicit Vector2(const IntVector2<U>& v) noexcept;
	explicit Vector2(const tuples::templates::Tuple2<float>& t) noexcept { xy = float4::set2(t.x, t.y); }
	template<typename U> explicit Vector2(const tuples::templates::Tuple2<U>& t) noexcept { xy = float4::set2((float)t.x, (float)t.y); }
	explicit Vector2(const float* const v) noexcept { xy = float4::load2(v); }
#endif
	explicit Vector2(const Axis axis) noexcept { set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f); }
	explicit Vector2(const float4::Type v) noexcept : xy(v) {}

	operator float4::Type() const noexcept { return xy; }
	explicit operator tuples::templates::Tuple2<float>() noexcept { return tuples::templates::Tuple2<float>(x, y); }
	template<typename U> explicit operator tuples::templates::Tuple2<U>() noexcept { return tuples::templates::Tuple2<U>(U(x), U(y)); }
	explicit operator float*() noexcept { return &x; }
	explicit operator const float*() const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Vector2 operator+() const noexcept { return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2 operator-() const noexcept { return Vector2(float4::neg4(xy)); }
#else
	Vector2 operator-() const noexcept { return Vector2(float4::neg2(xy)); }
#endif
	Vector2& operator+=(ConstArg v) noexcept { xy = float4::add4(xy, v); return *this; }
	Vector2& operator-=(ConstArg v) noexcept { xy = float4::sub4(xy, v); return *this; }
	Vector2& operator*=(ConstArg v) noexcept { xy = float4::mul4(xy, v); return *this; }
	Vector2& operator*=(const float f) noexcept { xy = float4::mul4(xy, float4::set4(f)); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2& operator/=(ConstArg v) noexcept { xy = float4::div4(xy, v); return *this; }
#else
	Vector2& operator/=(ConstArg v) noexcept { xy = float4::div2(xy, v); return *this; }
#endif
	Vector2& operator/=(const float f) noexcept { xy = float4::div4(xy, float4::set4(f)); return *this; }
	Vector2& operator*=(const Matrix2<float>& m) noexcept; // #TODO
	friend Vector2 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector2(float4::add4(v1, v2)); }
	friend Vector2 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector2(float4::sub4(v1, v2)); }
	friend Vector2 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector2(float4::mul4(v1, v2)); }
	friend Vector2 operator*(const float f, ConstArg v) noexcept { return Vector2(float4::mul4(float4::set4(f), v)); }
	friend Vector2 operator*(ConstArg v, const float f) noexcept { return Vector2(float4::mul4(v, float4::set4(f))); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	friend Vector2 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector2(float4::div4(v1, v2)); }
	friend Vector2 operator/(const float f, ConstArg v) noexcept { return Vector2(float4::div4(float4::set4(f), v)); }
#else
	friend Vector2 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector2(float4::div2(v1, v2)); }
	friend Vector2 operator/(const float f, ConstArg v) noexcept { return Vector2(float4::div2(float4::set2(f), v)); }
#endif
	friend Vector2 operator/(ConstArg v, const float f) noexcept { return Vector2(float4::div4(v, float4::set4(f))); }
	friend Vector2 operator*(ConstArg v, const Matrix2<float>& m) noexcept; // #TODO
	//friend Vector2 operator*(const Matrix2<float>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector2& v) const noexcept { return float4::all2(float4::equal(xy, v)); }
	bool operator!=(const Vector2& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector2& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector2& v) { return s << v.x << ' ' << v.y; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; } // #FIXME use float4::set(x, y, y, y)

	bool isZero() const noexcept { return float4::all2(float4::equal(xy, float4::zero())); }
	bool isApproxZero() const noexcept { float4::all2(float4::lessThan(float4::abs4(xy), TOLERANCE)); }
	bool isApproxEqual(ConstArg v) const noexcept { float4::all2(float4::lessThan(float4::abs4(float4::sub4(xy, v)), TOLERANCE)); }
	bool isApproxEqual(ConstArg v, const float tolerance) const noexcept { float4::all2(float4::lessThan(float4::abs4(float4::sub4(xy, v)), float4::set4(tolerance))); }
	bool allLessThan(ConstArg v) const noexcept { return float4::all2(float4::lessThan(xy, v)); }
	bool allLessThanEqual(ConstArg v) const noexcept { return float4::all2(float4::lessThanEqual(xy, v)); }
	bool allGreaterThan(ConstArg v) const noexcept { return float4::all2(float4::greaterThan(xy, v)); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return float4::all2(float4::greaterThanEqual(xy, v)); }
	bool anyLessThan(ConstArg v) const noexcept { return float4::any2(float4::lessThan(xy, v)); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return float4::any2(float4::lessThanEqual(xy, v)); }
	bool anyGreaterThan(ConstArg v) const noexcept { return float4::any2(float4::greaterThan(xy, v)); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return float4::any2(float4::greaterThanEqual(xy, v)); }
	bool isFinite() const { return float4::all2(float4::isFinite(xy)); }
	float getMagnitude() const noexcept { return float4::toFloat(float4::sqrt1(float4::dot2(xy, xy))); }
	float getMagnitudeSquared() const noexcept { return float4::toFloat(float4::dot2(xy, xy)); }
	void setMagnitude(const float magnitude) noexcept;
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const float length) noexcept { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { (Axis)float4::asIndex(float4::equal(xy, float4::hMax2(xy))); }
	float getMinComponent() const noexcept { return float4::toFloat(float4::hMin2(xy)); }
	float getMaxComponent() const noexcept { return float4::toFloat(float4::hMax2(xy)); }
	Vector2& setZero() noexcept { xy = float4::zero(); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2& set(const float x, const float y) noexcept { xy = float4::set4(x, y, y, y); return *this; }
#else
	Vector2& set(const float x, const float y) noexcept { xy = float4::set2(x, y); return *this; }
#endif
	Vector2& setMinimum(ConstArg v1, ConstArg v2) noexcept { xy = float4::min4(v1, v2); return *this; }
	Vector2& setMaximum(ConstArg v1, ConstArg v2) noexcept { xy = float4::max4(v1, v2); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2& negate() noexcept { xy = float4::neg4(xy); return *this; }
#else
	Vector2& negate() noexcept { xy = float4::neg2(xy); return *this; }
#endif
	Vector2& normalize() noexcept;
	Vector2& rotate(const float angle);
	Vector2& scale(ConstArg v) noexcept { xy = float4::mul4(xy, v); return *this; }
	Vector2& transform(const Matrix2<float>& m); // #TODO

	static const Vector2& getZero() noexcept { return ZERO; }
	static const Vector2& getUnitX() noexcept { return UNIT_X; }
	static const Vector2& getUnitY() noexcept { return UNIT_Y; }

	static const Vector2 ZERO;
	static const Vector2 UNIT_X;
	static const Vector2 UNIT_Y;
	static const Vector2 ONE;
	static const Vector2 TOLERANCE;
	static const Vector2 INF;
	static const Vector2 MINUS_INF;

	union
	{
		float4::Type xy;
		struct { float x, y; };
	};
};

inline std::istream& Vector2<float>::operator>>(std::istream& s, Vector2<float>& v) 
{ 
	float x, y; 
	s >> x >> std::skipws >> y; 
	v.set(x, y); 
	return s; 
}

inline void Vector2<float>::setMagnitude(const float magnitude)
{ 
	const float m = getMagnitude();
	if (m > 0.f) 
		*this *= magnitude/m;
}

inline Vector2<float>& Vector2<float>::normalize()
{
#if MATHEMATICS_FAST_NORMALIZE
	const float m = rcpSqrtApprox(getMagnitudeSquared()); 
	if (m <= std::numeric_limits<float>::max()) 
		*this *= m;
#else
	const float m = getMagnitude(); 
	if (m > 0.f) 
		*this /= m;
#endif
	return *this;
}

inline Vector2<float>& Vector2<float>::rotate(const float angle)
{
	if (angle != 0.f)
	{
		const float4::Type sinAngle = float4::set2/*4*/(std::sin(angle));
		const float4::Type cosAngle = float4::set2/*4*/(std::cos(angle));
		xy = float4::subAdd4(float4::mul4(xy, cosAngle), 
			float4::mul4(float4::swizzle4<float4::YXXX>/*swizzle2<float4::YX>*/(xy), sinAngle));
	}

	return *this;
}

const Vector2<float> Vector2<float>::ZERO{};
const Vector2<float> Vector2<float>::UNIT_X{ 1.f, 0.f };
const Vector2<float> Vector2<float>::UNIT_Y{ 0.f, 1.f };
const Vector2<float> Vector2<float>::ONE{ 1.f, 1.f };
const Vector2<float> Vector2<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };
const Vector2<float> Vector2<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
const Vector2<float> Vector2<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Vector2f = templates::Vector2<float>;
using Vector2d = templates::Vector2<double>;

#if MATHEMATICS_DOUBLE
using Vector2 = templates::Vector2<double>;
using Vector2Arg = templates::Vector2<double>::ConstArg;
using Vector2Result = templates::Vector2<double>::ConstResult;
#else
using Vector2 = templates::Vector2<float>;
using Vector2Arg = templates::Vector2<float>::ConstArg;
using Vector2Result = templates::Vector2<float>::ConstResult;
#endif

//template<typename T>
//inline T length(templates::Vector2<T>::ConstArg v) { return v.getMagnitude(); }
//
//template<typename T>
//inline T lengthSquared(templates::Vector2<T>::ConstArg v) { return v.getMagnitudeSquared(); }

} // namespace mathematics
} // namespace core

#include "IntVector2.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
template<typename U> 
inline Vector2<T>::Vector2(const IntVector2<U>& v) : x(T(v.x)), y(T(v.y))
{
}

#if SIMD_HAS_FLOAT4

template<typename U>
inline Vector2<float>::Vector2(const IntVector2<U>& v)
{
#if MATHEMATICS_SIMD_EXPAND_LAST
	const float t = (float)v.y;
	xy = float4::set4((float)v.x, t, t, t);
#else
	xy = float4::set2((float)v.x, (float)v.y);
#endif
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_VECTOR2_HPP */

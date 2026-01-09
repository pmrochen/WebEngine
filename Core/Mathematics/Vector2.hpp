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
#include <Simd/Intrinsics.hpp>
#include <Tuples/Tuple2.hpp>
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
	constexpr explicit Vector2(T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr Vector2(T x, T y) noexcept : x(x), y(y) {}
	template<typename U> explicit Vector2(const IntVector2<U>& v) noexcept;
	explicit Vector2(const tuples::templates::Tuple2<T>& t) noexcept : x(t.x), y(t.y) {}
	template<typename U> explicit Vector2(const tuples::templates::Tuple2<U>& t) noexcept : x(T(t.x)), y(T(t.y)) {}
	explicit Vector2(const T* v) noexcept { x = v[0]; y = v[1]; }
	explicit Vector2(Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)) {}

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
	Vector2& operator*=(T f) noexcept { x *= f; y *= f; return *this; }
	Vector2& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; return *this; }
	Vector2& operator/=(T f) noexcept { T s = T(1)/f; x *= s; y *= s; return *this; }
	Vector2& operator*=(const Matrix2<T>& m) noexcept; // #TODO
	friend Vector2 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x + v2.x, v1.y + v2.y); }
	friend Vector2 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x - v2.x, v1.y - v2.y); }
	friend Vector2 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x*v2.x, v1.y*v2.y); }
	friend Vector2 operator*(T f, ConstArg v) noexcept { return Vector2(f*v.x, f*v.y); }
	friend Vector2 operator*(ConstArg v, T f) noexcept { return Vector2(v.x*f, v.y*f); }
	friend Vector2 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector2(v1.x/v2.x, v1.y/v2.y); }
	friend Vector2 operator/(T f, ConstArg v) noexcept { return Vector2(f/v.x, f/v.y); }
	friend Vector2 operator/(ConstArg v, T f) noexcept { T s = T(1)/f; return Vector2(v.x*s, v.y*s); }
	friend Vector2 operator*(ConstArg v, const Matrix2<T>& m) noexcept; // #TODO
	//friend Vector2 operator*(const Matrix2<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector2& v) const noexcept { return (x == v.x) && (y == v.y); }
	bool operator!=(const Vector2& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector2& v) { return s >> v.x >> std::skipws >> v.y; }
	friend std::ostream& operator<<(std::ostream& s, const Vector2& v) { return s << v.x << ' ' << v.y; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	bool isZero() const noexcept { return (x == T()) && (y == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(const Vector2& v) const noexcept;
	bool isApproxEqual(const Vector2& v, T tolerance) const noexcept;
	bool allLessThan(const Vector2& v) const noexcept { return (x < v.x) && (y < v.y); }
	bool allLessThanEqual(const Vector2& v) const noexcept { return (x <= v.x) && (y <= v.y); }
	bool allGreaterThan(const Vector2& v) const noexcept { return (x > v.x) && (y > v.y); }
	bool allGreaterThanEqual(const Vector2& v) const noexcept { return (x >= v.x) && (y >= v.y); }
	bool anyLessThan(const Vector2& v) const noexcept { return (x < v.x) || (y < v.y); }
	bool anyLessThanEqual(const Vector2& v) const noexcept { return (x <= v.x) || (y <= v.y); }
	bool anyGreaterThan(const Vector2& v) const noexcept { return (x > v.x) || (y > v.y); }
	bool anyGreaterThanEqual(const Vector2& v) const noexcept { return (x >= v.x) || (y >= v.y); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y); }
	T getMagnitude() const { return std::sqrt(x*x + y*y); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y); }
	void setMagnitude(T magnitude);
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(T length) { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { return (std::fabs(y) > std::fabs(x)) ? Axis::Y : Axis::X; }
	T getMinComponent() const noexcept { return std::min(x, y); }
	T getMaxComponent() const noexcept { return std::max(x, y); }
	Vector2& setZero() noexcept { x = T(); y = T(); return *this; }
	Vector2& set(T x, T y) noexcept { this->x = x; this->y = y; return *this; }
	Vector2& setMinimum(const Vector2& v1, const Vector2& v2) noexcept;
	Vector2& setMaximum(const Vector2& v1, const Vector2& v2) noexcept;
	Vector2& negate() noexcept { x = -x; y = -y; return *this; }
	//template<std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
	Vector2& normalize();
	Vector2& rotate(T angle);
	//Vector2& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; return *this; }
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
inline T dot(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	return v1.x*v2.x + v1.y*v2.y;
}

template<typename T>
inline T cross(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	return v1.x*v2.y - v1.y*v2.x;
}

template<typename T>
inline T distance(const Vector2<T>& v1, const Vector2<T>& v2)
{
	T x = v2.x - v1.x;
	T y = v2.y - v1.y;
	return std::sqrt(x*x + y*y);
}

template<typename T>
inline T distanceSquared(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	T x = v2.x - v1.x;
	T y = v2.y - v1.y;
	return x*x + y*y;
}

template<typename T>
inline T length(const Vector2<T>& v)
{
	return std::sqrt(v.x*v.x + v.y*v.y);
}

template<typename T>
inline T lengthSquared(const Vector2<T>& v) noexcept
{
	return v.x*v.x + v.y*v.y;
}

template<typename T>
inline Vector2<T> normalize(const Vector2<T>& v)
{
	Vector2<T> u(v);
	u.normalize();
	return u;
}

template<typename T>
inline Vector2<T> project(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	T m = v2.getMagnitudeSquared();
	return (m > T(0)) ? (dot(v1, v2)/m)*v2 : Vector4<T>::ZERO;
}

template<typename T>
inline T angle(const Vector2<T>& v1, const Vector2<T>& v2)
{
#if MATHEMATICS_FAST_NORMALIZE
	Vector2<T> u(v1);
	Vector2<T> v(v2);
	u.normalize();
	v.normalize();
	return std::acos(std::clamp(dot(u, v), T(-1), T(1)));
#else
	T q = v1.getMagnitude()*v2.getMagnitude();
	if (q > T(0))
		return std::acos(std::clamp(dot(v1, v2)/q, T(-1), T(1)));
	else
		return (dot(v1, v2) >= T(0)) ? T(0) : Constants<T>::PI;
#endif
}

template<typename T>
inline Vector2<T> minimum(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	return Vector2<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
}

template<typename T>
inline Vector2<T> maximum(const Vector2<T>& v1, const Vector2<T>& v2) noexcept
{
	return Vector2<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
}

template<typename T>
inline Vector2<T> lerp(const Vector2<T>& v1, const Vector2<T>& v2, T t) noexcept
{
	// #TODO
}

template<typename T>
inline Vector2<T> slerp(const Vector2<T>& v1, const Vector2<T>& v2, T t) noexcept
{
	// #TODO
}

template<typename T>
inline Vector2<T> perpendicular(const Vector2<T>& v) noexcept
{
	// #TODO
}

//template<typename T>
//inline Vector2<T> transform(const Vector2<T>& v, const Matrix2<T>& m) noexcept; // -> Matrix2

template<typename T>
inline bool Vector2<T>::isApproxZero() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Vector2<T>::isApproxEqual(const Vector2<T>& v) const
{ 
	return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && (std::fabs(v.y - y) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Vector2<T>::isApproxEqual(const Vector2<T>& v, T tolerance) const
{ 
	return (std::fabs(v.x - x) < tolerance) && (std::fabs(v.y - y) < tolerance); 
}

template<typename T>
inline void Vector2<T>::setMagnitude(T magnitude) 
{ 
	T m = getMagnitude(); 
	if (m > T(0)) 
		*this *= magnitude/m;
}

template<typename T>
inline Vector2<T>& Vector2<T>::setMinimum(const Vector2<T>& v1, const Vector2<T>& v2)
{ 
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y); 
	return *this; 
}

template<typename T>
inline Vector2<T>& Vector2<T>::setMaximum(const Vector2<T>& v1, const Vector2<T>& v2)
{ 
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y); 
	return *this; 
}

template<typename T>
inline Vector2<T>& Vector2<T>::normalize()
{
#if MATHEMATICS_FAST_NORMALIZE
	T m = rcpSqrtApprox(getMagnitudeSquared()); 
	if (m <= std::numeric_limits<T>::max()) 
		*this *= m;
#else
	T m = getMagnitude(); 
	if (m > T(0)) 
		*this /= m;
#endif
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::rotate(T angle)
{
	if (angle != T(0))
	{
		T sinAngle = std::sin(angle);
		T cosAngle = std::cos(angle);
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

template<>
struct Vector2<float>
{
	using Real = float;
	using ConstArg = const Vector2;
	using ConstResult = const Vector2;

	static constexpr int NUM_COMPONENTS = 2;

	/*constexpr*/ Vector2() noexcept { xy = simd::zero<simd::float4>(); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	/*constexpr*/ explicit Vector2(float scalar) noexcept { xy = simd::set4(scalar); }
	/*constexpr*/ Vector2(float x, float y) noexcept { xy = simd::set4(x, y, y, y); }
	template<typename U> explicit Vector2(const IntVector2<U>& v) noexcept;
	explicit Vector2(const tuples::templates::Tuple2<float>& t) noexcept { xy = simd::set4(t.x, t.y, t.y, t.y); }
	template<typename U> explicit Vector2(const tuples::templates::Tuple2<U>& t) noexcept { float y = (float)t.y; xy = simd::set4((float)t.x, y, y, y); }
	explicit Vector2(const float* v) noexcept { set(v[0], v[1]); }
#else
	/*constexpr*/ explicit Vector2(float scalar) noexcept { xy = simd::set2(scalar); }
	/*constexpr*/ Vector2(float x, float y) noexcept { xy = simd::set2(x, y); }
	template<typename U> explicit Vector2(const IntVector2<U>& v) noexcept;
	explicit Vector2(const tuples::templates::Tuple2<float>& t) noexcept { xy = simd::set2(t.x, t.y); }
	template<typename U> explicit Vector2(const tuples::templates::Tuple2<U>& t) noexcept { xy = simd::set2((float)t.x, (float)t.y); }
	explicit Vector2(const float* v) noexcept { xy = simd::load2(v); }
#endif
	explicit Vector2(Axis axis) noexcept { set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f); }
	explicit Vector2(simd::float4 v) noexcept : xy(v) {}

	operator simd::float4() const noexcept { return xy; }
	explicit operator tuples::templates::Tuple2<float>() noexcept { return tuples::templates::Tuple2<float>(x, y); }
	template<typename U> explicit operator tuples::templates::Tuple2<U>() noexcept { return tuples::templates::Tuple2<U>(U(x), U(y)); }
	explicit operator float*() noexcept { return &x; }
	explicit operator const float*() const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Vector2 operator+() const noexcept { return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2 operator-() const noexcept { return Vector2(simd::neg4(xy)); }
#else
	Vector2 operator-() const noexcept { return Vector2(simd::neg2(xy)); }
#endif
	Vector2& operator+=(ConstArg v) noexcept { xy = simd::add4(xy, v); return *this; }
	Vector2& operator-=(ConstArg v) noexcept { xy = simd::sub4(xy, v); return *this; }
	Vector2& operator*=(ConstArg v) noexcept { xy = simd::mul4(xy, v); return *this; }
	Vector2& operator*=(float f) noexcept { xy = simd::mul4(xy, simd::set4(f)); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2& operator/=(ConstArg v) noexcept { xy = simd::div4(xy, v); return *this; }
#else
	Vector2& operator/=(ConstArg v) noexcept { xy = simd::div2(xy, v); return *this; }
#endif
	Vector2& operator/=(float f) noexcept { xy = simd::div4(xy, simd::set4(f)); return *this; }
	Vector2& operator*=(const Matrix2<float>& m) noexcept; // #TODO
	friend Vector2 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector2(simd::add4(v1, v2)); }
	friend Vector2 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector2(simd::sub4(v1, v2)); }
	friend Vector2 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector2(simd::mul4(v1, v2)); }
	friend Vector2 operator*(float f, ConstArg v) noexcept { return Vector2(simd::mul4(simd::set4(f), v)); }
	friend Vector2 operator*(ConstArg v, float f) noexcept { return Vector2(simd::mul4(v, simd::set4(f))); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	friend Vector2 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector2(simd::div4(v1, v2)); }
	friend Vector2 operator/(float f, ConstArg v) noexcept { return Vector2(simd::div4(simd::set4(f), v)); }
#else
	friend Vector2 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector2(simd::div2(v1, v2)); }
	friend Vector2 operator/(float f, ConstArg v) noexcept { return Vector2(simd::div2(simd::set2(f), v)); }
#endif
	friend Vector2 operator/(ConstArg v, float f) noexcept { return Vector2(simd::div4(v, simd::set4(f))); }
	friend Vector2 operator*(ConstArg v, const Matrix2<float>& m) noexcept; // #TODO
	//friend Vector2 operator*(const Matrix2<float>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector2& v) const noexcept { return simd::all2(simd::equal(xy, v)); }
	bool operator!=(const Vector2& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector2& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector2& v) { return s << v.x << ' ' << v.y; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; } // #FIXME use simd::set(x, y, y, y)

	bool isZero() const noexcept { return simd::all2(simd::equal(xy, simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept { simd::all2(simd::lessThan(simd::abs4(xy), TOLERANCE)); }
	bool isApproxEqual(const Vector2& v) const noexcept { simd::all2(simd::lessThan(simd::abs4(simd::sub4(xy, v)), TOLERANCE)); }
	bool isApproxEqual(const Vector2& v, float tolerance) const noexcept { simd::all2(simd::lessThan(simd::abs4(simd::sub4(xy, v)), simd::set4(tolerance))); }
	bool allLessThan(const Vector2& v) const noexcept { return simd::all2(simd::lessThan(xy, v)); }
	bool allLessThanEqual(const Vector2& v) const noexcept { return simd::all2(simd::lessThanEqual(xy, v)); }
	bool allGreaterThan(const Vector2& v) const noexcept { return simd::all2(simd::greaterThan(xy, v)); }
	bool allGreaterThanEqual(const Vector2& v) const noexcept { return simd::all2(simd::greaterThanEqual(xy, v)); }
	bool anyLessThan(const Vector2& v) const noexcept { return simd::any2(simd::lessThan(xy, v)); }
	bool anyLessThanEqual(const Vector2& v) const noexcept { return simd::any2(simd::lessThanEqual(xy, v)); }
	bool anyGreaterThan(const Vector2& v) const noexcept { return simd::any2(simd::greaterThan(xy, v)); }
	bool anyGreaterThanEqual(const Vector2& v) const noexcept { return simd::any2(simd::greaterThanEqual(xy, v)); }
	bool isFinite() const { return simd::all2(simd::isFinite(xy)); }
	float getMagnitude() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot2(xy, xy))); }
	float getMagnitudeSquared() const noexcept { return simd::toFloat(simd::dot2(xy, xy)); }
	void setMagnitude(float magnitude) noexcept;
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(float length) noexcept { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { (Axis)simd::asIndex(simd::equal(xy, simd::hMax2(xy))); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin2(xy)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax2(xy)); }
	Vector2& setZero() noexcept { xy = simd::zero<simd::float4>(); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2& set(float x, float y) noexcept { xy = simd::set4(x, y, y, y); return *this; }
#else
	Vector2& set(float x, float y) noexcept { xy = simd::set2(x, y); return *this; }
#endif
	Vector2& setMinimum(const Vector2& v1, const Vector2& v2) noexcept { xy = simd::min4(v1, v2); return *this; }
	Vector2& setMaximum(const Vector2& v1, const Vector2& v2) noexcept { xy = simd::max4(v1, v2); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2& negate() noexcept { xy = simd::neg4(xy); return *this; }
#else
	Vector2& negate() noexcept { xy = simd::neg2(xy); return *this; }
#endif
	Vector2& normalize() noexcept;
	Vector2& rotate(float angle);
	//Vector2& scale(ConstArg v) noexcept { xy = simd::mul4(xy, v); return *this; }
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
		simd::float4 xy;
		struct { float x, y; };
	};
};

template<>
inline float dot(const Vector2<float>& v1, const Vector2<float>& v2) noexcept
{
	return simd::toFloat(simd::dot2(v1, v2));
}

template<>
inline float cross(const Vector2<float>& v1, const Vector2<float>& v2) noexcept
{
	return simd::toFloat(simd::dot2(v1, simd::swizzle4<simd::YXZW>(simd::neg1(v2))));
}

template<>
inline float distance(const Vector2<float>& v1, const Vector2<float>& v2)
{
	auto v = simd::sub4(v2, v1);
	return simd::toFloat(simd::sqrt1(simd::dot2(v, v)));
}

template<>
inline float distanceSquared(const Vector2<float>& v1, const Vector2<float>& v2) noexcept
{
	auto v = simd::sub4(v2, v1);
	return simd::toFloat(simd::dot2(v, v));
}

template<>
inline float length(const Vector2<float>& v)
{
	return simd::toFloat(simd::sqrt1(simd::dot2(v, v)));
}

template<>
inline float lengthSquared(const Vector2<float>& v) noexcept
{
	return simd::toFloat(simd::dot2(v, v));
}

template<>
inline Vector2<float> minimum(const Vector2<float>& v1, const Vector2<float>& v2) noexcept
{
	return Vector2<float>(simd::min4(v1, v2));
}

template<>
inline Vector2<float> maximum(const Vector2<float>& v1, const Vector2<float>& v2) noexcept
{
	return Vector2<float>(simd::max4(v1, v2));
}

inline std::istream& Vector2<float>::operator>>(std::istream& s, Vector2<float>& v) 
{ 
	float x, y; 
	s >> x >> std::skipws >> y; 
	v.set(x, y); 
	return s; 
}

inline void Vector2<float>::setMagnitude(float magnitude)
{ 
	float m = getMagnitude();
	if (m > 0.f) 
		*this *= magnitude/m;
}

inline Vector2<float>& Vector2<float>::normalize()
{
#if MATHEMATICS_FAST_NORMALIZE
	float m = rcpSqrtApprox(getMagnitudeSquared()); 
	if (m <= std::numeric_limits<float>::max()) 
		*this *= m;
#else
	float m = getMagnitude(); 
	if (m > 0.f) 
		*this /= m;
#endif
	return *this;
}

inline Vector2<float>& Vector2<float>::rotate(float angle)
{
	if (angle != 0.f)
	{
		auto sinAngle = simd::set2/*4*/(std::sin(angle));
		auto cosAngle = simd::set2/*4*/(std::cos(angle));
		xy = simd::subAdd4(simd::mul4(xy, cosAngle), 
			simd::mul4(simd::swizzle4<simd::YXXX>/*swizzle2<simd::YX>*/(xy), sinAngle));
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
	float t = (float)v.y;
	xy = simd::set4((float)v.x, t, t, t);
#else
	xy = simd::set2((float)v.x, (float)v.y);
#endif
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_VECTOR2_HPP */

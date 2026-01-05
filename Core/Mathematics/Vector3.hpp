/*
 *	Name: Vector3
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_VECTOR3_HPP
#define CORE_MATHEMATICS_VECTOR3_HPP

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
#include "Vector2.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
struct IntVector3;

template<typename T>
struct Quaternion;

template<typename T>
struct Matrix3;

template<typename T>
struct AffineTransform;

template<typename T>
struct Vector3
{
	using Real = T;
	using ConstArg = const Vector3&;
	using ConstResult = const Vector3&;

	static constexpr int NUM_COMPONENTS = 3;

	constexpr Vector3() noexcept : x(), y(), z() {}
	constexpr explicit Vector3(const T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
	constexpr Vector3(const T x, const T y, const T z) noexcept : x(x), y(y), z(z) {}
	constexpr Vector3(Vector2<T>::ConstArg v) noexcept : x(v.x), y(v.y), z() {}
	constexpr Vector3(Vector2<T>::ConstArg v, const T z) noexcept : x(v.x), y(v.y), z(z) {}
	template<typename U> explicit Vector3(const IntVector3<U>& v) noexcept;
	explicit Vector3(const tuples::templates::Tuple3<T>& t) noexcept : x(t.x), y(t.y), z(t.z) {}
	template<typename U> explicit Vector3(const tuples::templates::Tuple3<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)) {}
	explicit Vector3(const T* const v) noexcept { x = v[0]; y = v[1]; z = v[2]; }
	explicit Vector3(const Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)), z((axis == Axis::Z) ? T(1) : T(0)) {}

	explicit operator tuples::templates::Tuple3<T>() noexcept { return tuples::templates::Tuple3<T>(x, y, z); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(x), U(y), U(z)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	Vector3 operator+() const noexcept { return *this; }
	Vector3 operator-() const noexcept { return Vector3(-x, -y, -z); }
	Vector3& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector3& operator*=(const T f) noexcept { x *= f; y *= f; z *= f; return *this; }
	Vector3& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; z /= v.z; return *this; }
	Vector3& operator/=(const T f) noexcept { const T s = T(1)/f; x *= s; y *= s; z *= s; return *this; }
	Vector3& operator*=(const Matrix3<T>& m) noexcept; // #TODO
	friend Vector3 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
	friend Vector3 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	friend Vector3 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z); }
	friend Vector3 operator*(const T f, ConstArg v) noexcept { return Vector3(f*v.x, f*v.y, f*v.z); }
	friend Vector3 operator*(ConstArg v, const T f) noexcept { return Vector3(v.x*f, v.y*f, v.z*f); }
	friend Vector3 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z); }
	friend Vector3 operator/(const T f, ConstArg v) noexcept { return Vector3(f/v.x, f/v.y, f/v.z); }
	friend Vector3 operator/(ConstArg v, const T f) noexcept { const T s = T(1)/f; return Vector3(v.x*s, v.y*s, v.z*s); }
	friend Vector3 operator*(ConstArg v, const Matrix3<T>& m) noexcept; // #TODO
	//friend Vector3 operator*(const Matrix3<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector3& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const Vector3& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector3& v) { return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z; }
	friend std::ostream& operator<<(std::ostream& s, const Vector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	Vector2<T>::ConstResult getXY() const noexcept { return *reinterpret_cast<const Vector2*>(this); }
	void setXY(Vector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	Vector2 getXZ() const noexcept { return Vector2(x, z); }
	Vector2 getZY() const noexcept { return Vector2(z, y); }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(ConstArg v) const noexcept;
	bool isApproxEqual(ConstArg v, const T tolerance) const noexcept;
	bool allLessThan(ConstArg v) const noexcept { return (x < v.x) && (y < v.y) && (z < v.z); }
	bool allLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) && (y <= v.y) && (z <= v.z); }
	bool allGreaterThan(ConstArg v) const noexcept { return (x > v.x) && (y > v.y) && (z > v.z); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) && (y >= v.y) && (z >= v.z); }
	bool anyLessThan(ConstArg v) const noexcept { return (x < v.x) || (y < v.y) || (z < v.z); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) || (y <= v.y) || (z <= v.z); }
	bool anyGreaterThan(ConstArg v) const noexcept { return (x > v.x) || (y > v.y) || (z > v.z); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) || (y >= v.y) || (z >= v.z); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }
	T getMagnitude() const { return std::sqrt(x*x + y*y + z*z); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y + z*z); }
	void setMagnitude(const T magnitude);
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const T length) { setMagnitude(length); }
	Axis getMajorAxis() const noexcept;
	T getMinComponent() const noexcept { return std::min(std::min(x, y), z); }
	T getMaxComponent() const noexcept { return std::max(std::max(x, y), z); }
	Vector3& setZero() noexcept { x = T(); y = T(); z = T(); return *this; }
	Vector3& set(const T x, const T y, const T z) noexcept { this->x = x; this->y = y; this->z = z; return *this; }
	Vector3& setMinimum(ConstArg v1, ConstArg v2) noexcept;
	Vector3& setMaximum(ConstArg v1, ConstArg v2) noexcept;
	Vector3& negate() noexcept { x = -x; y = -y; z = -z; return *this; }
	Vector3& normalize();
	Vector3& rotate(const Axis axis, const T angle);
	Vector3& rotate(const Quaternion<T>& q);
	Vector3& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector3& transform(const Matrix3<T>& m); // #TODO
	Vector3& transform(const AffineTransform<T>& m); // #TODO

	static const Vector3& getZero() noexcept { return ZERO; }
	static const Vector3& getUnitX() noexcept { return UNIT_X; }
	static const Vector3& getUnitY() noexcept { return UNIT_Y; }
	static const Vector3& getUnitZ() noexcept { return UNIT_Z; }

	static const Vector3 ZERO;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z; 
	static const Vector3 ONE;
	static const Vector3 TOLERANCE;
	static const Vector3 INF;
	static const Vector3 MINUS_INF;

	T x, y, z;
};

template<typename T>
inline bool Vector3<T>::isApproxZero() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE) && 
		(std::fabs(z) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Vector3<T>::isApproxEqual(ConstArg v) const
{ 
	return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && (std::fabs(v.y - y) < Constants<T>::TOLERANCE) &&
		(std::fabs(v.z - z) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Vector3<T>::isApproxEqual(ConstArg v, const T tolerance) const
{ 
	return (std::fabs(v.x - x) < tolerance) && (std::fabs(v.y - y) < tolerance) &&
		(std::fabs(v.z - z) < tolerance);
}

template<typename T>
inline void Vector3<T>::setMagnitude(const T magnitude) 
{ 
	const T m = getMagnitude(); 
	if (m > T(0)) 
		*this *= magnitude/m;
}

template<typename T>
inline Axis Vector3<T>::getMajorAxis() const
{ 
	Axis axis = Axis::X; 
	if (std::fabs(y) > std::fabs(x)) 
		axis = Axis::Y;
	if (std::fabs(z) > std::fabs((&x)[(int)axis])) 
		axis = Axis::Z; 
	return axis;
}

template<typename T>
inline Vector3<T>& Vector3<T>::setMinimum(ConstArg v1, ConstArg v2)
{ 
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y);
	z = std::min(v1.z, v2.z); 
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::setMaximum(ConstArg v1, ConstArg v2)
{ 
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y);
	z = std::max(v1.z, v2.z); 
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
inline Vector3<T>& Vector3<T>::rotate(const Axis axis, const T angle)
{
	if (angle != T(0))
	{
		const T sinAngle = std::sin(angle);
		const T cosAngle = std::cos(angle);

		switch (axis)
		{
			case Axis::X:
			{
				const T j = y, k = z;
				y = j*cosAngle - k*sinAngle;
				z = k*cosAngle + j*sinAngle;
			} break;

			case Axis::Y:
			{
				const T i = x, k = z;
				x = i*cosAngle + k*sinAngle;
				z = k*cosAngle - i*sinAngle;
			} break;

			case Axis::Z:
			{
				const T i = x, j = y;
				x = i*cosAngle - j*sinAngle;
				y = j*cosAngle + i*sinAngle;
			} break;
		}
	}

	return *this;
}

template<typename T> const Vector3<T> Vector3<T>::ZERO{};
template<typename T> const Vector3<T> Vector3<T>::UNIT_X{ T(1), T(0), T(0) };
template<typename T> const Vector3<T> Vector3<T>::UNIT_Y{ T(0), T(1), T(0) };
template<typename T> const Vector3<T> Vector3<T>::UNIT_Z{ T(0), T(0), T(1) };
template<typename T> const Vector3<T> Vector3<T>::ONE{ T(1), T(1), T(1) };
template<typename T> const Vector3<T> Vector3<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };
template<typename T> const Vector3<T> Vector3<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() };
template<typename T> const Vector3<T> Vector3<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity() };

#if SIMD_HAS_FLOAT4

namespace float4 = simd::float4;

template<>
struct Vector3<float>
{
	using Real = float;
	using ConstArg = const Vector3;
	using ConstResult = const Vector3;

	static constexpr int NUM_COMPONENTS = 3;

	/*constexpr*/ Vector3() noexcept { xyz = float4::zero(); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	/*constexpr*/ explicit Vector3(const float scalar) noexcept { xyz = float4::set4(scalar); }
	/*constexpr*/ Vector3(const float x, const float y, const float z) noexcept { xyz = float4::set4(x, y, z, z); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v) noexcept { xyz = float4::cutoff2(v); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v, const float z) noexcept { xyz = float4::combine2(v, float4::set2(z)); }
	template<typename U> explicit Vector3(const IntVector3<U>& v) noexcept;
	explicit Vector3(const tuples::templates::Tuple3<float>& t) noexcept { xyz = float4::set4(t.x, t.y, t.z, t.z); }
	template<typename U> explicit Vector3(const tuples::templates::Tuple3<U>& t) noexcept { float z = (float)t.z; xyz = float4::set4((float)t.x, (float)t.y, z, z); }
	explicit Vector3(const float* const v) noexcept { set(v[0], v[1], v[2]); }
#else
	/*constexpr*/ explicit Vector3(const float scalar) noexcept { xyz = float4::set3(scalar); }
	/*constexpr*/ Vector3(const float x, const float y, const float z) noexcept { xyz = float4::set3(x, y, z); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v) noexcept { xyz = float4::cutoff2(v); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v, const float z) noexcept { xyz = float4::combine2(v, float4::set1(z)); }
	template<typename U> explicit Vector3(const IntVector3<U>& v) noexcept;
	explicit Vector3(const tuples::templates::Tuple3<float>& t) noexcept { xyz = float4::set3(t.x, t.y, t.z); }
	template<typename U> explicit Vector3(const tuples::templates::Tuple3<U>& t) noexcept { xyz = float4::set3((float)t.x, (float)t.y, (float)t.z); }
	explicit Vector3(const float* const v) noexcept { xyz = float4::load3(v); }
#endif
	explicit Vector3(const Axis axis) noexcept { set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f, (axis == Axis::Z) ? 1.f : 0.f); }
	explicit Vector3(const float4::Type v) noexcept : xyz(v) {}

	operator float4::Type() const noexcept { return xyz; }
	explicit operator tuples::templates::Tuple3<float>() noexcept { return tuples::templates::Tuple3<float>(x, y, z); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(x), U(y), U(z)); }
	explicit operator float*() noexcept { return &x; }
	explicit operator const float*() const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Vector3 operator+() const noexcept { return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3 operator-() const noexcept { return Vector3(float4::neg4(xyz)); }
#else
	Vector3 operator-() const noexcept { return Vector3(float4::neg3(xyz)); }
#endif
	Vector3& operator+=(ConstArg v) noexcept { xyz = float4::add4(xyz, v); return *this; }
	Vector3& operator-=(ConstArg v) noexcept { xyz = float4::sub4(xyz, v); return *this; }
	Vector3& operator*=(ConstArg v) noexcept { xyz = float4::mul4(xyz, v); return *this; }
	Vector3& operator*=(const float f) noexcept { xyz = float4::mul4(xyz, float4::set4(f)); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3& operator/=(ConstArg v) noexcept { xyz = float4::div4(xyz, v); return *this; }
#else
	Vector3& operator/=(ConstArg v) noexcept { xyz = float4::div3(xyz, v); return *this; }
#endif
	Vector3& operator/=(const float f) noexcept { xyz = float4::div4(xyz, float4::set4(f)); return *this; }
	Vector3& operator*=(const Matrix3<float>& m) noexcept; // #TODO
	friend Vector3 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector3(float4::add4(v1, v2)); }
	friend Vector3 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector3(float4::sub4(v1, v2)); }
	friend Vector3 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector3(float4::mul4(v1, v2)); }
	friend Vector3 operator*(const float f, ConstArg v) noexcept { return Vector3(float4::mul4(float4::set4(f), v)); }
	friend Vector3 operator*(ConstArg v, const float f) noexcept { return Vector3(float4::mul4(v, float4::set4(f))); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	friend Vector3 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector3(float4::div4(v1, v2)); }
	friend Vector3 operator/(const float f, ConstArg v) noexcept { return Vector3(float4::div4(float4::set4(f), v)); }
#else
	friend Vector3 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector3(float4::div3(v1, v2)); }
	friend Vector3 operator/(const float f, ConstArg v) noexcept { return Vector3(float4::div3(float4::set3(f), v)); }
#endif
	friend Vector3 operator/(ConstArg v, const float f) noexcept { return Vector3(float4::div4(v, float4::set4(f))); }
	friend Vector3 operator*(ConstArg v, const Matrix3<float>& m) noexcept; // #TODO
	//friend Vector3 operator*(const Matrix3<float>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector3& v) const noexcept { return float4::all3(float4::equal(xyz, v)); }
	bool operator!=(const Vector3& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector3& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; } // #FIXME use float4::set(x, y, z, z)

#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2<float> getXY() const noexcept { return Vector2<float>(float4::swizzle<float4::XYYY>(xyz)); }
	Vector2<float> getXZ() const noexcept { return Vector2<float>(float4::swizzle<float4::XZZZ>(xyz)); }
	Vector2<float> getZY() const noexcept { return Vector2<float>(float4::swizzle<float4::ZYYY>(xyz)); }
#else
	Vector2<float> getXY() const noexcept { return Vector2<float>(float4::cutoff2(xyz)); }
	Vector2<float> getXZ() const noexcept { return Vector2<float>(float4::swizzle/*2*/<float4::XZWW>(xyz)); }
	Vector2<float> getZY() const noexcept { return Vector2<float>(float4::swizzle/*2*/<float4::ZYWW>(xyz)); }
#endif
	void setXY(Vector2<float>::ConstArg v) noexcept { xyz = float4::insert2(v, xyz); }
	bool isZero() const noexcept { return float4::all3(float4::equal(xyz, float4::zero())); }
	bool isApproxZero() const noexcept { float4::all3(float4::lessThan(float4::abs4(xyz), TOLERANCE)); }
	bool isApproxEqual(ConstArg v) const noexcept { float4::all3(float4::lessThan(float4::abs4(float4::sub4(xyz, v)), TOLERANCE)); }
	bool isApproxEqual(ConstArg v, const float tolerance) const noexcept { float4::all3(float4::lessThan(float4::abs4(float4::sub4(xyz, v)), float4::set4(tolerance))); }
	bool allLessThan(ConstArg v) const noexcept { return float4::all3(float4::lessThan(xyz, v)); }
	bool allLessThanEqual(ConstArg v) const noexcept { return float4::all3(float4::lessThanEqual(xyz, v)); }
	bool allGreaterThan(ConstArg v) const noexcept { return float4::all3(float4::greaterThan(xyz, v)); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return float4::all3(float4::greaterThanEqual(xyz, v)); }
	bool anyLessThan(ConstArg v) const noexcept { return float4::any3(float4::lessThan(xyz, v)); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return float4::any3(float4::lessThanEqual(xyz, v)); }
	bool anyGreaterThan(ConstArg v) const noexcept { return float4::any3(float4::greaterThan(xyz, v)); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return float4::any3(float4::greaterThanEqual(xyz, v)); }
	bool isFinite() const { return float4::all3(float4::isFinite(xyz)); }
	float getMagnitude() const noexcept { return float4::toFloat(float4::sqrt1(float4::dot3(xyz, xyz))); }
	float getMagnitudeSquared() const noexcept { return float4::toFloat(float4::dot3(xyz, xyz)); }
	void setMagnitude(const float magnitude) noexcept;
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const float length) noexcept { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { (Axis)float4::asIndex(float4::equal(xyz, float4::hMax3(xyz))); }
	float getMinComponent() const noexcept { return float4::toFloat(float4::hMin3(xyz)); }
	float getMaxComponent() const noexcept { return float4::toFloat(float4::hMax3(xyz)); }
	Vector3& setZero() noexcept { xyz = float4::zero(); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3& set(const float x, const float y, const float z) noexcept { xyz = float4::set4(x, y, z, z); return *this; }
#else
	Vector3& set(const float x, const float y, const float z) noexcept { xyz = float4::set3(x, y, z); return *this; }
#endif
	Vector3& setMinimum(ConstArg v1, ConstArg v2) noexcept { xyz = float4::min4(v1, v2); return *this; }
	Vector3& setMaximum(ConstArg v1, ConstArg v2) noexcept { xyz = float4::max4(v1, v2); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3& negate() noexcept { xyz = float4::neg4(xyz); return *this; }
#else
	Vector3& negate() noexcept { xyz = float4::neg3(xyz); return *this; }
#endif
	Vector3& normalize() noexcept;
	Vector3& rotate(const Axis axis, const float angle);
	Vector3& rotate(const Quaternion<float>& q);
	Vector3& scale(ConstArg v) noexcept { xyz = float4::mul4(xyz, v); return *this; }
	Vector3& transform(const Matrix3<float>& m); // #TODO
	Vector3& transform(const AffineTransform<float>& m); // #TODO

	static const Vector3& getZero() noexcept { return ZERO; }
	static const Vector3& getUnitX() noexcept { return UNIT_X; }
	static const Vector3& getUnitY() noexcept { return UNIT_Y; }
	static const Vector3& getUnitZ() noexcept { return UNIT_Z; }

	static const Vector3 ZERO;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;
	static const Vector3 ONE;
	static const Vector3 TOLERANCE;
	static const Vector3 INF;
	static const Vector3 MINUS_INF;

	union
	{
		float4::Type xyz;
		struct { float x, y, z/*, reserved_*/; };
	};
};

inline std::istream& Vector3<float>::operator>>(std::istream& s, Vector3<float>& v) 
{ 
	float x, y, z; 
	s >> x >> std::skipws >> y >> std::skipws >> z; 
	v.set(x, y, z); 
	return s; 
}

inline void Vector3<float>::setMagnitude(const float magnitude)
{ 
	const float m = getMagnitude(); 
	if (m > 0.f) 
		*this *= magnitude/m;
}

inline Vector3<float>& Vector3<float>::normalize()
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

inline Vector3<float>& Vector3<float>::rotate(const Axis axis, const float angle)
{
	if (angle != 0.f)
	{
		const float sinAngle = std::sin(angle);
		const float cosAngle = std::cos(angle);

		switch (axis)
		{
			case Axis::X:
			{
				const float j = y, k = z;
				set(x, j*cosAngle - k*sinAngle, k*cosAngle + j*sinAngle); // #TODO SIMD
			} break;

			case Axis::Y:
			{
				const float i = x, k = z;
				set(i*cosAngle + k*sinAngle, y, k*cosAngle - i*sinAngle); // #TODO SIMD
			} break;

			case Axis::Z:
			{
				const float i = x, j = y;
				set(i*cosAngle - j*sinAngle, j*cosAngle + i*sinAngle, z); // #TODO SIMD
			} break;
		}
	}

	return *this;
}

const Vector3<float> Vector3<float>::ZERO{};
const Vector3<float> Vector3<float>::UNIT_X{ 1.f, 0.f, 0.f };
const Vector3<float> Vector3<float>::UNIT_Y{ 0.f, 1.f, 0.f };
const Vector3<float> Vector3<float>::UNIT_Z{ 0.f, 0.f, 1.f };
const Vector3<float> Vector3<float>::ONE{ 1.f, 1.f, 1.f };
const Vector3<float> Vector3<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };
const Vector3<float> Vector3<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
const Vector3<float> Vector3<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Vector3f = templates::Vector3<float>;
using Vector3d = templates::Vector3<double>;

#if MATHEMATICS_DOUBLE
using Vector3 = templates::Vector3<double>;
using Vector3Arg = templates::Vector3<double>::ConstArg;
using Vector3Result = templates::Vector3<double>::ConstResult;
#else
using Vector3 = templates::Vector3<float>;
using Vector3Arg = templates::Vector3<float>::ConstArg;
using Vector3Result = templates::Vector3<float>::ConstResult;
#endif

//template<typename T>
//inline T length(templates::Vector3<T>::ConstArg v) { return v.getMagnitude(); }
//
//template<typename T>
//inline T lengthSquared(templates::Vector3<T>::ConstArg v) { return v.getMagnitudeSquared(); }

} // namespace mathematics
} // namespace core

#include "IntVector3.hpp"
#include "Quaternion.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
template<typename U> 
inline Vector3<T>::Vector3(const IntVector3<U>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z))
{
}

template<typename T>
Vector3<T>& Vector3<T>::rotate(const Quaternion<T>& q)
{
	const T x1 = q.y*z - q.z*y;
    const T y1 = q.z*x - q.x*z;
    const T z1 = q.x*y - q.y*x;
    const T x2 = q.w*x1 + q.y*z1 - q.z*y1;
    const T y2 = q.w*y1 + q.z*x1 - q.x*z1;
    const T z2 = q.w*z1 + q.x*y1 - q.y*x1;
    x += T(2)*x2;
    y += T(2)*y2;
    z += T(2)*z2;
	return *this;
}

#if SIMD_HAS_FLOAT4

template<typename U> 
inline Vector3<float>::Vector3(const IntVector3<U>& v)
{
#if MATHEMATICS_SIMD_EXPAND_LAST
	const float t = (float)v.z;
	xyz = float4::set4((float)v.x, (float)v.y, t, t); 
#else
	xyz = float4::set4((float)v.x, (float)v.y, (float)v.z);
#endif
}

Vector3<float>& Vector3<float>::rotate(const Quaternion<float>& q)
{
	const float4::Type yzx = float4::swizzle<float4::YZXX>(q);
	const float4::Type zxy = float4::swizzle<float4::ZXYY>(q);
	const float4::Type w = float4::broadcast<float4::W>(q);
	const float4::Type t1 = float4::sub4(float4::mul4(yzx, float4::swizzle<float4::ZXYY>(xyz)),
		float4::mul4(zxy, float4::swizzle<float4::YZXX>(xyz)));
	const float4::Type t2 = float4::mulAdd4(w, t1, 
		float4::sub4(float4::mul4(yzx, float4::swizzle<float4::ZXYY>(t1)),
			float4::mul4(zxy, float4::swizzle<float4::YZXX>(t1))));
	static const float4::Type two = float4::set3(2.0f);
	xyz = float4::mulAdd4(two, t2, xyz);
	return *this;
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_VECTOR3_HPP */

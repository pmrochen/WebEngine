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
	template<typename U> explicit Vector3(const IntVector3<U>&/*IntVector3<U>::ConstArg*/ v) noexcept;
	explicit Vector3(const Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)),
		z((axis == Axis::Z) ? T(1) : T(0)) {}
	explicit Vector3(const T* const v) noexcept { /*if (v) {*/ x = v[0]; y = v[1]; z = v[2]; /*} else zero();*/ }
	/*explicit*/ operator T* () noexcept { return &x; }
	/*explicit*/ operator const T* () const noexcept { return &x; }

	Vector3 operator+() const noexcept { return *this; }
	Vector3 operator-() const noexcept { return Vector3(-x, -y, -z); }
	Vector3& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(const T f) noexcept { x *= f; y *= f; z *= f; return *this; }
	Vector3& operator/=(const T f) noexcept { T s = T(1)/f; x *= s; y *= s; z *= s; return *this; }
	//Vector3& operator*=(const Matrix3<T>& m) noexcept; // #TODO
	friend Vector3 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
	friend Vector3 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	friend Vector3 operator*(const T f, ConstArg v) noexcept { return Vector3(f*v.x, f*v.y, f*v.z); }
	friend Vector3 operator*(ConstArg v, const T f) noexcept { return Vector3(v.x*f, v.y*f, v.z*f); }
	friend Vector3 operator/(const T f, ConstArg v) noexcept { return Vector3(f/v.x, f/v.y, f/v.z); }
	friend Vector3 operator/(ConstArg v, const T f) noexcept { T s = T(1)/f; return Vector3(v.x*s, v.y*s, v.z*s); }
	//friend Vector3 operator*(ConstArg v, const Matrix3<T>& m) noexcept; // #TODO
	////friend Vector3 operator*(const Matrix3<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(ConstArg v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(ConstArg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector3& v) { return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z; }
	friend std::ostream& operator<<(std::ostream& s, const Vector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	Vector2<T>::ConstResult getXY() const noexcept { return *reinterpret_cast<const Vector2*>(this); }
	void setXY(Vector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	Vector2 getXZ() const noexcept { return Vector2(x, z); }
	Vector2 getZY() const noexcept { return Vector2(z, y); }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()); }
	bool isApproxZero() const noexcept { return (std::fabs(x) < Constants<T>::TOLERANCE) &&
		(std::fabs(y) < Constants<T>::TOLERANCE) && (std::fabs(z) < Constants<T>::TOLERANCE); }
	bool isApproxEqual(ConstArg v) const noexcept { return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && 
		(std::fabs(v.y - y) < Constants<T>::TOLERANCE) && (std::fabs(v.z - z) < Constants<T>::TOLERANCE); }
	bool isApproxEqual(ConstArg v, const T tolerance) const noexcept { return (std::fabs(v.x - x) < tolerance) &&
		(std::fabs(v.y - y) < tolerance) && (std::fabs(v.z - z) < tolerance); }
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
	void setMagnitude(const T magnitude) { T m = getMagnitude(); if (m > T(0)) *this *= magnitude/m; }
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const T length) { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { Axis axis = Axis::X; if (std::fabs(y) > std::fabs(x)) axis = Axis::Y;
		if (std::fabs(z) > std::fabs((&x)[(int)axis])) axis = Axis::Z; return axis; }
	T getMinComponent() const noexcept { return std::min(std::min(x, y), z); }
	T getMaxComponent() const noexcept { return std::max(std::max(x, y), z); }
	Vector3& setZero() noexcept { x = T(); y = T(); z = T(); return *this; }
	Vector3& set(const T x, const T y, const T z) noexcept { this->x = x; this->y = y; this->z = z; return *this; }
	Vector3& setMinimumOf(ConstArg v1, ConstArg v2) noexcept { x = std::min(v1.x, v2.x); y = std::min(v1.y, v2.y);
		z = std::min(v1.z, v2.z); return *this; }
	Vector3& setMaximumOf(ConstArg v1, ConstArg v2) noexcept { x = std::max(v1.x, v2.x); y = std::max(v1.y, v2.y);
		z = std::max(v1.z, v2.z); return *this; }
	Vector3& negate() noexcept { x = -x; y = -y; z = -z; return *this; }
#if MATHEMATICS_FAST_NORMALIZE
	Vector3& normalize() noexcept { T m = rcpSqrtApprox(getMagnitudeSquared()); if (m <= std::numeric_limits<T>::max()) *this *= m; return *this; }
#else
	Vector3& normalize() { T m = getMagnitude(); if (m > T(0)) *this /= m; return *this; }
#endif
	Vector3& rotate(const Axis axis, const T angle);
	//Vector3& rotate(const Quaternion<T>& q); // #TODO
	Vector3& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	//Vector3& transform(const Matrix3<T>& m); // #TODO
	//Vector3& transform(const AffineTransform<T>& m); // #TODO
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
inline Vector3<T>& Vector3<T>::rotate(const Axis axis, const T angle)
{
	if (angle != T(0))
	{
		float sinAngle, cosAngle;
		sinCos(angle, sinAngle, cosAngle);

		switch (axis)
		{
			case Axis::X:
			{
				const float j = y, k = z;
				y = j*cosAngle - k*sinAngle;
				z = k*cosAngle + j*sinAngle;
			} break;

			case Axis::Y:
			{
				const float i = x, k = z;
				x = i*cosAngle + k*sinAngle;
				z = k*cosAngle - i*sinAngle;
			} break;

			case Axis::Z:
			{
				const float i = x, j = y;
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
template<typename T> const Vector3<T> Vector3<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE,
	Constants<T>::TOLERANCE };
template<typename T> const Vector3<T> Vector3<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), 
	std::numeric_limits<T>::infinity() };
template<typename T> const Vector3<T> Vector3<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(),
	-std::numeric_limits<T>::infinity() };

#if SIMD_HAS_FLOAT4

template<>
/*_MM_ALIGN16*/ struct Vector3<float>
{
	using Real = float;
	using ConstArg = const Vector3;
	using ConstResult = const Vector3;

	static constexpr int NUM_COMPONENTS = 3;

	/*constexpr*/ Vector3() noexcept { xyz = simd::setZero(); }
	/*constexpr*/ explicit Vector3(const float scalar) noexcept { xyz = simd::set4(scalar); }
	/*constexpr*/ Vector3(const float x, const float y, const float z) noexcept { xyz = simd::set4(x, y, z, z); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v) noexcept { xyz = simd::cutoff2(v); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v, const float z) noexcept { xyz = simd::combine2(v, simd::set2(z)); }
	template<typename U> explicit Vector3(const IntVector3<U>&/*IntVector3<U>::ConstArg*/ v) noexcept;
	explicit Vector3(const Axis axis) noexcept { set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f, (axis == Axis::Z) ? 1.f : 0.f); }
	explicit Vector3(const float* const v) noexcept { set(v[0], v[1], v[2]); }

	explicit Vector3(const simd::Float4 v) noexcept : xyz(v) {}
	operator simd::Float4() const noexcept { return xyz; }
	/*explicit*/ operator float* () noexcept { return &x; }
	/*explicit*/ operator const float* () const noexcept { return &x; }

	Vector3 operator+() const noexcept { return *this; }
	Vector3 operator-() const noexcept { return Vector3(simd::neg4(xyz)); }
	Vector3& operator+=(ConstArg v) noexcept { xyz = simd::add4(xyz, v); return *this; }
	Vector3& operator-=(ConstArg v) noexcept { xyz = simd::sub4(xyz, v); return *this; }
	Vector3& operator*=(const float f) noexcept { xyz = simd::mul4(xyz, simd::set4(f)); return *this; }
	Vector3& operator/=(const float f) noexcept { xyz = simd::mul4(xyz, simd::set4(1.f/f)); return *this; }
	//Vector3& operator*=(const Matrix3<T>& m) noexcept; // #TODO
	friend Vector3 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector3(simd::add4(v1, v2)); }
	friend Vector3 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector3(simd::sub4(v1, v2)); }
	friend Vector3 operator*(const float f, ConstArg v) noexcept { return Vector3(simd::mul4(simd::set4(f), v)); }
	friend Vector3 operator*(ConstArg v, const float f) noexcept { return Vector3(simd::mul4(v, simd::set4(f))); }
	friend Vector3 operator/(const float f, ConstArg v) noexcept { return Vector3(simd::div4(simd::set4(f), v)); }
	friend Vector3 operator/(ConstArg v, const float f) noexcept { return Vector3(simd::mul4(v, simd::set4(1.f/f))); }
	//friend Vector3 operator*(ConstArg v, const Matrix3<T>& m) noexcept; // #TODO
	////friend Vector3 operator*(const Matrix3<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(ConstArg v) const noexcept { return simd::all3(simd::equal4(xyz, v)); }
	bool operator!=(ConstArg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector3& v) { float x, y, z; s >> x >> std::skipws >> y >> std::skipws >> z; v.set(x, y, z); return s; }
	friend std::ostream& operator<<(std::ostream& s, const Vector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; } // #FIXME use simd::set(x, y, z, z)

	Vector2<float> getXY() const noexcept { return Vector2<float>(simd::swizzle<simd::XYYY>(xyz)); }
	void setXY(Vector2<float>::ConstArg v) noexcept { xyz = simd::insert2(v, xyz); }
	Vector2<float> getXZ() const noexcept { return Vector2<float>(simd::swizzle<simd::XZZZ>(xyz)); }
	Vector2<float> getZY() const noexcept { return Vector2<float>(simd::swizzle<simd::ZYYY>(xyz)); }
	bool isZero() const noexcept { return simd::all3(simd::equal4(xyz, simd::setZero())); }
	bool isApproxZero() const noexcept { simd::all3(simd::lessThan4(simd::abs4(xyz), TOLERANCE)); }
	bool isApproxEqual(ConstArg v) const noexcept { simd::all3(simd::lessThan4(simd::abs4(simd::sub4(xyz, v)), TOLERANCE)); }
	bool isApproxEqual(ConstArg v, const float tolerance) const noexcept { simd::all3(simd::lessThan4(simd::abs4(simd::sub4(xyz, v)), simd::set4(tolerance))); }
	bool allLessThan(ConstArg v) const noexcept { return simd::all3(simd::lessThan4(xyz, v)); }
	bool allLessThanEqual(ConstArg v) const noexcept { return simd::all3(simd::lessThanEqual4(xyz, v)); }
	bool allGreaterThan(ConstArg v) const noexcept { return simd::all3(simd::greaterThan4(xyz, v)); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return simd::all3(simd::greaterThanEqual4(xyz, v)); }
	bool anyLessThan(ConstArg v) const noexcept { return simd::any3(simd::lessThan4(xyz, v)); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return simd::any3(simd::lessThanEqual4(xyz, v)); }
	bool anyGreaterThan(ConstArg v) const noexcept { return simd::any3(simd::greaterThan4(xyz, v)); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return simd::any3(simd::greaterThanEqual4(xyz, v)); }
	bool isFinite() const { return simd::all3(simd::isFinite4(xyz)); }
	float getMagnitude() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot3(xyz, xyz))); }
	float getMagnitudeSquared() const noexcept { return simd::toFloat(simd::dot3(xyz, xyz)); }
	void setMagnitude(const float magnitude) noexcept { float m = getMagnitude(); if (m > 0.f) *this *= magnitude/m; }
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const float length) noexcept { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { (Axis)simd::asIndex(simd::equal4(xyz, simd::hMax3(xyz))); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin3(xyz)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax3(xyz)); }
	Vector3& setZero() noexcept { xyz = simd::setZero(); return *this; }
	Vector3& set(const float x, const float y, const float z) noexcept { xyz = simd::set4(x, y, z, z); return *this; }
	Vector3& setMinimumOf(ConstArg v1, ConstArg v2) noexcept { xyz = simd::min4(v1, v2); return *this; }
	Vector3& setMaximumOf(ConstArg v1, ConstArg v2) noexcept { xyz = simd::max4(v1, v2); return *this; }
	Vector3& negate() noexcept { xyz = simd::neg4(xyz); return *this; }
#if MATHEMATICS_FAST_NORMALIZE
	Vector3& normalize() noexcept { float m = rcpSqrtApprox(getMagnitudeSquared()); if (m <= std::numeric_limits<float>::max()) *this *= m; return *this; }
#else
	Vector3& normalize() noexcept { float m = getMagnitude(); if (m > 0.f) *this /= m; return *this; }
#endif
	Vector3& rotate(const Axis axis, const float angle);
	//Vector3& rotate(const Quaternion<float>& q); // #TODO
	Vector3& scale(ConstArg v) noexcept { xyz = simd::mul4(xyz, v); return *this; }
	//Vector3& transform(const Matrix3<float>& m); // #TODO
	//Vector3& transform(const AffineTransform<float>& m); // #TODO
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
		simd::Float4 xyz;
		struct { float x, y, z/*, reserved_*/; };
	};
};

inline Vector3<float>& Vector3<float>::rotate(const Axis axis, const float angle)
{
	if (angle != 0.f)
	{
		float sinAngle, cosAngle;
		sinCos(angle, sinAngle, cosAngle);

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
const Vector3<float> Vector3<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE,
	Constants<float>::TOLERANCE };
const Vector3<float> Vector3<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
	std::numeric_limits<float>::infinity() };
const Vector3<float> Vector3<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
	-std::numeric_limits<float>::infinity() };

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

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
template<typename U> 
inline Vector3<T>::Vector3(const IntVector3<U>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z))
{
}

#if SIMD_HAS_FLOAT4

template<typename U> 
inline Vector3<float>::Vector3(const IntVector3<U>& v)
{
	const float t = (float)v.z;
	xyz = simd::set4((float)v.x, (float)v.y, t, t); 
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_VECTOR3_HPP */

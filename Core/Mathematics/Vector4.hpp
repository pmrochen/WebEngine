/*
 *	Name: Vector4
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_VECTOR4_HPP
#define CORE_MATHEMATICS_VECTOR4_HPP

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
#include "Vector3.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
struct IntVector4;

template<typename T>
struct Vector4
{
	using Real = T;
	using ConstArg = const Vector4&;
	using ConstResult = const Vector4&;

	static constexpr int NUM_COMPONENTS = 4;

	constexpr Vector4() noexcept : x(), y(), z(), w() {}
	constexpr explicit Vector4(const T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr Vector4(const T x, const T y, const T z, const T w) noexcept : x(x), y(y), z(z), w(w) {}
	constexpr Vector4(Vector2<T>::ConstArg v) noexcept : x(v.x), y(v.y), z(), w(T(1)) {}
	constexpr Vector4(Vector2<T>::ConstArg v, const T z, const T w) noexcept : x(v.x), y(v.y), z(z), w(w) {}
	constexpr Vector4(Vector2<T>::ConstArg xy, Vector2<T>::ConstArg zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	constexpr Vector4(Vector3<T>::ConstArg v) noexcept : x(v.x), y(v.y), z(v.z), w(T(1)) {}
	constexpr Vector4(Vector3<T>::ConstArg v, const T w) noexcept : x(v.x), y(v.y), z(v.z), w(w) {}
	template<typename U> explicit Vector4(const IntVector4<U>&/*IntVector4<U>::ConstArg*/ v) noexcept;
	explicit Vector4(const tuples::templates::Tuple4<T>& t) noexcept : x(t.x), y(t.y), z(t.z), w(t.w) {}
	template<typename U> explicit Vector4(const tuples::templates::Tuple4<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)), w(T(t.w)) {}
	explicit Vector4(const T* const v) noexcept { x = v[0]; y = v[1]; z = v[2]; w = v[3]; }
	explicit Vector4(const Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)), z((axis == Axis::Z) ? T(1) : T(0)), w((axis == Axis::W) ? T(1) : T(0)) {}

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	Vector4 operator+() const noexcept { return *this; }
	Vector4 operator-() const noexcept { return Vector4(-x, -y, -z, -w); }
	Vector4& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	Vector4& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	Vector4& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	Vector4& operator*=(const T f) noexcept { x *= f; y *= f; z *= f; w *= f; return *this; }
	Vector4& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	Vector4& operator/=(const T f) noexcept { const T s = T(1)/f; x *= s; y *= s; z *= s; w *= s; return *this; }
	//Vector4& operator*=(const Matrix4<T>& m) noexcept; // #TODO
	friend Vector4 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }
	friend Vector4 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }
	friend Vector4 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w); }
	friend Vector4 operator*(const T f, ConstArg v) noexcept { return Vector4(f*v.x, f*v.y, f*v.z, f*v.w); }
	friend Vector4 operator*(ConstArg v, const T f) noexcept { return Vector4(v.x*f, v.y*f, v.z*f, v.w*f); }
	friend Vector4 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w); }
	friend Vector4 operator/(const T f, ConstArg v) noexcept { return Vector4(f/v.x, f/v.y, f/v.z, f/v.w); }
	friend Vector4 operator/(ConstArg v, const T f) noexcept { const T s = T(1)/f; return Vector4(v.x*s, v.y*s, v.z*s, v.w*s); }
	//friend Vector4 operator*(ConstArg v, const Matrix4<T>& m) noexcept; // #TODO
	////friend Vector4 operator*(const Matrix4<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector4& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const Vector4& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector4& v) { return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z >> std::skipws >> v.w; }
	friend std::ostream& operator<<(std::ostream& s, const Vector4& v) { return s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	Vector2<T>::ConstResult getXY() const noexcept { return *reinterpret_cast<const Vector2*>(this); }
	void setXY(Vector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	Vector3<T>::ConstResult getXYZ() const noexcept { return *reinterpret_cast<const Vector3*>(this); }
	void setXYZ(Vector3<T>::ConstArg v) noexcept { x = v.x; y = v.y; z = v.z; }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(ConstArg v) const noexcept;
	bool isApproxEqual(ConstArg v, const T tolerance) const noexcept;
	bool allLessThan(ConstArg v) const noexcept { return (x < v.x) && (y < v.y) && (z < v.z) && (w < v.w); }
	bool allLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w); }
	bool allGreaterThan(ConstArg v) const noexcept { return (x > v.x) && (y > v.y) && (z > v.z) && (w > v.w); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w); }
	bool anyLessThan(ConstArg v) const noexcept { return (x < v.x) || (y < v.y) || (z < v.z) || (w < v.w); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) || (y <= v.y) || (z <= v.z) || (w <= v.w); }
	bool anyGreaterThan(ConstArg v) const noexcept { return (x > v.x) || (y > v.y) || (z > v.z) || (w > v.w); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) || (y >= v.y) || (z >= v.z) || (w >= v.w); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w); }
	T getMagnitude() const { return std::sqrt(x*x + y*y + z*z + w*w); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y + z*z + w*w); }
	void setMagnitude(const T magnitude);
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const T length) { setMagnitude(length); }
	T getMinComponent() const noexcept { return std::min(std::min(std::min(x, y), z), w); }
	T getMaxComponent() const noexcept { return std::max(std::max(std::max(x, y), z), w); }
	Vector4& setZero() noexcept { x = T(); y = T(); z = T(); w = T(); return *this; }
	Vector4& set(const T x, const T y, const T z, const T w) noexcept { this->x = x; this->y = y; this->z = z; this->w = w; return *this; }
	Vector4& setMinimum(ConstArg v1, ConstArg v2) noexcept;
	Vector4& setMaximum(ConstArg v1, ConstArg v2) noexcept;
	Vector4& negate() noexcept { x = -x; y = -y; z = -z; w = -w; return *this; }
	Vector4& normalize();
	Vector4& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	//Vector4& transform(const Matrix4<T>& m); // #TODO

	static const Vector4& getZero() noexcept { return ZERO; }
	static const Vector4& getUnitX() noexcept { return UNIT_X; }
	static const Vector4& getUnitY() noexcept { return UNIT_Y; }
	static const Vector4& getUnitZ() noexcept { return UNIT_Z; }
	static const Vector4& getUnitW() noexcept { return UNIT_W; }

	static const Vector4 ZERO;
	static const Vector4 UNIT_X;
	static const Vector4 UNIT_Y;
	static const Vector4 UNIT_Z; 
	static const Vector4 UNIT_W; 
	static const Vector4 ONE;
	static const Vector4 TOLERANCE;
	static const Vector4 INF;
	static const Vector4 MINUS_INF;

	T x, y, z, w;
};

template<typename T>
inline bool Vector4<T>::isApproxZero() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE) && 
		(std::fabs(z) < Constants<T>::TOLERANCE) && (std::fabs(w) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Vector4<T>::isApproxEqual(ConstArg v) const
{ 
	return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && (std::fabs(v.y - y) < Constants<T>::TOLERANCE) && 
		(std::fabs(v.z - z) < Constants<T>::TOLERANCE) && (std::fabs(v.w - w) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Vector4<T>::isApproxEqual(ConstArg v, const T tolerance) const
{
	return (std::fabs(v.x - x) < tolerance) && (std::fabs(v.y - y) < tolerance) && 
		(std::fabs(v.z - z) < tolerance) && (std::fabs(v.w - w) < tolerance); 
}

template<typename T>
inline void Vector4<T>::setMagnitude(const T magnitude) 
{ 
	const T m = getMagnitude(); 
	if (m > T(0)) 
		*this *= magnitude/m;
}

template<typename T>
inline Vector4<T>& Vector4<T>::setMinimum(ConstArg v1, ConstArg v2)
{ 
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y);
	z = std::min(v1.z, v2.z); 
	w = std::min(v1.w, v2.w); 
	return *this; 
}

template<typename T>
inline Vector4<T>& Vector4<T>::setMaximum(ConstArg v1, ConstArg v2)
{ 
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y);
	z = std::max(v1.z, v2.z); 
	w = std::max(v1.w, v2.w); 
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

template<typename T> const Vector4<T> Vector4<T>::ZERO{};
template<typename T> const Vector4<T> Vector4<T>::UNIT_X{ T(1), T(0), T(0), T(0) };
template<typename T> const Vector4<T> Vector4<T>::UNIT_Y{ T(0), T(1), T(0), T(0) };
template<typename T> const Vector4<T> Vector4<T>::UNIT_Z{ T(0), T(0), T(1), T(0) };
template<typename T> const Vector4<T> Vector4<T>::UNIT_W{ T(0), T(0), T(0), T(1) };
template<typename T> const Vector4<T> Vector4<T>::ONE{ T(1), T(1), T(1), T(1) };
template<typename T> const Vector4<T> Vector4<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };
template<typename T> const Vector4<T> Vector4<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() };
template<typename T> const Vector4<T> Vector4<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity() };

#if SIMD_HAS_FLOAT4

namespace float4 = simd::float4;

template<>
struct Vector4<float>
{
	using Real = float;
	using ConstArg = const Vector4;
	using ConstResult = const Vector4;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Vector4() noexcept { xyzw = float4::zero(); }
	/*constexpr*/ explicit Vector4(const float scalar) noexcept { xyzw = float4::set4(scalar); }
	/*constexpr*/ Vector4(const float x, const float y, const float z, const float w) noexcept { xyzw = float4::set4(x, y, z, w); }
	/*constexpr*/ Vector4(Vector2<float>::ConstArg v) noexcept { xyzw = float4::insert2(v, UNIT_W); }
	/*constexpr*/ Vector4(Vector2<float>::ConstArg v, const float z, const float w) noexcept { xyzw = float4::combine2(v, float4::set2(z, w)); }
	/*constexpr*/ Vector4(Vector2<float>::ConstArg xy, Vector2<float>::ConstArg zw) noexcept { xyzw = float4::combine2(xy, zw); }
	/*constexpr*/ Vector4(Vector3<float>::ConstArg v) noexcept { xyzw = float4::insert3(v, UNIT_W); }
	/*constexpr*/ Vector4(Vector3<float>::ConstArg v, const float w) noexcept { xyzw = float4::insert<float4::W>(w, v); }
	template<typename U> explicit Vector4(const IntVector4<U>&/*IntVector4<U>::ConstArg*/ v) noexcept;
	explicit Vector4(const tuples::templates::Tuple4<float>& t) noexcept { xyzw = float4::set4(t.x, t.y, t.z, t.w); }
	template<typename U> explicit Vector4(const tuples::templates::Tuple4<U>& t) noexcept { xyzw = float4::set4((float)t.x, (float)t.y, (float)t.z, (float)t.w); }
	explicit Vector4(const float* const v) noexcept { xyzw = float4::load4(v); }
	explicit Vector4(const Axis axis) noexcept { set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f, (axis == Axis::Z) ? 1.f : 0.f, (axis == Axis::W) ? 1.f : 0.f); }
	explicit Vector4(const float4::Type v) noexcept : xyzw(v) {}

	operator float4::Type() const noexcept { return xyzw; }
	explicit operator tuples::templates::Tuple4<float>() noexcept { return tuples::templates::Tuple4<float>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator float*() noexcept { return &x; }
	explicit operator const float*() const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Vector4 operator+() const noexcept { return *this; }
	Vector4 operator-() const noexcept { return Vector4(float4::neg4(xyzw)); }
	Vector4& operator+=(ConstArg v) noexcept { xyzw = float4::add4(xyzw, v); return *this; }
	Vector4& operator-=(ConstArg v) noexcept { xyzw = float4::sub4(xyzw, v); return *this; }
	Vector4& operator*=(ConstArg v) noexcept { xyzw = float4::mul4(xyzw, v); return *this; }
	Vector4& operator*=(const float f) noexcept { xyzw = float4::mul4(xyzw, float4::set4(f)); return *this; }
	Vector4& operator/=(ConstArg v) noexcept { xyzw = float4::div4(xyzw, v); return *this; }
	Vector4& operator/=(const float f) noexcept { xyzw = float4::mul4(xyzw, float4::set4(1.f/f)); return *this; }
	//Vector4& operator*=(const Matrix4<T>& m) noexcept; // #TODO
	friend Vector4 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector4(float4::add4(v1, v2)); }
	friend Vector4 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector4(float4::sub4(v1, v2)); }
	friend Vector4 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector4(float4::mul4(v1, v2)); }
	friend Vector4 operator*(const float f, ConstArg v) noexcept { return Vector4(float4::mul4(float4::set4(f), v)); }
	friend Vector4 operator*(ConstArg v, const float f) noexcept { return Vector4(float4::mul4(v, float4::set4(f))); }
	friend Vector4 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector4(float4::div4(v1, v2)); }
	friend Vector4 operator/(const float f, ConstArg v) noexcept { return Vector4(float4::div4(float4::set4(f), v)); }
	friend Vector4 operator/(ConstArg v, const float f) noexcept { return Vector4(float4::mul4(v, float4::set4(1.f/f))); }
	//friend Vector4 operator*(ConstArg v, const Matrix4<T>& m) noexcept; // #TODO
	////friend Vector4 operator*(const Matrix4<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector4& v) const noexcept { return float4::all4(float4::equal(xyzw, v)); }
	bool operator!=(const Vector4& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector4& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector4& v) { return s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; } // #FIXME use float4::set(x, y, z, w)

#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2<float> getXY() const noexcept { return Vector2<float>(float4::swizzle<float4::XYYY>(xyzw)); }
	Vector3<float> getXYZ() const noexcept { return Vector3<float>(float4::swizzle<float4::XYZZ>(xyzw)); }
#else
	Vector2<float> getXY() const noexcept { return Vector2<float>(float4::cutoff2(xyzw)); }
	Vector3<float> getXYZ() const noexcept { return Vector3<float>(float4::cutoff3(xyzw)); }
#endif
	void setXY(Vector2<float>::ConstArg v) noexcept { xyzw = float4::insert2(v, xyzw); }
	void setXYZ(Vector3<float>::ConstArg v) noexcept { xyzw = float4::insert3(v, xyzw); }
	bool isZero() const noexcept { return float4::all4(float4::equal(xyzw, float4::zero())); }
	bool isApproxZero() const noexcept { float4::all4(float4::lessThan(float4::abs4(xyzw), TOLERANCE)); }
	bool isApproxEqual(ConstArg v) const noexcept { float4::all4(float4::lessThan(float4::abs4(float4::sub4(xyzw, v)), TOLERANCE)); }
	bool isApproxEqual(ConstArg v, const float tolerance) const noexcept { float4::all4(float4::lessThan(float4::abs4(float4::sub4(xyzw, v)), float4::set4(tolerance))); }
	bool allLessThan(ConstArg v) const noexcept { return float4::all4(float4::lessThan(xyzw, v)); }
	bool allLessThanEqual(ConstArg v) const noexcept { return float4::all4(float4::lessThanEqual(xyzw, v)); }
	bool allGreaterThan(ConstArg v) const noexcept { return float4::all4(float4::greaterThan(xyzw, v)); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return float4::all4(float4::greaterThanEqual(xyzw, v)); }
	bool anyLessThan(ConstArg v) const noexcept { return float4::any4(float4::lessThan(xyzw, v)); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return float4::any4(float4::lessThanEqual(xyzw, v)); }
	bool anyGreaterThan(ConstArg v) const noexcept { return float4::any4(float4::greaterThan(xyzw, v)); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return float4::any4(float4::greaterThanEqual(xyzw, v)); }
	bool isFinite() const { return float4::all4(float4::isFinite(xyzw)); }
	float getMagnitude() const noexcept { return float4::toFloat(float4::sqrt1(float4::dot4(xyzw, xyzw))); }
	float getMagnitudeSquared() const noexcept { return float4::toFloat(float4::dot4(xyzw, xyzw)); }
	void setMagnitude(const float magnitude) noexcept;
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const float length) noexcept { setMagnitude(length); }
	float getMinComponent() const noexcept { return float4::toFloat(float4::hMin4(xyzw)); }
	float getMaxComponent() const noexcept { return float4::toFloat(float4::hMax4(xyzw)); }
	Vector4& setZero() noexcept { xyzw = float4::zero(); return *this; }
	Vector4& set(const float x, const float y, const float z, const float w) noexcept { xyzw = float4::set4(x, y, z, w); return *this; }
	Vector4& setMinimum(ConstArg v1, ConstArg v2) noexcept { xyzw = float4::min4(v1, v2); return *this; }
	Vector4& setMaximum(ConstArg v1, ConstArg v2) noexcept { xyzw = float4::max4(v1, v2); return *this; }
	Vector4& negate() noexcept { xyzw = float4::neg4(xyzw); return *this; }
	Vector4& normalize() noexcept;
	Vector4& scale(ConstArg v) noexcept { xyzw = float4::mul4(xyzw, v); return *this; }
	//Vector4& transform(const Matrix4<float>& m); // #TODO

	static const Vector4& getZero() noexcept { return ZERO; }
	static const Vector4& getUnitX() noexcept { return UNIT_X; }
	static const Vector4& getUnitY() noexcept { return UNIT_Y; }
	static const Vector4& getUnitZ() noexcept { return UNIT_Z; }
	static const Vector4& getUnitW() noexcept { return UNIT_W; }

	static const Vector4 ZERO;
	static const Vector4 UNIT_X;
	static const Vector4 UNIT_Y;
	static const Vector4 UNIT_Z;
	static const Vector4 UNIT_W;
	static const Vector4 ONE;
	static const Vector4 TOLERANCE;
	static const Vector4 INF;
	static const Vector4 MINUS_INF;

	union
	{
		float4::Type xyzw;
		struct { float x, y, z, w; };
	};
};

inline std::istream& Vector4<float>::operator>>(std::istream& s, Vector4<float>& v)
{ 
	float x, y, z, w; 
	s >> x >> std::skipws >> y >> std::skipws >> z >> std::skipws >> w; 
	v.set(x, y, z, w); 
	return s; 
}

inline void Vector4<float>::setMagnitude(const float magnitude)
{ 
	const float m = getMagnitude();
	if (m > 0.f) 
		*this *= magnitude/m;
}

inline Vector4<float>& Vector4<float>::normalize()
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

const Vector4<float> Vector4<float>::ZERO{};
const Vector4<float> Vector4<float>::UNIT_X{ 1.f, 0.f, 0.f, 0.f };
const Vector4<float> Vector4<float>::UNIT_Y{ 0.f, 1.f, 0.f, 0.f };
const Vector4<float> Vector4<float>::UNIT_Z{ 0.f, 0.f, 1.f, 0.f };
const Vector4<float> Vector4<float>::UNIT_W{ 0.f, 0.f, 0.f, 1.f };
const Vector4<float> Vector4<float>::ONE{ 1.f, 1.f, 1.f, 1.f };
const Vector4<float> Vector4<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };
const Vector4<float> Vector4<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
const Vector4<float> Vector4<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Vector4f = templates::Vector4<float>;
using Vector4d = templates::Vector4<double>;

#if MATHEMATICS_DOUBLE
using Vector4 = templates::Vector4<double>;
using Vector4Arg = templates::Vector4<double>::ConstArg;
using Vector4Result = templates::Vector4<double>::ConstResult;
#else
using Vector4 = templates::Vector4<float>;
using Vector4Arg = templates::Vector4<float>::ConstArg;
using Vector4Result = templates::Vector4<float>::ConstResult;
#endif

//template<typename T>
//inline T length(templates::Vector4<T>::ConstArg v) { return v.getMagnitude(); }
//
//template<typename T>
//inline T lengthSquared(templates::Vector4<T>::ConstArg v) { return v.getMagnitudeSquared(); }

} // namespace mathematics
} // namespace core

#include "IntVector4.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
template<typename U> 
inline Vector4<T>::Vector4(const IntVector4<U>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) 
{
}

#if SIMD_HAS_FLOAT4

template<typename U> 
inline Vector4<float>::Vector4(const IntVector4<U>& v)
{ 
	xyzw = simd::float4::set4((float)v.x, (float)v.y, (float)v.z, (float)v.w); 
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_VECTOR4_HPP */

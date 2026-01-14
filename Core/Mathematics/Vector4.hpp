/*
 *	Name: Vector4
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cmath>
#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <Simd/Intrinsics.hpp>
#include <Tuples/Tuple4.hpp>
#include "Constants.hpp"
#include "Axis.hpp"
#include "Scalar.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

#ifndef MATHEMATICS_FAST_NORMALIZE
#define MATHEMATICS_FAST_NORMALIZE 1
#endif

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
struct IntVector4;

template<typename T>
struct Matrix4;

template<typename T>
struct Vector4
{
	using Real = T;
	using ConstArg = const Vector4&;
	using ConstResult = const Vector4&;

	static constexpr int NUM_COMPONENTS = 4;

	constexpr Vector4() noexcept : x(), y(), z(), w() {}
	constexpr explicit Vector4(T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr Vector4(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}
	constexpr Vector4(Vector2<T>::ConstArg v) noexcept : x(v.x), y(v.y), z(), w(T(1)) {}
	constexpr Vector4(Vector2<T>::ConstArg v, T z, T w) noexcept : x(v.x), y(v.y), z(z), w(w) {}
	constexpr Vector4(Vector2<T>::ConstArg xy, Vector2<T>::ConstArg zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	constexpr Vector4(Vector3<T>::ConstArg v) noexcept : x(v.x), y(v.y), z(v.z), w(T(1)) {}
	constexpr Vector4(Vector3<T>::ConstArg v, T w) noexcept : x(v.x), y(v.y), z(v.z), w(w) {}
	template<typename U> explicit Vector4(const IntVector4<U>& v) noexcept;
	explicit Vector4(const tuples::templates::Tuple4<T>& t) noexcept : x(t.x), y(t.y), z(t.z), w(t.w) {}
	template<typename U> explicit Vector4(const tuples::templates::Tuple4<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)), w(T(t.w)) {}
	explicit Vector4(const std::tuple<T, T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)), w(std::get<3>(t)) {}
	template<typename U> explicit Vector4(const std::tuple<U, U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))), w(T(std::get<3>(t))) {}
	explicit Vector4(const T* v) noexcept { x = v[0]; y = v[1]; z = v[2]; w = v[3]; }
	explicit Vector4(Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)), z((axis == Axis::Z) ? T(1) : T(0)), w((axis == Axis::W) ? T(1) : T(0)) {}

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator std::tuple<T, T, T, T>() noexcept { return std::tuple<T, T, T, T>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() noexcept { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	Vector4 operator+() const noexcept { return *this; }
	Vector4 operator-() const noexcept { return Vector4(-x, -y, -z, -w); }
	Vector4& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	Vector4& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	Vector4& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	Vector4& operator*=(T f) noexcept { x *= f; y *= f; z *= f; w *= f; return *this; }
	Vector4& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	Vector4& operator/=(T f) noexcept { T s = T(1)/f; x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4& operator*=(const Matrix4<T>& m) noexcept; // #TODO
	friend Vector4 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }
	friend Vector4 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }
	friend Vector4 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w); }
	friend Vector4 operator*(T f, ConstArg v) noexcept { return Vector4(f*v.x, f*v.y, f*v.z, f*v.w); }
	friend Vector4 operator*(ConstArg v, T f) noexcept { return Vector4(v.x*f, v.y*f, v.z*f, v.w*f); }
	friend Vector4 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector4(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w); }
	friend Vector4 operator/(T f, ConstArg v) noexcept { return Vector4(f/v.x, f/v.y, f/v.z, f/v.w); }
	friend Vector4 operator/(ConstArg v, T f) noexcept { T s = T(1)/f; return Vector4(v.x*s, v.y*s, v.z*s, v.w*s); }
	friend Vector4 operator*(ConstArg v, const Matrix4<T>& m) noexcept; // #TODO
	//friend Vector4 operator*(const Matrix4<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector4& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const Vector4& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector4& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector4& v) { return s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	Vector2<T>::ConstResult xy/*getXY*/() const noexcept { return reinterpret_cast<const Vector2&>(*this); }
	Vector3<T>::ConstResult xyz/*getXYZ*/() const noexcept { return reinterpret_cast<const Vector3&>(*this); }
	//void setXY(Vector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	//void setXYZ(Vector3<T>::ConstArg v) noexcept { x = v.x; y = v.y; z = v.z; }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(const Vector4& v) const noexcept;
	bool isApproxEqual(const Vector4& v, T tolerance) const noexcept;
	bool allLessThan(const Vector4& v) const noexcept { return (x < v.x) && (y < v.y) && (z < v.z) && (w < v.w); }
	bool allLessThanEqual(const Vector4& v) const noexcept { return (x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w); }
	bool allGreaterThan(const Vector4& v) const noexcept { return (x > v.x) && (y > v.y) && (z > v.z) && (w > v.w); }
	bool allGreaterThanEqual(const Vector4& v) const noexcept { return (x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w); }
	bool anyLessThan(const Vector4& v) const noexcept { return (x < v.x) || (y < v.y) || (z < v.z) || (w < v.w); }
	bool anyLessThanEqual(const Vector4& v) const noexcept { return (x <= v.x) || (y <= v.y) || (z <= v.z) || (w <= v.w); }
	bool anyGreaterThan(const Vector4& v) const noexcept { return (x > v.x) || (y > v.y) || (z > v.z) || (w > v.w); }
	bool anyGreaterThanEqual(const Vector4& v) const noexcept { return (x >= v.x) || (y >= v.y) || (z >= v.z) || (w >= v.w); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w); }
	T getMagnitude() const { return std::sqrt(x*x + y*y + z*z + w*w); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y + z*z + w*w); }
	void setMagnitude(T magnitude);
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(T length) { setMagnitude(length); }
	T getMinComponent() const { return std::min(std::min(std::min(x, y), z), w); }
	T getMaxComponent() const { return std::max(std::max(std::max(x, y), z), w); }
	Vector4& setZero() noexcept { x = T(); y = T(); z = T(); w = T(); return *this; }
	Vector4& set(T x, T y, T z, T w) noexcept { this->x = x; this->y = y; this->z = z; this->w = w; return *this; }
	Vector4& setMinimum(const Vector4& v1, const Vector4& v2);
	Vector4& setMaximum(const Vector4& v1, const Vector4& v2);
	Vector4& negate() noexcept { x = -x; y = -y; z = -z; w = -w; return *this; }
	Vector4& normalize();
	//Vector4& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	Vector4& transform(const Matrix4<T>& m); // #TODO

	//static const Vector4& getZero() noexcept { return ZERO; }
	//static const Vector4& getUnitX() noexcept { return UNIT_X; }
	//static const Vector4& getUnitY() noexcept { return UNIT_Y; }
	//static const Vector4& getUnitZ() noexcept { return UNIT_Z; }
	//static const Vector4& getUnitW() noexcept { return UNIT_W; }

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
inline T dot(const Vector4<T>& v1, const Vector4<T>& v2) noexcept
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

template<typename T>
inline Vector4<T> cross(const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3) noexcept
{
	T d1 = v2.z*v3.w - v2.w*v3.z;
	T d2 = v2.y*v3.w - v2.w*v3.y;
	T d3 = v2.y*v3.z - v2.z*v3.y;
	T d4 = v2.x*v3.w - v2.w*v3.x;
	T d5 = v2.x*v3.z - v2.z*v3.x;
	T d6 = v2.x*v3.y - v2.y*v3.x;
	return Vector4(-v1.y*d1 + v1.z*d2 - v1.w*d3, v1.x*d1 - v1.z*d4 + v1.w*d5, -v1.x*d2 + v1.y*d4 - v1.w*d6, v1.x*d3 - v1.y*d5 + v1.z*d6);
}

//template<typename T>
//inline Matrix4<T> tensor(const Vector4<T>& v1, const Vector4<T>& v2) noexcept; // -> Matrix4

template<typename T>
inline T distance(const Vector4<T>& v1, const Vector4<T>& v2)
{
	T x = v2.x - v1.x;
	T y = v2.y - v1.y;
	T z = v2.z - v1.z;
	T w = v2.w - v1.w;
	return std::sqrt(x*x + y*y + z*z + w*w);
}

template<typename T>
inline T distanceSquared(const Vector4<T>& v1, const Vector4<T>& v2) noexcept
{
	T x = v2.x - v1.x;
	T y = v2.y - v1.y;
	T z = v2.z - v1.z;
	T w = v2.w - v1.w;
	return x*x + y*y + z*z + w*w;
}

template<typename T>
inline T length(const Vector4<T>& v)
{
	return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

template<typename T>
inline T lengthSquared(const Vector4<T>& v) noexcept
{
	return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
}

template<typename T>
inline Vector4<T> normalize(const Vector4<T>& v)
{
	Vector4<T> u(v);
	u.normalize();
	return u;
}

//template<typename T>
//inline Vector4<T> project(const Vector4<T>& v1, const Vector4<T>& v2) noexcept
//{
//	T m = v2.getMagnitudeSquared();
//	return (m > T(0)) ? (dot(v1, v2)/m)*v2 : Vector4<T>::ZERO;
//}
//
//template<typename T>
//inline T angle(const Vector4<T>& v1, const Vector4<T>& v2)
//{
//#if MATHEMATICS_FAST_NORMALIZE
//	Vector4<T> u(v1);
//	Vector4<T> v(v2);
//	u.normalize();
//	v.normalize();
//	return std::acos(std::clamp(dot(u, v), T(-1), T(1)));
//#else
//	T q = v1.getMagnitude()*v2.getMagnitude();
//	if (q > T(0))
//		return std::acos(std::clamp(dot(v1, v2)/q, T(-1), T(1)));
//	else
//		return (dot(v1, v2) >= T(0)) ? T(0) : Constants<T>::PI;
//#endif
//}

template<typename T>
inline Vector4<T> minimum(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return Vector4<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z), std::min(v1.w, v2.w));
}

template<typename T>
inline Vector4<T> maximum(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return Vector4<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z), std::max(v1.w, v2.w));
}

template<typename T>
inline Vector4<T> lerp(const Vector4<T>& v1, const Vector4<T>& v2, T t) noexcept
{
	// #TODO
}

template<typename T>
inline Vector4<T> slerp(const Vector4<T>& v1, const Vector4<T>& v2, T t) noexcept
{
	// #TODO
}

//template<typename T>
//inline Vector4<T> transform(const Vector4<T>& v, const Matrix4<T>& m) noexcept; // -> Matrix4

template<typename T>
inline std::istream& operator>>(std::istream& s, Vector4<T>& v) 
{ 
	return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z >> std::skipws >> v.w; 
}

template<typename T>
inline bool Vector4<T>::isApproxZero() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE) && 
		(std::fabs(z) < Constants<T>::TOLERANCE) && (std::fabs(w) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Vector4<T>::isApproxEqual(const Vector4<T>& v) const
{ 
	return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && (std::fabs(v.y - y) < Constants<T>::TOLERANCE) && 
		(std::fabs(v.z - z) < Constants<T>::TOLERANCE) && (std::fabs(v.w - w) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Vector4<T>::isApproxEqual(const Vector4<T>& v, T tolerance) const
{
	return (std::fabs(v.x - x) < tolerance) && (std::fabs(v.y - y) < tolerance) && 
		(std::fabs(v.z - z) < tolerance) && (std::fabs(v.w - w) < tolerance); 
}

template<typename T>
inline void Vector4<T>::setMagnitude(T magnitude) 
{ 
	T m = getMagnitude(); 
	if (m > T(0)) 
		*this *= magnitude/m;
}

template<typename T>
inline Vector4<T>& Vector4<T>::setMinimum(const Vector4<T>& v1, const Vector4<T>& v2)
{ 
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y);
	z = std::min(v1.z, v2.z); 
	w = std::min(v1.w, v2.w); 
	return *this; 
}

template<typename T>
inline Vector4<T>& Vector4<T>::setMaximum(const Vector4<T>& v1, const Vector4<T>& v2)
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
//#if MATHEMATICS_FAST_NORMALIZE
//	if costexpr(std::is_same_v<T, float>)
//	{
//		T m = rcpSqrtApprox(getMagnitudeSquared());
//		if (m <= std::numeric_limits<T>::max())
//			*this *= m;
//	}
//	else
//#endif
	{
		T m = getMagnitude();
		if (m > T(0))
			*this /= m;
	}
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

template<>
struct Vector4<float>
{
	using Real = float;
	using ConstArg = const Vector4;
	using ConstResult = const Vector4;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Vector4() noexcept { xyzw = simd::zero<simd::float4>(); }
	/*constexpr*/ explicit Vector4(float scalar) noexcept { xyzw = simd::set4(scalar); }
	/*constexpr*/ Vector4(float x, float y, float z, float w) noexcept { xyzw = simd::set4(x, y, z, w); }
	/*constexpr*/ Vector4(Vector2<float>::ConstArg v) noexcept { xyzw = simd::insert2(v, UNIT_W); }
	/*constexpr*/ Vector4(Vector2<float>::ConstArg v, float z, float w) noexcept { xyzw = simd::combine2(v, simd::set2(z, w)); }
	/*constexpr*/ Vector4(Vector2<float>::ConstArg xy, Vector2<float>::ConstArg zw) noexcept { xyzw = simd::combine2(xy, zw); }
	/*constexpr*/ Vector4(Vector3<float>::ConstArg v) noexcept { xyzw = simd::insert3(v, UNIT_W); }
	/*constexpr*/ Vector4(Vector3<float>::ConstArg v, float w) noexcept { xyzw = simd::insert<simd::W>(w, v); }
	template<typename U> explicit Vector4(const IntVector4<U>& v) noexcept;
	explicit Vector4(const tuples::templates::Tuple4<float>& t) noexcept { xyzw = simd::set4(t.x, t.y, t.z, t.w); }
	template<typename U> explicit Vector4(const tuples::templates::Tuple4<U>& t) noexcept { xyzw = simd::set4((float)t.x, (float)t.y, (float)t.z, (float)t.w); }
	explicit Vector4(const std::tuple<float, float, float, float>& t) noexcept { xyzw = simd::set4(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t)); }
	template<typename U> explicit Vector4(const std::tuple<U, U, U, U>& t) noexcept { xyzw = simd::set4((float)std::get<0>(t), (float)std::get<1>(t), (float)std::get<2>(t), (float)std::get<3>(t)); }
	explicit Vector4(const float* v) noexcept { xyzw = simd::load4(v); }
	explicit Vector4(Axis axis) noexcept { set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f, (axis == Axis::Z) ? 1.f : 0.f, (axis == Axis::W) ? 1.f : 0.f); }
	explicit Vector4(simd::float4 v) noexcept : xyzw(v) {}

	operator simd::float4() const noexcept { return xyzw; }
	explicit operator tuples::templates::Tuple4<float>() noexcept { return tuples::templates::Tuple4<float>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator std::tuple<float, float, float, float>() noexcept { return std::tuple<float, float, float, float>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() noexcept { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
	explicit operator float*() noexcept { return &x; }
	explicit operator const float*() const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Vector4 operator+() const noexcept { return *this; }
	Vector4 operator-() const noexcept { return Vector4(simd::neg4(xyzw)); }
	Vector4& operator+=(ConstArg v) noexcept { xyzw = simd::add4(xyzw, v); return *this; }
	Vector4& operator-=(ConstArg v) noexcept { xyzw = simd::sub4(xyzw, v); return *this; }
	Vector4& operator*=(ConstArg v) noexcept { xyzw = simd::mul4(xyzw, v); return *this; }
	Vector4& operator*=(float f) noexcept { xyzw = simd::mul4(xyzw, simd::set4(f)); return *this; }
	Vector4& operator/=(ConstArg v) noexcept { xyzw = simd::div4(xyzw, v); return *this; }
	Vector4& operator/=(float f) noexcept { xyzw = simd::div4(xyzw, simd::set4(f)); return *this; }
	Vector4& operator*=(const Matrix4<float>& m) noexcept; // #TODO
	friend Vector4 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector4(simd::add4(v1, v2)); }
	friend Vector4 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector4(simd::sub4(v1, v2)); }
	friend Vector4 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector4(simd::mul4(v1, v2)); }
	friend Vector4 operator*(float f, ConstArg v) noexcept { return Vector4(simd::mul4(simd::set4(f), v)); }
	friend Vector4 operator*(ConstArg v, float f) noexcept { return Vector4(simd::mul4(v, simd::set4(f))); }
	friend Vector4 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector4(simd::div4(v1, v2)); }
	friend Vector4 operator/(float f, ConstArg v) noexcept { return Vector4(simd::div4(simd::set4(f), v)); }
	friend Vector4 operator/(ConstArg v, float f) noexcept { return Vector4(simd::div4(v, simd::set4(f))); }
	friend Vector4 operator*(ConstArg v, const Matrix4<float>& m) noexcept; // #TODO
	//friend Vector4 operator*(const Matrix4<float>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector4& v) const noexcept { return simd::all4(simd::equal(xyzw, v)); }
	bool operator!=(const Vector4& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector4& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector4& v) { return s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; } // #FIXME use simd::set(x, y, z, w)

#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2<float> xy/*getXY*/() const noexcept { return Vector2<float>(simd::swizzle<simd::XYYY>(xyzw)); }
	Vector3<float> xyz/*getXYZ*/() const noexcept { return Vector3<float>(simd::swizzle<simd::XYZZ>(xyzw)); }
#else
	Vector2<float> xy/*getXY*/() const noexcept { return Vector2<float>(simd::cutoff2(xyzw)); }
	Vector3<float> xyz/*getXYZ*/() const noexcept { return Vector3<float>(simd::cutoff3(xyzw)); }
#endif
	//void setXY(Vector2<float>::ConstArg v) noexcept { xyzw = simd::insert2(v, xyzw); }
	//void setXYZ(Vector3<float>::ConstArg v) noexcept { xyzw = simd::insert3(v, xyzw); }
	bool isZero() const noexcept { return simd::all4(simd::equal(xyzw, simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept { simd::all4(simd::lessThan(simd::abs4(xyzw), TOLERANCE)); }
	bool isApproxEqual(const Vector4& v) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, v)), TOLERANCE)); }
	bool isApproxEqual(const Vector4& v, float tolerance) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, v)), simd::set4(tolerance))); }
	bool allLessThan(const Vector4& v) const noexcept { return simd::all4(simd::lessThan(xyzw, v)); }
	bool allLessThanEqual(const Vector4& v) const noexcept { return simd::all4(simd::lessThanEqual(xyzw, v)); }
	bool allGreaterThan(const Vector4& v) const noexcept { return simd::all4(simd::greaterThan(xyzw, v)); }
	bool allGreaterThanEqual(const Vector4& v) const noexcept { return simd::all4(simd::greaterThanEqual(xyzw, v)); }
	bool anyLessThan(const Vector4& v) const noexcept { return simd::any4(simd::lessThan(xyzw, v)); }
	bool anyLessThanEqual(const Vector4& v) const noexcept { return simd::any4(simd::lessThanEqual(xyzw, v)); }
	bool anyGreaterThan(const Vector4& v) const noexcept { return simd::any4(simd::greaterThan(xyzw, v)); }
	bool anyGreaterThanEqual(const Vector4& v) const noexcept { return simd::any4(simd::greaterThanEqual(xyzw, v)); }
	bool isFinite() const { return simd::all4(simd::isFinite(xyzw)); }
	float getMagnitude() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot4(xyzw, xyzw))); }
	float getMagnitudeSquared() const noexcept { return simd::toFloat(simd::dot4(xyzw, xyzw)); }
	void setMagnitude(float magnitude) noexcept;
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(float length) noexcept { setMagnitude(length); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin4(xyzw)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax4(xyzw)); }
	Vector4& setZero() noexcept { xyzw = simd::zero<simd::float4>(); return *this; }
	Vector4& set(float x, float y, float z, float w) noexcept { xyzw = simd::set4(x, y, z, w); return *this; }
	Vector4& setMinimum(const Vector4& v1, const Vector4& v2) noexcept { xyzw = simd::min4(v1, v2); return *this; }
	Vector4& setMaximum(const Vector4& v1, const Vector4& v2) noexcept { xyzw = simd::max4(v1, v2); return *this; }
	Vector4& negate() noexcept { xyzw = simd::neg4(xyzw); return *this; }
	Vector4& normalize() noexcept;
	//Vector4& scale(ConstArg v) noexcept { xyzw = simd::mul4(xyzw, v); return *this; }
	Vector4& transform(const Matrix4<float>& m); // #TODO

	//static const Vector4& getZero() noexcept { return ZERO; }
	//static const Vector4& getUnitX() noexcept { return UNIT_X; }
	//static const Vector4& getUnitY() noexcept { return UNIT_Y; }
	//static const Vector4& getUnitZ() noexcept { return UNIT_Z; }
	//static const Vector4& getUnitW() noexcept { return UNIT_W; }

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
		simd::float4 xyzw;
		struct { float x, y, z, w; };
	};
};

template<>
inline float dot(const Vector4<float>& v1, const Vector4<float>& v2) noexcept
{
	return simd::toFloat(simd::dot4(v1, v2));
}

//template<>
//inline Vector4<float> cross(const Vector4<float>& v1, const Vector4<float>& v2, const Vector4<float>& v3) noexcept; // #TODO

template<>
inline float distance(const Vector4<float>& v1, const Vector4<float>& v2)
{
	auto v = simd::sub4(v2, v1);
	return simd::toFloat(simd::sqrt1(simd::dot4(v, v)));
}

template<>
inline float distanceSquared(const Vector4<float>& v1, const Vector4<float>& v2) noexcept
{
	auto v = simd::sub4(v2, v1);
	return simd::toFloat(simd::dot4(v, v));
}

template<>
inline float length(const Vector4<float>& v)
{
	return simd::toFloat(simd::sqrt1(simd::dot4(v, v)));
}

template<>
inline float lengthSquared(const Vector4<float>& v) noexcept
{
	return simd::toFloat(simd::dot4(v, v));
}

template<>
inline Vector4<float> minimum(const Vector4<float>& v1, const Vector4<float>& v2)
{
	return Vector4<float>(simd::min4(v1, v2));
}

template<>
inline Vector4<float> maximum(const Vector4<float>& v1, const Vector4<float>& v2)
{
	return Vector4<float>(simd::max4(v1, v2));
}

template<>
inline std::istream& operator>>(std::istream& s, Vector4<float>& v)
{ 
	float x, y, z, w; 
	s >> x >> std::skipws >> y >> std::skipws >> z >> std::skipws >> w; 
	v.set(x, y, z, w); 
	return s; 
}

inline void Vector4<float>::setMagnitude(float magnitude)
{ 
	float m = getMagnitude();
	if (m > 0.f) 
		*this *= magnitude/m;
}

inline Vector4<float>& Vector4<float>::normalize()
{
#if MATHEMATICS_FAST_NORMALIZE
	float m = simd::toFloat(simd::rcpSqrtApprox1(simd::dot4(xyzw, xyzw)));
	if (m <= std::numeric_limits<float>::max()) 
		*this *= m;
#else
	float m = getMagnitude(); 
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
	xyzw = simd::set4((float)v.x, (float)v.y, (float)v.z, (float)v.w); 
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates
} // namespace mathematics
} // namespace core

namespace std
{
	template<typename T>
	struct tuple_size<core::mathematics::templates::Vector4<T>> : std::integral_constant<std::size_t, 4> {};
} // namespace std

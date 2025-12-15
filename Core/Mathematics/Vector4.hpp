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
#include "Constants.hpp"
#include "Axis.hpp"
#include "Scalar.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace core {
namespace mathematics {

namespace templates {

template<typename T>
struct Vector4
{
	using Real = T;
//#if SIMD_HAS_FLOAT4
//	using Arg = const Vector4;
//#else
	using Arg = const Vector4&;
//#endif

	static constexpr int NUM_COMPONENTS = 4;

	constexpr Vector4() noexcept : x(), y(), z(), w() {}
	constexpr explicit Vector4(const T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr Vector4(const T x, const T y, const T z, const T w) noexcept : x(x), y(y), z(z), w(w) {}
	constexpr Vector4(Vector2::Arg v) noexcept : x(v.x), y(v.y), z(), w(T(1)) {}
	constexpr Vector4(Vector2::Arg v, const T z, const T w) noexcept : x(v.x), y(v.y), z(z), w(w) {}
	constexpr Vector4(Vector2::Arg xy, Vector2::Arg zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	constexpr Vector4(Vector3::Arg v) noexcept : x(v.x), y(v.y), z(v.z), w(T(1)) {}
	constexpr Vector4(Vector3::Arg v, const T w) noexcept : x(v.x), y(v.y), z(v.z), w(w) {}
	//explicit Vector4(const IntVector4<T>& v) noexcept; // #TODO
	explicit Vector4(const Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)),
		z((axis == Axis::Z) ? T(1) : T(0)), w((axis == Axis::W) ? T(1) : T(0)) {}
	explicit Vector4(const T* const v) noexcept { /*if (v) {*/ x = v[0]; y = v[1]; z = v[2]; w = v[3]; /*} else zero();*/ }
	/*explicit*/ operator T* () noexcept { return &x; }
	/*explicit*/ operator const T* () const noexcept { return &x; }

	Vector4 operator+() const noexcept { return *this; }
	Vector4 operator-() const noexcept { return Vector4(-x, -y, -z, -w); }
	Vector4& operator+=(Arg v) noexcept { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	Vector4& operator-=(Arg v) noexcept { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	Vector4& operator*=(const T f) noexcept { x *= f; y *= f; z *= f; w *= f; return *this; }
	Vector4& operator/=(const T f) noexcept { T s = T(1)/f; x *= s; y *= s; z *= s; w *= s; return *this; }
	//Vector4& operator*=(const Matrix4<T>& m) noexcept; // #TODO
	friend Vector4 operator+(Arg v1, Arg v2) noexcept { return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }
	friend Vector4 operator-(Arg v1, Arg v2) noexcept { return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }
	friend Vector4 operator*(const T f, Arg v) noexcept { return Vector4(f*v.x, f*v.y, f*v.z, f*v.w); }
	friend Vector4 operator*(Arg v, const T f) noexcept { return Vector4(v.x*f, v.y*f, v.z*f, v.w*f); }
	friend Vector4 operator/(const T f, Arg v) noexcept { return Vector4(f/v.x, f/v.y, f/v.z, f/v.w); }
	friend Vector4 operator/(Arg v, const T f) noexcept { T s = T(1)/f; return Vector4(v.x*s, v.y*s, v.z*s, v.w*s); }
	//friend Vector4 operator*(Arg v, const Matrix4<T>& m) noexcept; // #TODO
	////friend Vector4 operator*(const Matrix4<T>& m, Arg v) noexcept; // valid for column vectors only
	bool operator==(Arg v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(Arg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector4& v) { return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z >> std::skipws >> v.w; }
	friend std::ostream& operator<<(std::ostream& s, const Vector4& v) { return s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	//const Vector2& getXY() const noexcept { return *(const Vector2*)this; } // #TODO
	//void setXY(const Vector2& v) noexcept { x = v.x; y = v.y; }
	//const Vector3& getXYZ() const noexcept { return *(const Vector3*)this; }
	//void setXYZ(const Vector3& v) noexcept { x = v.x; y = v.y; z = v.z; }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T()); }
	bool isApproxZero() const noexcept { return (std::fabs(x) < Constants<T>::TOLERANCE) &&
		(std::fabs(y) < Constants<T>::TOLERANCE) && (std::fabs(z) < Constants<T>::TOLERANCE) && (std::fabs(w) < Constants<T>::TOLERANCE); }
	bool isApproxEqualTo(Arg v) const noexcept { return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && 
		(std::fabs(v.y - y) < Constants<T>::TOLERANCE) && (std::fabs(v.z - z) < Constants<T>::TOLERANCE) && (std::fabs(v.w - w) < Constants<T>::TOLERANCE); }
	bool isApproxEqualTo(Arg v, const T tolerance) const noexcept { return (std::fabs(v.x - x) < tolerance) &&
		(std::fabs(v.y - y) < tolerance) && (std::fabs(v.z - z) < tolerance) && (std::fabs(v.w - w) < tolerance); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w); }
	T getMagnitude() const { return std::sqrt(x*x + y*y + z*z + w*w); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y + z*z + w*w); }
	void setMagnitude(const T magnitude) { T m = getMagnitude(); if (m > T(0)) *this *= magnitude/m; }
	T getLength() const { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const T length) { setMagnitude(length); }
	T getMinComponent() const noexcept { return std::min(std::min(std::min(x, y), z), w); }
	T getMaxComponent() const noexcept { return std::max(std::max(std::max(x, y), z), w); }
	Vector4& zero() noexcept { x = T(); y = T(); z = T(); w = T(); return *this; } // setZero()
	Vector4& set(const T x, const T y, const T z, const T w) noexcept { this->x = x; this->y = y; this->z = z; this->w = w; return *this; }
	Vector4& minimumOf(Arg v1, Arg v2) noexcept { x = std::min(v1.x, v2.x); y = std::min(v1.y, v2.y);
		z = std::min(v1.z, v2.z); w = std::min(v1.w, v2.w); return *this; }
	Vector4& maximumOf(Arg v1, Arg v2) noexcept { x = std::max(v1.x, v2.x); y = std::max(v1.y, v2.y);
		z = std::max(v1.z, v2.z); w = std::max(v1.w, v2.w); return *this; }
	Vector4& negate() noexcept { x = -x; y = -y; z = -z; w = -w; return *this; }
#if MATHEMATICS_FAST_NORMALIZE
	Vector4& normalize() noexcept { T m = rcpSqrtApprox(getMagnitudeSquared()); if (m <= std::numeric_limits<T>::max()) *this *= m; return *this; }
#else
	Vector4& normalize() { T m = getMagnitude(); if (m > T(0)) *this /= m; return *this; }
#endif
	Vector4& scale(Arg v) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
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

	float x, y, z, w;
};

#if SIMD_HAS_FLOAT4
template<>
/*_MM_ALIGN16*/ struct Vector4<float>
{
	using Real = float;
	using Arg = const Vector4;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Vector4() noexcept { xyzw = simd::setZero(); }
	/*constexpr*/ explicit Vector4(const float scalar) noexcept { xyzw = simd::set4(scalar); }
	/*constexpr*/ Vector4(const float x, const float y, const float z, const float w) noexcept { xyzw = simd::set(x, y, z, w); }
	/*constexpr*/ Vector4(Vector2::Arg v) noexcept; // #TODO
	/*constexpr*/ Vector4(Vector2::Arg v, const float z, const float w) noexcept; // #TODO
	/*constexpr*/ Vector4(Vector2::Arg xy, Vector2::Arg zw) noexcept; // #TODO
	/*constexpr*/ Vector4(Vector3::Arg v) noexcept; // #TODO
	/*constexpr*/ Vector4(Vector3::Arg v, const float w) noexcept; // #TODO
	//explicit Vector4(const IntVector4<float>& v) noexcept; // #TODO
	explicit Vector4(const Axis axis) noexcept { xyzw = simd::set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f,
		(axis == Axis::Z) ? 1.f : 0.f, (axis == Axis::W) ? 1.f : 0.f); }
	explicit Vector4(const float* const v) noexcept { xyzw = simd::load4(v); }

	explicit Vector4(const simd::Float4 v) noexcept : xyzw(v) {}
	operator simd::Float4() const noexcept { return xyzw; }
	/*explicit*/ operator float* () noexcept { return &x; }
	/*explicit*/ operator const float* () const noexcept { return &x; }

	Vector4 operator+() const noexcept { return *this; }
	Vector4 operator-() const noexcept { return Vector4(simd::neg4(xyzw)); }
	Vector4& operator+=(Arg v) noexcept { xyzw = simd::add4(xyzw, v); return *this; }
	Vector4& operator-=(Arg v) noexcept { xyzw = simd::sub4(xyzw, v); return *this; }
	Vector4& operator*=(const float f) noexcept { xyzw = simd::mul4(xyzw, simd::set4(f)); return *this; }
	Vector4& operator/=(const float f) noexcept { xyzw = simd::mul4(xyzw, simd::set4(1.f/f)); return *this; }
	//Vector4& operator*=(const Matrix4<T>& m) noexcept; // #TODO
	friend Vector4 operator+(Arg v1, Arg v2) noexcept { return Vector4(simd::add4(v1, v2)); }
	friend Vector4 operator-(Arg v1, Arg v2) noexcept { return Vector4(simd::sub4(v1, v2)); }
	friend Vector4 operator*(const float f, Arg v) noexcept { return Vector4(simd::mul4(simd::set4(f), v)); }
	friend Vector4 operator*(Arg v, const float f) noexcept { return Vector4(simd::mul4(v, simd::set4(f))); }
	friend Vector4 operator/(const float f, Arg v) noexcept { return Vector4(simd::div4(simd::set4(f), v)); }
	friend Vector4 operator/(Arg v, const float f) noexcept { return Vector4(simd::mul4(v, simd::set4(1.f/f))); }
	//friend Vector4 operator*(Arg v, const Matrix4<T>& m) noexcept; // #TODO
	////friend Vector4 operator*(const Matrix4<T>& m, Arg v) noexcept; // valid for column vectors only
	bool operator==(Arg v) const noexcept { return simd::all4(simd::equal4(xyzw, v)); }
	bool operator!=(Arg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector4& v) { float x, y, z, w; s >> x >> std::skipws >> y >> std::skipws >> z >> std::skipws >> w;
		v.set(x, y, z); return s; }
	friend std::ostream& operator<<(std::ostream& s, const Vector4& v) { return s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; } // #FIXME use simd::set(x, y, z, w)

	//const Vector2& getXY() const noexcept { return *(const Vector2*)this; } // #TODO
	//void setXY(const Vector2& v) noexcept { x = v.x; y = v.y; }
	//const Vector3& getXYZ() const noexcept { return *(const Vector3*)this; }
	//void setXYZ(const Vector3& v) noexcept { x = v.x; y = v.y; z = v.z; }
	bool isZero() const noexcept { return simd::all4(simd::equal4(xyzw, ZERO)); }
	bool isApproxZero() const noexcept { simd::all4(simd::lessThan4(simd::abs4(xyzw), TOLERANCE)); }
	bool isApproxEqualTo(Arg v) const noexcept { simd::all4(simd::lessThan4(simd::abs4(simd::sub4(xyzw, v)), TOLERANCE)); }
	bool isApproxEqualTo(Arg v, const float tolerance) const noexcept { simd::all4(simd::lessThan4(simd::abs4(simd::sub4(xyzw, v)), 
		simd::set4(tolerance))); }
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w); } // #TODO SSE
	float getMagnitude() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot4(xyzw, xyzw))); }
	float getMagnitudeSquared() const noexcept { return simd::toFloat(simd::dot4(xyzw, xyzw)); }
	void setMagnitude(const float magnitude) noexcept { float m = getMagnitude(); if (m > 0.f) *this *= magnitude/m; }
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(const float length) noexcept { setMagnitude(length); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin4(xyzw)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax4(xyzw)); }
	Vector4& zero() noexcept { xyzw = simd::setZero(); return *this; } // setZero()
	Vector4& set(const float x, const float y, const float z, const float w) noexcept { xyzw = simd::set(x, y, z, w); return *this; }
	Vector4& minimumOf(Arg v1, Arg v2) noexcept { xyzw = simd::min4(v1, v2); return *this; }
	Vector4& maximumOf(Arg v1, Arg v2) noexcept { xyzw = simd::max4(v1, v2); return *this; }
	Vector4& negate() noexcept { xyzw = simd::neg4(xyzw); return *this; }
#if MATHEMATICS_FAST_NORMALIZE
	Vector4& normalize() noexcept { float m = rcpSqrtApprox(getMagnitudeSquared()); if (m <= std::numeric_limits<float>::max()) *this *= m; return *this; }
#else
	Vector4& normalize() noexcept { float m = getMagnitude(); if (m > 0.f) *this /= m; return *this; }
#endif
	Vector4& scale(Arg v) noexcept { xyzw = simd::mul4(xyzw, v); return *this; }
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
	static const Vector4 TOLERANCE;

	union
	{
		simd::Float4 xyzw;
		struct { float x, y, z, w; };
	};
};
#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Vector3f = templates::Vector3<float>;
using Vector3d = templates::Vector3<double>;

#if MATHEMATICS_DOUBLE
using Vector3 = templates::Vector3<double>;
using Vector3Arg = templates::Vector3<double>::Arg;
#else
using Vector3 = templates::Vector3<float>;
using Vector3Arg = templates::Vector3<float>::Arg;
#endif

//template<typename T>
//inline T length(templates::Vector3<T>::Arg v) { return v.getMagnitude(); }
//
//template<typename T>
//inline T lengthSquared(templates::Vector3<T>::Arg v) { return v.getMagnitudeSquared(); }

} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_VECTOR4_HPP */

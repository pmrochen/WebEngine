/*
 *	Name: Quaternion
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <tuple>
#include <Simd/Intrinsics.hpp>
#include <Tuples/Tuple4.hpp>
#include "Constants.hpp"
#include "Scalar.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "YawPitchRoll.hpp"
#include "Euler.hpp"

#ifndef MATHEMATICS_FAST_NORMALIZE
#define MATHEMATICS_FAST_NORMALIZE 1
#endif

namespace core {
namespace mathematics {
namespace templates {

//template<typename T>
//struct Matrix3;

template<typename T>
struct Quaternion
{
	using Real = T;
	using ConstArg = const Quaternion&;
	using ConstResult = const Quaternion&;

	static constexpr int NUM_COMPONENTS = 4;

	constexpr Quaternion() noexcept : x(), y(), z(), w() {}
	constexpr explicit Quaternion(T scalar) noexcept : x(), y(), z(), w(scalar) {}
	constexpr Quaternion(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}
	constexpr explicit Quaternion(Vector3<T>::ConstArg vector) noexcept : x(vector.x), y(vector.y), z(vector.z), w() {}
	constexpr Quaternion(Vector3<T>::ConstArg vector, T scalar) noexcept : x(vector.x), y(vector.y), z(vector.z), w(scalar) {}
	explicit Quaternion(const Matrix3<T>& m);
	explicit Quaternion(const YawPitchRoll<T>& ypr);
	explicit Quaternion(const Euler<T>& e);
	explicit Quaternion(const tuples::templates::Tuple4<T>& t) noexcept : x(t.x), y(t.y), z(t.z), w(t.w) {}
	template<typename U> explicit Quaternion(const tuples::templates::Tuple4<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)), w(T(t.w)) {}
	explicit Quaternion(const std::tuple<T, T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)), w(std::get<3>(t)) {}
	template<typename U> explicit Quaternion(const std::tuple<U, U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))), w(T(std::get<3>(t))) {}
	explicit Quaternion(const T* q) noexcept { x = q[0]; y = q[1]; z = q[2]; w = q[3]; }

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator std::tuple<T, T, T, T>() noexcept { return std::tuple<T, T, T, T>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() noexcept { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	Quaternion operator+() const noexcept { return *this; }
	Quaternion operator-() const noexcept { return Quaternion(-x, -y, -z, -w); }
	Quaternion& operator+=(ConstArg q) noexcept { x += q.x; y += q.y; z += q.z; w += q.w; return *this; }
	Quaternion& operator-=(ConstArg q) noexcept { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return *this; }
	Quaternion& operator*=(ConstArg q) noexcept;
	Quaternion& operator*=(Vector3<T>::ConstArg v) noexcept;
	Quaternion& operator*=(T f) noexcept { x *= f; y *= f; z *= f; w *= f; return *this; }
	//Quaternion& operator/=(ConstArg q) noexcept { *this = *this/q; return *this; }
	Quaternion& operator/=(T f) noexcept { T s = T(1)/f; x *= s; y *= s; z *= s; w *= s; return *this; }
	friend Quaternion operator+(ConstArg q1, ConstArg q2) noexcept { return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w); }
	friend Quaternion operator-(ConstArg q1, ConstArg q2) noexcept { return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w); }
	friend Quaternion operator*(ConstArg q1, ConstArg q2) noexcept;
	friend Quaternion operator*(Vector3<T>::ConstArg v, ConstArg q) noexcept;
	friend Quaternion operator*(ConstArg q, Vector3<T>::ConstArg v) noexcept;
	friend Quaternion operator*(T f, ConstArg q) noexcept { return Quaternion(f*q.x, f*q.y, f*q.z, f*q.w); }
	friend Quaternion operator*(ConstArg q, T f) noexcept { return Quaternion(q.x*f, q.y*f, q.z*f, q.w*f); }
	//friend Quaternion operator/(ConstArg q1, ConstArg q2) noexcept { return q1*(Quaternion(-q2.x, -q2.y, -q2.z, q2.w)/q2.getNorm()); }
	friend Quaternion operator/(T f, ConstArg q) noexcept { return f*(Quaternion(-q.x, -q.y, -q.z, q.w)/q.getNorm()); }
	friend Quaternion operator/(ConstArg q, T f) noexcept { T s = T(1)/f; return Quaternion(q.x*s, q.y*s, q.z*s, q.w*s); }
	bool operator==(const Quaternion& q) const noexcept { return (x == q.x) && (y == q.y) && (z == q.z) && (w == q.w); }
	bool operator!=(const Quaternion& q) const noexcept { return !(*this == q); }
	friend std::istream& operator>>(std::istream& s, Quaternion& q);
	friend std::ostream& operator<<(std::ostream& s, const Quaternion& q) { return s << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T()); }
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T(1)); }
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Quaternion& q) const noexcept;
	bool isApproxEqual(const Quaternion& q, T tolerance) const noexcept;
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w); }
	Vector3<T>::ConstResult /*getImaginary*/getVector() const noexcept { return reinterpret_cast<const Vector3&>(*this); }
	void setVector(Vector3<T>::ConstArg vector) noexcept { x = vector.x; y = vector.y; z = vector.z; }
	T /*getReal*/getScalar() const noexcept { return w; }
	void setScalar(T scalar) noexcept { w = scalar; }
	Vector3<T> getAxis() const;	// quaternion must be normalized (rotation quaternion)
	T getAngle() const;			// quaternion must be normalized (rotation quaternion)
	T getAbsoluteValue() const { return std::sqrt(x*x + y*y + z*z + w*w); }
	T getNorm() const noexcept { return (x*x + y*y + z*z + w*w); }
	T getMagnitude() const { return getAbsoluteValue(); }
	T getMagnitudeSquared() const noexcept { return getNorm(); }
	Quaternion& setZero() noexcept { x = T(); y = T(); z = T(); w = T(); return *this; }
	Quaternion& setIdentity() noexcept { x = T(); y = T(); z = T(); w = T(1); return *this; }
	Quaternion& set(Vector3<T>::ConstArg vector, T scalar) noexcept { x = vector.x; y = vector.y; z = vector.z; w = scalar; return *this; }
	Quaternion& set(T x, T y, T z, T w) noexcept { this->x = x; this->y = y; this->z = z; this->w = w; return *this; }
	Quaternion& setConjugate(ConstArg q) noexcept { x = -q.x; y = -q.y; z = -q.z; w = q.w; return *this; }
	Quaternion& setInverse(ConstArg q) noexcept { *this = Quaternion(-q.x, -q.y, -q.z, q.w)/q.getNorm(); return *this; }
	Quaternion& rotate(ConstArg q) noexcept { *this = q*(*this)*Quaternion(-q.x, -q.y, -q.z, q.w); return *this; }
	Quaternion& preConcatenate(ConstArg q) noexcept;
	Quaternion& concatenate(ConstArg q) noexcept;
	Quaternion& negate() noexcept { x = -x; y = -y; z = -z; w = -w; return *this; }
	Quaternion& normalize();
	Quaternion& conjugate() noexcept { x = -x; y = -y; z = -z; return *this; }
	Quaternion& invert() noexcept { *this = Quaternion(-x, -y, -z, w)/getNorm(); return *this; }
 
	//static const Quaternion& getZero() noexcept { return ZERO; }
	//static const Quaternion& getIdentity() noexcept { return IDENTITY; }
	static Quaternion fromAxisAngle(const Vector3<T>& axis, T angle);

	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static const Quaternion TOLERANCE;

	T x, y, z, w;
};

template<typename T>
inline T dot(const Quaternion<T>& q1, const Quaternion<T>& q2) noexcept
{
	return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

template<typename T>
inline Quaternion<T> normalize(const Quaternion<T>& q)
{
	Quaternion<T> r(q);
	r.normalize();
	return r;
}

//template<typename T>
//inline T angle(const Quaternion<T>& q1, const Quaternion<T>& q1)
//{
//	//T q = q1.getMagnitude()*q2.getMagnitude();
//	//if (q > T(0))
//	//	return std::acos(std::clamp(dot(q1, q2)/q, T(-1), T(1)));
//	//else
//	//	return (dot(q1, q2) >= T(0)) ? T(0) : Constants<T>::PI;
//
//	return std::acos(std::clamp(dot(q1, q2), T(-1), T(1)));
//}

template<typename T>
inline Quaternion<T> arc(const Vector3<T>& v1, const Vector3<T>& v2)
{
	Vector3<T> u(v1);
	Vector3<T> v(v2);
	u.normalize();
	v.normalize();
	Vector3<T> h(u + v);
	h.normalize();
	return Quaternion<T>(cross(u, h), dot(u, h));
}

template<typename T>
inline Quaternion<T> lerp(const Quaternion<T>& q1, const Quaternion<T>& q2, T t) noexcept
{
	// #TODO
}

template<typename T>
inline Quaternion<T> slerp(const Quaternion<T>& q1, const Quaternion<T>& q2, T t) noexcept
{
	// #TODO
}

template<typename T>
inline Vector3<T> rotate(const Vector3<T>& v, const Quaternion<T>& q) noexcept
{
	// #TODO
}

template<typename T>
inline Quaternion<T> rotate(const Quaternion<T>& q1, const Quaternion<T>& q2) noexcept
{
	// #TODO
}

template<typename T>
inline Quaternion<T> concatenate(const Quaternion<T>& q1, const Quaternion<T>& q2) noexcept
{
	// #TODO
}

template<typename T>
inline Quaternion<T> concatenate(const Quaternion<T>& q1, const Quaternion<T>& q2, const Quaternion<T>& q3) noexcept
{
	// #TODO
}

template<typename T>
inline Quaternion<T> concatenate(const Quaternion<T>& q1, const Quaternion<T>& q2, const Quaternion<T>& q3, 
	const Quaternion<T>& q4) noexcept
{
	// #TODO
}

template<typename T>
inline Quaternion<T> conjugate(const Quaternion<T>& q) noexcept
{
	// #TODO
}

template<typename T>
inline Quaternion<T> inverse(const Quaternion<T>& q) noexcept
{
	// #TODO
}

template<typename T>
inline std::istream& operator>>(std::istream& s, Quaternion<T>& q) 
{ 
	return s >> q.x >> std::skipws >> q.y >> std::skipws >> q.z >> std::skipws >> q.w; 
}

template<typename T>
inline bool Quaternion<T>::isApproxZero() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE) && 
		(std::fabs(z) < Constants<T>::TOLERANCE) && (std::fabs(w) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Quaternion<T>::isApproxIdentity() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE) && 
		(std::fabs(z) < Constants<T>::TOLERANCE) && (std::fabs(w - T(1)) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Quaternion<T>::isApproxEqual(const Quaternion<T>& q) const
{ 
	return (std::fabs(q.x - x) < Constants<T>::TOLERANCE) && (std::fabs(q.y - y) < Constants<T>::TOLERANCE) && 
		(std::fabs(q.z - z) < Constants<T>::TOLERANCE) && (std::fabs(q.w - w) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Quaternion<T>::isApproxEqual(const Quaternion<T>& q, T tolerance) const
{
	return (std::fabs(q.x - x) < tolerance) && (std::fabs(q.y - y) < tolerance) && 
		(std::fabs(q.z - z) < tolerance) && (std::fabs(q.w - w) < tolerance); 
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::normalize()
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

template<typename T>
inline Quaternion<T> Quaternion<T>::fromAxisAngle(const Vector3<T>& axis, T angle)
{
	T m = axis.getMagnitude();
	if ((m > T(0)) && (angle != T(0)))
	{
		T half = angle*T(0.5);
		T sinHalfN = std::sin(half)/m;
		return Quaternion<T>(axis.x*sinHalfN, axis.y*sinHalfN, axis.z*sinHalfN, std::cos(half));
	}

	return IDENTITY;
}

template<typename T> const Quaternion<T> Quaternion<T>::ZERO{};
template<typename T> const Quaternion<T> Quaternion<T>::IDENTITY{ T(0), T(0), T(0), T(1) };
template<typename T> const Quaternion<T> Quaternion<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };

#if SIMD_HAS_FLOAT4

template<>
struct Quaternion<float>
{
	using Real = float;
	using ConstArg = const Quaternion;
	using ConstResult = const Quaternion;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Quaternion() noexcept { xyzw = simd::zero<simd::float4>(); }
	/*constexpr*/ explicit Quaternion(float scalar) noexcept { xyzw = simd::insert<simd::W>(scalar, simd::zero<simd::float4>()); }
	/*constexpr*/ Quaternion(float x, float y, float z, float w) noexcept { xyzw = simd::set4(x, y, z, w); }
	/*constexpr*/ explicit Quaternion(Vector3<float>::ConstArg vector) noexcept { xyzw = simd::cutoff3(vector); }
	/*constexpr*/ Quaternion(Vector3<float>::ConstArg vector, float scalar) noexcept { xyzw = simd::insert<simd::W>(scalar, vector); }
	explicit Quaternion(const Matrix3<float>& m);
	explicit Quaternion(const YawPitchRoll<float>& ypr);
	explicit Quaternion(const Euler<float>& e);
	explicit Quaternion(const tuples::templates::Tuple4<float>& t) noexcept { xyzw = simd::set4(t.x, t.y, t.z, t.w); }
	template<typename U> explicit Quaternion(const tuples::templates::Tuple4<U>& t) noexcept { xyzw = simd::set4((float)t.x, (float)t.y, (float)t.z, (float)t.w); }
	explicit Quaternion(const std::tuple<float, float, float, float>& t) noexcept { xyzw = simd::set4(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t)); }
	template<typename U> explicit Quaternion(const std::tuple<U, U, U, U>& t) noexcept { xyzw = simd::set4((float)std::get<0>(t), (float)std::get<1>(t), (float)std::get<2>(t), (float)std::get<3>(t)); }
	explicit Quaternion(const float* q) noexcept { xyzw = simd::load4(q); }
	explicit Quaternion(simd::float4 q) noexcept : xyzw(q) {}

	operator simd::float4() const noexcept { return xyzw; }
	explicit operator tuples::templates::Tuple4<float>() noexcept { return tuples::templates::Tuple4<float>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator std::tuple<float, float, float, float>() noexcept { return std::tuple<float, float, float, float>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() noexcept { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
	explicit operator float*() noexcept { return &x; }
	explicit operator const float*() const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Quaternion operator+() const noexcept { return *this; }
	Quaternion operator-() const noexcept { return Quaternion(simd::neg4(xyzw)); }
	Quaternion& operator+=(ConstArg q) noexcept { xyzw = simd::add4(xyzw, q); return *this; }
	Quaternion& operator-=(ConstArg q) noexcept { xyzw = simd::sub4(xyzw, q); return *this; }
	Quaternion& operator*=(ConstArg q) noexcept;
	Quaternion& operator*=(Vector3<float>::ConstArg v) noexcept;
	Quaternion& operator*=(float f) noexcept { xyzw = simd::mul4(xyzw, simd::set4(f)); return *this; }
	//Quaternion& operator/=(ConstArg q) noexcept { *this = *this/q; return *this; }
	Quaternion& operator/=(float f) noexcept { xyzw = simd::div4(xyzw, simd::set4(f)); return *this; }
	friend Quaternion operator+(ConstArg q1, ConstArg q2) noexcept { return Quaternion(simd::add4(q1, q2)); }
	friend Quaternion operator-(ConstArg q1, ConstArg q2) noexcept { return Quaternion(simd::sub4(q1, q2)); }
	friend Quaternion operator*(ConstArg q1, ConstArg q2) noexcept;
	friend Quaternion operator*(Vector3<float>::ConstArg v, ConstArg q) noexcept;
	friend Quaternion operator*(ConstArg q, Vector3<float>::ConstArg v) noexcept;
	friend Quaternion operator*(float f, ConstArg q) noexcept { return Quaternion(simd::mul4(simd::set4(f), q)); }
	friend Quaternion operator*(ConstArg q, float f) noexcept { return Quaternion(simd::mul4(q, simd::set4(f))); }
	//friend Quaternion operator/(ConstArg q1, ConstArg q2) noexcept { return q1*(Quaternion(simd::neg3(q2))/q2.getNorm()); }
	friend Quaternion operator/(float f, ConstArg q) noexcept { return f*(Quaternion(simd::neg3(q))/q.getNorm()); }
	friend Quaternion operator/(ConstArg q, float f) noexcept { return Quaternion(simd::div4(q, simd::set4(f))); }
	bool operator==(const Quaternion& q) const noexcept { return simd::all4(simd::equal(xyzw, q)); }
	bool operator!=(const Quaternion& q) const noexcept { return !(*this == q); }
	friend std::istream& operator>>(std::istream& s, Quaternion& q);
	friend std::ostream& operator<<(std::ostream& s, const Quaternion& q) { return s << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; } // #FIXME use simd::set(x, y, z, w)

	bool isZero() const noexcept { return simd::all4(simd::equal(xyzw, simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept { simd::all4(simd::lessThan(simd::abs4(xyzw), TOLERANCE)); }
	bool isIdentity() const noexcept { return simd::all4(simd::equal(xyzw, IDENTITY)); }
	bool isApproxIdentity() const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, IDENTITY)), TOLERANCE)); }
	bool isApproxEqual(const Quaternion& q) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, q)), TOLERANCE)); }
	bool isApproxEqual(const Quaternion& q, float tolerance) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, q)), simd::set4(tolerance))); }
	bool isFinite() const { return simd::all4(simd::isFinite(xyzw)); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3<float> /*getImaginary*/getVector() const noexcept { return Vector3<float>(simd::swizzle<simd::XYZZ>(xyzw)); }
#else
	Vector3<float> /*getImaginary*/getVector() const noexcept { return Vector3<float>(simd::cutoff3(xyzw)); }
#endif
	void setVector(Vector3<float>::ConstArg vector) noexcept { xyzw = simd::insert3(vector, xyzw); }
	float /*getReal*/getScalar() const noexcept { return simd::extract<simd::W>(xyzw); }
	void setScalar(float scalar) noexcept { xyzw = simd::insert<simd::W>(scalar, xyzw); }
	Vector3<float> getAxis() const;	// quaternion must be normalized (rotation quaternion)
	float getAngle() const;		// quaternion must be normalized (rotation quaternion)
	float getAbsoluteValue() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot4(xyzw, xyzw))); }
	float getNorm() const noexcept { return simd::toFloat(simd::dot4(xyzw, xyzw)); }
	float getMagnitude() const { return getAbsoluteValue(); }
	float getMagnitudeSquared() const noexcept { return getNorm(); }
	Quaternion& setZero() noexcept { xyzw = simd::zero<simd::float4>(); return *this; }
	Quaternion& setIdentity() noexcept { *this = IDENTITY; return *this; }
	Quaternion& set(Vector3<float>::ConstArg vector, float scalar) noexcept { xyzw = simd::insert<simd::W>(scalar, vector); return *this; }
	Quaternion& set(float x, float y, float z, float w) noexcept { xyzw = simd::set4(x, y, z, w); return *this; }
	Quaternion& setConjugate(ConstArg q) noexcept { xyzw = simd::neg3(q); return *this; }
	Quaternion& setInverse(ConstArg q) noexcept { *this = Quaternion(simd::neg3(q))/q.getNorm(); return *this; }
	Quaternion& rotate(ConstArg q) noexcept;
	Quaternion& preConcatenate(ConstArg q) noexcept;
	Quaternion& concatenate(ConstArg q) noexcept;
	Quaternion& negate() noexcept { xyzw = simd::neg4(xyzw); return *this; }
	Quaternion& normalize() noexcept;
	Quaternion& conjugate() noexcept { xyzw = simd::neg3(xyzw); return *this; }
	Quaternion& invert() noexcept { *this = Quaternion(simd::neg3(xyzw))/getNorm(); return *this; }

	//static const Quaternion& getZero() noexcept { return ZERO; }
	//static const Quaternion& getIdentity() noexcept { return IDENTITY; }
	static Quaternion fromAxisAngle(const Vector3<float>& axis, float angle);

	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static const Quaternion TOLERANCE;

	union
	{
		simd::float4 xyzw;
		struct { float x, y, z, w; };
	};
};

template<>
inline float dot(const Quaternion<float>& v1, const Quaternion<float>& v2) noexcept
{
	return simd::toFloat(simd::dot4(v1, v2));
}

template<>
inline std::istream& operator>>(std::istream& s, Quaternion<float>& q)
{ 
	float x, y, z, w; 
	s >> x >> std::skipws >> y >> std::skipws >> z >> std::skipws >> w; 
	q.set(x, y, z, w); 
	return s; 
}

inline Quaternion<float>& Quaternion<float>::normalize()
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

inline Quaternion<float> Quaternion<float>::fromAxisAngle(const Vector3<float>& axis, float angle)
{
	float m = axis.getMagnitude();
	if ((m > 0.f) && (angle != 0.f))
	{
		float half = angle*0.5f;
		float sinHalfN = std::sin(half)/m;
		return Quaternion<float>(axis*sinHalfN, std::cos(half));
	}

	return IDENTITY;
}

const Quaternion<float> Quaternion<float>::ZERO{};
const Quaternion<float> Quaternion<float>::IDENTITY{ 0.f, 0.f, 0.f, 1.f };
const Quaternion<float> Quaternion<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

#if MATHEMATICS_DOUBLE
using Quaternion = templates::Quaternion<double>;
using QuaternionArg = templates::Quaternion<double>::ConstArg;
using QuaternionResult = templates::Quaternion<double>::ConstResult;
#else
using Quaternion = templates::Quaternion<float>;
using QuaternionArg = templates::Quaternion<float>::ConstArg;
using QuaternionResult = templates::Quaternion<float>::ConstResult;
#endif

} // namespace mathematics
} // namespace core

namespace std
{

template<typename T>
struct tuple_size<core::mathematics::templates::Quaternion<T>> : std::integral_constant<std::size_t, 4> {};

template<std::size_t I, typename T>
inline T& get(core::mathematics::templates::Quaternion<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	else if constexpr (I == 3)
		return v.w;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const core::mathematics::templates::Quaternion<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	else if constexpr (I == 3)
		return v.w;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(core::mathematics::templates::Quaternion<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	else if constexpr (I == 3)
		return v.w;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const core::mathematics::templates::Quaternion<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	else if constexpr (I == 3)
		return v.w;
	static_assert(false);
}

} // namespace std

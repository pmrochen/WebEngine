/*
 *	Name: Quaternion
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_QUATERNION_HPP
#define CORE_MATHEMATICS_QUATERNION_HPP

#include <cmath>
#include <istream>
#include <ostream>
#include <limits>
#include <Simd.hpp>
#include <Tuples.hpp>
#include "Constants.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "YawPitchRoll.hpp"
#include "Euler.hpp"

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
	constexpr explicit Quaternion(const T scalar) noexcept : x(), y(), z(), w(scalar) {}
	constexpr Quaternion(const T x, const T y, const T z, const T w) noexcept : x(x), y(y), z(z), w(w) {}
	constexpr explicit Quaternion(Vector3<T>::ConstArg vector) noexcept : x(vector.x), y(vector.y), z(vector.z), w() {}
	//constexpr Quaternion(Vector3<T>::ConstArg vector, const T scalar) noexcept : x(vector.x), y(vector.y), z(vector.z), w(scalar) {}
	Quaternion(Vector3<T>::ConstArg axis, float angle);
	explicit Quaternion(Matrix3<T>::ConstArg m);
	explicit Quaternion(YawPitchRoll<T>::ConstArg ypr);
	explicit Quaternion(Euler<T>::ConstArg e);
	explicit Quaternion(const tuples::templates::Tuple4<T>& t) noexcept : x(t.x), y(t.y), z(t.z), w(t.w) {}
	template<typename U> explicit Quaternion(const tuples::templates::Tuple4<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)), w(T(t.w)) {}
	explicit Quaternion(const T* const q) noexcept { x = q[0]; y = q[1]; z = q[2]; w = q[3]; }

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
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
	Quaternion& operator*=(const T f) noexcept { x *= f; y *= f; z *= f; w *= f; return *this; }
	//Quaternion& operator/=(ConstArg q) noexcept { *this = *this/q; return *this; }
	Quaternion& operator/=(const T f) noexcept { const T s = T(1)/f; x *= s; y *= s; z *= s; w *= s; return *this; }
	friend Quaternion operator+(ConstArg q1, ConstArg q2) noexcept { return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w); }
	friend Quaternion operator-(ConstArg q1, ConstArg q2) noexcept { return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w); }
	friend Quaternion operator*(ConstArg q1, ConstArg q2) noexcept;
	friend Quaternion operator*(Vector3<T>::ConstArg v, ConstArg q) noexcept;
	friend Quaternion operator*(ConstArg q, Vector3<T>::ConstArg v) noexcept;
	friend Quaternion operator*(const T f, ConstArg q) noexcept { return Quaternion(f*q.x, f*q.y, f*q.z, f*q.w); }
	friend Quaternion operator*(ConstArg q, const T f) noexcept { return Quaternion(q.x*f, q.y*f, q.z*f, q.w*f); }
	//friend Quaternion operator/(ConstArg q1, ConstArg q2) noexcept { return q1*(Quaternion(-q2.x, -q2.y, -q2.z, q2.w)/q2.getNorm()); }
	friend Quaternion operator/(const T f, ConstArg q) noexcept { return f*(Quaternion(-q.x, -q.y, -q.z, q.w)/q.getNorm()); }
	friend Quaternion operator/(ConstArg q, const T f) noexcept { const T s = T(1)/f; return Quaternion(q.x*s, q.y*s, q.z*s, q.w*s); }
	bool operator==(const Quaternion& q) const noexcept { return (x == q.x) && (y == q.y) && (z == q.z) && (w == q.w); }
	bool operator!=(const Quaternion& q) const noexcept { return !(*this == q); }
	friend std::istream& operator>>(std::istream& s, Quaternion& q) { return s >> q.x >> std::skipws >> q.y >> std::skipws >> q.z >> std::skipws >> q.w; }
	friend std::ostream& operator<<(std::ostream& s, const Quaternion& q) { return s << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T()); }
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T(1)); }
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Quaternion& q) const noexcept;
	bool isApproxEqual(const Quaternion& q, const T tolerance) const noexcept;
	bool isFinite() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w); }
	Vector3<T>::ConstResult /*getImaginary*/getVector() const noexcept { return *reinterpret_cast<const Vector3*>(this); }
	void setVector(Vector3<T>::ConstArg vector) noexcept { x = vector.x; y = vector.y; z = vector.z; }
	T /*getReal*/getScalar() const noexcept { return w; }
	void setScalar(const T scalar) noexcept { w = scalar; }
	Vector3 getAxis() const;	// quaternion must be normalized (rotation quaternion)
	T getAngle() const;			// quaternion must be normalized (rotation quaternion)
	T getAbsoluteValue() const { return std::sqrt(x*x + y*y + z*z + w*w); }
	T getNorm() const noexcept { return (x*x + y*y + z*z + w*w); }
	T getMagnitude() const { return getAbsoluteValue(); }
	T getMagnitudeSquared() const noexcept { return getNorm(); }
	Quaternion& setZero() noexcept { x = T(); y = T(); z = T(); w = T(); return *this; }
	Quaternion& setIdentity() noexcept { x = T(); y = T(); z = T(); w = T(1); return *this; }
	Quaternion& set(Vector3<T>::ConstArg vector, const T scalar) noexcept { x = vector.x; y = vector.y; z = vector.z; w = scalar; return *this; }
	Quaternion& set(const T x, const T y, const T z, const T w) noexcept { this->x = x; this->y = y; this->z = z; this->w = w; return *this; }
	Quaternion& setConjugate(ConstArg q) noexcept { x = -q.x; y = -q.y; z = -q.z; w = q.w; return *this; }
	Quaternion& setInverse(ConstArg q) noexcept { *this = Quaternion(-q.x, -q.y, -q.z, q.w)/q.getNorm(); return *this; }
	Quaternion& rotate(ConstArg q) noexcept { *this = q*(*this)*Quaternion(-q.x, -q.y, -q.z, q.w); return *this; }
	Quaternion& preConcatenate(ConstArg q) noexcept;
	Quaternion& concatenate(ConstArg q) noexcept;
	Quaternion& negate() noexcept { x = -x; y = -y; z = -z; w = -w; return *this; }
	Quaternion& normalize() noexcept;
	Quaternion& conjugate() noexcept { x = -x; y = -y; z = -z; return *this; }
	Quaternion& invert() noexcept { *this = Quaternion(-x, -y, -z, w)/getNorm(); return *this; }
 
	static const Quaternion& getZero() noexcept { return ZERO; }
	static const Quaternion& getIdentity() noexcept { return IDENTITY; }

	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static const Quaternion TOLERANCE;

	T x, y, z, w;
};

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
inline bool Quaternion<T>::isApproxEqual(ConstArg q) const
{ 
	return (std::fabs(q.x - x) < Constants<T>::TOLERANCE) && (std::fabs(q.y - y) < Constants<T>::TOLERANCE) && 
		(std::fabs(q.z - z) < Constants<T>::TOLERANCE) && (std::fabs(q.w - w) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool Quaternion<T>::isApproxEqual(ConstArg q, const T tolerance) const
{
	return (std::fabs(q.x - x) < tolerance) && (std::fabs(q.y - y) < tolerance) && 
		(std::fabs(q.z - z) < tolerance) && (std::fabs(q.w - w) < tolerance); 
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::normalize()
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

template<typename T> const Quaternion<T> Quaternion<T>::ZERO{};
template<typename T> const Quaternion<T> Quaternion<T>::IDENTITY{ T(0), T(0), T(0), T(1) };
template<typename T> const Quaternion<T> Quaternion<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };

#if SIMD_HAS_FLOAT4

namespace float4 = simd::float4;

template<>
struct Quaternion<float>
{
	using Real = float;
	using ConstArg = const Quaternion;
	using ConstResult = const Quaternion;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Quaternion() noexcept { xyzw = float4::zero(); }
	/*constexpr*/ explicit Quaternion(const float scalar) noexcept { xyzw = float4::insert<float4::W>(scalar, float4::zero()); }
	/*constexpr*/ Quaternion(const float x, const float y, const float z, const float w) noexcept { xyzw = float4::set4(x, y, z, w); }
	/*constexpr*/ explicit Quaternion(Vector3<float>::ConstArg vector) noexcept { xyzw = float4::cutoff3(vector); }
	// /*constexpr*/ Quaternion(Vector3<float>::ConstArg vector, float scalar) noexcept { xyzw = float4::insert<float4::W>(scalar, vector); }
	Quaternion(Vector3<T>::ConstArg axis, float angle);
	explicit Quaternion(Matrix3<float>::ConstArg m);
	explicit Quaternion(YawPitchRoll<float>::ConstArg ypr);
	explicit Quaternion(Euler<float>::ConstArg e);
	explicit Quaternion(const tuples::templates::Tuple4<float>& t) noexcept { xyzw = float4::set4(t.x, t.y, t.z, t.w); }
	template<typename U> explicit Quaternion(const tuples::templates::Tuple4<U>& t) noexcept { xyzw = float4::set4((float)t.x, (float)t.y, (float)t.z, (float)t.w); }
	explicit Quaternion(const float* const q) noexcept { xyzw = float4::load4(q); }
	explicit Quaternion(const float4::Type q) noexcept : xyzw(q) {}

	operator float4::Type() const noexcept { return xyzw; }
	explicit operator tuples::templates::Tuple4<float>() noexcept { return tuples::templates::Tuple4<float>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator float*() noexcept { return &x; }
	explicit operator const float*() const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Quaternion operator+() const noexcept { return *this; }
	Quaternion operator-() const noexcept { return Quaternion(float4::neg4(xyzw)); }
	Quaternion& operator+=(ConstArg q) noexcept { xyzw = float4::add4(xyzw, q); return *this; }
	Quaternion& operator-=(ConstArg q) noexcept { xyzw = float4::sub4(xyzw, q); return *this; }
	Quaternion& operator*=(ConstArg q) noexcept;
	Quaternion& operator*=(Vector3<float>::ConstArg v) noexcept;
	Quaternion& operator*=(const float f) noexcept { xyzw = float4::mul4(xyzw, float4::set4(f)); return *this; }
	//Quaternion& operator/=(ConstArg q) noexcept { *this = *this/q; return *this; }
	Quaternion& operator/=(const float f) noexcept { xyzw = float4::div4(xyzw, float4::set4(f)); return *this; }
	friend Quaternion operator+(ConstArg q1, ConstArg q2) noexcept { return Quaternion(float4::add4(q1, q2)); }
	friend Quaternion operator-(ConstArg q1, ConstArg q2) noexcept { return Quaternion(float4::sub4(q1, q2)); }
	friend Quaternion operator*(ConstArg q1, ConstArg q2) noexcept;
	friend Quaternion operator*(Vector3<float>::ConstArg v, ConstArg q) noexcept;
	friend Quaternion operator*(ConstArg q, Vector3<float>::ConstArg v) noexcept;
	friend Quaternion operator*(const float f, ConstArg q) noexcept { return Quaternion(float4::mul4(float4::set4(f), q)); }
	friend Quaternion operator*(ConstArg q, const float f) noexcept { return Quaternion(float4::mul4(q, float4::set4(f))); }
	//friend Quaternion operator/(ConstArg q1, ConstArg q2) noexcept { return q1*(Quaternion(float4::neg3(q2))/q2.getNorm()); }
	friend Quaternion operator/(const float f, ConstArg q) noexcept { return f*(Quaternion(float4::neg3(q))/q.getNorm()); }
	friend Quaternion operator/(ConstArg q, const float f) noexcept { return Quaternion(float4::div4(q, float4::set4(f))); }
	bool operator==(const Quaternion& q) const noexcept { return float4::all4(float4::equal(xyzw, q)); }
	bool operator!=(const Quaternion& q) const noexcept { return !(*this == q); }
	friend std::istream& operator>>(std::istream& s, Quaternion& q);
	friend std::ostream& operator<<(std::ostream& s, const Quaternion& q) { return s << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; } // #FIXME use float4::set(x, y, z, w)

	bool isZero() const noexcept { return float4::all4(float4::equal(xyzw, float4::zero())); }
	bool isApproxZero() const noexcept { float4::all4(float4::lessThan(float4::abs4(xyzw), TOLERANCE)); }
	bool isIdentity() const noexcept { return float4::all4(float4::equal(xyzw, IDENTITY)); }
	bool isApproxIdentity() const noexcept { float4::all4(float4::lessThan(float4::abs4(float4::sub4(xyzw, IDENTITY)), TOLERANCE)); }
	bool isApproxEqual(const Quaternion& q) const noexcept { float4::all4(float4::lessThan(float4::abs4(float4::sub4(xyzw, q)), TOLERANCE)); }
	bool isApproxEqual(const Quaternion& q, const float tolerance) const noexcept { float4::all4(float4::lessThan(float4::abs4(float4::sub4(xyzw, q)), float4::set4(tolerance))); }
	bool isFinite() const { return float4::all4(float4::isFinite(xyzw)); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3<float> /*getImaginary*/getVector() const noexcept { return Vector3<float>(float4::swizzle<float4::XYZZ>(xyzw)); }
#else
	Vector3<float> /*getImaginary*/getVector() const noexcept { return Vector3<float>(float4::cutoff3(xyzw)); }
#endif
	void setVector(Vector3<float>::ConstArg vector) noexcept { xyzw = float4::insert3(vector, xyzw); }
	float /*getReal*/getScalar() const noexcept { return float4::extract<float4::W>(xyzw); }
	void setScalar(const float scalar) noexcept { xyzw = float4::insert<float4::W>(scalar, xyzw); }
	Vector3 getAxis() const;	// quaternion must be normalized (rotation quaternion)
	float getAngle() const;		// quaternion must be normalized (rotation quaternion)
	float getAbsoluteValue() const noexcept { return float4::toFloat(float4::sqrt1(float4::dot4(xyzw, xyzw))); }
	float getNorm() const noexcept { return float4::toFloat(float4::dot4(xyzw, xyzw)); }
	float getMagnitude() const { return getAbsoluteValue(); }
	float getMagnitudeSquared() const noexcept { return getNorm(); }
	Quaternion& setZero() noexcept { xyzw = float4::zero(); return *this; }
	Quaternion& setIdentity() noexcept { *this = IDENTITY; return *this; }
	Quaternion& set(Vector3<T>::ConstArg vector, const float scalar) noexcept { xyzw = float4::insert<float4::W>(scalar, vector); return *this; }
	Quaternion& set(const float x, const float y, const float z, const float w) noexcept { xyzw = float4::set4(x, y, z, w); return *this; }
	Quaternion& setConjugate(ConstArg q) noexcept { xyzw = float4::neg3(q); return *this; }
	Quaternion& setInverse(ConstArg q) noexcept { *this = Quaternion(float4::neg3(q))/q.getNorm(); return *this; }
	Quaternion& rotate(ConstArg q) noexcept;
	Quaternion& preConcatenate(ConstArg q) noexcept;
	Quaternion& concatenate(ConstArg q) noexcept;
	Quaternion& negate() noexcept { xyzw = float4::neg4(xyzw); return *this; }
	Quaternion& normalize() noexcept;
	Quaternion& conjugate() noexcept { xyzw = float4::neg3(xyzw); return *this; }
	Quaternion& invert() noexcept { *this = Quaternion(float4::neg3(xyzw))/getNorm(); return *this; }

	static const Quaternion& getZero() noexcept { return ZERO; }
	static const Quaternion& getIdentity() noexcept { return IDENTITY; }

	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static const Quaternion TOLERANCE;

	union
	{
		float4::Type xyzw;
		struct { float x, y, z, w; };
	};
};

inline std::istream& Quaternion<float>::operator>>(std::istream& s, Quaternion<float>& q)
{ 
	float x, y, z, w; 
	s >> x >> std::skipws >> y >> std::skipws >> z >> std::skipws >> w; 
	q.set(x, y, z, w); 
	return s; 
}

inline Quaternion<float>& Quaternion<float>::normalize()
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

#endif /* CORE_MATHEMATICS_QUATERNION_HPP */

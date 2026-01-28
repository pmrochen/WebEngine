/*
 *	Name: Quaternion
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <tuple>
#include <cstddef>
#include <cmath>
#include <Simd/Intrinsics.hpp>
#include <Tuples/Tuple4.hpp>
#include "Constants.hpp"
#include "Scalar.hpp"
#include "Vector3.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct Matrix3;

template<typename T>
struct YawPitchRoll;

template<typename T>
struct Euler;

template<typename T>
struct Quaternion
{
	using Real = T;
	using ConstArg = const Quaternion&;
	using ConstResult = const Quaternion&;

	static constexpr int NUM_COMPONENTS = 4;

	constexpr Quaternion() noexcept : x(), y(), z(), w() {}
	explicit Quaternion(Uninitialized) noexcept {}
	//explicit Quaternion(Identity) noexcept : x(), y(), z(), w(1) {}
	constexpr explicit Quaternion(T scalar) noexcept : x(), y(), z(), w(scalar) {}
	constexpr Quaternion(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}
	constexpr explicit Quaternion(Vector3<T>::ConstArg vector) noexcept : x(vector.x), y(vector.y), z(vector.z), w() {}
	constexpr Quaternion(Vector3<T>::ConstArg vector, T scalar) noexcept : x(vector.x), y(vector.y), z(vector.z), w(scalar) {}
	explicit Quaternion(const Matrix3<T>& m) noexcept;
	explicit Quaternion(const YawPitchRoll<T>& ypr) noexcept;
	explicit Quaternion(const Euler<T>& e) noexcept;
	explicit Quaternion(const tuples::templates::Tuple4<T>& t) noexcept : x(t.x), y(t.y), z(t.z), w(t.w) {}
	template<typename U> explicit Quaternion(const tuples::templates::Tuple4<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)), w(T(t.w)) {}
	explicit Quaternion(const std::tuple<T, T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)), w(std::get<3>(t)) {}
	template<typename U> explicit Quaternion(const std::tuple<U, U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))), w(T(std::get<3>(t))) {}
	explicit Quaternion(const T* q) noexcept : x(q[0]), y(q[1]), z(q[2]), w(q[3]) {}

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator std::tuple<T, T, T, T>() { return std::tuple<T, T, T, T>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
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
	Quaternion& operator/=(T f) noexcept { return operator*=(T(1)/f); }
	friend Quaternion operator+(ConstArg q1, ConstArg q2) noexcept { return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w); }
	friend Quaternion operator-(ConstArg q1, ConstArg q2) noexcept { return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w); }
	friend Quaternion operator*(ConstArg q1, ConstArg q2) noexcept;
	friend Quaternion operator*(Vector3<T>::ConstArg v, ConstArg q) noexcept;
	friend Quaternion operator*(ConstArg q, Vector3<T>::ConstArg v) noexcept;
	friend Quaternion operator*(T f, ConstArg q) noexcept { return Quaternion(f*q.x, f*q.y, f*q.z, f*q.w); }
	friend Quaternion operator*(ConstArg q, T f) noexcept { return Quaternion(q.x*f, q.y*f, q.z*f, q.w*f); }
	//friend Quaternion operator/(ConstArg q1, ConstArg q2) noexcept { return q1*(Quaternion(-q2.x, -q2.y, -q2.z, q2.w)/q2.getNorm()); }
	friend Quaternion operator/(T f, ConstArg q) noexcept { return f*(Quaternion(-q.x, -q.y, -q.z, q.w)/q.getNorm()); }
	friend Quaternion operator/(ConstArg q, T f) noexcept { return operator*(q, T(1)/f); }
	bool operator==(const Quaternion& q) const noexcept { return (x == q.x) && (y == q.y) && (z == q.z) && (w == q.w); }
	bool operator!=(const Quaternion& q) const noexcept { return !(*this == q); }
	friend std::istream& operator>>(std::istream& s, Quaternion& q);
	friend std::ostream& operator<<(std::ostream& s, const Quaternion& q) { return s << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	static Quaternion fromAxisAngle(const Vector3<T>& axis, T angle) noexcept;

	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T()); }
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T(1)); }
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Quaternion& q) const noexcept;
	bool isApproxEqual(const Quaternion& q, T tolerance) const noexcept;
	bool isFinite() const noexcept { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w); }
	Vector3<T>::ConstResult /*getImaginary*/getVector() const noexcept { return reinterpret_cast<const Vector3<T>&>(*this); }
	void setVector(Vector3<T>::ConstArg vector) noexcept { x = vector.x; y = vector.y; z = vector.z; }
	T /*getReal*/getScalar() const noexcept { return w; }
	void setScalar(T scalar) noexcept { w = scalar; }
	Vector3<T> getAxis() const noexcept;	// quaternion must be normalized (rotation quaternion)
	T getAngle() const;						// quaternion must be normalized (rotation quaternion)
	T getAbsoluteValue() const noexcept { return std::sqrt(x*x + y*y + z*z + w*w); }
	T getNorm() const noexcept { return (x*x + y*y + z*z + w*w); }
	T getMagnitude() const noexcept { return getAbsoluteValue(); }
	T getMagnitudeSquared() const noexcept { return getNorm(); }
	Quaternion& setZero/*zero*/() noexcept { x = T(); y = T(); z = T(); w = T(); return *this; }
	Quaternion& setIdentity/*makeIdentity*/() noexcept { x = T(); y = T(); z = T(); w = T(1); return *this; }
	Quaternion& set(Vector3<T>::ConstArg vector, T scalar) noexcept { x = vector.x; y = vector.y; z = vector.z; w = scalar; return *this; }
	Quaternion& set(T x, T y, T z, T w) noexcept { this->x = x; this->y = y; this->z = z; this->w = w; return *this; }
	Quaternion& setConjugate/*conjugateOf*/(ConstArg q) noexcept { x = -q.x; y = -q.y; z = -q.z; w = q.w; return *this; }
	Quaternion& setInverse/*inverseOf*/(ConstArg q) noexcept { *this = Quaternion(-q.x, -q.y, -q.z, q.w)/q.getNorm(); return *this; }
	Quaternion& conjugate() noexcept { x = -x; y = -y; z = -z; return *this; }
	Quaternion& invert() noexcept { *this = Quaternion(-x, -y, -z, w)/getNorm(); return *this; }
	Quaternion& rotate(ConstArg q) noexcept { *this = q*(*this)*Quaternion(-q.x, -q.y, -q.z, q.w); return *this; }
	Quaternion& preConcatenate(ConstArg q) noexcept { *this = (*this)*q; return *this; }
	Quaternion& concatenate(ConstArg q) noexcept { *this = q*(*this); return *this; }
	Quaternion& negate() noexcept { x = -x; y = -y; z = -z; w = -w; return *this; }
	Quaternion& normalize() noexcept;
 
	//static const Quaternion& getZero() noexcept { return ZERO; }
	//static const Quaternion& getIdentity() noexcept { return IDENTITY; }

	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static const Quaternion TOLERANCE;

	T x, y, z, w;
};

template<typename T> const Quaternion<T> Quaternion<T>::ZERO{};
template<typename T> const Quaternion<T> Quaternion<T>::IDENTITY{ T(0), T(0), T(0), T(1) };
template<typename T> const Quaternion<T> Quaternion<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };

#if SIMD_HAS_FLOAT4

template<>
struct Matrix3<float>;

template<>
struct YawPitchRoll<float>;

template<>
struct Euler<float>;

template<>
struct Quaternion<float>
{
	using Real = float;
	using ConstArg = const Quaternion;
	using ConstResult = const Quaternion;

	static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Quaternion() noexcept : xyzw(simd::zero<simd::float4>()) {}
	explicit Quaternion(Uninitialized) noexcept {}
	//explicit Quaternion(Identity) noexcept : xyzw(IDENTITY) {}
	/*constexpr*/ explicit Quaternion(float scalar) noexcept : xyzw(simd::insert<simd::W>(scalar, simd::zero<simd::float4>())) {}
	/*constexpr*/ Quaternion(float x, float y, float z, float w) noexcept : xyzw(simd::set4(x, y, z, w)) {}
	/*constexpr*/ explicit Quaternion(Vector3<float>::ConstArg vector) noexcept : xyzw(simd::cutoff3(vector)) {}
	/*constexpr*/ Quaternion(Vector3<float>::ConstArg vector, float scalar) noexcept : xyzw(simd::insert<simd::W>(scalar, vector)) {}
	explicit Quaternion(const Matrix3<float>& m) noexcept;
	explicit Quaternion(const YawPitchRoll<float>& ypr) noexcept;
	explicit Quaternion(const Euler<float>& e) noexcept;
	explicit Quaternion(const tuples::templates::Tuple4<float>& t) noexcept : xyzw(simd::set4(t.x, t.y, t.z, t.w)) {}
	template<typename U> explicit Quaternion(const tuples::templates::Tuple4<U>& t) noexcept : xyzw(simd::set4((float)t.x, (float)t.y, (float)t.z, (float)t.w)) {}
	explicit Quaternion(const std::tuple<float, float, float, float>& t) noexcept : xyzw(simd::set4(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t))) {}
	template<typename U> explicit Quaternion(const std::tuple<U, U, U, U>& t) noexcept : xyzw(simd::set4((float)std::get<0>(t), (float)std::get<1>(t), (float)std::get<2>(t), (float)std::get<3>(t))) {}
	explicit Quaternion(const float* q) noexcept : xyzw(simd::load4(q)) {}
	explicit Quaternion(simd::float4 q) noexcept : xyzw(q) {}
	Quaternion(const Quaternion& q) noexcept : xyzw(q.xyzw) {}
	Quaternion& operator=(const Quaternion& q) noexcept { xyzw = q.xyzw; return *this; }

	operator simd::float4() const noexcept { return xyzw; }
	explicit operator tuples::templates::Tuple4<float>() noexcept { return tuples::templates::Tuple4<float>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator std::tuple<float, float, float, float>() { return std::tuple<float, float, float, float>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
	explicit operator float* () noexcept { return &x; }
	explicit operator const float* () const noexcept { return &x; }
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

	template<std::size_t I> float& get() noexcept;
	template<std::size_t I> const float& get() const noexcept;
	template<typename U> U& get() noexcept;				// intentionally undefined
	template<typename U> const U& get() const noexcept;	// intentionally undefined
	template<> simd::float4& get() noexcept { return xyzw; }
	template<> const simd::float4& get() const noexcept { return xyzw; }

	static Quaternion fromAxisAngle(const Vector3<float>& axis, float angle) noexcept;

	bool isZero() const noexcept { return simd::all4(simd::equal(xyzw, simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept { simd::all4(simd::lessThan(simd::abs4(xyzw), TOLERANCE)); }
	bool isIdentity() const noexcept { return simd::all4(simd::equal(xyzw, IDENTITY)); }
	bool isApproxIdentity() const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, IDENTITY)), TOLERANCE)); }
	bool isApproxEqual(const Quaternion& q) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, q)), TOLERANCE)); }
	bool isApproxEqual(const Quaternion& q, float tolerance) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(xyzw, q)), simd::set4(tolerance))); }
	bool isFinite() const noexcept { return simd::all4(simd::isFinite(xyzw)); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3<float> /*getImaginary*/getVector() const noexcept { return Vector3<float>(simd::swizzle<simd::XYZZ>(xyzw)); }
#else
	Vector3<float> /*getImaginary*/getVector() const noexcept { return Vector3<float>(simd::cutoff3(xyzw)); }
#endif
	void setVector(Vector3<float>::ConstArg vector) noexcept { xyzw = simd::insert3(vector, xyzw); }
	float /*getReal*/getScalar() const noexcept { return simd::extract<simd::W>(xyzw); }
	void setScalar(float scalar) noexcept { xyzw = simd::insert<simd::W>(scalar, xyzw); }
	Vector3<float> getAxis() const noexcept;	// quaternion must be normalized (rotation quaternion)
	float getAngle() const;						// quaternion must be normalized (rotation quaternion)
	float getAbsoluteValue() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot4(xyzw, xyzw))); }
	float getNorm() const noexcept { return simd::toFloat(simd::dot4(xyzw, xyzw)); }
	float getMagnitude() const noexcept { return getAbsoluteValue(); }
	float getMagnitudeSquared() const noexcept { return getNorm(); }
	Quaternion& setZero/*zero*/() noexcept { xyzw = simd::zero<simd::float4>(); return *this; }
	Quaternion& setIdentity/*makeIdentity*/() noexcept { *this = IDENTITY; return *this; }
	Quaternion& set(Vector3<float>::ConstArg vector, float scalar) noexcept { xyzw = simd::insert<simd::W>(scalar, vector); return *this; }
	Quaternion& set(float x, float y, float z, float w) noexcept { xyzw = simd::set4(x, y, z, w); return *this; }
	Quaternion& setConjugate/*conjugateOf*/(ConstArg q) noexcept { xyzw = simd::neg3(q); return *this; }
	Quaternion& setInverse/*inverseOf*/(ConstArg q) noexcept { *this = Quaternion(simd::neg3(q))/q.getNorm(); return *this; }
	Quaternion& conjugate() noexcept { xyzw = simd::neg3(xyzw); return *this; }
	Quaternion& invert() noexcept { *this = Quaternion(simd::neg3(xyzw))/getNorm(); return *this; }
	Quaternion& rotate(ConstArg q) noexcept { *this = q*(*this)*Quaternion(simd::neg3(q.xyzw)); return *this; }
	Quaternion& preConcatenate(ConstArg q) noexcept { *this = (*this)*q; return *this; }
	Quaternion& concatenate(ConstArg q) noexcept { *this = q*(*this); return *this; }
	Quaternion& negate() noexcept { xyzw = simd::neg4(xyzw); return *this; }
	Quaternion& normalize() noexcept;

	//static const Quaternion& getZero() noexcept { return ZERO; }
	//static const Quaternion& getIdentity() noexcept { return IDENTITY; }

	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static const Quaternion TOLERANCE;

	union
	{
		simd::float4 xyzw;
		struct { float x, y, z, w; };
	};
};

const Quaternion<float> Quaternion<float>::ZERO{};
const Quaternion<float> Quaternion<float>::IDENTITY{ 0.f, 0.f, 0.f, 1.f };
const Quaternion<float> Quaternion<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };

#endif /* SIMD_HAS_FLOAT4 */

template<std::size_t I, typename T>
inline T& get(Quaternion<T>& v) noexcept
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
inline const T& get(const Quaternion<T>& v) noexcept
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
inline T&& get(Quaternion<T>&& v) noexcept
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
inline const T&& get(const Quaternion<T>&& v) noexcept
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

template<typename T>
inline T dot(const Quaternion<T>& q1, const Quaternion<T>& q2) noexcept
{
	return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

template<typename T>
inline Quaternion<T> lerp(const Quaternion<T>& q1, const Quaternion<T>& q2, T t) noexcept
{
	T cosTheta = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	T t0 = T(1) - t;
	T t1 = t;
	if (/*shortestArc &&*/ (cosTheta < T(0))) // If q2 is on the oposite hemisphere use -q2 instead of q2
		t1 = -t1;

	return /*normalize*/(Quaternion<T>(t0*q1.x + t1*q2.x, t0*q1.y + t1*q2.y, t0*q1.z + t1*q2.z, t0*q1.w + t1*q2.w));
}

template<typename T>
inline Quaternion<T> slerp(const Quaternion<T>& q1, const Quaternion<T>& q2, T t)
{
	T cosTheta = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	T signOfT1 = T(1);
	if (/*shortestArc &&*/ (cosTheta < T(0))) // If q2 is on the oposite hemisphere use -q2 instead of q2
	{
		cosTheta = -cosTheta;
		signOfT1 = T(-1);
	}

	T t0, t1;
	if ((T(1) - cosTheta) < Constants<T>::TOLERANCE) // If q1 is nearly the same as q2 we just linearly interpolate
	{
		t0 = T(1) - t;
		t1 = t;
	}
	else
	{
		cosTheta = std::clamp(cosTheta, T(-1), T(1));
		T theta = std::acos(cosTheta);
		//if (std::fabs(theta) < Constants<T>::TOLERANCE) // If theta == 0 then return q1
		//	return q1;
//#if MATHEMATICS_FAST_NORMALIZE
//		T iSinTheta = rcpSqrtApprox(T(1) - cosTheta*cosTheta); // instead of 1/sin(theta)
//#else
		T iSinTheta = T(1)/std::sqrt(T(1) - cosTheta*cosTheta); // instead of 1/sin(theta)
//#endif
		//if (std::fabs(sinTheta) < Constants<T>::TOLERANCE) // If theta*2 == 180 degrees then result is undefined
		//	return q1*T(0.5) + q2*T(0.5);
		T tTheta = t*theta;
		t0 = std::sin(theta - tTheta)*iSinTheta;
		t1 = std::sin(tTheta)*iSinTheta;
	}

	t1 *= signOfT1;
	return /*normalize*/(Quaternion<T>(t0*q1.x + t1*q2.x, t0*q1.y + t1*q2.y, t0*q1.z + t1*q2.z, t0*q1.w + t1*q2.w));
}

template<typename T>
inline Quaternion<T> conjugate(const Quaternion<T>& q) noexcept
{
	return Quaternion<T>(-q.x, -q.y, -q.z, q.w);
}

template<typename T>
inline Quaternion<T> inverse(const Quaternion<T>& q) noexcept
{
	return Quaternion<T>(-q.x, -q.y, -q.z, q.w)/q.getNorm();
}

#if SIMD_HAS_FLOAT4

template<>
inline float dot(const Quaternion<float>& q1, const Quaternion<float>& q2) noexcept
{
	return simd::toFloat(simd::dot4(q1, q2));
}

template<>
inline Quaternion<float> lerp(const Quaternion<float>& q1, const Quaternion<float>& q2, float t) noexcept
{
	float cosTheta = simd::toFloat(simd::dot4(q1, q2));
	float t0 = 1.f - t;
	float t1 = t;
	if (/*shortestArc &&*/ (cosTheta < 0.f)) // If q2 is on the oposite hemisphere use -q2 instead of q2
		t1 = -t1;

	return /*normalize*/(t0*q1 + t1*q2);
}

template<>
inline Quaternion<float> slerp(const Quaternion<float>& q1, const Quaternion<float>& q2, float t)
{
	float cosTheta = simd::toFloat(simd::dot4(q1, q2));
	float signOfT1 = 1.f;
	if (/*shortestArc &&*/ (cosTheta < 0.f)) // If q2 is on the oposite hemisphere use -q2 instead of q2
	{
		cosTheta = -cosTheta;
		signOfT1 = -1.f;
	}

	float t0, t1;
	if ((1.f - cosTheta) < Constants<float>::TOLERANCE) // If q1 is nearly the same as q2 we just linearly interpolate
	{
		t0 = 1.f - t;
		t1 = t;
	}
	else
	{
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		float theta = std::acos(cosTheta);
		//if (std::fabs(theta) < Constants<float>::TOLERANCE) // If theta == 0 then return q1
		//	return q1;
//#if MATHEMATICS_FAST_NORMALIZE
//		float iSinTheta = rcpSqrtApprox(1.f - cosTheta*cosTheta); // instead of 1/sin(theta)
//#else
		float iSinTheta = 1.f/std::sqrt(1.f - cosTheta*cosTheta); // instead of 1/sin(theta)
		//#endif
				//if (std::fabs(sinTheta) < Constants<float>::TOLERANCE) // If theta*2 == 180 degrees then result is undefined
				//	return q1*0.5f + q2*0.5f;
		float tTheta = t*theta;
		t0 = std::sin(theta - tTheta)*iSinTheta;
		t1 = std::sin(tTheta)*iSinTheta;
	}

	t1 *= signOfT1;
	return /*normalize*/(t0*q1 + t1*q2);
}

template<>
inline Quaternion<float> conjugate(const Quaternion<float>& q) noexcept
{
	return Quaternion<float>(simd::neg3(q.xyzw));
}

template<>
inline Quaternion<float> inverse(const Quaternion<float>& q) noexcept
{
	return Quaternion<float>(simd::neg3(q.xyzw))/q.getNorm();
}

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Quaternion<T>& Quaternion<T>::operator*=(Quaternion<T>::ConstArg q)
{
	set(w*q.x + x*q.w + y*q.z - z*q.y, w*q.y - x*q.z + y*q.w + z*q.x,
		w*q.z + x*q.y - y*q.x + z*q.w, w*q.w - x*q.x - y*q.y - z*q.z);
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::operator*=(Vector3<T>::ConstArg v)
{
	set(w*v.x + y*v.z - z*v.y, w*v.y + z*v.x - x*v.z, w*v.z + x*v.y - y*v.x, -(x*v.x + y*v.y + z*v.z));
	return *this;
}

template<typename T>
inline Quaternion<T> operator*(Quaternion<T>::ConstArg q1, Quaternion<T>::ConstArg q2) noexcept
{
	return Quaternion<T>(q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y, q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
		q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w, q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
}

template<typename T>
inline Quaternion<T> operator*(Vector3<T>::ConstArg v, Quaternion<T>::ConstArg q) noexcept
{
	return Quaternion<T>(q.w*v.x + q.z*v.y - q.y*v.z, q.w*v.y + q.x*v.z - q.z*v.x, q.w*v.z + q.y*v.x - q.x*v.y,
		-(q.x*v.x + q.y*v.y + q.z*v.z));
}

template<typename T>
inline Quaternion<T> operator*(Quaternion<T>::ConstArg q, Vector3<T>::ConstArg v) noexcept
{
	return Quaternion<T>(q.w*v.x + q.y*v.z - q.z*v.y, q.w*v.y + q.z*v.x - q.x*v.z, q.w*v.z + q.x*v.y - q.y*v.x,
		-(q.x*v.x + q.y*v.y + q.z*v.z));
}

template<typename T>
inline std::istream& operator>>(std::istream& s, Quaternion<T>& q) 
{ 
	return s >> q.x >> std::skipws >> q.y >> std::skipws >> q.z >> std::skipws >> q.w; 
}

template<typename T>
template<std::size_t I>
inline T& Quaternion<T>::get()
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	else if constexpr (I == 3)
		return w;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& Quaternion<T>::get() const
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	else if constexpr (I == 3)
		return w;
	static_assert(false);
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
inline Vector3<T> Quaternion<T>::getAxis() const
{
	T cosine = T(1) - w*w;
	if (cosine <= T(0))
		return Vector3<T>();

//#if MATHEMATICS_FAST_NORMALIZE
//	if costexpr(std::is_same_v<T, float>)
//	{
//		float m = rcpSqrtApprox(cosine);
//		if (m <= std::numeric_limits<T>::max())
//		{
//			//if (w < T(0)) m = -m;	// <0, PI>
//			return Vector3<T>(x*m, y*m, z*m);
//		}
//		else
//			return Vector3<T>();
//	}
//	else
//#else
	{
		T m = std::sqrt(cosine);
		if (m > T(0))
		{
			//if (w < T(0)) m = -m;	// <0, PI>
			return Vector3<T>(x/m, y/m, z/m);
		}
		else
			return Vector3<T>();
	}
//#endif
}

template<typename T>
inline T Quaternion<T>::getAngle() const
{
	//T invAbs = getInverseAbsoluteValue();
	//if (invAbs <= std::numeric_limits<T>::max())
	//	return T(2)*std::acos(std::clamp(w*invAbs, T(-1), T(1)));				// <-PI, PI>
	//	//return T(2)*std::acos(std::clamp(std::fabs(w*invAbs), T(-1), T(1)));	// <0, PI>
	//else
	//	return T(0);

	return T(2)*std::acos(std::clamp(w, T(-1), T(1)));				// <-PI, PI>
	//return T(2)*std::acos(std::clamp(std::fabs(w), T(-1), T(1)));	// <0, PI>
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

#if SIMD_HAS_FLOAT4

inline Quaternion<float>& Quaternion<float>::operator*=(Quaternion<float>::ConstArg q)
{
	auto t0 = simd::broadcast<3>(xyzw);
	auto t1 = simd::swizzle<1, 0, 3, 2>(q.xyzw);
	auto t3 = simd::broadcast<0>(xyzw);
	auto t4 = simd::swizzle<2, 3, 0, 1>(q.xyzw);
	auto t5 = simd::broadcast<1>(xyzw);
	auto t6 = simd::swizzle<1, 3, 0, 2>(q.xyzw);
	auto m0 = simd::mul4(t0, t1);
	auto m1 = simd::mul4(t3, t4);
	auto m2 = simd::mul4(t5, t6);
	auto t7 = simd::broadcast<2>(xyzw);
	auto t8 = simd::swizzle<1, 0, 2, 3>(q.xyzw);
	auto m3 = simd::mul4(t7, t8);
	auto e = simd::subAdd4(m0, m1);
	e = simd::swizzle<2, 0, 3, 1>(e);
	e = simd::subAdd4(e, m2);
	e = simd::swizzle<3, 1, 0, 2>(e);
	e = simd::subAdd4(e, m3);
	xyzw = simd::swizzle<0, 1, 3, 2>(e);
	return *this;
}

inline Quaternion<float>& Quaternion<float>::operator*=(Vector3<float>::ConstArg v) // #TODO SIMD
{
	set(w*v.x + y*v.z - z*v.y, w*v.y + z*v.x - x*v.z, w*v.z + x*v.y - y*v.x, -(x*v.x + y*v.y + z*v.z));
	return *this;
}

template<>
inline Quaternion<float> operator*(Quaternion<float>::ConstArg q1, Quaternion<float>::ConstArg q2) noexcept
{
	auto t0 = simd::broadcast<3>(q1.xyzw);
	auto t1 = simd::swizzle<1, 0, 3, 2>(q2.xyzw);
	auto t3 = simd::broadcast<0>(q1.xyzw);
	auto t4 = simd::swizzle<2, 3, 0, 1>(q2.xyzw);
	auto t5 = simd::broadcast<1>(q1.xyzw);
	auto t6 = simd::swizzle<1, 3, 0, 2>(q2.xyzw);
	auto m0 = simd::mul4(t0, t1);
	auto m1 = simd::mul4(t3, t4);
	auto m2 = simd::mul4(t5, t6);
	auto t7 = simd::broadcast<2>(q1.xyzw);
	auto t8 = simd::swizzle<1, 0, 2, 3>(q2.xyzw);
	auto m3 = simd::mul4(t7, t8);
	auto e = simd::subAdd4(m0, m1);
	e = simd::swizzle<2, 0, 3, 1>(e);
	e = simd::subAdd4(e, m2);
	e = simd::swizzle<3, 1, 0, 2>(e);
	e = simd::subAdd4(e, m3);
	return Quaternion<float>(simd::swizzle<0, 1, 3, 2>(e));
}

template<>
inline Quaternion<float> operator*(Vector3<float>::ConstArg v, Quaternion<float>::ConstArg q) noexcept // #TODO SIMD
{
	return Quaternion<float>(q.w*v.x + q.z*v.y - q.y*v.z, q.w*v.y + q.x*v.z - q.z*v.x, q.w*v.z + q.y*v.x - q.x*v.y,
		-(q.x*v.x + q.y*v.y + q.z*v.z));
}

template<>
inline Quaternion<float> operator*(Quaternion<float>::ConstArg q, Vector3<float>::ConstArg v) noexcept // #TODO SIMD
{
	return Quaternion<float>(q.w*v.x + q.y*v.z - q.z*v.y, q.w*v.y + q.z*v.x - q.x*v.z, q.w*v.z + q.x*v.y - q.y*v.x,
		-(q.x*v.x + q.y*v.y + q.z*v.z));
}

template<>
inline std::istream& operator>>(std::istream& s, Quaternion<float>& q)
{
	float x, y, z, w;
	s >> x >> std::skipws >> y >> std::skipws >> z >> std::skipws >> w;
	q.set(x, y, z, w);
	return s;
}

template<std::size_t I>
inline float& Quaternion<float>::get()
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	else if constexpr (I == 3)
		return w;
	static_assert(false);
}

template<std::size_t I>
inline const float& Quaternion<float>::get() const
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	else if constexpr (I == 3)
		return w;
	static_assert(false);
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

inline Vector3<float> Quaternion<float>::getAxis() const
{
	float cosine = 1.f - w*w;
	if (cosine <= 0.f)
		return Vector3<float>();

#if MATHEMATICS_FAST_NORMALIZE
	float m = simd::toFloat(simd::rcpSqrtApprox1(simd::set1(cosine)));
	if (m <= std::numeric_limits<float>::max())
	{
		//if (w < 0.f) m = -m;	// <0, PI>
		return getVector()*m;
	}
	else
		return Vector3<float>();
#else
	float m = std::sqrt(cosine);
	if (m > 0.f)
	{
		//if (w < 0.f) m = -m;	// <0, PI>
		return getVector()/m;
	}
	else
		return Vector3<float>();
#endif
}

inline float Quaternion<float>::getAngle() const
{
	//float invAbs = getInverseAbsoluteValue();
	//if (invAbs <= std::numeric_limits<float>::max())
	//	return 2.f*std::acos(std::clamp(w*invAbs, -1.f, 1.f));				// <-PI, PI>
	//	//return 2.f*std::acos(std::clamp(std::fabs(w*invAbs), -1.f, 1.f));	// <0, PI>
	//else
	//	return 0.f;

	return 2.f*std::acos(std::clamp(w, -1.f, 1.f));					// <-PI, PI>
	//return 2.f*std::acos(std::clamp(std::fabs(w), -1.f, 1.f));	// <0, PI>
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

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Quaternion<T> rotate(const Quaternion<T>& q1, const Quaternion<T>& q2) noexcept
{
	return q2*q1*Quaternion<T>(-q2.x, -q2.y, -q2.z, q2.w);
}

template<typename T>
inline Quaternion<T> concatenate(const Quaternion<T>& q1, const Quaternion<T>& q2) noexcept
{
	return q2*q1;
}

template<typename T>
inline Quaternion<T> concatenate(const Quaternion<T>& q1, const Quaternion<T>& q2, const Quaternion<T>& q3) noexcept
{
	return concatenate(concatenate(q1, q2), q3);
}

template<typename T>
inline Quaternion<T> concatenate(const Quaternion<T>& q1, const Quaternion<T>& q2, const Quaternion<T>& q3,
	const Quaternion<T>& q4) noexcept
{
	return concatenate(concatenate(concatenate(q1, q2), q3), q4);
}

template<typename T>
inline Quaternion<T> normalize(const Quaternion<T>& q) noexcept
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
inline Quaternion<T> arc(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	Vector3<T> u(v1);
	Vector3<T> v(v2);
	u.normalize();
	v.normalize();
	Vector3<T> h(u + v);
	h.normalize();
	return Quaternion<T>(cross(u, h), dot(u, h));
}

#if SIMD_HAS_FLOAT4

template<>
inline Quaternion<float> rotate(const Quaternion<float>& q1, const Quaternion<float>& q2) noexcept
{
	return q2*q1*Quaternion<float>(simd::neg3(q2.xyzw));
}

//template<>
//inline Quaternion<float> concatenate(const Quaternion<float>& q1, const Quaternion<float>& q2) noexcept
//{
//	return q2*q1;
//}
//
//template<>
//inline Quaternion<float> concatenate(const Quaternion<float>& q1, const Quaternion<float>& q2, const Quaternion<float>& q3) noexcept
//{
//	return concatenate(concatenate(q1, q2), q3);
//}
//
//template<>
//inline Quaternion<float> concatenate(const Quaternion<float>& q1, const Quaternion<float>& q2, const Quaternion<float>& q3,
//	const Quaternion<float>& q4) noexcept
//{
//	return concatenate(concatenate(concatenate(q1, q2), q3), q4);
//}

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

} // namespace core::mathematics

namespace std
{

template<size_t I, typename T>
struct tuple_element;

template<typename T>
struct tuple_size;

template<size_t I, typename T>
struct tuple_element<I, core::mathematics::templates::Quaternion<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size<core::mathematics::templates::Quaternion<T>> : integral_constant<size_t, 4> 
{
};

} // namespace std

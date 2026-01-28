/*
 *	Name: Matrix3
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <Simd/Intrinsics.hpp>
#include "Constants.hpp"
#include "Axis.hpp"
#include "Scalar.hpp"
#include "Vector3.hpp"
#include "Matrix2.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct YawPitchRoll;

template<typename T>
struct Euler;

template<typename T>
struct Quaternion;

template<typename T>
struct Matrix3
{
	using Real = T;
	using ConstArg = const Matrix3&;
	using ConstResult = const Matrix3&;

	//static constexpr int NUM_COMPONENTS = 9;

	constexpr Matrix3() noexcept : m00(), m01(), m02(), m10(), m11(), m12(), m20(), m21(), m22() {}
	explicit Matrix3(Uninitialized) noexcept {}
	//explicit Matrix3(Identity) noexcept {}
	constexpr Matrix3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) noexcept;
	constexpr Matrix3(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2) noexcept;
	constexpr Matrix3(const Matrix2<T>& m) noexcept;
	explicit Matrix3(const YawPitchRoll<T>& r) noexcept;
	explicit Matrix3(const Euler<T>& e) noexcept;
	explicit Matrix3(const Quaternion<T>& q) noexcept; // the input quaternion has to be normalized
	explicit Matrix3(const Vector3<T>& forward) noexcept;
	Matrix3(const Vector3<T>& up, const Vector3<T>& forward) noexcept;
	explicit Matrix3(const T* m) noexcept;

	//explicit operator T*() noexcept { return &m00; }
	//explicit operator const T*() const noexcept { return &m00; }
	Vector3<T>& operator[](int i) noexcept { return reinterpret_cast<Vector3<T>*>(&m00)[i]; }
	const Vector3<T>& operator[](int i) const noexcept { return reinterpret_cast<const Vector3<T>*>(&m00)[i]; }

	Matrix3 operator+() const noexcept { return *this; }
	Matrix3 operator-() const noexcept;
	Matrix3& operator+=(const Matrix3& m) noexcept;
	Matrix3& operator-=(const Matrix3& m) noexcept;
	Matrix3& operator*=(T f) noexcept;
	Matrix3& operator/=(T f) noexcept { return operator*=(T(1)/f); }
	Matrix3& operator*=(const Matrix3& m) noexcept;
	friend Matrix3 operator+(const Matrix3& m1, const Matrix3& m2) noexcept;
	friend Matrix3 operator-(const Matrix3& m1, const Matrix3& m2) noexcept;
	friend Matrix3 operator*(T f, const Matrix3& m) noexcept;
	friend Matrix3 operator*(const Matrix3& m, T f) noexcept;
	friend Matrix3 operator/(const Matrix3& m, T f) noexcept { return operator*(m, T(1)/f); }
	friend Matrix3 operator*(const Matrix3& m1, const Matrix3& m2) noexcept;
	bool operator==(const Matrix3& m) const noexcept;
	bool operator!=(const Matrix3& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, Matrix3& m);
	friend std::ostream& operator<<(std::ostream& s, const Matrix3& m);

	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m02 & m10 & m11 & m12 & m20 & m21 & m22; }

	// #TODO rename get...() to make...()
	static Matrix3 getScaling(const Vector3<T>& v) noexcept { return Matrix3(Uninitialized()).setScaling(v); }
	static Matrix3 getScaling(T f) noexcept { return Matrix3(Uninitialized()).setScaling(f); }
	static Matrix3 getRotation(Axis axis, T angle) noexcept { return Matrix3(Uninitialized()).setRotation(axis, angle); }
	static Matrix3 getRotation(const Vector3<T>& axis, T angle) noexcept { return Matrix3(Uninitialized()).setRotation(axis, angle); }
	static Matrix3 getRotation(const YawPitchRoll<T>& r) noexcept { return Matrix3(Uninitialized()).setRotation(r); }
	static Matrix3 getRotation(const Euler<T>& e) noexcept { return Matrix3(Uninitialized()).setRotation(e); }
	static Matrix3 getRotation(const Quaternion<T>& q) noexcept { return Matrix3(Uninitialized()).setRotation(q); }
	static Matrix3 getScalingRotation(const Vector3<T>& s, Axis axis, T angle) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix3 getScalingRotation(const Vector3<T>& s, const Vector3<T>& axis, T angle) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix3 getScalingRotation(const Vector3<T>& s, const YawPitchRoll<T>& r) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, r); }
	static Matrix3 getScalingRotation(const Vector3<T>& s, const Euler<T>& e) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, e); }
	static Matrix3 getScalingRotation(const Vector3<T>& s, const Quaternion<T>& q) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, q); }
	static Matrix3 getShearing(T xy, T xz, T yx, T yz, T zx, T zy) noexcept { return Matrix3(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy); }

	bool isZero() const noexcept;
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept;
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Matrix3& m) const noexcept;
	bool isApproxEqual(const Matrix3& m, T tolerance) const noexcept;
	bool isApproxOrthogonal() const noexcept;
	bool hasApproxUniformScaling() const noexcept;
	bool isFinite() const noexcept;
	const Vector3<T>& getRow(int i) const noexcept { return reinterpret_cast<const Vector3<T>*>(&m00)[i]; }
	Vector3<T> getColumn(int i) const noexcept;
	Vector3<T> getScaleComponent() const noexcept { return Vector3<T>((*this)[0].getMagnitude(), (*this)[1].getMagnitude(), (*this)[2].getMagnitude()); }
	T getTrace() const noexcept { return m00 + m11 + m22; }
	T getDeterminant() const noexcept;
	bool isSingular() const noexcept { return (getDeterminant() == T(0)); }
	Matrix3& setZero/*zero*/() noexcept;
	Matrix3& setIdentity/*makeIdentity*/() noexcept;
	Matrix3& set(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2) noexcept;
	Matrix3& set(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) noexcept;
	Matrix3& setScaling/*makeScaling*/(const Vector3<T>& v) noexcept;
	Matrix3& setScaling/*makeScaling*/(T f) noexcept;
	Matrix3& setRotation(Axis axis, T angle) noexcept;
	Matrix3& setRotation(const Vector3<T>& axis, T angle) noexcept;
	Matrix3& setRotation(const YawPitchRoll<T>& r) noexcept;
	Matrix3& setRotation(const Euler<T>& e) noexcept;
	Matrix3& setRotation(const Quaternion<T>& q) noexcept;
	Matrix3& setScalingRotation(const Vector3<T>& s, Axis axis, T angle) noexcept;
	Matrix3& setScalingRotation(const Vector3<T>& s, const Vector3<T>& axis, T angle) noexcept;
	Matrix3& setScalingRotation(const Vector3<T>& s, const YawPitchRoll<T>& r) noexcept;
	Matrix3& setScalingRotation(const Vector3<T>& s, const Euler<T>& e) noexcept;
	Matrix3& setScalingRotation(const Vector3<T>& s, const Quaternion<T>& q) noexcept;
	Matrix3& setShearing(T xy, T xz, T yx, T yz, T zx, T zy) noexcept;
	Matrix3& setTranspose/*transposeOf*/(const Matrix3& m) noexcept;
	Matrix3& setInverse/*inverseOf*/(const Matrix3& m) noexcept;
	Matrix3& setInverseTranspose/*inverseTransposeOf*/(const Matrix3& m) noexcept;
	Matrix3& preConcatenate(const Matrix3& m) noexcept;
	Matrix3& concatenate(const Matrix3& m) noexcept;
	Matrix3& preScale(const Vector3<T>& v) noexcept;
	Matrix3& preScale(T f) noexcept { (*this) *= f; return *this; }
	Matrix3& scale(const Vector3<T>& v) noexcept;
	Matrix3& scale(T f) noexcept { (*this) *= f; return *this; }
	Matrix3& rotate(Axis axis, T angle) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix3& rotate(const Vector3<T>& axis, T angle) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix3& rotate(const YawPitchRoll<T>& r) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(r)); return *this; }
	Matrix3& rotate(const Euler<T>& e) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(e)); return *this; }
	Matrix3& rotate(const Quaternion<T>& q) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(q)); return *this; }
	Matrix3& shear(T xy, T xz, T yx, T yz, T zx, T zy) noexcept { concatenate(Matrix3(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy)); return *this; }
	Matrix3& negate() noexcept;
	Matrix3& transpose() noexcept;
	Matrix3& invert() noexcept;
	Matrix3& orthonormalize() noexcept;

	//static const Matrix3& getZero() noexcept { return ZERO; }
	//static const Matrix3& getIdentity() noexcept { return IDENTITY; }

	static const Matrix3 ZERO;
	static const Matrix3 IDENTITY;

	T m00, m01, m02;
	T m10, m11, m12;
	T m20, m21, m22;
};

template<typename T> const Matrix3<T> Matrix3<T>::ZERO{};
template<typename T> const Matrix3<T> Matrix3<T>::IDENTITY{ T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1) };

#if SIMD_HAS_FLOAT4

template<>
struct YawPitchRoll<float>;

template<>
struct Euler<float>;

template<>
struct Quaternion<float>;

template<>
struct Matrix3<float>
{
	using Real = float;
	using ConstArg = const Matrix3;
	using ConstResult = const Matrix3;

	//static constexpr int NUM_COMPONENTS = 9;

	constexpr Matrix3() noexcept;
	explicit Matrix3(Uninitialized) noexcept {}
	//explicit Matrix3(Identity) noexcept {}
	constexpr Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) noexcept;
	constexpr Matrix3(const Vector3<float>& row0, const Vector3<float>& row1, const Vector3<float>& row2) noexcept;
	constexpr Matrix3(const Matrix2<float>& m) noexcept;
	explicit Matrix3(const YawPitchRoll<float>& r) noexcept;
	explicit Matrix3(const Euler<float>& e) noexcept;
	explicit Matrix3(const Quaternion<float>& q) noexcept; // the input quaternion has to be normalized
	explicit Matrix3(const Vector3<float>& forward) noexcept;
	Matrix3(const Vector3<float>& up, const Vector3<float>& forward) noexcept;
	explicit Matrix3(const float* m) noexcept;
	explicit Matrix3(const simd::float4* m) noexcept : row0(m[0]), row1(m[1]), row2(m[2]) {}
	Matrix3(const Matrix3& m) noexcept : row0(m.row0), row1(m.row1), row2(m.row2) {}
	Matrix3& operator=(const Matrix3& m) noexcept { row0 = m.row0; row1 = m.row1; row2 = m.row2; return *this; }

	//explicit operator float*() noexcept { return &m00; }
	//explicit operator const float*() const noexcept { return &m00; }
	explicit operator simd::float4* () noexcept { return &row0; }
	explicit operator const simd::float4* () const noexcept { return &row0; }
	Vector3<float>& operator[](int i) noexcept { return reinterpret_cast<Vector3<float>&>((&row0)[i]); }
	const Vector3<float>& operator[](int i) const noexcept { return reinterpret_cast<const Vector3<float>&>((&row0)[i]); }

	Matrix3 operator+() const noexcept { return *this; }
	Matrix3 operator-() const noexcept;
	Matrix3& operator+=(const Matrix3& m) noexcept;
	Matrix3& operator-=(const Matrix3& m) noexcept;
	Matrix3& operator*=(float f) noexcept;
	Matrix3& operator/=(float f) noexcept { return operator*=(1.f/f); }
	Matrix3& operator*=(const Matrix3& m) noexcept;
	friend Matrix3 operator+(const Matrix3& m1, const Matrix3& m2) noexcept;
	friend Matrix3 operator-(const Matrix3& m1, const Matrix3& m2) noexcept;
	friend Matrix3 operator*(float f, const Matrix3& m) noexcept;
	friend Matrix3 operator*(const Matrix3& m, float f) noexcept;
	friend Matrix3 operator/(const Matrix3& m, float f) noexcept { return operator*(m, 1.f/f); }
	friend Matrix3 operator*(const Matrix3& m1, const Matrix3& m2) noexcept;
	bool operator==(const Matrix3& m) const noexcept;
	bool operator!=(const Matrix3& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, Matrix3& m);
	friend std::ostream& operator<<(std::ostream& s, const Matrix3& m);

	// #FIXME use simd::set()
	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m02 & m10 & m11 & m12 & m20 & m21 & m22; }

	// #TODO rename get...() to make...()
	static Matrix3 getScaling(const Vector3<float>& v) noexcept { return Matrix3(Uninitialized()).setScaling(v); }
	static Matrix3 getScaling(float f) noexcept { return Matrix3(Uninitialized()).setScaling(f); }
	static Matrix3 getRotation(Axis axis, float angle) noexcept { return Matrix3(Uninitialized()).setRotation(axis, angle); }
	static Matrix3 getRotation(const Vector3<float>& axis, float angle) noexcept { return Matrix3(Uninitialized()).setRotation(axis, angle); }
	static Matrix3 getRotation(const YawPitchRoll<float>& r) noexcept { return Matrix3(Uninitialized()).setRotation(r); }
	static Matrix3 getRotation(const Euler<float>& e) noexcept { return Matrix3(Uninitialized()).setRotation(e); }
	static Matrix3 getRotation(const Quaternion<float>& q) noexcept { return Matrix3(Uninitialized()).setRotation(q); }
	static Matrix3 getScalingRotation(const Vector3<float>& s, Axis axis, float angle) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix3 getScalingRotation(const Vector3<float>& s, const Vector3<float>& axis, float angle) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix3 getScalingRotation(const Vector3<float>& s, const YawPitchRoll<float>& r) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, r); }
	static Matrix3 getScalingRotation(const Vector3<float>& s, const Euler<float>& e) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, e); }
	static Matrix3 getScalingRotation(const Vector3<float>& s, const Quaternion<float>& q) noexcept { return Matrix3(Uninitialized()).setScalingRotation(s, q); }
	static Matrix3 getShearing(float xy, float xz, float yx, float yz, float zx, float zy) noexcept { return Matrix3(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy); }

	bool isZero() const noexcept;
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept;
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Matrix3& m) const noexcept;
	bool isApproxEqual(const Matrix3& m, float tolerance) const noexcept;
	bool isApproxOrthogonal() const noexcept;
	bool hasApproxUniformScaling() const noexcept;
	bool isFinite() const noexcept;
	const Vector3<float>& getRow(int i) const noexcept { return reinterpret_cast<const Vector3<float>&>((&row0)[i]); }
	Vector3<float> getColumn(int i) const noexcept;
	Vector3<float> getScaleComponent() const noexcept { return Vector3<float>((*this)[0].getMagnitude(), (*this)[1].getMagnitude(), (*this)[2].getMagnitude()); }
	float getTrace() const noexcept { return m00 + m11 + m22; }
	float getDeterminant() const noexcept;
	bool isSingular() const noexcept { return (getDeterminant() == 0.f); }
	Matrix3& setZero/*zero*/() noexcept;
	Matrix3& setIdentity/*makeIdentity*/() noexcept;
	Matrix3& set(const Vector3<float>& row0, const Vector3<float>& row1, const Vector3<float>& row2) noexcept;
	Matrix3& set(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) noexcept;
	Matrix3& setScaling/*makeScaling*/(const Vector3<float>& v) noexcept;
	Matrix3& setScaling/*makeScaling*/(float f) noexcept;
	Matrix3& setRotation(Axis axis, float angle) noexcept;
	Matrix3& setRotation(const Vector3<float>& axis, float angle) noexcept;
	Matrix3& setRotation(const YawPitchRoll<float>& r) noexcept;
	Matrix3& setRotation(const Euler<float>& e) noexcept;
	Matrix3& setRotation(const Quaternion<float>& q) noexcept;
	Matrix3& setScalingRotation(const Vector3<float>& s, Axis axis, float angle) noexcept;
	Matrix3& setScalingRotation(const Vector3<float>& s, const Vector3<float>& axis, float angle) noexcept;
	Matrix3& setScalingRotation(const Vector3<float>& s, const YawPitchRoll<float>& r) noexcept;
	Matrix3& setScalingRotation(const Vector3<float>& s, const Euler<float>& e) noexcept;
	Matrix3& setScalingRotation(const Vector3<float>& s, const Quaternion<float>& q) noexcept;
	Matrix3& setShearing(float xy, float xz, float yx, float yz, float zx, float zy) noexcept;
	Matrix3& setTranspose/*transposeOf*/(const Matrix3& m) noexcept;
	Matrix3& setInverse/*inverseOf*/(const Matrix3& m) noexcept;
	Matrix3& setInverseTranspose/*inverseTransposeOf*/(const Matrix3& m) noexcept;
	Matrix3& preConcatenate(const Matrix3& m) noexcept;
	Matrix3& concatenate(const Matrix3& m) noexcept;
	Matrix3& preScale(const Vector3<float>& v) noexcept;
	Matrix3& preScale(float f) noexcept { (*this) *= f; return *this; }
	Matrix3& scale(const Vector3<float>& v) noexcept;
	Matrix3& scale(float f) noexcept { (*this) *= f; return *this; }
	Matrix3& rotate(Axis axis, float angle) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix3& rotate(const Vector3<float>& axis, float angle) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix3& rotate(const YawPitchRoll<float>& r) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(r)); return *this; }
	Matrix3& rotate(const Euler<float>& e) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(e)); return *this; }
	Matrix3& rotate(const Quaternion<float>& q) noexcept { concatenate(Matrix3(Uninitialized()).setRotation(q)); return *this; }
	Matrix3& shear(float xy, float xz, float yx, float yz, float zx, float zy) noexcept { concatenate(Matrix3(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy)); return *this; }
	Matrix3& negate() noexcept;
	Matrix3& transpose() noexcept;
	Matrix3& invert() noexcept;
	Matrix3& orthonormalize() noexcept;

	//static const Matrix3& getZero() noexcept { return ZERO; }
	//static const Matrix3& getIdentity() noexcept { return IDENTITY; }

	static const Matrix3 ZERO;
	static const Matrix3 IDENTITY;

	union
	{
		simd::float4 row0;
		struct { float m00, m01, m02; };
	};

	union
	{
		simd::float4 row1;
		struct { float m10, m11, m12; };
	};

	union
	{
		simd::float4 row2;
		struct { float m20, m21, m22; };
	};
};

const Matrix3<float> Matrix3<float>::ZERO{};
const Matrix3<float> Matrix3<float>::IDENTITY{ 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };

#endif /* SIMD_HAS_FLOAT4 */

#if SIMD_HAS_FLOAT4

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Matrix3<T>::Matrix3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) :
	m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22)
{
}

template<typename T>
inline Matrix3<T>::Matrix3(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2) :
	m00(row0.x), m01(row0.y), m02(row0.z), m10(row1.x), m11(row1.y), m12(row1.z), m20(row2.x), m21(row2.y), m22(row2.z)
{
}

template<typename T>
inline Matrix3<T>::Matrix3(const Matrix2<T>& m) : 
	m00(m.m00), m01(m.m01), m02(), m10(m.m10), m11(m.m11), m12(), m20(), m21(), m22(1) 
{
}

template<typename T>
inline Matrix3<T>::Matrix3(const T* m) : 
	m00(m[0]), m01(m[1]), m02(m[2]), m10(m[3]), m11(m[4]), m12(m[5]), m20(m[6]), m21(m[7]), m22(m[8]) 
{
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::setZero()
{
	m00 = T(); m01 = T(); m02 = T();
	m10 = T(); m11 = T(); m12 = T();
	m20 = T(); m21 = T(); m22 = T();
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::setIdentity()
{
	m00 = T(1); m01 = T(); m02 = T();
	m10 = T(); m11 = T(1); m12 = T();
	m20 = T(); m21 = T(); m22 = T(1);
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::set(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2)
{
	m00 = row0.x; m01 = row0.y; m02 = row0.z;
	m10 = row1.x; m11 = row1.y; m12 = row1.z;
	m20 = row2.x; m21 = row2.y; m22 = row2.z; 
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::set(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22)
{
	this->m00 = m00; this->m01 = m01; this->m02 = m02; 
	this->m10 = m10; this->m11 = m11; this->m12 = m12; 
	this->m20 = m20; this->m21 = m21; this->m22 = m22; 
	return *this;
}

#if SIMD_HAS_FLOAT4

inline Matrix3<float>::Matrix3() : 
	row0(simd::zero<simd::float4>()), 
	row1(simd::zero<simd::float4>()), 
	row2(simd::zero<simd::float4>()) 
{
}

inline Matrix3<float>::Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) :
#if MATHEMATICS_SIMD_EXPAND_LAST
	row0(simd::set4(m00, m01, m02, m02)),
	row1(simd::set4(m10, m11, m12, m12)),
	row2(simd::set4(m20, m21, m22, m22))
#else
	row0(simd::set3(m00, m01, m02)),
	row1(simd::set3(m10, m11, m12)),
	row2(simd::set3(m20, m21, m22))
#endif
{
}

inline Matrix3<float>::Matrix3(const Vector3<float>& row0, const Vector3<T>& row1, const Vector3<T>& row2) :
	row0(row0),
	row1(row1),
	row2(row2)
{
}

inline Matrix3<float>::Matrix3(const Matrix2<float>& m) :
	row0(simd::cutoff2(m.row0)),
	row1(simd::cutoff2(m.row1)),
	row2(Vector3<float>::UNIT_Z)
{
}

#if MATHEMATICS_SIMD_EXPAND_LAST
inline Matrix3<float>::Matrix3(const float* m) :
	Matrix3(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8])
{
}
#else
inline Matrix3<float>::Matrix3(const float* m)
{
	simd::unpack3x3(m, row0, row1, row2);
}
#endif

inline bool Matrix3<float>::operator==(const Matrix3<float>& m) const
{ 
	return simd::all3(simd::equal(row0, m.row0)) && 
		simd::all3(simd::equal(row1, m.row1)) && 
		simd::all3(simd::equal(row2, m.row2));
}

inline bool Matrix3<float>::isZero() const
{
	const auto zero = simd::zero<simd::float4>();
	return simd::all3(simd::equal(row0, zero)) && 
		simd::all3(simd::equal(row1, zero)) && 
		simd::all3(simd::equal(row2, zero));
}

inline bool Matrix3<float>::isIdentity() const
{
	return simd::all3(simd::equal(row0, Vector3<float>::UNIT_X)) &&
		simd::all3(simd::equal(row1, Vector3<float>::UNIT_Y)) &&
		simd::all3(simd::equal(row2, Vector3<float>::UNIT_Z));
}

inline bool Matrix3<float>::isFinite() const
{
	return simd::all3(simd::isFinite(row0)) &&
		simd::all3(simd::isFinite(row1)) &&
		simd::all3(simd::isFinite(row2));
}

inline Matrix3<float>& Matrix3<float>::setZero()
{
	row0 = simd::zero<simd::float4>();
	row1 = simd::zero<simd::float4>();
	row2 = simd::zero<simd::float4>();
	return *this;
}

inline Matrix3<float>& Matrix3<float>::setIdentity()
{
	row0 = Vector3<float>::UNIT_X;
	row1 = Vector3<float>::UNIT_Y;
	row2 = Vector3<float>::UNIT_Z;
	return *this;
}

inline Matrix3<float>& Matrix3<float>::set(const Vector3<float>& row0, const Vector3<float>& row1, const Vector3<float>& row2)
{
	this->row0 = row0;
	this->row1 = row1;
	this->row2 = row2;
	return *this;
}

inline Matrix3<float>& Matrix3<float>::set(float m00, float m01, float m02, float m10, float m11, float m12,
	float m20, float m21, float m22)
{
#if MATHEMATICS_SIMD_EXPAND_LAST
	row0 = simd::set4(m00, m01, m02, m02);
	row1 = simd::set4(m10, m11, m12, m12);
	row2 = simd::set4(m20, m21, m22, m22);
#else
	row0 = simd::set3(m00, m01, m02);
	row1 = simd::set3(m10, m11, m12);
	row2 = simd::set3(m20, m21, m22);
#endif
	return *this;
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Matrix3f = templates::Matrix3<float>;
using Matrix3d = templates::Matrix3<double>;

#if MATHEMATICS_DOUBLE
using Matrix3 = templates::Matrix3<double>;
using Matrix3Arg = templates::Matrix3<double>::ConstArg;
using Matrix3Result = templates::Matrix3<double>::ConstResult;
#else
using Matrix3 = templates::Matrix3<float>;
using Matrix3Arg = templates::Matrix3<float>::ConstArg;
using Matrix3Result = templates::Matrix3<float>::ConstResult;
#endif

} // namespace core::mathematics

#include "YawPitchRoll.hpp"
#include "Euler.hpp"
#include "Quaternion.hpp"

namespace core::mathematics::templates {

#if SIMD_HAS_FLOAT4

#endif /* SIMD_HAS_FLOAT4 */

} // namespace core::mathematics::templates

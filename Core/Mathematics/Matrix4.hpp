/*
 *	Name: Matrix4
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
#include "Vector4.hpp"
#include "Matrix2.hpp"
#include "Matrix3.hpp"
#include "AffineTransform.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct YawPitchRoll;

template<typename T>
struct Euler;

template<typename T>
struct Quaternion;

template<typename T>
struct Plane;

template<typename T>
struct Matrix4
{
	using Real = T;
	using ConstArg = const Matrix4&;
	using ConstResult = const Matrix4&;

	//static constexpr int NUM_COMPONENTS = 16;

	constexpr Matrix4() noexcept : m00(), m01(), m02(), m03(), m10(), m11(), m12(), m13(), m20(), m21(), m22(), m23(), m30(), m31(), m32(), m33() {}
	explicit Matrix4(Uninitialized) noexcept {}
	//explicit Matrix4(Identity) noexcept {}
	constexpr Matrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33) noexcept;
	constexpr Matrix4(const Vector4<T>& row0, const Vector4<T>& row1, const Vector4<T>& row2, const Vector4<T>& row3) noexcept;
	constexpr Matrix4(const Matrix2<T>& m) noexcept;
	constexpr Matrix4(const Matrix3<T>& m) noexcept;
	constexpr Matrix4(const AffineTransform<T>& m) noexcept;
	explicit Matrix4(const T* m) noexcept;

	explicit operator T*() noexcept { return &m00; }
	explicit operator const T*() const noexcept { return &m00; }
	Vector4<T>& operator[](int i) noexcept { return reinterpret_cast<Vector4<T>*>(&m00)[i]; }
	const Vector4<T>& operator[](int i) const noexcept { return reinterpret_cast<const Vector4<T>*>(&m00)[i]; }

	Matrix4 operator+() const noexcept { return *this; }
	Matrix4 operator-() const noexcept;
	Matrix4& operator+=(const Matrix4& m) noexcept;
	Matrix4& operator-=(const Matrix4& m) noexcept;
	Matrix4& operator*=(T f) noexcept;
	Matrix4& operator/=(T f) noexcept { return operator*=(T(1)/f); }
	Matrix4& operator*=(const Matrix4& m) noexcept;
	friend Matrix4 operator+(const Matrix4& m1, const Matrix4& m2) noexcept;
	friend Matrix4 operator-(const Matrix4& m1, const Matrix4& m2) noexcept;
	friend Matrix4 operator*(T f, const Matrix4& m) noexcept;
	friend Matrix4 operator*(const Matrix4& m, T f) noexcept;
	friend Matrix4 operator/(const Matrix4& m, T f) noexcept { return operator*(m, T(1)/f); }
	friend Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) noexcept;
	bool operator==(const Matrix4& m) const noexcept;
	bool operator!=(const Matrix4& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, Matrix4& m);
	friend std::ostream& operator<<(std::ostream& s, const Matrix4& m);

	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m02 & m03 & m10 & m11 & m12 & m13 & m20 & m21 & m22 & m23 & m30 & m31 & m32 & m33; }

	// #TODO rename get...() to make...()
	static Matrix4 getTranslation(const Vector3<T>& v) noexcept { return Matrix4(Uninitialized()).setTranslation(v); }
	static Matrix4 getScaling(const Vector3<T>& v) noexcept { return Matrix4(Uninitialized()).setScaling(v); }
	static Matrix4 getRotation(Axis axis, T angle) noexcept { return Matrix4(Uninitialized()).setRotation(axis, angle); }
	static Matrix4 getRotation(const Vector3<T>& axis, T angle) noexcept { return Matrix4(Uninitialized()).setRotation(axis, angle); }
	static Matrix4 getRotation(const YawPitchRoll<T>& r) noexcept { return Matrix4(Uninitialized()).setRotation(r); }
	static Matrix4 getRotation(const Euler<T>& e) noexcept { return Matrix4(Uninitialized()).setRotation(e); }
	static Matrix4 getRotation(const Quaternion<T>& q) noexcept { return Matrix4(Uninitialized()).setRotation(q); }
	static Matrix4 getRotationTranslation(Axis axis, T angle, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static Matrix4 getRotationTranslation(const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static Matrix4 getRotationTranslation(const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(r, t); }
	static Matrix4 getRotationTranslation(const Euler<T>& e, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(e, t); }
	static Matrix4 getRotationTranslation(const Quaternion<T>& q, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(q, t); }
	static Matrix4 getScalingRotation(const Vector3<T>& s, Axis axis, T angle) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix4 getScalingRotation(const Vector3<T>& s, const Vector3<T>& axis, T angle) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix4 getScalingRotation(const Vector3<T>& s, const YawPitchRoll<T>& r) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, r); }
	static Matrix4 getScalingRotation(const Vector3<T>& s, const Euler<T>& e) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, e); }
	static Matrix4 getScalingRotation(const Vector3<T>& s, const Quaternion<T>& q) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, q); }
	static Matrix4 getScalingTranslation(const Vector3<T>& s, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setScalingTranslation(s, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<T>& s, Axis axis, T angle, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<T>& s, const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<T>& s, const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, r, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<T>& s, const Euler<T>& e, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, e, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<T>& s, const Quaternion<T>& q, const Vector3<T>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, q, t); }
	static Matrix4 getShearing(T xy, T xz, T yx, T yz, T zx, T zy) noexcept { return Matrix4(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy); }
	static Matrix4 getReflection(const Plane<T>& plane) noexcept { return Matrix4(Uninitialized()).setReflection(plane); }

	bool isZero() const noexcept;
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept;
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Matrix4& m) const noexcept;
	bool isApproxEqual(const Matrix4& m, T tolerance) const noexcept;
	bool isFinite() const noexcept;
	const Vector4<T>& getRow(int i) const noexcept { return reinterpret_cast<const Vector4<T>*>(&m00)[i]; }
	Vector4<T> getColumn(int i) const noexcept;
	T getTrace() const noexcept { return m00 + m11 + m22 + m33; }
	T getDeterminant() const noexcept;
	bool isSingular() const noexcept { return (getDeterminant() == T(0)); }
	Matrix4& setZero/*zero*/() noexcept;
	Matrix4& setIdentity/*makeIdentity*/() noexcept;
	Matrix4& set(const Vector4<T>& row0, const Vector4<T>& row1, const Vector4<T>& row2, const Vector4<T>& row3) noexcept;
	Matrix4& set(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33) noexcept;
	Matrix4& setTranslation/*makeTranslation*/(const Vector3<T>& v) noexcept;
	Matrix4& setScaling/*makeScaling*/(const Vector3<T>& v) noexcept;
	Matrix4& setRotation(Axis axis, T angle) noexcept;
	Matrix4& setRotation(const Vector3<T>& axis, T angle) noexcept;
	Matrix4& setRotation(const YawPitchRoll<T>& r) noexcept;
	Matrix4& setRotation(const Euler<T>& e) noexcept;
	Matrix4& setRotation(const Quaternion<T>& q) noexcept;
	Matrix4& setRotationTranslation(Axis axis, T angle, const Vector3<T>& t) noexcept;
	Matrix4& setRotationTranslation(const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept;
	Matrix4& setRotationTranslation(const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept;
	Matrix4& setRotationTranslation(const Euler<T>& e, const Vector3<T>& t) noexcept;
	Matrix4& setRotationTranslation(const Quaternion<T>& q, const Vector3<T>& t) noexcept;
	Matrix4& setScalingRotation(const Vector3<T>& s, Axis axis, T angle) noexcept;
	Matrix4& setScalingRotation(const Vector3<T>& s, const Vector3<T>& axis, T angle) noexcept;
	Matrix4& setScalingRotation(const Vector3<T>& s, const YawPitchRoll<T>& r) noexcept;
	Matrix4& setScalingRotation(const Vector3<T>& s, const Euler<T>& e) noexcept;
	Matrix4& setScalingRotation(const Vector3<T>& s, const Quaternion<T>& q) noexcept;
	Matrix4& setScalingTranslation(const Vector3<T>& s, const Vector3<T>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<T>& s, Axis axis, T angle, const Vector3<T>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<T>& s, const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<T>& s, const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<T>& s, const Euler<T>& e, const Vector3<T>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<T>& s, const Quaternion<T>& q, const Vector3<T>& t) noexcept;
	Matrix4& setShearing(T xy, T xz, T yx, T yz, T zx, T zy) noexcept;
	Matrix4& setReflection(const Plane<T>& plane) noexcept;
	Matrix4& setTranspose/*transposeOf*/(const Matrix4& m) noexcept;
	Matrix4& setInverse/*inverseOf*/(const Matrix4& m) noexcept;
	Matrix4& setInverseTranspose/*inverseTransposeOf*/(const Matrix4& m) noexcept;
	Matrix4& preConcatenate(const Matrix4& m) noexcept;
	Matrix4& concatenate(const Matrix4& m) noexcept;
	Matrix4& translate(const Vector3<T>& v) noexcept;
	Matrix4& preScale(const Vector3<T>& v) noexcept;
	Matrix4& scale(const Vector3<T>& v) noexcept;
	Matrix4& rotate(Axis axis, T angle) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix4& rotate(const Vector3<T>& axis, float angle) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix4& rotate(const YawPitchRoll<T>& r) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(r)); return *this; }
	Matrix4& rotate(const Euler<T>& e) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(e)); return *this; }
	Matrix4& rotate(const Quaternion<T>& q) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(q)); return *this; }
	Matrix4& shear(T xy, T xz, T yx, T yz, T zx, T zy) noexcept { concatenate(Matrix4(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy)); return *this; }
	Matrix4& negate() noexcept;
	Matrix4& transpose() noexcept;
	Matrix4& invert() noexcept;

	//static const Matrix4& getZero() noexcept { return ZERO; }
	//static const Matrix4& getIdentity() noexcept { return IDENTITY; }

	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;

	T m00, m01, m02, m03;
	T m10, m11, m12, m13;
	T m20, m21, m22, m23;
	T m30, m31, m32, m33;
};

template<typename T> const Matrix4<T> Matrix4<T>::ZERO{};
template<typename T> const Matrix4<T> Matrix4<T>::IDENTITY{ T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1) };

#if SIMD_HAS_FLOAT4

template<>
struct YawPitchRoll<float>;

template<>
struct Euler<float>;

template<>
struct Quaternion<float>;

template<>
struct Plane<float>;

template<>
struct Matrix4<float>
{
	using Real = float;
	using ConstArg = const Matrix4;
	using ConstResult = const Matrix4;

	//static constexpr int NUM_COMPONENTS = 16;

	constexpr Matrix4() noexcept;
	explicit Matrix4(Uninitialized) noexcept {}
	//explicit Matrix4(Identity) noexcept {}
	constexpr Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) noexcept;
	constexpr Matrix4(const Vector4<float>& row0, const Vector4<float>& row1, const Vector4<float>& row2, const Vector4<float>& row3) noexcept;
	constexpr Matrix4(const Matrix2<float>& m) noexcept;
	constexpr Matrix4(const Matrix3<float>& m) noexcept;
	constexpr Matrix4(const Affinefloatransform<float>& m) noexcept;
	explicit Matrix4(const float* m) noexcept;
	explicit Matrix4(const simd::float4* m) noexcept : row0(m[0]), row1(m[1]), row2(m[2]), row3(m[3]) {}
	Matrix4(const Matrix4& m) noexcept : row0(m.row0), row1(m.row1), row2(m.row2), row3(m.row3) {}
	Matrix4& operator=(const Matrix4& m) noexcept { row0 = m.row0; row1 = m.row1; row2 = m.row2; row3 = m.row3; return *this; }

	explicit operator float*() noexcept { return &m00; }
	explicit operator const float*() const noexcept { return &m00; }
	explicit operator simd::float4*() noexcept { return &row0; }
	explicit operator const simd::float4*() const noexcept { return &row0; }
	Vector4<float>& operator[](int i) noexcept { return reinterpret_cast<Vector4<float>&>((&row0)[i]); }
	const Vector4<float>& operator[](int i) const noexcept { return reinterpret_cast<const Vector4<float>&>((&row0)[i]); }

	Matrix4 operator+() const noexcept { return *this; }
	Matrix4 operator-() const noexcept;
	Matrix4& operator+=(const Matrix4& m) noexcept;
	Matrix4& operator-=(const Matrix4& m) noexcept;
	Matrix4& operator*=(float f) noexcept;
	Matrix4& operator/=(float f) noexcept { return operator*=(1.f/f); }
	Matrix4& operator*=(const Matrix4& m) noexcept;
	friend Matrix4 operator+(const Matrix4& m1, const Matrix4& m2) noexcept;
	friend Matrix4 operator-(const Matrix4& m1, const Matrix4& m2) noexcept;
	friend Matrix4 operator*(float f, const Matrix4& m) noexcept;
	friend Matrix4 operator*(const Matrix4& m, float f) noexcept;
	friend Matrix4 operator/(const Matrix4& m, float f) noexcept { return operator*(m, 1.f/f); }
	friend Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) noexcept;
	bool operator==(const Matrix4& m) const noexcept;
	bool operator!=(const Matrix4& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, Matrix4& m);
	friend std::ostream& operator<<(std::ostream& s, const Matrix4& m);

	// #FIXME use simd::set()
	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m02 & m03 & m10 & m11 & m12 & m13 & m20 & m21 & m22 & m23 & m30 & m31 & m32 & m33; }

	// #TODO rename get...() to make...()
	static Matrix4 getTranslation(const Vector3<float>& v) noexcept { return Matrix4(Uninitialized()).setTranslation(v); }
	static Matrix4 getScaling(const Vector3<float>& v) noexcept { return Matrix4(Uninitialized()).setScaling(v); }
	static Matrix4 getRotation(Axis axis, float angle) noexcept { return Matrix4(Uninitialized()).setRotation(axis, angle); }
	static Matrix4 getRotation(const Vector3<float>& axis, float angle) noexcept { return Matrix4(Uninitialized()).setRotation(axis, angle); }
	static Matrix4 getRotation(const YawPitchRoll<float>& r) noexcept { return Matrix4(Uninitialized()).setRotation(r); }
	static Matrix4 getRotation(const Euler<float>& e) noexcept { return Matrix4(Uninitialized()).setRotation(e); }
	static Matrix4 getRotation(const Quaternion<float>& q) noexcept { return Matrix4(Uninitialized()).setRotation(q); }
	static Matrix4 getRotationTranslation(Axis axis, float angle, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static Matrix4 getRotationTranslation(const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static Matrix4 getRotationTranslation(const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(r, t); }
	static Matrix4 getRotationTranslation(const Euler<float>& e, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(e, t); }
	static Matrix4 getRotationTranslation(const Quaternion<float>& q, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setRotationTranslation(q, t); }
	static Matrix4 getScalingRotation(const Vector3<float>& s, Axis axis, float angle) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix4 getScalingRotation(const Vector3<float>& s, const Vector3<float>& axis, float angle) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, axis, angle); }
	static Matrix4 getScalingRotation(const Vector3<float>& s, const YawPitchRoll<float>& r) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, r); }
	static Matrix4 getScalingRotation(const Vector3<float>& s, const Euler<float>& e) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, e); }
	static Matrix4 getScalingRotation(const Vector3<float>& s, const Quaternion<float>& q) noexcept { return Matrix4(Uninitialized()).setScalingRotation(s, q); }
	static Matrix4 getScalingTranslation(const Vector3<float>& s, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setScalingTranslation(s, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<float>& s, Axis axis, float angle, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<float>& s, const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<float>& s, const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, r, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<float>& s, const Euler<float>& e, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, e, t); }
	static Matrix4 getScalingRotationTranslation(const Vector3<float>& s, const Quaternion<float>& q, const Vector3<float>& t) noexcept { return Matrix4(Uninitialized()).setScalingRotationTranslation(s, q, t); }
	static Matrix4 getShearing(float xy, float xz, float yx, float yz, float zx, float zy) noexcept { return Matrix4(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy); }
	static Matrix4 getReflection(const Plane<float>& plane) noexcept { return Matrix4(Uninitialized()).setReflection(plane); }

	bool isZero() const noexcept;
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept;
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Matrix4& m) const noexcept;
	bool isApproxEqual(const Matrix4& m, float tolerance) const noexcept;
	bool isFinite() const noexcept;
	const Vector4<float>& getRow(int i) const noexcept { return reinterpret_cast<const Vector4<float>&>((&row0)[i]); }
	Vector4<float> getColumn(int i) const noexcept;
	float getfloatrace() const noexcept { return m00 + m11 + m22 + m33; }
	float getDeterminant() const noexcept;
	bool isSingular() const noexcept { return (getDeterminant() == 0.f); }
	Matrix4& setZero/*zero*/() noexcept;
	Matrix4& setIdentity/*makeIdentity*/() noexcept;
	Matrix4& set(const Vector4<float>& row0, const Vector4<float>& row1, const Vector4<float>& row2, const Vector4<float>& row3) noexcept;
	Matrix4& set(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) noexcept;
	Matrix4& setTranslation/*makeTranslation*/(const Vector3<float>& v) noexcept;
	Matrix4& setScaling/*makeScaling*/(const Vector3<float>& v) noexcept;
	Matrix4& setRotation(Axis axis, float angle) noexcept;
	Matrix4& setRotation(const Vector3<float>& axis, float angle) noexcept;
	Matrix4& setRotation(const YawPitchRoll<float>& r) noexcept;
	Matrix4& setRotation(const Euler<float>& e) noexcept;
	Matrix4& setRotation(const Quaternion<float>& q) noexcept;
	Matrix4& setRotationTranslation(Axis axis, float angle, const Vector3<float>& t) noexcept;
	Matrix4& setRotationTranslation(const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept;
	Matrix4& setRotationTranslation(const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept;
	Matrix4& setRotationTranslation(const Euler<float>& e, const Vector3<float>& t) noexcept;
	Matrix4& setRotationTranslation(const Quaternion<float>& q, const Vector3<float>& t) noexcept;
	Matrix4& setScalingRotation(const Vector3<float>& s, Axis axis, float angle) noexcept;
	Matrix4& setScalingRotation(const Vector3<float>& s, const Vector3<float>& axis, float angle) noexcept;
	Matrix4& setScalingRotation(const Vector3<float>& s, const YawPitchRoll<float>& r) noexcept;
	Matrix4& setScalingRotation(const Vector3<float>& s, const Euler<float>& e) noexcept;
	Matrix4& setScalingRotation(const Vector3<float>& s, const Quaternion<float>& q) noexcept;
	Matrix4& setScalingTranslation(const Vector3<float>& s, const Vector3<float>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<float>& s, Axis axis, float angle, const Vector3<float>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<float>& s, const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<float>& s, const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<float>& s, const Euler<float>& e, const Vector3<float>& t) noexcept;
	Matrix4& setScalingRotationTranslation(const Vector3<float>& s, const Quaternion<float>& q, const Vector3<float>& t) noexcept;
	Matrix4& setShearing(float xy, float xz, float yx, float yz, float zx, float zy) noexcept;
	Matrix4& setReflection(const Plane<float>& plane) noexcept;
	Matrix4& setTranspose/*transposeOf*/(const Matrix4& m) noexcept;
	Matrix4& setInverse/*inverseOf*/(const Matrix4& m) noexcept;
	Matrix4& setInverseTranspose/*inverseTransposeOf*/(const Matrix4& m) noexcept;
	Matrix4& preConcatenate(const Matrix4& m) noexcept;
	Matrix4& concatenate(const Matrix4& m) noexcept;
	Matrix4& translate(const Vector3<float>& v) noexcept;
	Matrix4& preScale(const Vector3<float>& v) noexcept;
	Matrix4& scale(const Vector3<float>& v) noexcept;
	Matrix4& rotate(Axis axis, float angle) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix4& rotate(const Vector3<float>& axis, float angle) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(axis, angle)); return *this; }
	Matrix4& rotate(const YawPitchRoll<float>& r) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(r)); return *this; }
	Matrix4& rotate(const Euler<float>& e) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(e)); return *this; }
	Matrix4& rotate(const Quaternion<float>& q) noexcept { concatenate(Matrix4(Uninitialized()).setRotation(q)); return *this; }
	Matrix4& shear(float xy, float xz, float yx, float yz, float zx, float zy) noexcept { concatenate(Matrix4(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy)); return *this; }
	Matrix4& negate() noexcept;
	Matrix4& transpose() noexcept;
	Matrix4& invert() noexcept;

	//static const Matrix4& getZero() noexcept { return ZERO; }
	//static const Matrix4& getIdentity() noexcept { return IDENTITY; }

	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;

	union
	{
		simd::float4 row0;
		struct { float m00, m01, m02, m03; };
	};

	union
	{
		simd::float4 row1;
		struct { float m10, m11, m12, m13; };
	};

	union
	{
		simd::float4 row2;
		struct { float m20, m21, m22, m23; };
	};

	union
	{
		simd::float4 row3;
		struct { float m30, m31, m32, m33; };
	};
};

const Matrix4<float> Matrix4<float>::ZERO{};
const Matrix4<float> Matrix4<float>::IDENTITY{ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };

#endif /* SIMD_HAS_FLOAT4 */

#if SIMD_HAS_FLOAT4

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Matrix4<T>::Matrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33) :
	m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), 
	m30(m30), m31(m31), m32(m32), m33(m33) 
{
}

template<typename T>
inline Matrix4<T>::Matrix4(const Vector4<T>& row0, const Vector4<T>& row1, const Vector4<T>& row2, const Vector4<T>& row3) :
	m00(row0.x), m01(row0.y), m02(row0.z), m03(row0.w), m10(row1.x), m11(row1.y), m12(row1.z), m13(row1.w),
	m20(row2.x), m21(row2.y), m22(row2.z), m23(row2.w), m30(row3.x), m31(row3.y), m32(row3.z), m33(row3.w) 
{
}

template<typename T>
inline Matrix4<T>::Matrix4(const Matrix2<T>& m) : 
	m00(m.m00), m01(m.m01), m02(), m03(), m10(m.m10), m11(m.m11), m12(), m13(), m20(), m21(), m22(1), m23(), m30(), m31(), m32(), m33(1) 
{
}

template<typename T>
inline Matrix4<T>::Matrix4(const Matrix3<T>& m) : 
	m00(m.m00), m01(m.m01), m02(m.m02), m03(), m10(m.m10), m11(m.m11), m12(m.m12), m13(), m20(m.m20), m21(m.m21), m22(m.m22), m23(), 
	m30(), m31(), m32(), m33(1) 
{
}

template<typename T>
inline Matrix4<T>::Matrix4(const AffineTransform<T>& m) : 
	m00(m.m00), m01(m.m01), m02(m.m02), m03(), m10(m.m10), m11(m.m11), m12(m.m12), m13(),
	m20(m.m20), m21(m.m21), m22(m.m22), m23(), m30(m.x), m31(m.y), m32(m.z), m33(1) 
{
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setZero()
{
	m00 = T(); m01 = T(); m02 = T(); m03 = T();
	m10 = T(); m11 = T(); m12 = T(); m13 = T();
	m20 = T(); m21 = T(); m22 = T(); m23 = T();
	m30 = T(); m31 = T(); m32 = T(); m33 = T();
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setIdentity()
{
	m00 = T(1); m01 = T(); m02 = T(); m03 = T();
	m10 = T(); m11 = T(1); m12 = T(); m13 = T();
	m20 = T(); m21 = T(); m22 = T(1); m23 = T();
	m30 = T(); m31 = T(); m32 = T(); m33 = T(1);
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::set(const Vector4<T>& row0, const Vector4<T>& row1, const Vector4<T>& row2, const Vector4<T>& row3)
{
	m00 = row0.x; m01 = row0.y; m02 = row0.z; m03 = row0.w; 
	m10 = row1.x; m11 = row1.y; m12 = row1.z; m13 = row1.w; 
	m20 = row2.x; m21 = row2.y; m22 = row2.z; m23 = row2.w; 
	m30 = row3.x; m31 = row3.y; m32 = row3.z; m33 = row3.w; 
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::set(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, 
	T m30, T m31, T m32, T m33)
{
	this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
	this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13; 
	this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23; 
	this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33; 
	return *this;
}

#if SIMD_HAS_FLOAT4

inline Matrix4<float>::Matrix4() : 
	row0(simd::zero<simd::float4>()), 
	row1(simd::zero<simd::float4>()), 
	row2(simd::zero<simd::float4>()), 
	row3(simd::zero<simd::float4>()) 
{
}

inline Matrix4<float>::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) :
	row0(m00, m01, m02, m03),
	row1(m10, m11, m12, m13),
	row2(m20, m21, m22, m23),
	row3(m30, m31, m32, m33)
{
}

inline Matrix4<float>::Matrix4(const Vector4<float>& row0, const Vector4<float>& row1, const Vector4<float>& row2, 
	const Vector4<float>& row3) :
	row0(row0),
	row1(row1),
	row2(row2),
	row3(row3)
{
}

inline Matrix4<float>::Matrix4(const Matrix2<float>& m) : 
	row0(simd::cutoff2(m.row0)),
	row1(simd::cutoff2(m.row1)),
	row2(Vector4<float>::UNIT_Z),
	row3(Vector4<float>::UNIT_W)
{
}

inline Matrix4<float>::Matrix4(const Matrix3<float>& m) :
	row0(simd::cutoff3(m.row0)),
	row1(simd::cutoff3(m.row1)),
	row2(simd::cutoff3(m.row2)),
	row3(Vector4<float>::UNIT_W)
{
}

inline Matrix4<float>::Matrix4(const AffineTransform<float>& m) :
	row0(simd::cutoff3(m.row0)),
	row1(simd::cutoff3(m.row1)),
	row2(simd::cutoff3(m.row2)),
	row3(simd::insert3(m.row3, Vector4<float>::UNIT_W))
{
}

inline Matrix4<float>::Matrix4(const float* m) : 
	row0(simd::load4(m)),
	row1(simd::load4(m + 4)),
	row2(simd::load4(m + 8)),
	row3(simd::load4(m + 12))
{
}

inline bool Matrix4<float>::operator==(const Matrix4<float>& m) const
{
	return simd::all4(simd::equal(row0, m.row0)) && 
		simd::all4(simd::equal(row1, m.row1)) && 
		simd::all4(simd::equal(row2, m.row2)) &&
		simd::all4(simd::equal(row3, m.row3));
}

inline bool Matrix4<float>::isZero() const
{
	const auto zero = simd::zero<simd::float4>();
	return simd::all4(simd::equal(row0, zero)) && 
		simd::all4(simd::equal(row1, zero)) && 
		simd::all4(simd::equal(row2, zero)) && 
		simd::all4(simd::equal(row3, zero));
}

inline bool Matrix4<float>::isIdentity() const
{
	return simd::all4(simd::equal(row0, Vector4<float>::UNIT_X)) && 
		simd::all4(simd::equal(row1, Vector4<float>::UNIT_Y)) &&
		simd::all4(simd::equal(row2, Vector4<float>::UNIT_Z)) &&
		simd::all4(simd::equal(row3, Vector4<float>::UNIT_W));
}

inline bool Matrix4<float>::isFinite() const
{
	return simd::all4(simd::isFinite(row0)) &&
		simd::all4(simd::isFinite(row1)) &&
		simd::all4(simd::isFinite(row2)) &&
		simd::all4(simd::isFinite(row3));
}

inline Matrix4<float>& Matrix4<float>::setZero()
{
	row0 = simd::zero<simd::float4>();
	row1 = simd::zero<simd::float4>();
	row2 = simd::zero<simd::float4>();
	row3 = simd::zero<simd::float4>();
	return *this;
}

inline Matrix4<float>& Matrix4<float>::setIdentity()
{
	row0 = Vector4<float>::UNIT_X;
	row1 = Vector4<float>::UNIT_Y;
	row2 = Vector4<float>::UNIT_Z;
	row3 = Vector4<float>::UNIT_W;
	return *this;
}

inline Matrix4<float>& Matrix4<float>::set(const Vector4<float>& row0, const Vector4<float>& row1, const Vector4<float>& row2,
	const Vector4<float>& row3)
{
	this->row0 = row0;
	this->row1 = row1;
	this->row2 = row2;
	this->row3 = row3;
	return *this;
}

inline Matrix4<float>& Matrix4<float>::set(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
#if MATHEMATICS_SIMD_EXPAND_LAST
	row0 = 
	row1 = 
	row2 = 
	row3 = 
#else
	row0 =
	row1 =
	row2 =
	row3 =
#endif
	return *this;
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Matrix4f = templates::Matrix4<float>;
using Matrix4d = templates::Matrix4<double>;

#if MATHEMATICS_DOUBLE
using Matrix4 = templates::Matrix4<double>;
using Matrix4Arg = templates::Matrix4<double>::ConstArg;
using Matrix4Result = templates::Matrix4<double>::ConstResult;
#else
using Matrix4 = templates::Matrix4<float>;
using Matrix4Arg = templates::Matrix4<float>::ConstArg;
using Matrix4Result = templates::Matrix4<float>::ConstResult;
#endif

} // namespace core::mathematics

#include "YawPitchRoll.hpp"
#include "Euler.hpp"
#include "Quaternion.hpp"
#include "Plane.hpp"

namespace core::mathematics::templates {

#if SIMD_HAS_FLOAT4

#endif /* SIMD_HAS_FLOAT4 */

} // namespace core::mathematics::templates

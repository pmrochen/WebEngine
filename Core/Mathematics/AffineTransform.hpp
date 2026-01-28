/*
 *	Name: AffineTransform
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

namespace core::mathematics {

struct Orthogonal
{
};

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
struct AffineTransform
{
	using Real = T;
	using ConstArg = const AffineTransform&;
	using ConstResult = const AffineTransform&;

	//static constexpr int NUM_COMPONENTS = 12;

	constexpr AffineTransform() noexcept : m00(), m01(), m02(), m10(), m11(), m12(), m20(), m21(), m22(), m30(), m31(), m32() {}
	explicit AffineTransform(Uninitialized) noexcept {}
	//explicit AffineTransform(Identity) noexcept {}
	constexpr AffineTransform(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22, T m30, T m31, T m32) noexcept;
	constexpr AffineTransform(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2, const Vector3<T>& row3) noexcept;
	constexpr AffineTransform(const Matrix2<T>& m) noexcept;
	constexpr AffineTransform(const Matrix3<T>& m) noexcept;
	constexpr AffineTransform(const Matrix3<T>& r, const Vector3<T>& t) noexcept;
	AffineTransform(const Vector3<T>& forward, const Vector3<T>& origin) noexcept;
	AffineTransform(const Vector3<T>& up, const Vector3<T>& forward, const Vector3<T>& origin) noexcept;
	explicit AffineTransform(const T* m) noexcept;

	//explicit operator T*() noexcept { return &m00; }
	//explicit operator const T*() const noexcept { return &m00; }
	Vector3<T>& operator[](int i) noexcept { return reinterpret_cast<Vector3<T>*>(&m00)[i]; }
	const Vector3<T>& operator[](int i) const noexcept { return reinterpret_cast<const Vector3<T>*>(&m00)[i]; }

	bool operator==(const AffineTransform& m) const noexcept;
	bool operator!=(const AffineTransform& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, AffineTransform& m);
	friend std::ostream& operator<<(std::ostream& s, const AffineTransform& m);

	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m02 & m10 & m11 & m12 & m20 & m21 & m22 & m30 & m31 & m32; }

	// #TODO rename get...() to make...()
	static AffineTransform getTranslation(const Vector3<T>& v) noexcept { return AffineTransform(Uninitialized()).setTranslation(v); }
	static AffineTransform getScaling(const Vector3<T>& v) noexcept { return AffineTransform(Uninitialized()).setScaling(v); }
	static AffineTransform getScaling(T f) noexcept { return AffineTransform(Uninitialized()).setScaling(f); }
	static AffineTransform getRotation(Axis axis, T angle) noexcept { return AffineTransform(Uninitialized()).setRotation(axis, angle); }
	static AffineTransform getRotation(const Vector3<T>& axis, T angle) noexcept { return AffineTransform(Uninitialized()).setRotation(axis, angle); }
	static AffineTransform getRotation(const YawPitchRoll<T>& r) noexcept { return AffineTransform(Uninitialized()).setRotation(r); }
	static AffineTransform getRotation(const Euler<T>& e) noexcept { return AffineTransform(Uninitialized()).setRotation(e); }
	static AffineTransform getRotation(const Quaternion<T>& q) noexcept { return AffineTransform(Uninitialized()).setRotation(q); }
	static AffineTransform getRotationTranslation(Axis axis, T angle, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static AffineTransform getRotationTranslation(const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static AffineTransform getRotationTranslation(const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(r, t); }
	static AffineTransform getRotationTranslation(const Euler<T>& e, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(e, t); }
	static AffineTransform getRotationTranslation(const Quaternion<T>& q, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(q, t); }
	static AffineTransform getScalingRotation(const Vector3<T>& s, Axis axis, T angle) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, axis, angle); }
	static AffineTransform getScalingRotation(const Vector3<T>& s, const Vector3<T>& axis, T angle) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, axis, angle); }
	static AffineTransform getScalingRotation(const Vector3<T>& s, const YawPitchRoll<T>& r) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, r); }
	static AffineTransform getScalingRotation(const Vector3<T>& s, const Euler<T>& e) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, e); }
	static AffineTransform getScalingRotation(const Vector3<T>& s, const Quaternion<T>& q) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, q); }
	static AffineTransform getScalingTranslation(const Vector3<T>& s, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setScalingTranslation(s, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<T>& s, Axis axis, T angle, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<T>& s, const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<T>& s, const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, r, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<T>& s, const Euler<T>& e, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, e, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<T>& s, const Quaternion<T>& q, const Vector3<T>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, q, t); }
	static AffineTransform getShearing(T xy, T xz, T yx, T yz, T zx, T zy) noexcept { return AffineTransform(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy); }
	static AffineTransform getReflection(const Plane<T>& plane) noexcept { return AffineTransform(Uninitialized()).setReflection(plane); }

	//bool isZero() const noexcept;
	//bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept;
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const AffineTransform& m) const noexcept;
	bool isApproxEqual(const AffineTransform& m, T tolerance) const noexcept;
	bool isApproxOrthogonal() const noexcept { return getBasis().isApproxOrthogonal(); }
	bool hasApproxUniformScaling() const noexcept { return getBasis().hasApproxUniformScaling(); }
	bool isFinite() const noexcept;
	const Vector3<T>& getRow(int i) const noexcept { return reinterpret_cast<const Vector3<T>*>(&m00)[i]; }
	Vector4<T> getColumn(int i) const noexcept;
	Vector3<T> getScaleComponent() const noexcept { return getBasis().getScaleComponent(); }
	//Vector3<T>& getOrigin() noexcept { return *reinterpret_cast<Vector3<T>*>(&m30); }
	const Vector3<T>& getOrigin() const noexcept { return *reinterpret_cast<const Vector3<T>*>(&m30); }
	void setOrigin(const Vector3<T>& t) noexcept { *reinterpret_cast<Vector3<T>*>(&m30) = t; }
	//Matrix3<T>& getBasis() noexcept { return *reinterpret_cast<Matrix3<T>*>(&m00); }
	const Matrix3<T>& getBasis() const noexcept { return *reinterpret_cast<const Matrix3<T>*>(&m00); }
	void setBasis(const Matrix3<T>& m) noexcept { *reinterpret_cast<Matrix3<T>*>(&m00) = m; }
	AffineTransform& setZero/*zero*/() noexcept;
	AffineTransform& setIdentity/*makeIdentity*/() noexcept;
	AffineTransform& set(const Matrix3<T>& r, const Vector3<T>& t) noexcept;
	AffineTransform& set(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2, const Vector3<T>& row3) noexcept;
	AffineTransform& set(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22, T m30, T m31, T m32) noexcept;
	AffineTransform& setTranslation/*makeTranslation*/(const Vector3<T>& v) noexcept;
	AffineTransform& setScaling/*makeScaling*/(const Vector3<T>& v) noexcept;
	AffineTransform& setScaling/*makeScaling*/(T f) noexcept;
	AffineTransform& setRotation(Axis axis, T angle) noexcept;
	AffineTransform& setRotation(const Vector3<T>& axis, T angle) noexcept;
	AffineTransform& setRotation(const YawPitchRoll<T>& r) noexcept;
	AffineTransform& setRotation(const Euler<T>& e) noexcept;
	AffineTransform& setRotation(const Quaternion<T>& q) noexcept;
	AffineTransform& setRotationTranslation(Axis axis, T angle, const Vector3<T>& t) noexcept;
	AffineTransform& setRotationTranslation(const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept;
	AffineTransform& setRotationTranslation(const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept;
	AffineTransform& setRotationTranslation(const Euler<T>& e, const Vector3<T>& t) noexcept;
	AffineTransform& setRotationTranslation(const Quaternion<T>& q, const Vector3<T>& t) noexcept;
	AffineTransform& setScalingRotation(const Vector3<T>& s, Axis axis, T angle) noexcept;
	AffineTransform& setScalingRotation(const Vector3<T>& s, const Vector3<T>& axis, T angle) noexcept;
	AffineTransform& setScalingRotation(const Vector3<T>& s, const YawPitchRoll<T>& r) noexcept;
	AffineTransform& setScalingRotation(const Vector3<T>& s, const Euler<T>& e) noexcept;
	AffineTransform& setScalingRotation(const Vector3<T>& s, const Quaternion<T>& q) noexcept;
	AffineTransform& setScalingTranslation(const Vector3<T>& s, const Vector3<T>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<T>& s, Axis axis, T angle, const Vector3<T>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<T>& s, const Vector3<T>& axis, T angle, const Vector3<T>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<T>& s, const YawPitchRoll<T>& r, const Vector3<T>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<T>& s, const Euler<T>& e, const Vector3<T>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<T>& s, const Quaternion<T>& q, const Vector3<T>& t) noexcept;
	AffineTransform& setShearing(T xy, T xz, T yx, T yz, T zx, T zy) noexcept;
	AffineTransform& setReflection(const Plane<T>& plane) noexcept;
	AffineTransform& setInverse/*inverseOf*/(const AffineTransform& m) noexcept;
	template<typename U = void> AffineTransform& setInverse/*inverseOf*/(const AffineTransform& m) noexcept;
	//template<> AffineTransform& setInverse<Orthogonal>/*inverseOfOrthogonal*/(const AffineTransform& m) noexcept;
	AffineTransform& preConcatenate(const AffineTransform& m) noexcept;
	AffineTransform& concatenate(const AffineTransform& m) noexcept;
	AffineTransform& translate(const Vector3<T>& v) noexcept { m30 += v.x; m31 += v.y; m32 += v.z; return *this; }
	AffineTransform& preScale(const Vector3<T>& v) noexcept;
	AffineTransform& preScale(T f) noexcept;
	AffineTransform& scale(const Vector3<T>& v) noexcept;
	AffineTransform& scale(T f) noexcept;
	AffineTransform& rotate(Axis axis, T angle) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(axis, angle)); return *this; }
	AffineTransform& rotate(const Vector3<T>& axis, T angle) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(axis, angle)); return *this; }
	AffineTransform& rotate(const YawPitchRoll<T>& r) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(r)); return *this; }
	AffineTransform& rotate(const Euler<T>& e) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(e)); return *this; }
	AffineTransform& rotate(const Quaternion<T>& q) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(q)); return *this; }
	AffineTransform& shear(T xy, T xz, T yx, T yz, T zx, T zy) noexcept { concatenate(AffineTransform(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy)); return *this; }
	AffineTransform& invert() noexcept;
	template<typename U = void> AffineTransform& invert() noexcept;
	//template<> AffineTransform& invert<Orthogonal>/*invertOrthogonal*/() noexcept;
	AffineTransform& orthonormalize() noexcept { reinterpret_cast<Matrix3<T>&>(*this).orthonormalize(); return *this; }

	//static const AffineTransform& getZero() noexcept { return ZERO; }
	//static const AffineTransform& getIdentity() noexcept { return IDENTITY; }

	static const AffineTransform ZERO;
	static const AffineTransform IDENTITY;

	T m00, m01, m02;
	T m10, m11, m12;
	T m20, m21, m22;
	T m30, m31, m32;
};

template<typename T> const AffineTransform<T> AffineTransform<T>::ZERO{};
template<typename T> const AffineTransform<T> AffineTransform<T>::IDENTITY{ T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0) };

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
struct AffineTransform<float>
{
	using Real = float;
	using ConstArg = const AffineTransform;
	using ConstResult = const AffineTransform;

	//static constexpr int NUM_COMPONENTS = 12;

	constexpr AffineTransform() noexcept;
	explicit AffineTransform(Uninitialized) noexcept {}
	//explicit AffineTransform(Identity) noexcept {}
	constexpr AffineTransform(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32) noexcept;
	constexpr AffineTransform(const Vector3<float>& row0, const Vector3<float>& row1, const Vector3<float>& row2, const Vector3<float>& row3) noexcept;
	constexpr AffineTransform(const Matrix2<float>& m) noexcept;
	constexpr AffineTransform(const Matrix3<float>& m) noexcept;
	constexpr AffineTransform(const Matrix3<float>& r, const Vector3<float>& t) noexcept;
	AffineTransform(const Vector3<float>& forward, const Vector3<float>& origin) noexcept;
	AffineTransform(const Vector3<float>& up, const Vector3<float>& forward, const Vector3<float>& origin) noexcept;
	explicit AffineTransform(const float* m) noexcept;
	explicit AffineTransform(const simd::float4* m) noexcept : row0(m[0]), row1(m[1]), row2(m[2]), row3(m[3]) {}
	AffineTransform(const AffineTransform& m) noexcept : row0(m.row0), row1(m.row1), row2(m.row2), row3(m.row3) {}
	AffineTransform& operator=(const AffineTransform& m) noexcept { row0 = m.row0; row1 = m.row1; row2 = m.row2; row3 = m.row3; return *this; }

	//explicit operator float*() noexcept { return &m00; }
	//explicit operator const float*() const noexcept { return &m00; }
	explicit operator simd::float4* () noexcept { return &row0; }
	explicit operator const simd::float4* () const noexcept { return &row0; }
	Vector3<float>& operator[](int i) noexcept { return reinterpret_cast<Vector3<float>&>((&row0)[i]); }
	const Vector3<float>& operator[](int i) const noexcept { return reinterpret_cast<const Vector3<float>&>((&row0)[i]); }

	bool operator==(const AffineTransform& m) const noexcept;
	bool operator!=(const AffineTransform& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, AffineTransform& m);
	friend std::ostream& operator<<(std::ostream& s, const AffineTransform& m);

	// #FIXME use simd::set()
	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m02 & m10 & m11 & m12 & m20 & m21 & m22 & m30 & m31 & m32; }

	// #TODO rename get...() to make...()
	static AffineTransform getTranslation(const Vector3<float>& v) noexcept { return AffineTransform(Uninitialized()).setTranslation(v); }
	static AffineTransform getScaling(const Vector3<float>& v) noexcept { return AffineTransform(Uninitialized()).setScaling(v); }
	static AffineTransform getScaling(float f) noexcept { return AffineTransform(Uninitialized()).setScaling(f); }
	static AffineTransform getRotation(Axis axis, float angle) noexcept { return AffineTransform(Uninitialized()).setRotation(axis, angle); }
	static AffineTransform getRotation(const Vector3<float>& axis, float angle) noexcept { return AffineTransform(Uninitialized()).setRotation(axis, angle); }
	static AffineTransform getRotation(const YawPitchRoll<float>& r) noexcept { return AffineTransform(Uninitialized()).setRotation(r); }
	static AffineTransform getRotation(const Euler<float>& e) noexcept { return AffineTransform(Uninitialized()).setRotation(e); }
	static AffineTransform getRotation(const Quaternion<float>& q) noexcept { return AffineTransform(Uninitialized()).setRotation(q); }
	static AffineTransform getRotationTranslation(Axis axis, float angle, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static AffineTransform getRotationTranslation(const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(axis, angle, t); }
	static AffineTransform getRotationTranslation(const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(r, t); }
	static AffineTransform getRotationTranslation(const Euler<float>& e, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(e, t); }
	static AffineTransform getRotationTranslation(const Quaternion<float>& q, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setRotationTranslation(q, t); }
	static AffineTransform getScalingRotation(const Vector3<float>& s, Axis axis, float angle) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, axis, angle); }
	static AffineTransform getScalingRotation(const Vector3<float>& s, const Vector3<float>& axis, float angle) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, axis, angle); }
	static AffineTransform getScalingRotation(const Vector3<float>& s, const YawPitchRoll<float>& r) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, r); }
	static AffineTransform getScalingRotation(const Vector3<float>& s, const Euler<float>& e) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, e); }
	static AffineTransform getScalingRotation(const Vector3<float>& s, const Quaternion<float>& q) noexcept { return AffineTransform(Uninitialized()).setScalingRotation(s, q); }
	static AffineTransform getScalingTranslation(const Vector3<float>& s, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setScalingTranslation(s, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<float>& s, Axis axis, float angle, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<float>& s, const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, axis, angle, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<float>& s, const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, r, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<float>& s, const Euler<float>& e, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, e, t); }
	static AffineTransform getScalingRotationTranslation(const Vector3<float>& s, const Quaternion<float>& q, const Vector3<float>& t) noexcept { return AffineTransform(Uninitialized()).setScalingRotationTranslation(s, q, t); }
	static AffineTransform getShearing(float xy, float xz, float yx, float yz, float zx, float zy) noexcept { return AffineTransform(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy); }
	static AffineTransform getReflection(const Plane<float>& plane) noexcept { return AffineTransform(Uninitialized()).setReflection(plane); }

	//bool isZero() const noexcept;
	//bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept;
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const AffineTransform& m) const noexcept;
	bool isApproxEqual(const AffineTransform& m, float tolerance) const noexcept;
	bool isApproxOrthogonal() const noexcept { return getBasis().isApproxOrthogonal(); }
	bool hasApproxUniformScaling() const noexcept { return getBasis().hasApproxUniformScaling(); }
	bool isFinite() const noexcept;
	const Vector3<float>& getRow(int i) const noexcept { return reinterpret_cast<const Vector3<float>&>((&row0)[i]); }
	Vector4<float> getColumn(int i) const noexcept;
	Vector3<float> getScaleComponent() const noexcept { return getBasis().getScaleComponent(); }
	//Vector3<float>& getOrigin() noexcept { return *reinterpret_cast<Vector3<float>*>(&m30); }
	const Vector3<float>& getOrigin() const noexcept { return *reinterpret_cast<const Vector3<float>*>(&m30); }
	void setOrigin(const Vector3<float>& t) noexcept { *reinterpret_cast<Vector3<float>*>(&m30) = t; }
	//Matrix3<float>& getBasis() noexcept { return *reinterpret_cast<Matrix3<float>*>(&m00); }
	const Matrix3<float>& getBasis() const noexcept { return *reinterpret_cast<const Matrix3<float>*>(&m00); }
	void setBasis(const Matrix3<float>& m) noexcept { *reinterpret_cast<Matrix3<float>*>(&m00) = m; }
	AffineTransform& setZero/*zero*/() noexcept;
	AffineTransform& setIdentity/*makeIdentity*/() noexcept;
	AffineTransform& set(const Matrix3<float>& r, const Vector3<float>& t) noexcept;
	AffineTransform& set(const Vector3<float>& row0, const Vector3<float>& row1, const Vector3<float>& row2, const Vector3<float>& row3) noexcept;
	AffineTransform& set(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32) noexcept;
	AffineTransform& setTranslation/*makeTranslation*/(const Vector3<float>& v) noexcept;
	AffineTransform& setScaling/*makeScaling*/(const Vector3<float>& v) noexcept;
	AffineTransform& setScaling/*makeScaling*/(float f) noexcept;
	AffineTransform& setRotation(Axis axis, float angle) noexcept;
	AffineTransform& setRotation(const Vector3<float>& axis, float angle) noexcept;
	AffineTransform& setRotation(const YawPitchRoll<float>& r) noexcept;
	AffineTransform& setRotation(const Euler<float>& e) noexcept;
	AffineTransform& setRotation(const Quaternion<float>& q) noexcept;
	AffineTransform& setRotationTranslation(Axis axis, float angle, const Vector3<float>& t) noexcept;
	AffineTransform& setRotationTranslation(const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept;
	AffineTransform& setRotationTranslation(const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept;
	AffineTransform& setRotationTranslation(const Euler<float>& e, const Vector3<float>& t) noexcept;
	AffineTransform& setRotationTranslation(const Quaternion<float>& q, const Vector3<float>& t) noexcept;
	AffineTransform& setScalingRotation(const Vector3<float>& s, Axis axis, float angle) noexcept;
	AffineTransform& setScalingRotation(const Vector3<float>& s, const Vector3<float>& axis, float angle) noexcept;
	AffineTransform& setScalingRotation(const Vector3<float>& s, const YawPitchRoll<float>& r) noexcept;
	AffineTransform& setScalingRotation(const Vector3<float>& s, const Euler<float>& e) noexcept;
	AffineTransform& setScalingRotation(const Vector3<float>& s, const Quaternion<float>& q) noexcept;
	AffineTransform& setScalingTranslation(const Vector3<float>& s, const Vector3<float>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<float>& s, Axis axis, float angle, const Vector3<float>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<float>& s, const Vector3<float>& axis, float angle, const Vector3<float>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<float>& s, const YawPitchRoll<float>& r, const Vector3<float>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<float>& s, const Euler<float>& e, const Vector3<float>& t) noexcept;
	AffineTransform& setScalingRotationTranslation(const Vector3<float>& s, const Quaternion<float>& q, const Vector3<float>& t) noexcept;
	AffineTransform& setShearing(float xy, float xz, float yx, float yz, float zx, float zy) noexcept;
	AffineTransform& setReflection(const Plane<float>& plane) noexcept;
	AffineTransform& setInverse/*inverseOf*/(const AffineTransform& m) noexcept;
	template<typename U = void> AffineTransform& setInverse/*inverseOf*/(const AffineTransform& m) noexcept;
	//template<> AffineTransform& setInverse<Orthogonal>/*inverseOfOrthogonal*/(const AffineTransform& m) noexcept;
	AffineTransform& preConcatenate(const AffineTransform& m) noexcept;
	AffineTransform& concatenate(const AffineTransform& m) noexcept;
	AffineTransform& translate(const Vector3<float>& v) noexcept { row3 = simd::add4(row3, v); return *this; }
	AffineTransform& preScale(const Vector3<float>& v) noexcept;
	AffineTransform& preScale(float f) noexcept;
	AffineTransform& scale(const Vector3<float>& v) noexcept;
	AffineTransform& scale(float f) noexcept;
	AffineTransform& rotate(Axis axis, float angle) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(axis, angle)); return *this; }
	AffineTransform& rotate(const Vector3<float>& axis, float angle) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(axis, angle)); return *this; }
	AffineTransform& rotate(const YawPitchRoll<float>& r) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(r)); return *this; }
	AffineTransform& rotate(const Euler<float>& e) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(e)); return *this; }
	AffineTransform& rotate(const Quaternion<float>& q) noexcept { concatenate(AffineTransform(Uninitialized()).setRotation(q)); return *this; }
	AffineTransform& shear(float xy, float xz, float yx, float yz, float zx, float zy) noexcept { concatenate(AffineTransform(Uninitialized()).setShearing(xy, xz, yx, yz, zx, zy)); return *this; }
	AffineTransform& invert() noexcept;
	template<typename U = void> AffineTransform& invert() noexcept;
	//template<> AffineTransform& invert<Orthogonal>/*invertOrthogonal*/() noexcept;
	AffineTransform& orthonormalize() noexcept { reinterpret_cast<Matrix3<float>&>(*this).orthonormalize(); return *this; }

	//static const AffineTransform& getZero() noexcept { return ZERO; }
	//static const AffineTransform& getIdentity() noexcept { return IDENTITY; }

	static const AffineTransform ZERO;
	static const AffineTransform IDENTITY;

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

	union
	{
		simd::float4 row3;
		struct { float m30, m31, m32; };
	};
};

const AffineTransform<float> AffineTransform<float>::ZERO{};
const AffineTransform<float> AffineTransform<float>::IDENTITY{ 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };

#endif /* SIMD_HAS_FLOAT4 */

#if SIMD_HAS_FLOAT4

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline AffineTransform<T>::AffineTransform(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22, T m30, T m31, T m32) :
	m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22), m30(m30), m31(m31), m32(m32)
{
}

template<typename T>
inline AffineTransform<T>::AffineTransform(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2, const Vector3<T>& row3) :
	m00(row0.x), m01(row0.y), m02(row0.z), m10(row1.x), m11(row1.y), m12(row1.z), m20(row2.x), m21(row2.y), m22(row2.z),
	m30(row3.x), m31(row3.y), m32(row3.z)
{
}

template<typename T>
inline AffineTransform<T>::AffineTransform(const Matrix2<T>& m) :
	m00(m.m00), m01(m.m01), m02(), m10(m.m10), m11(m.m11), m12(), m20(), m21(), m22(1), m30(), m31(), m32()
{
}

template<typename T>
inline AffineTransform<T>::AffineTransform(const Matrix3<T>& m) :
	m00(m.m00), m01(m.m01), m02(m.m02), m10(m.m10), m11(m.m11), m12(m.m12), m20(m.m20), m21(m.m21), m22(m.m22), m30(), m31(), m32()
{
}

template<typename T>
inline AffineTransform<T>::AffineTransform(const Matrix3<T>& r, const Vector3<T>& t) :
	m00(r.m00), m01(r.m01), m02(r.m02), m10(r.m10), m11(r.m11), m12(r.m12), m20(r.m20), m21(r.m21), m22(r.m22), m30(t.x), m31(t.y), m32(t.z)
{
}

template<typename T>
inline AffineTransform<T>& AffineTransform<T>::setZero()
{
	m00 = T(); m01 = T(); m02 = T();
	m10 = T(); m11 = T(); m12 = T();
	m20 = T(); m21 = T(); m22 = T();
	m30 = T(); m31 = T(); m32 = T();
	return *this;
}

template<typename T>
inline AffineTransform<T>& AffineTransform<T>::setIdentity()
{
	m00 = T(1); m01 = T(); m02 = T();
	m10 = T(); m11 = T(1); m12 = T();
	m20 = T(); m21 = T(); m22 = T(1);
	m30 = T(); m31 = T(); m32 = T();
	return *this;
}

template<typename T>
inline AffineTransform<T>& AffineTransform<T>::set(const Matrix3<T>& r, const Vector3<T>& t)
{
	m00 = r.m00; m01 = r.m01; m02 = r.m02; 
	m10 = r.m10; m11 = r.m11; m12 = r.m12;
	m20 = r.m20; m21 = r.m21; m22 = r.m22; 
	m30 = t.x; m31 = t.y; m32 = t.z; 
	return *this;
}

template<typename T>
inline AffineTransform<T>& AffineTransform<T>::set(const Vector3<T>& row0, const Vector3<T>& row1, const Vector3<T>& row2, 
	const Vector3<T>& row3)
{
	m00 = row0.x; m01 = row0.y; m02 = row0.z; 
	m10 = row1.x; m11 = row1.y; m12 = row1.z; 
	m20 = row2.x; m21 = row2.y; m22 = row2.z;
	m30 = row3.x; m31 = row3.y; m32 = row3.z;
	return *this;
}

template<typename T>
inline AffineTransform<T>& AffineTransform<T>::set(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22, T m30, T m31, T m32)
{
	this->m00 = m00; this->m01 = m01; this->m02 = m02; 
	this->m10 = m10; this->m11 = m11; this->m12 = m12; 
	this->m20 = m20; this->m21 = m21; this->m22 = m22; 
	this->m30 = m30; this->m31 = m31; this->m32 = m32; 
	return *this;
}

#if SIMD_HAS_FLOAT4

inline AffineTransform<float>::AffineTransform() : 
	row0(simd::zero<simd::float4>()), 
	row1(simd::zero<simd::float4>()), 
	row2(simd::zero<simd::float4>()), 
	row3(simd::zero<simd::float4>()) 
{
}

inline AffineTransform<float>::AffineTransform(float m00, float m01, float m02, float m10, float m11, float m12, 
	float m20, float m21, float m22, float m30, float m31, float m32) :
#if MATHEMATICS_SIMD_EXPAND_LAST
	row0(simd::set4(m00, m01, m02, m02)),
	row1(simd::set4(m10, m11, m12, m12)),
	row2(simd::set4(m20, m21, m22, m22)),
	row3(simd::set4(m30, m31, m32, m32))
#else
	row0(simd::set3(m00, m01, m02)),
	row1(simd::set3(m10, m11, m12)),
	row2(simd::set3(m20, m21, m22)),
	row3(simd::set3(m30, m31, m32))
#endif
{
}

inline AffineTransform<float>::AffineTransform(const Vector3<float>& row0, const Vector3<float>& row1, const Vector3<float>& row2,
	const Vector3<float>& row3) :
	row0(row0),
	row1(row1),
	row2(row2),
	row3(row3)
{
}

inline AffineTransform<float>::AffineTransform(const Matrix2<float>& m) :
	row0(simd::cutoff2(m.row0)),
	row1(simd::cutoff2(m.row1)),
	row2(Vector3<float>::UNIT_Z),
	row3(),
{
}

inline AffineTransform<float>::AffineTransform(const Matrix3<float>& m) :
	row0(m.row0),
	row1(m.row1),
	row2(m.row2),
	row3(),
{
}

inline AffineTransform<float>::AffineTransform(const Matrix3<float>& r, const Vector3<float>& t) :
	row0(r.row0),
	row1(r.row1),
	row2(r.row2),
	row3(t)
{
}

#if MATHEMATICS_SIMD_EXPAND_LAST
inline AffineTransform<float>::AffineTransform(const float* m) :
	AffineTransform(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11])
{
}
#else
inline AffineTransform<float>::AffineTransform(const float* m)
{
	simd::unpack4x3(m, row0, row1, row2, row3);
}
#endif

inline bool AffineTransform<float>::operator==(const AffineTransform<float>& m) const
{
	return simd::all3(simd::equal(row0, m.row0)) && 
		simd::all3(simd::equal(row1, m.row1)) && 
		simd::all3(simd::equal(row2, m.row2)) &&
		simd::all3(simd::equal(row3, m.row3));
}

inline bool AffineTransform<float>::isZero() const
{
	const auto zero = simd::zero<simd::float4>();
	return simd::all3(simd::equal(row0, zero)) && 
		simd::all3(simd::equal(row1, zero)) && 
		simd::all3(simd::equal(row2, zero)) && 
		simd::all3(simd::equal(row3, zero));
}

inline bool AffineTransform<float>::isIdentity() const
{
	return simd::all3(simd::equal(row0, Vector3<float>::UNIT_X)) &&
		simd::all3(simd::equal(row1, Vector3<float>::UNIT_Y)) &&
		simd::all3(simd::equal(row2, Vector3<float>::UNIT_Z)) &&
		simd::all3(simd::equal(row3, simd::zero<simd::float4>()));
}

inline bool AffineTransform<float>::isFinite() const
{
	return simd::all3(simd::isFinite(row0)) &&
		simd::all3(simd::isFinite(row1)) &&
		simd::all3(simd::isFinite(row2)) &&
		simd::all3(simd::isFinite(row3));
}

inline AffineTransform<float>& AffineTransform<float>::setZero()
{
	row0 = simd::zero<simd::float4>();
	row1 = simd::zero<simd::float4>();
	row2 = simd::zero<simd::float4>();
	row3 = simd::zero<simd::float4>();
	return *this;
}

inline AffineTransform<float>& AffineTransform<float>::setIdentity()
{
	row0 = Vector3<float>::UNIT_X;
	row1 = Vector3<float>::UNIT_Y;
	row2 = Vector3<float>::UNIT_Z;
	row3 = simd::zero<simd::float4>();
	return *this;
}

inline AffineTransform<float>& AffineTransform<float>::set(const Matrix3<float>& r, const Vector3<float>& t)
{
	row0 = r.row0;
	row1 = r.row1;
	row2 = r.row2;
	row3 = t;
	return *this;
}

inline AffineTransform<float>& AffineTransform<float>::set(const Vector3<float>& row0, const Vector3<float>& row1, 
	const Vector3<float>& row2, const Vector3<float>& row3)
{
	this->row0 = row0;
	this->row1 = row1;
	this->row2 = row2;
	this->row3 = row3;
	return *this;
}

inline AffineTransform<float>& AffineTransform<float>::set(float m00, float m01, float m02, float m10, float m11, float m12,
	float m20, float m21, float m22, float m30, float m31, float m32)
{
#if MATHEMATICS_SIMD_EXPAND_LAST
	row0 = simd::set4(m00, m01, m02, m02);
	row1 = simd::set4(m10, m11, m12, m12);
	row2 = simd::set4(m20, m21, m22, m22);
	row3 = simd::set4(m30, m31, m32, m32);
#else
	row0 = simd::set3(m00, m01, m02);
	row1 = simd::set3(m10, m11, m12);
	row2 = simd::set3(m20, m21, m22);
	row3 = simd::set3(m30, m31, m32);
#endif
	return *this;
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

//using AffineTransformf = templates::AffineTransform<float>;
//using AffineTransformd = templates::AffineTransform<double>;

#if MATHEMATICS_DOUBLE
using AffineTransform = templates::AffineTransform<double>;
using AffineTransformArg = templates::AffineTransform<double>::ConstArg;
using AffineTransformResult = templates::AffineTransform<double>::ConstResult;
#else
using AffineTransform = templates::AffineTransform<float>;
using AffineTransformArg = templates::AffineTransform<float>::ConstArg;
using AffineTransformResult = templates::AffineTransform<float>::ConstResult;
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

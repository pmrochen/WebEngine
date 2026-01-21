/*
 *	Name: Vector3
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <Simd/Intrinsics.hpp>
#include <Tuples/Tuple3.hpp>
#include "Constants.hpp"
#include "Axis.hpp"
#include "Scalar.hpp"
#include "Vector2.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct IntVector3;

template<typename T>
struct Quaternion;

template<typename T>
struct Matrix3;

template<typename T>
struct AffineTransform;

template<typename T>
struct Vector3
{
	using Real = T;
	using ConstArg = const Vector3&;
	using ConstResult = const Vector3&;

	static constexpr int NUM_COMPONENTS = 3;

	constexpr Vector3() noexcept : x(), y(), z() {}
	constexpr explicit Vector3(T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
	constexpr Vector3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
	constexpr Vector3(Vector2<T>::ConstArg v) noexcept : x(v.x), y(v.y), z() {}
	constexpr Vector3(Vector2<T>::ConstArg v, T z) noexcept : x(v.x), y(v.y), z(z) {}
	template<typename U> explicit Vector3(const IntVector3<U>& v) noexcept;
	explicit Vector3(const tuples::templates::Tuple3<T>& t) noexcept : x(t.x), y(t.y), z(t.z) {}
	template<typename U> explicit Vector3(const tuples::templates::Tuple3<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)) {}
	explicit Vector3(const std::tuple<T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)) {}
	template<typename U> explicit Vector3(const std::tuple<U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))) {}
	explicit Vector3(const T* v) noexcept { x = v[0]; y = v[1]; z = v[2]; }
	explicit Vector3(Axis axis) noexcept : x((axis == Axis::X) ? T(1) : T(0)), y((axis == Axis::Y) ? T(1) : T(0)), z((axis == Axis::Z) ? T(1) : T(0)) {}

	explicit operator tuples::templates::Tuple3<T>() noexcept { return tuples::templates::Tuple3<T>(x, y, z); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(x), U(y), U(z)); }
	explicit operator std::tuple<T, T, T>() { return std::tuple<T, T, T>(x, y, z); }
	template<typename U> explicit operator std::tuple<U, U, U>() { return std::tuple<U, U, U>(U(x), U(y), U(z)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	Vector3 operator+() const noexcept { return *this; }
	Vector3 operator-() const noexcept { return Vector3(-x, -y, -z); }
	Vector3& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector3& operator*=(T f) noexcept { x *= f; y *= f; z *= f; return *this; }
	Vector3& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; z /= v.z; return *this; }
	Vector3& operator/=(T f) noexcept { T s = T(1)/f; x *= s; y *= s; z *= s; return *this; }
	Vector3& operator*=(const Matrix3<T>& m) noexcept; // #TODO
	friend Vector3 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
	friend Vector3 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	friend Vector3 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z); }
	friend Vector3 operator*(T f, ConstArg v) noexcept { return Vector3(f*v.x, f*v.y, f*v.z); }
	friend Vector3 operator*(ConstArg v, T f) noexcept { return Vector3(v.x*f, v.y*f, v.z*f); }
	friend Vector3 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z); }
	friend Vector3 operator/(T f, ConstArg v) noexcept { return Vector3(f/v.x, f/v.y, f/v.z); }
	friend Vector3 operator/(ConstArg v, T f) noexcept { T s = T(1)/f; return Vector3(v.x*s, v.y*s, v.z*s); }
	friend Vector3 operator*(ConstArg v, const Matrix3<T>& m) noexcept; // #TODO
	//friend Vector3 operator*(const Matrix3<T>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector3& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const Vector3& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector3& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	Vector2<T>::ConstResult xy/*getXY*/() const noexcept { return reinterpret_cast<const Vector2<T>&>(*this); }
	Vector2 xz/*getXZ*/() const noexcept { return Vector2(x, z); }
	Vector2 zy/*getZY*/() const noexcept { return Vector2(z, y); }
	//void setXY(Vector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(const Vector3& v) const noexcept;
	bool isApproxEqual(const Vector3& v, T tolerance) const noexcept;
	bool allLessThan(const Vector3& v) const noexcept { return (x < v.x) && (y < v.y) && (z < v.z); }
	bool allLessThanEqual(const Vector3& v) const noexcept { return (x <= v.x) && (y <= v.y) && (z <= v.z); }
	bool allGreaterThan(const Vector3& v) const noexcept { return (x > v.x) && (y > v.y) && (z > v.z); }
	bool allGreaterThanEqual(const Vector3& v) const noexcept { return (x >= v.x) && (y >= v.y) && (z >= v.z); }
	bool anyLessThan(const Vector3& v) const noexcept { return (x < v.x) || (y < v.y) || (z < v.z); }
	bool anyLessThanEqual(const Vector3& v) const noexcept { return (x <= v.x) || (y <= v.y) || (z <= v.z); }
	bool anyGreaterThan(const Vector3& v) const noexcept { return (x > v.x) || (y > v.y) || (z > v.z); }
	bool anyGreaterThanEqual(const Vector3& v) const noexcept { return (x >= v.x) || (y >= v.y) || (z >= v.z); }
	bool isFinite() const noexcept { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }
	T getMagnitude() const noexcept { return std::sqrt(x*x + y*y + z*z); }
	T getMagnitudeSquared() const noexcept { return (x*x + y*y + z*z); }
	void setMagnitude(T magnitude) noexcept;
	T getLength() const noexcept { return getMagnitude(); }
	T getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(T length) noexcept { setMagnitude(length); }
	Axis getMajorAxis() const noexcept;
	T getMinComponent() const { return std::min(std::min(x, y), z); }
	T getMaxComponent() const { return std::max(std::max(x, y), z); }
	Vector3& setZero() noexcept { x = T(); y = T(); z = T(); return *this; }
	Vector3& set(T x, T y, T z) noexcept { this->x = x; this->y = y; this->z = z; return *this; }
	Vector3& setMinimum(const Vector3& v1, const Vector3& v2);
	Vector3& setMaximum(const Vector3& v1, const Vector3& v2);
	Vector3& negate() noexcept { x = -x; y = -y; z = -z; return *this; }
	Vector3& normalize() noexcept;
	Vector3& rotate(Axis axis, T angle) noexcept;
	Vector3& rotate(const Quaternion<T>& q) noexcept;
	//Vector3& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector3& transform(const Matrix3<T>& m) noexcept; // #TODO
	Vector3& transform(const AffineTransform<T>& m) noexcept; // #TODO

	//static const Vector3& getZero() noexcept { return ZERO; }
	//static const Vector3& getUnitX() noexcept { return UNIT_X; }
	//static const Vector3& getUnitY() noexcept { return UNIT_Y; }
	//static const Vector3& getUnitZ() noexcept { return UNIT_Z; }

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

template<typename T> const Vector3<T> Vector3<T>::ZERO{};
template<typename T> const Vector3<T> Vector3<T>::UNIT_X{ T(1), T(0), T(0) };
template<typename T> const Vector3<T> Vector3<T>::UNIT_Y{ T(0), T(1), T(0) };
template<typename T> const Vector3<T> Vector3<T>::UNIT_Z{ T(0), T(0), T(1) };
template<typename T> const Vector3<T> Vector3<T>::ONE{ T(1), T(1), T(1) };
template<typename T> const Vector3<T> Vector3<T>::TOLERANCE{ Constants<T>::TOLERANCE, Constants<T>::TOLERANCE, Constants<T>::TOLERANCE };
template<typename T> const Vector3<T> Vector3<T>::INF{ std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() };
template<typename T> const Vector3<T> Vector3<T>::MINUS_INF{ -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity() };

#if SIMD_HAS_FLOAT4

template<>
struct Quaternion<float>;

template<>
struct Matrix3<float>;

template<>
struct AffineTransform<float>;

template<>
struct Vector3<float>
{
	using Real = float;
	using ConstArg = const Vector3;
	using ConstResult = const Vector3;

	static constexpr int NUM_COMPONENTS = 3;

	/*constexpr*/ Vector3() noexcept { xyz = simd::zero<simd::float4>(); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	/*constexpr*/ explicit Vector3(float scalar) noexcept { xyz = simd::set4(scalar); }
	/*constexpr*/ Vector3(float x, float y, float z) noexcept { xyz = simd::set4(x, y, z, z); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v) noexcept { xyz = simd::cutoff2(v); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v, float z) noexcept { xyz = simd::combine2(v, simd::set2(z)); }
	template<typename U> explicit Vector3(const IntVector3<U>& v) noexcept;
	explicit Vector3(const tuples::templates::Tuple3<float>& t) noexcept { xyz = simd::set4(t.x, t.y, t.z, t.z); }
	template<typename U> explicit Vector3(const tuples::templates::Tuple3<U>& t) noexcept { float z = (float)t.z; xyz = simd::set4((float)t.x, (float)t.y, z, z); }
	explicit Vector3(const std::tuple<float, float, float>& t) noexcept { float z = std::get<2>(t); xyz = simd::set4(std::get<0>(t), std::get<1>(t), z, z); }
	template<typename U> explicit Vector3(const std::tuple<U, U, U>& t) noexcept { float z = (float)std::get<2>(t); xyz = simd::set4((float)std::get<0>(t), (float)std::get<1>(t), z, z); }
	explicit Vector3(const float* v) noexcept { set(v[0], v[1], v[2]); }
#else
	/*constexpr*/ explicit Vector3(float scalar) noexcept { xyz = simd::set3(scalar); }
	/*constexpr*/ Vector3(float x, float y, float z) noexcept { xyz = simd::set3(x, y, z); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v) noexcept { xyz = simd::cutoff2(v); }
	/*constexpr*/ Vector3(Vector2<float>::ConstArg v, float z) noexcept { xyz = simd::combine2(v, simd::set1(z)); }
	template<typename U> explicit Vector3(const IntVector3<U>& v) noexcept;
	explicit Vector3(const tuples::templates::Tuple3<float>& t) noexcept { xyz = simd::set3(t.x, t.y, t.z); }
	template<typename U> explicit Vector3(const tuples::templates::Tuple3<U>& t) noexcept { xyz = simd::set3((float)t.x, (float)t.y, (float)t.z); }
	explicit Vector3(const std::tuple<float, float, float>& t) noexcept { xyz = simd::set3(std::get<0>(t), std::get<1>(t), std::get<2>(t)); }
	template<typename U> explicit Vector3(const std::tuple<U, U, U>& t) noexcept { xyz = simd::set3((float)std::get<0>(t), (float)std::get<1>(t), (float)std::get<2>(t)); }
	explicit Vector3(const float* v) noexcept { xyz = simd::load3(v); }
#endif
	explicit Vector3(Axis axis) noexcept { set((axis == Axis::X) ? 1.f : 0.f, (axis == Axis::Y) ? 1.f : 0.f, (axis == Axis::Z) ? 1.f : 0.f); }
	explicit Vector3(simd::float4 v) noexcept : xyz(v) {}

	operator simd::float4() const noexcept { return xyz; }
	explicit operator tuples::templates::Tuple3<float>() noexcept { return tuples::templates::Tuple3<float>(x, y, z); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(x), U(y), U(z)); }
	explicit operator std::tuple<float, float, float>() { return std::tuple<float, float, float>(x, y, z); }
	template<typename U> explicit operator std::tuple<U, U, U>() { return std::tuple<U, U, U>(U(x), U(y), U(z)); }
	explicit operator float* () noexcept { return &x; }
	explicit operator const float* () const noexcept { return &x; }
	float& operator[](int i) noexcept { return (&x)[i]; }
	const float& operator[](int i) const noexcept { return (&x)[i]; }

	Vector3 operator+() const noexcept { return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3 operator-() const noexcept { return Vector3(simd::neg4(xyz)); }
#else
	Vector3 operator-() const noexcept { return Vector3(simd::neg3(xyz)); }
#endif
	Vector3& operator+=(ConstArg v) noexcept { xyz = simd::add4(xyz, v); return *this; }
	Vector3& operator-=(ConstArg v) noexcept { xyz = simd::sub4(xyz, v); return *this; }
	Vector3& operator*=(ConstArg v) noexcept { xyz = simd::mul4(xyz, v); return *this; }
	Vector3& operator*=(float f) noexcept { xyz = simd::mul4(xyz, simd::set4(f)); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3& operator/=(ConstArg v) noexcept { xyz = simd::div4(xyz, v); return *this; }
#else
	Vector3& operator/=(ConstArg v) noexcept { xyz = simd::div3(xyz, v); return *this; }
#endif
	Vector3& operator/=(float f) noexcept { xyz = simd::div4(xyz, simd::set4(f)); return *this; }
	Vector3& operator*=(const Matrix3<float>& m) noexcept; // #TODO
	friend Vector3 operator+(ConstArg v1, ConstArg v2) noexcept { return Vector3(simd::add4(v1, v2)); }
	friend Vector3 operator-(ConstArg v1, ConstArg v2) noexcept { return Vector3(simd::sub4(v1, v2)); }
	friend Vector3 operator*(ConstArg v1, ConstArg v2) noexcept { return Vector3(simd::mul4(v1, v2)); }
	friend Vector3 operator*(float f, ConstArg v) noexcept { return Vector3(simd::mul4(simd::set4(f), v)); }
	friend Vector3 operator*(ConstArg v, float f) noexcept { return Vector3(simd::mul4(v, simd::set4(f))); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	friend Vector3 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector3(simd::div4(v1, v2)); }
	friend Vector3 operator/(float f, ConstArg v) noexcept { return Vector3(simd::div4(simd::set4(f), v)); }
#else
	friend Vector3 operator/(ConstArg v1, ConstArg v2) noexcept { return Vector3(simd::div3(v1, v2)); }
	friend Vector3 operator/(float f, ConstArg v) noexcept { return Vector3(simd::div3(simd::set3(f), v)); }
#endif
	friend Vector3 operator/(ConstArg v, float f) noexcept { return Vector3(simd::div4(v, simd::set4(f))); }
	friend Vector3 operator*(ConstArg v, const Matrix3<float>& m) noexcept; // #TODO
	//friend Vector3 operator*(const Matrix3<float>& m, ConstArg v) noexcept; // valid for column vectors only
	bool operator==(const Vector3& v) const noexcept { return simd::all3(simd::equal(xyz, v)); }
	bool operator!=(const Vector3& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, Vector3& v);
	friend std::ostream& operator<<(std::ostream& s, const Vector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; } // #FIXME use simd::set(x, y, z, z)

#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector2<float> xy/*getXY*/() const noexcept { return Vector2<float>(simd::swizzle<simd::XYYY>(xyz)); }
	Vector2<float> xz/*getXZ*/() const noexcept { return Vector2<float>(simd::swizzle<simd::XZZZ>(xyz)); }
	Vector2<float> zy/*getZY*/() const noexcept { return Vector2<float>(simd::swizzle<simd::ZYYY>(xyz)); }
#else
	Vector2<float> xy/*getXY*/() const noexcept { return Vector2<float>(simd::cutoff2(xyz)); }
	Vector2<float> xz/*getXZ*/() const noexcept { return Vector2<float>(simd::swizzle<simd::XZWW>(xyz)); }
	Vector2<float> zy/*getZY*/() const noexcept { return Vector2<float>(simd::swizzle<simd::ZYWW>(xyz)); }
#endif
	//void setXY(Vector2<float>::ConstArg v) noexcept { xyz = simd::insert2(v, xyz); }
	bool isZero() const noexcept { return simd::all3(simd::equal(xyz, simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept { simd::all3(simd::lessThan(simd::abs4(xyz), TOLERANCE)); }
	bool isApproxEqual(const Vector3& v) const noexcept { simd::all3(simd::lessThan(simd::abs4(simd::sub4(xyz, v)), TOLERANCE)); }
	bool isApproxEqual(const Vector3& v, float tolerance) const noexcept { simd::all3(simd::lessThan(simd::abs4(simd::sub4(xyz, v)), simd::set4(tolerance))); }
	bool allLessThan(const Vector3& v) const noexcept { return simd::all3(simd::lessThan(xyz, v)); }
	bool allLessThanEqual(const Vector3& v) const noexcept { return simd::all3(simd::lessThanEqual(xyz, v)); }
	bool allGreaterThan(const Vector3& v) const noexcept { return simd::all3(simd::greaterThan(xyz, v)); }
	bool allGreaterThanEqual(const Vector3& v) const noexcept { return simd::all3(simd::greaterThanEqual(xyz, v)); }
	bool anyLessThan(const Vector3& v) const noexcept { return simd::any3(simd::lessThan(xyz, v)); }
	bool anyLessThanEqual(const Vector3& v) const noexcept { return simd::any3(simd::lessThanEqual(xyz, v)); }
	bool anyGreaterThan(const Vector3& v) const noexcept { return simd::any3(simd::greaterThan(xyz, v)); }
	bool anyGreaterThanEqual(const Vector3& v) const noexcept { return simd::any3(simd::greaterThanEqual(xyz, v)); }
	bool isFinite() const noexcept { return simd::all3(simd::isFinite(xyz)); }
	float getMagnitude() const noexcept { return simd::toFloat(simd::sqrt1(simd::dot3(xyz, xyz))); }
	float getMagnitudeSquared() const noexcept { return simd::toFloat(simd::dot3(xyz, xyz)); }
	void setMagnitude(float magnitude) noexcept;
	float getLength() const noexcept { return getMagnitude(); }
	float getLengthSquared() const noexcept { return getMagnitudeSquared(); }
	void setLength(float length) noexcept { setMagnitude(length); }
	Axis getMajorAxis() const noexcept { (Axis)simd::asIndex(simd::equal(xyz, simd::hMax3(xyz))); }
	float getMinComponent() const noexcept { return simd::toFloat(simd::hMin3(xyz)); }
	float getMaxComponent() const noexcept { return simd::toFloat(simd::hMax3(xyz)); }
	Vector3& setZero() noexcept { xyz = simd::zero<simd::float4>(); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3& set(float x, float y, float z) noexcept { xyz = simd::set4(x, y, z, z); return *this; }
#else
	Vector3& set(float x, float y, float z) noexcept { xyz = simd::set3(x, y, z); return *this; }
#endif
	Vector3& setMinimum(const Vector3& v1, const Vector3& v2) noexcept { xyz = simd::min4(v1, v2); return *this; }
	Vector3& setMaximum(const Vector3& v1, const Vector3& v2) noexcept { xyz = simd::max4(v1, v2); return *this; }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3& negate() noexcept { xyz = simd::neg4(xyz); return *this; }
#else
	Vector3& negate() noexcept { xyz = simd::neg3(xyz); return *this; }
#endif
	Vector3& normalize() noexcept;
	Vector3& rotate(Axis axis, float angle) noexcept;
	Vector3& rotate(const Quaternion<float>& q) noexcept;
	//Vector3& scale(ConstArg v) noexcept { xyz = simd::mul4(xyz, v); return *this; }
	Vector3& transform(const Matrix3<float>& m) noexcept; // #TODO
	Vector3& transform(const AffineTransform<float>& m) noexcept; // #TODO

	//static const Vector3& getZero() noexcept { return ZERO; }
	//static const Vector3& getUnitX() noexcept { return UNIT_X; }
	//static const Vector3& getUnitY() noexcept { return UNIT_Y; }
	//static const Vector3& getUnitZ() noexcept { return UNIT_Z; }

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
		simd::float4 xyz;
		struct { float x, y, z/*, reserved_*/; };
	};
};

const Vector3<float> Vector3<float>::ZERO{};
const Vector3<float> Vector3<float>::UNIT_X{ 1.f, 0.f, 0.f };
const Vector3<float> Vector3<float>::UNIT_Y{ 0.f, 1.f, 0.f };
const Vector3<float> Vector3<float>::UNIT_Z{ 0.f, 0.f, 1.f };
const Vector3<float> Vector3<float>::ONE{ 1.f, 1.f, 1.f };
const Vector3<float> Vector3<float>::TOLERANCE{ Constants<float>::TOLERANCE, Constants<float>::TOLERANCE, Constants<float>::TOLERANCE };
const Vector3<float> Vector3<float>::INF{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
const Vector3<float> Vector3<float>::MINUS_INF{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline T dot(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<typename T>
inline Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

//template<typename T>
//inline Matrix3<T> tensor(const Vector3<T>& v1, const Vector3<T>& v2) noexcept; // -> Matrix3

template<typename T>
inline T distance(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	T x = v2.x - v1.x;
	T y = v2.y - v1.y;
	T z = v2.z - v1.z;
	return std::sqrt(x*x + y*y + z*z);
}

template<typename T>
inline T distanceSquared(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	T x = v2.x - v1.x;
	T y = v2.y - v1.y;
	T z = v2.z - v1.z;
	return x*x + y*y + z*z;
}

template<typename T>
inline T length(const Vector3<T>& v) noexcept
{
	return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

template<typename T>
inline T lengthSquared(const Vector3<T>& v) noexcept
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

template<typename T>
inline Vector3<T> minimum(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
}

template<typename T>
inline Vector3<T> maximum(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
}

template<typename T>
inline Vector3<T> lerp(const Vector3<T>& v1, const Vector3<T>& v2, T t) noexcept
{
	// #TODO
}

template<typename T>
inline Vector3<T> slerp(const Vector3<T>& v1, const Vector3<T>& v2, T t)
{
	// #TODO
}

template<typename T>
inline Vector3<T> reflect(const Vector3<T>& i, const Vector3<T>& n) noexcept
{
	// #TODO
}

template<typename T>
inline Vector3<T> refract(const Vector3<T>& i, const Vector3<T>& n, T etaRatio) noexcept
{
	// #TODO
}

template<typename T>
inline Vector3<T> perpendicular(const Vector3<T>& v) noexcept
{
	// #TODO
}

//template<typename T>
//inline Vector3<T> transform(const Vector3<T>& v, const Matrix3<T>& m) noexcept; // -> Matrix3

//template<typename T>
//inline Vector3<T> transform(const Vector3<T>& v, const AffineTransform<T>& m) noexcept; // -> AffineTransform

#if SIMD_HAS_FLOAT4

template<>
inline float dot(const Vector3<float>& v1, const Vector3<float>& v2) noexcept
{
	return simd::toFloat(simd::dot3(v1, v2));
}

template<>
inline Vector3<float> cross(const Vector3<float>& v1, const Vector3<float>& v2) noexcept
{
	return Vector3<float>(simd::sub4(simd::mul4(simd::swizzle<simd::YZXW>(v1), simd::swizzle<simd::ZXYW>(v2)),
		simd::mul4(simd::swizzle<simd::ZXYW>(v1), simd::swizzle<simd::YZXW>(v2))));
}

template<>
inline float distance(const Vector3<float>& v1, const Vector3<float>& v2) noexcept
{
	auto v = simd::sub4(v2, v1);
	return simd::toFloat(simd::sqrt1(simd::dot3(v, v)));
}

template<>
inline float distanceSquared(const Vector3<float>& v1, const Vector3<float>& v2) noexcept
{
	auto v = simd::sub4(v2, v1);
	return simd::toFloat(simd::dot3(v, v));
}

template<>
inline float length(const Vector3<float>& v) noexcept
{
	return simd::toFloat(simd::sqrt1(simd::dot3(v, v)));
}

template<>
inline float lengthSquared(const Vector3<float>& v) noexcept
{
	return simd::toFloat(simd::dot3(v, v));
}

template<>
inline Vector3<float> minimum(const Vector3<float>& v1, const Vector3<float>& v2)
{
	return Vector3<float>(simd::min4(v1, v2));
}

template<>
inline Vector3<float> maximum(const Vector3<float>& v1, const Vector3<float>& v2)
{
	return Vector3<float>(simd::max4(v1, v2));
}

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline std::istream& operator>>(std::istream& s, Vector3<T>& v) 
{ 
	return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z; 
}

template<typename T>
inline bool Vector3<T>::isApproxZero() const
{ 
	return (std::fabs(x) < Constants<T>::TOLERANCE) && (std::fabs(y) < Constants<T>::TOLERANCE) && 
		(std::fabs(z) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Vector3<T>::isApproxEqual(const Vector3<T>& v) const
{ 
	return (std::fabs(v.x - x) < Constants<T>::TOLERANCE) && (std::fabs(v.y - y) < Constants<T>::TOLERANCE) &&
		(std::fabs(v.z - z) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool Vector3<T>::isApproxEqual(const Vector3<T>& v, T tolerance) const
{ 
	return (std::fabs(v.x - x) < tolerance) && (std::fabs(v.y - y) < tolerance) &&
		(std::fabs(v.z - z) < tolerance);
}

template<typename T>
inline void Vector3<T>::setMagnitude(T magnitude) 
{ 
	T m = getMagnitude(); 
	if (m > T(0)) 
		*this *= magnitude/m;
}

template<typename T>
inline Axis Vector3<T>::getMajorAxis() const
{ 
	Axis axis = Axis::X; 
	if (std::fabs(y) > std::fabs(x)) 
		axis = Axis::Y;
	if (std::fabs(z) > std::fabs((&x)[(int)axis])) 
		axis = Axis::Z; 
	return axis;
}

template<typename T>
inline Vector3<T>& Vector3<T>::setMinimum(const Vector3<T>& v1, const Vector3<T>& v2)
{ 
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y);
	z = std::min(v1.z, v2.z); 
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::setMaximum(const Vector3<T>& v1, const Vector3<T>& v2)
{ 
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y);
	z = std::max(v1.z, v2.z); 
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

template<typename T>
inline Vector3<T>& Vector3<T>::rotate(Axis axis, T angle)
{
	if (angle != T(0))
	{
		T sinAngle = std::sin(angle);
		T cosAngle = std::cos(angle);

		switch (axis)
		{
			case Axis::X:
			{
				T j = y, k = z;
				y = j*cosAngle - k*sinAngle;
				z = k*cosAngle + j*sinAngle;
			} break;

			case Axis::Y:
			{
				T i = x, k = z;
				x = i*cosAngle + k*sinAngle;
				z = k*cosAngle - i*sinAngle;
			} break;

			case Axis::Z:
			{
				T i = x, j = y;
				x = i*cosAngle - j*sinAngle;
				y = j*cosAngle + i*sinAngle;
			} break;
		}
	}

	return *this;
}

#if SIMD_HAS_FLOAT4

template<>
inline std::istream& operator>>(std::istream& s, Vector3<float>& v)
{ 
	float x, y, z; 
	s >> x >> std::skipws >> y >> std::skipws >> z; 
	v.set(x, y, z); 
	return s; 
}

inline void Vector3<float>::setMagnitude(float magnitude)
{ 
	float m = getMagnitude(); 
	if (m > 0.f) 
		*this *= magnitude/m;
}

inline Vector3<float>& Vector3<float>::normalize()
{
#if MATHEMATICS_FAST_NORMALIZE
	float m = simd::toFloat(simd::rcpSqrtApprox1(simd::dot3(xyz, xyz)));
	if (m <= std::numeric_limits<float>::max()) 
		xyz = simd::mul4(xyz, simd::set4(m));
#else
	float m = getMagnitude(); 
	if (m > 0.f) 
		xyz = simd::div4(xyz, simd::set4(m));
#endif
	return *this;
}

inline Vector3<float>& Vector3<float>::rotate(Axis axis, float angle)
{
	if (angle != 0.f)
	{
		float sinAngle = std::sin(angle);
		float cosAngle = std::cos(angle);

		switch (axis)
		{
			case Axis::X:
			{
				float j = y, k = z;
				set(x, j*cosAngle - k*sinAngle, k*cosAngle + j*sinAngle); // #TODO SIMD
			} break;

			case Axis::Y:
			{
				float i = x, k = z;
				set(i*cosAngle + k*sinAngle, y, k*cosAngle - i*sinAngle); // #TODO SIMD
			} break;

			case Axis::Z:
			{
				float i = x, j = y;
				set(i*cosAngle - j*sinAngle, j*cosAngle + i*sinAngle, z); // #TODO SIMD
			} break;
		}
	}

	return *this;
}

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Vector3<T> normalize(const Vector3<T>& v) noexcept
{
	Vector3<T> u(v);
	u.normalize();
	return u;
}

template<typename T>
inline Vector3<T> project(const Vector3<T>& v1, const Vector3<T>& v2) noexcept
{
	T m = v2.getMagnitudeSquared();
	return (m > T(0)) ? (dot(v1, v2)/m)*v2 : Vector4<T>::ZERO;
}

template<typename T>
inline T angle(const Vector3<T>& v1, const Vector3<T>& v2)
{
#if MATHEMATICS_FAST_NORMALIZE
	Vector3<T> u(v1);
	Vector3<T> v(v2);
	u.normalize();
	v.normalize();
	return std::acos(std::clamp(dot(u, v), T(-1), T(1)));
#else
	T q = v1.getMagnitude()*v2.getMagnitude();
	if (q > T(0))
		return std::acos(std::clamp(dot(v1, v2)/q, T(-1), T(1)));
	else
		return (dot(v1, v2) >= T(0)) ? T(0) : Constants<T>::PI;
#endif
}

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

} // namespace core::mathematics

#include "IntVector3.hpp"
#include "Quaternion.hpp"

namespace core::mathematics::templates {

template<typename T>
inline Vector3<T> rotate(const Vector3<T>& v, const Quaternion<T>& q) noexcept
{
	T x1 = q.y*v.z - q.z*v.y;
	T y1 = q.z*v.x - q.x*v.z;
	T z1 = q.x*v.y - q.y*v.x;
	T x2 = q.w*x1 + q.y*z1 - q.z*y1;
	T y2 = q.w*y1 + q.z*x1 - q.x*z1;
	T z2 = q.w*z1 + q.x*y1 - q.y*x1;
	return Vector3<T>(v.x + T(2)*x2, v.y + T(2)*y2, v.z + T(2)*z2);
}

template<typename T>
template<typename U> 
inline Vector3<T>::Vector3(const IntVector3<U>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z))
{
}

template<typename T>
inline Vector3<T>& Vector3<T>::rotate(const Quaternion<T>& q)
{
	T x1 = q.y*z - q.z*y;
    T y1 = q.z*x - q.x*z;
    T z1 = q.x*y - q.y*x;
    T x2 = q.w*x1 + q.y*z1 - q.z*y1;
    T y2 = q.w*y1 + q.z*x1 - q.x*z1;
    T z2 = q.w*z1 + q.x*y1 - q.y*x1;
    x += T(2)*x2;
    y += T(2)*y2;
    z += T(2)*z2;
	return *this;
}

#if SIMD_HAS_FLOAT4

template<>
inline Vector3<float> rotate(const Vector3<float>& v, const Quaternion<float>& q) noexcept
{
	auto qyzx = simd::swizzle<simd::YZXX>(q);
	auto qzxy = simd::swizzle<simd::ZXYY>(q);
	auto t1 = simd::sub4(simd::mul4(qyzx, simd::swizzle<simd::ZXYY>(v)), simd::mul4(qzxy, simd::swizzle<simd::YZXX>(v)));
	auto t2 = simd::mulAdd4(simd::broadcast<simd::W>(q), t1, simd::sub4(simd::mul4(qyzx, simd::swizzle<simd::ZXYY>(t1)),
		simd::mul4(qzxy, simd::swizzle<simd::YZXX>(t1))));
	static const simd::float4 two = simd::set3(2.0f);
	return Vector3<float>(simd::mulAdd4(two, t2, v));
}

template<typename U> 
inline Vector3<float>::Vector3(const IntVector3<U>& v)
{
#if MATHEMATICS_SIMD_EXPAND_LAST
	float t = (float)v.z;
	xyz = simd::set4((float)v.x, (float)v.y, t, t);
#else
	xyz = simd::set4((float)v.x, (float)v.y, (float)v.z);
#endif
}

inline Vector3<float>& Vector3<float>::rotate(const Quaternion<float>& q)
{
	auto qyzx = simd::swizzle<simd::YZXX>(q);
	auto qzxy = simd::swizzle<simd::ZXYY>(q);
	auto t1 = simd::sub4(simd::mul4(qyzx, simd::swizzle<simd::ZXYY>(xyz)), simd::mul4(qzxy, simd::swizzle<simd::YZXX>(xyz)));
	auto t2 = simd::mulAdd4(simd::broadcast<simd::W>(q), t1, simd::sub4(simd::mul4(qyzx, simd::swizzle<simd::ZXYY>(t1)),
		simd::mul4(qzxy, simd::swizzle<simd::YZXX>(t1))));
	static const simd::float4 two = simd::set3(2.0f);
	xyz = simd::mulAdd4(two, t2, xyz);
	return *this;
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace core::mathematics::templates

namespace std
{

template<std::size_t I, typename T>
struct tuple_element;

template<typename T>
struct tuple_size;

template<std::size_t I, typename T>
struct tuple_element<I, core::mathematics::templates::Vector3<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size<core::mathematics::templates::Vector3<T>> : std::integral_constant<std::size_t, 3> 
{
};

template<std::size_t I, typename T>
inline T& get(core::mathematics::templates::Vector3<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const core::mathematics::templates::Vector3<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(core::mathematics::templates::Vector3<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const core::mathematics::templates::Vector3<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	static_assert(false);
}

} // namespace std

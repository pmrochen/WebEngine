/*
 *	Name: IntVector3
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_INT_VECTOR3_HPP
#define CORE_MATHEMATICS_INT_VECTOR3_HPP

#include <istream>
#include <ostream>
#include <algorithm>
#include <Tuples.hpp>
#include "IntVector2.hpp"
#include "Vector3.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
struct IntVector3
{
	using Real = T;
	using ConstArg = const IntVector3&;
	using ConstResult = const IntVector3&;

	static constexpr int NUM_COMPONENTS = 3;

	constexpr IntVector3() noexcept : x(), y(), z() {}
	constexpr explicit IntVector3(const T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
	constexpr IntVector3(const T x, const T y, const T z) noexcept : x(x), y(y), z(z) {}
	constexpr IntVector3(IntVector2<T>::ConstArg v) noexcept : x(v.x), y(v.y), z() {}
	constexpr IntVector3(IntVector2<T>::ConstArg v, const T z) noexcept : x(v.x), y(v.y), z(z) {}
	template<typename U> explicit IntVector3(Vector3<U>::ConstArg v) noexcept : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}
	explicit IntVector3(const tuples::templates::Tuple3<T>& t) noexcept : x(t.x), y(t.y), z(t.z) {}
	template<typename U> explicit IntVector3(const tuples::templates::Tuple3<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)) {}
	explicit IntVector3(const T* const v) noexcept { x = v[0]; y = v[1]; z = v[2]; }

	explicit operator tuples::templates::Tuple3<T>() noexcept { return tuples::templates::Tuple3<T>(x, y, z); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(x), U(y), U(z)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) { return (&x)[i]; }
	const T& operator[](int i) const { return (&x)[i]; }

	IntVector3 operator+() const noexcept { return *this; }
	IntVector3 operator-() const noexcept { return IntVector3(-x, -y, -z); }
	IntVector3& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
	IntVector3& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
	IntVector3& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	IntVector3& operator*=(const T f) noexcept { x *= f; y *= f; z *= f; return *this; }
	IntVector3& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; z /= v.z; return *this; }
	IntVector3& operator/=(const T f) noexcept { x /= f; y /= f; z /= f; return *this; }
	friend IntVector3 operator+(ConstArg v1, ConstArg v2) noexcept { return IntVector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
	friend IntVector3 operator-(ConstArg v1, ConstArg v2) noexcept { return IntVector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	friend IntVector3 operator*(ConstArg v1, ConstArg v2) noexcept { return IntVector3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z); }
	friend IntVector3 operator*(const T f, ConstArg v) noexcept { return IntVector3(f*v.x, f*v.y, f*v.z); }
	friend IntVector3 operator*(ConstArg v, const T f) noexcept { return IntVector3(v.x*f, v.y*f, v.z*f); }
	friend IntVector3 operator/(ConstArg v1, ConstArg v2) noexcept { return IntVector3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z); }
	friend IntVector3 operator/(const T f, ConstArg v) noexcept { return IntVector3(f/v.x, f/v.y, f/v.z); }
	friend IntVector3 operator/(ConstArg v, const T f) noexcept { return IntVector3(v.x/f, v.y/f, v.z/f); }
	bool operator==(ConstArg v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(ConstArg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, IntVector3& v) { return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z; }
	friend std::ostream& operator<<(std::ostream& s, const IntVector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	IntVector2<T>::ConstResult getXY() const noexcept { return *reinterpret_cast<const IntVector2*>(this); }
	void setXY(IntVector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	IntVector2 getXZ() const noexcept { return IntVector2(x, z); }
	IntVector2 getZY() const noexcept { return IntVector2(z, y); }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()); }
	bool allLessThan(ConstArg v) const noexcept { return (x < v.x) && (y < v.y) && (z < v.z); }
	bool allLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) && (y <= v.y) && (z <= v.z); }
	bool allGreaterThan(ConstArg v) const noexcept { return (x > v.x) && (y > v.y) && (z > v.z); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) && (y >= v.y) && (z >= v.z); }
	bool anyLessThan(ConstArg v) const noexcept { return (x < v.x) || (y < v.y) || (z < v.z); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) || (y <= v.y) || (z <= v.z); }
	bool anyGreaterThan(ConstArg v) const noexcept { return (x > v.x) || (y > v.y) || (z > v.z); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) || (y >= v.y) || (z >= v.z); }
	T getMinComponent() const noexcept { return std::min(std::min(x, y), z); }
	T getMaxComponent() const noexcept { return std::max(std::max(x, y), z); }
	IntVector3& setZero() noexcept { x = T(); y = T(); z = T(); return *this; }
	IntVector3& set(const T x, const T y, const T z) noexcept { this->x = x; this->y = y; this->z = z; return *this; }
	IntVector3& setMinimum(ConstArg v1, ConstArg v2) noexcept;
	IntVector3& setMaximum(ConstArg v1, ConstArg v2) noexcept;
	IntVector3& negate() noexcept { x = -x; y = -y; z = -z; return *this; }
	IntVector3& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	static const IntVector3&/*IntVector3::ConstResult*/ getZero() noexcept { return ZERO; }

	static const IntVector3 ZERO;

	T x, y, z;
};

template<typename T>
inline IntVector3<T>& IntVector3<T>::setMinimum(ConstArg v1, ConstArg v2)
{
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y);
	z = std::min(v1.z, v2.z); 
	return *this; 
}

template<typename T>
inline IntVector3<T>& IntVector3<T>::setMaximum(ConstArg v1, ConstArg v2)
{
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y);
	z = std::max(v1.z, v2.z); 
	return *this; 
}

template<typename T> const IntVector3<T> IntVector3<T>::ZERO{};

} // namespace templates

using IntVector3 = templates::IntVector3<int>;
using IntVector3Arg = templates::IntVector3<int>::ConstArg;
using IntVector3Result = templates::IntVector3<int>::ConstResult;

} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_INT_VECTOR3_HPP */

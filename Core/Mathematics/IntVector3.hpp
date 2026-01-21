/*
 *	Name: IntVector3
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cstddef>
#include <istream>
#include <ostream>
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <Tuples/Tuple3.hpp>
#include "IntVector2.hpp"
#include "Vector3.hpp"

namespace core::mathematics {
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
	explicit IntVector3(const std::tuple<T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)) {}
	template<typename U> explicit IntVector3(const std::tuple<U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))) {}
	explicit IntVector3(const T* const v) noexcept { x = v[0]; y = v[1]; z = v[2]; }

	explicit operator tuples::templates::Tuple3<T>() noexcept { return tuples::templates::Tuple3<T>(x, y, z); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(x), U(y), U(z)); }
	explicit operator std::tuple<T, T, T>() { return std::tuple<T, T, T>(x, y, z); }
	template<typename U> explicit operator std::tuple<U, U, U>() { return std::tuple<U, U, U>(U(x), U(y), U(z)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

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
	bool operator==(const IntVector3& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const IntVector3& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, IntVector3& v);
	friend std::ostream& operator<<(std::ostream& s, const IntVector3& v) { return s << v.x << ' ' << v.y << ' ' << v.z; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	IntVector2<T>::ConstResult xy/*getXY*/() const noexcept { return reinterpret_cast<const IntVector2<T>&>(*this); }
	IntVector2 xz/*getXZ*/() const noexcept { return IntVector2(x, z); }
	IntVector2 zy/*getZY*/() const noexcept { return IntVector2(z, y); }
	//void setXY(IntVector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()); }
	bool allLessThan(const IntVector3& v) const noexcept { return (x < v.x) && (y < v.y) && (z < v.z); }
	bool allLessThanEqual(const IntVector3& v) const noexcept { return (x <= v.x) && (y <= v.y) && (z <= v.z); }
	bool allGreaterThan(const IntVector3& v) const noexcept { return (x > v.x) && (y > v.y) && (z > v.z); }
	bool allGreaterThanEqual(const IntVector3& v) const noexcept { return (x >= v.x) && (y >= v.y) && (z >= v.z); }
	bool anyLessThan(const IntVector3& v) const noexcept { return (x < v.x) || (y < v.y) || (z < v.z); }
	bool anyLessThanEqual(const IntVector3& v) const noexcept { return (x <= v.x) || (y <= v.y) || (z <= v.z); }
	bool anyGreaterThan(const IntVector3& v) const noexcept { return (x > v.x) || (y > v.y) || (z > v.z); }
	bool anyGreaterThanEqual(const IntVector3& v) const noexcept { return (x >= v.x) || (y >= v.y) || (z >= v.z); }
	T getMinComponent() const { return std::min(std::min(x, y), z); }
	T getMaxComponent() const { return std::max(std::max(x, y), z); }
	IntVector3& setZero() noexcept { x = T(); y = T(); z = T(); return *this; }
	IntVector3& set(const T x, const T y, const T z) noexcept { this->x = x; this->y = y; this->z = z; return *this; }
	IntVector3& setMinimum(const IntVector3& v1, const IntVector3& v2);
	IntVector3& setMaximum(const IntVector3& v1, const IntVector3& v2);
	IntVector3& negate() noexcept { x = -x; y = -y; z = -z; return *this; }
	//IntVector3& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }

	//static const IntVector3&/*IntVector3::ConstResult*/ getZero() noexcept { return ZERO; }

	static const IntVector3 ZERO;

	T x, y, z;
};

template<typename T>
inline IntVector3<T> minimum(const IntVector3<T>& v1, const IntVector3<T>& v2)
{
	return IntVector3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
}

template<typename T>
inline IntVector3<T> maximum(const IntVector3<T>& v1, const IntVector3<T>& v2)
{
	return IntVector3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
}

template<typename T>
inline std::istream& operator>>(std::istream& s, IntVector3<T>& v) 
{ 
	return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z; 
}

template<typename T>
inline IntVector3<T>& IntVector3<T>::setMinimum(const IntVector3<T>& v1, const IntVector3<T>& v2)
{
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y);
	z = std::min(v1.z, v2.z); 
	return *this; 
}

template<typename T>
inline IntVector3<T>& IntVector3<T>::setMaximum(const IntVector3<T>& v1, const IntVector3<T>& v2)
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

} // namespace core::mathematics

namespace std
{
	
template<std::size_t I, typename T>
struct tuple_element;

template<typename T>
struct tuple_size;

template<std::size_t I, typename T>
struct tuple_element<I, core::mathematics::templates::IntVector3<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size<core::mathematics::templates::IntVector3<T>> : std::integral_constant<std::size_t, 3> 
{
};

template<std::size_t I, typename T>
inline T& get(core::mathematics::templates::IntVector3<T>& v) noexcept
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
inline const T& get(const core::mathematics::templates::IntVector3<T>& v) noexcept
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
inline T&& get(core::mathematics::templates::IntVector3<T>&& v) noexcept
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
inline const T&& get(const core::mathematics::templates::IntVector3<T>&& v) noexcept
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

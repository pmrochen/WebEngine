/*
 *	Name: IntVector3
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <tuple>
#include <cstddef>
#include <Tuples/Tuple3.hpp>
#include "IntVector2.hpp"
#include "Vector3.hpp"

namespace core::mathematics 
{
namespace templates 
{

template<typename T>
struct IntVector3
{
	using Real = T;
	using ComponentType = T;
	using ConstArg = const IntVector3&;
	using ConstResult = const IntVector3&;

	static constexpr int NUM_COMPONENTS = 3;

	constexpr IntVector3() noexcept : x(), y(), z() {}
	explicit IntVector3(Uninitialized) noexcept {}
	constexpr explicit IntVector3(T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
	constexpr IntVector3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
	constexpr IntVector3(const IntVector2<T>& v) noexcept : x(v.x), y(v.y), z() {}
	constexpr IntVector3(const IntVector2<T>& v, T z) noexcept : x(v.x), y(v.y), z(z) {}
	template<typename U> explicit IntVector3(const Vector3<U>& v) noexcept : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}
	explicit IntVector3(const tuples::templates::Tuple3<T>& t) noexcept : x(t.x), y(t.y), z(t.z) {}
	template<typename U> explicit IntVector3(const tuples::templates::Tuple3<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)) {}
	explicit IntVector3(const std::tuple<T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)) {}
	template<typename U> explicit IntVector3(const std::tuple<U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))) {}
	explicit IntVector3(const T* v) noexcept : x(v[0]), y(v[1]), z(v[2]) {}

	explicit operator tuples::templates::Tuple3<T>() noexcept { return tuples::templates::Tuple3<T>(x, y, z); }
	template<typename U> explicit operator tuples::templates::Tuple3<U>() noexcept { return tuples::templates::Tuple3<U>(U(x), U(y), U(z)); }
	//explicit operator std::tuple<T, T, T>() { return std::tuple<T, T, T>(x, y, z); }
	//template<typename U> explicit operator std::tuple<U, U, U>() { return std::tuple<U, U, U>(U(x), U(y), U(z)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	IntVector3 operator+() const noexcept { return *this; }
	IntVector3 operator-() const noexcept { return IntVector3(-x, -y, -z); }
	IntVector3& operator+=(const IntVector3& v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
	IntVector3& operator-=(const IntVector3& v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
	IntVector3& operator*=(const IntVector3& v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }
	IntVector3& operator*=(T f) noexcept { x *= f; y *= f; z *= f; return *this; }
	IntVector3& operator/=(const IntVector3& v) noexcept { x /= v.x; y /= v.y; z /= v.z; return *this; }
	IntVector3& operator/=(T f) noexcept { x /= f; y /= f; z /= f; return *this; }
	bool operator==(const IntVector3& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const IntVector3& v) const noexcept { return !(*this == v); }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	const IntVector2<T>& xy/*getXY*/() const noexcept { return reinterpret_cast<const IntVector2<T>&>(*this); }
	IntVector2<T> xz/*getXZ*/() const noexcept { return IntVector2<T>(x, z); }
	IntVector2<T> zy/*getZY*/() const noexcept { return IntVector2<T>(z, y); }
	//void setXY(const IntVector2<T>& v) noexcept { x = v.x; y = v.y; }
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
	IntVector3& setZero/*zero*/() noexcept { x = T(); y = T(); z = T(); return *this; }
	IntVector3& set(T x, T y, T z) noexcept { this->x = x; this->y = y; this->z = z; return *this; }
	IntVector3& setMinimum(const IntVector3& v1, const IntVector3& v2);
	IntVector3& setMaximum(const IntVector3& v1, const IntVector3& v2);
	IntVector3& negate() noexcept { x = -x; y = -y; z = -z; return *this; }

	//static const IntVector3& getZero() noexcept { return ZERO; }

	static const IntVector3 ZERO;

	T x, y, z;
};

template<std::size_t I, typename T>
inline T& get(IntVector3<T>& v) noexcept
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
inline const T& get(const IntVector3<T>& v) noexcept
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
inline T&& get(IntVector3<T>&& v) noexcept
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
inline const T&& get(const IntVector3<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	static_assert(false);
}

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
inline IntVector3<T> operator+(const IntVector3<T>& v1, const IntVector3<T>& v2) noexcept 
{ 
	return IntVector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); 
}

template<typename T>
inline IntVector3<T> operator-(const IntVector3<T>& v1, const IntVector3<T>& v2) noexcept 
{ 
	return IntVector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); 
}

template<typename T>
inline IntVector3<T> operator*(const IntVector3<T>& v1, const IntVector3<T>& v2) noexcept 
{ 
	return IntVector3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z); 
}

template<typename T>
inline IntVector3<T> operator*(T f, const IntVector3<T>& v) noexcept 
{ 
	return IntVector3(f*v.x, f*v.y, f*v.z); 
}

template<typename T>
inline IntVector3<T> operator*(const IntVector3<T>& v, T f) noexcept 
{ 
	return IntVector3(v.x*f, v.y*f, v.z*f); 
}

template<typename T>
inline IntVector3<T> operator/(const IntVector3<T>& v1, const IntVector3<T>& v2) noexcept 
{ 
	return IntVector3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z); 
}

template<typename T>
inline IntVector3<T> operator/(T f, const IntVector3<T>& v) noexcept 
{ 
	return IntVector3(f/v.x, f/v.y, f/v.z); 
}

template<typename T>
inline IntVector3<T> operator/(const IntVector3<T>& v, T f) noexcept 
{ 
	return IntVector3(v.x/f, v.y/f, v.z/f); 
}

template<typename C, typename T, typename U>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, IntVector3<U>& v)
{ 
	return s >> v.x >> std::ws >> v.y >> std::ws >> v.z; 
}

template<typename C, typename T, typename U>
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const IntVector3<U>& v) 
{ 
	constexpr C WS(0x20);
	return s << v.x << WS << v.y << WS << v.z;
}

template<typename T>
template<std::size_t I>
inline T& IntVector3<T>::get()
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& IntVector3<T>::get() const
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	static_assert(false);
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
	
template<size_t I, typename T>
struct tuple_element;

template<size_t I, typename T>
struct tuple_element<I, ::core::mathematics::templates::IntVector3<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size;

template<typename T>
struct tuple_size<::core::mathematics::templates::IntVector3<T>> : integral_constant<size_t, 3> 
{
};

template<typename T>
struct hash;

template<typename T>
struct hash<::core::mathematics::templates::IntVector3<T>>
{
	std::size_t operator()(const ::core::mathematics::templates::IntVector3<T>& v) const noexcept
	{
		std::hash<T> hasher;
		std::size_t seed = hasher(v.x) + 0x9e3779b9;
		seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

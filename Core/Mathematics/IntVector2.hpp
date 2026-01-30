/*
 *	Name: IntVector2
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
#include <Tuples/Tuple2.hpp>
#include "Vector2.hpp"

namespace core::mathematics 
{
namespace templates 
{

template<typename T>
struct IntVector2
{
	using Real = T;
	using ComponentType = T;
	using ConstArg = const IntVector2&;
	using ConstResult = const IntVector2&;

	static constexpr int NUM_COMPONENTS = 2;

	constexpr IntVector2() noexcept : x(), y() {}
	explicit IntVector2(Uninitialized) noexcept {}
	constexpr explicit IntVector2(T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr IntVector2(T x, T y) noexcept : x(x), y(y) {}
	template<typename U> explicit IntVector2(const Vector2<U>& v) noexcept : x(T(v.x)), y(T(v.y)) {}
	explicit IntVector2(const tuples::templates::Tuple2<T>& t) noexcept : x(t.x), y(t.y) {}
	template<typename U> explicit IntVector2(const tuples::templates::Tuple2<U>& t) noexcept : x(T(t.x)), y(T(t.y)) {}
	explicit IntVector2(const std::pair<T, T>& t) noexcept : x(t.first), y(t.second) {}
	template<typename U> explicit IntVector2(const std::pair<U, U>& t) noexcept : x(T(t.first)), y(T(t.second)) {}
	explicit IntVector2(const std::tuple<T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)) {}
	template<typename U> explicit IntVector2(const std::tuple<U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))) {}
	explicit IntVector2(const T* v) noexcept : x(v[0]), y(v[1]) {}

	explicit operator tuples::templates::Tuple2<T>() noexcept { return tuples::templates::Tuple2<T>(x, y); }
	template<typename U> explicit operator tuples::templates::Tuple2<U>() noexcept { return tuples::templates::Tuple2<U>(U(x), U(y)); }
	//explicit operator std::pair<T, T>() { return std::pair<T, T>(x, y); }
	//template<typename U> explicit operator std::pair<U, U>() { return std::pair<U, U>(U(x), U(y)); }
	//explicit operator std::tuple<T, T>() { return std::tuple<T, T>(x, y); }
	//template<typename U> explicit operator std::tuple<U, U>() { return std::tuple<U, U>(U(x), U(y)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	IntVector2 operator+() const noexcept { return *this; }
	IntVector2 operator-() const noexcept { return IntVector2(-x, -y); }
	IntVector2& operator+=(const IntVector2& v) noexcept { x += v.x; y += v.y; return *this; }
	IntVector2& operator-=(const IntVector2& v) noexcept { x -= v.x; y -= v.y; return *this; }
	IntVector2& operator*=(const IntVector2& v) noexcept { x *= v.x; y *= v.y; return *this; }
	IntVector2& operator*=(T f) noexcept { x *= f; y *= f; return *this; }
	IntVector2& operator/=(const IntVector2& v) noexcept { x /= v.x; y /= v.y; return *this; }
	IntVector2& operator/=(T f) noexcept { x /= f; y /= f; return *this; }
	bool operator==(const IntVector2& v) const noexcept { return (x == v.x) && (y == v.y); }
	bool operator!=(const IntVector2& v) const noexcept { return !(*this == v); }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	bool isZero() const noexcept { return (x == T()) && (y == T()); }
	bool allLessThan(const IntVector2& v) const noexcept { return (x < v.x) && (y < v.y); }
	bool allLessThanEqual(const IntVector2& v) const noexcept { return (x <= v.x) && (y <= v.y); }
	bool allGreaterThan(const IntVector2& v) const noexcept { return (x > v.x) && (y > v.y); }
	bool allGreaterThanEqual(const IntVector2& v) const noexcept { return (x >= v.x) && (y >= v.y); }
	bool anyLessThan(const IntVector2& v) const noexcept { return (x < v.x) || (y < v.y); }
	bool anyLessThanEqual(const IntVector2& v) const noexcept { return (x <= v.x) || (y <= v.y); }
	bool anyGreaterThan(const IntVector2& v) const noexcept { return (x > v.x) || (y > v.y); }
	bool anyGreaterThanEqual(const IntVector2& v) const noexcept { return (x >= v.x) || (y >= v.y); }
	T getMinComponent() const { return std::min(x, y); }
	T getMaxComponent() const { return std::max(x, y); }
	IntVector2& setZero/*zero*/() noexcept { x = T(); y = T(); return *this; }
	IntVector2& set(T x, T y) noexcept { this->x = x; this->y = y; return *this; }
	IntVector2& setMinimum(const IntVector2& v1, const IntVector2& v2);
	IntVector2& setMaximum(const IntVector2& v1, const IntVector2& v2);
	IntVector2& negate() noexcept { x = -x; y = -y; return *this; }

	//static const IntVector2& getZero() noexcept { return ZERO; }

	static const IntVector2 ZERO;

	T x, y;
};

template<std::size_t I, typename T>
inline T& get(IntVector2<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const IntVector2<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(IntVector2<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const IntVector2<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<typename T>
inline IntVector2<T> minimum(const IntVector2<T>& v1, const IntVector2<T>& v2)
{
	return IntVector2<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
}

template<typename T>
inline IntVector2<T> maximum(const IntVector2<T>& v1, const IntVector2<T>& v2)
{
	return IntVector2<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
}

template<typename T>
inline IntVector2<T> operator+(const IntVector2<T>& v1, const IntVector2<T>& v2) noexcept
{ 
	return IntVector2(v1.x + v2.x, v1.y + v2.y); 
}

template<typename T>
inline IntVector2<T> operator-(const IntVector2<T>& v1, const IntVector2<T>& v2) noexcept
{ 
	return IntVector2(v1.x - v2.x, v1.y - v2.y); 
}

template<typename T>
inline IntVector2<T> operator*(const IntVector2<T>& v1, const IntVector2<T>& v2) noexcept
{ 
	return IntVector2(v1.x*v2.x, v1.y*v2.y); 
}

template<typename T>
inline IntVector2<T> operator*(T f, const IntVector2<T>& v) noexcept
{ 
	return IntVector2(f*v.x, f*v.y); 
}

template<typename T>
inline IntVector2<T> operator*(const IntVector2<T>& v, T f) noexcept
{ 
	return IntVector2(v.x*f, v.y*f); 
}

template<typename T>
inline IntVector2<T> operator/(const IntVector2<T>& v1, const IntVector2<T>& v2) noexcept
{ 
	return IntVector2(v1.x/v2.x, v1.y/v2.y); 
}

template<typename T>
inline IntVector2<T> operator/(T f, const IntVector2<T>& v) noexcept
{ 
	return IntVector2(f/v.x, f/v.y); 
}

template<typename T>
inline IntVector2<T> operator/(const IntVector2<T>& v, T f) noexcept
{ 
	return IntVector2(v.x/f, v.y/f); 
}

template<typename C, typename T, typename U>
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, IntVector2<U>& v)
{ 
	return s >> v.x >> std::ws >> v.y; 
}

template<typename C, typename T, typename U>
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const IntVector2<U>& v) 
{ 
	constexpr C WS(0x20);
	return s << v.x << WS << v.y;
}

template<typename T>
template<std::size_t I>
inline T& IntVector2<T>::get()
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& IntVector2<T>::get() const
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	static_assert(false);
}

template<typename T>
inline IntVector2<T>& IntVector2<T>::setMinimum(const IntVector2<T>& v1, const IntVector2<T>& v2)
{
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y); 
	return *this; 
}

template<typename T>
inline IntVector2<T>& IntVector2<T>::setMaximum(const IntVector2<T>& v1, const IntVector2<T>& v2)
{
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y); 
	return *this; 
}

template<typename T> const IntVector2<T> IntVector2<T>::ZERO{};

} // namespace templates

using IntVector2 = templates::IntVector2<int>;
using IntVector2Arg = templates::IntVector2<int>::ConstArg;
using IntVector2Result = templates::IntVector2<int>::ConstResult;

} // namespace core::mathematics

namespace std
{

template<size_t I, typename T>
struct tuple_element;

template<size_t I, typename T>
struct tuple_element<I, ::core::mathematics::templates::IntVector2<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size;

template<typename T>
struct tuple_size<::core::mathematics::templates::IntVector2<T>> : integral_constant<size_t, 2> 
{
};

template<typename T>
struct hash;

template<typename T> 
struct hash<::core::mathematics::templates::IntVector2<T>>
{
	std::size_t operator()(const ::core::mathematics::templates::IntVector2<T>& v) const noexcept
	{
		std::hash<T> hasher;
		std::size_t seed = hasher(v.x) + 0x9e3779b9;
		seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

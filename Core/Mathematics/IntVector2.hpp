/*
 *	Name: IntVector2
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_INT_VECTOR2_HPP
#define CORE_MATHEMATICS_INT_VECTOR2_HPP

#include <istream>
#include <ostream>
#include <algorithm>
#include <Tuples.hpp>
#include "Vector2.hpp"

namespace core {
namespace mathematics {
namespace templates {

template<typename T>
struct IntVector2
{
	using Real = T;
	using ConstArg = const IntVector2&;
	using ConstResult = const IntVector2&;

	static constexpr int NUM_COMPONENTS = 2;

	constexpr IntVector2() noexcept : x(), y() {}
	constexpr explicit IntVector2(const T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr IntVector2(const T x, const T y) noexcept : x(x), y(y) {}
	template<typename U> explicit IntVector2(Vector2<U>::ConstArg v) noexcept : x(T(v.x)), y(T(v.y)) {}
	explicit IntVector2(const tuples::templates::Tuple2<T>& t) noexcept : x(t.x), y(t.y) {}
	template<typename U> explicit IntVector2(const tuples::templates::Tuple2<U>& t) noexcept : x(T(t.x)), y(T(t.y)) {}
	explicit IntVector2(const T* const v) noexcept { x = v[0]; y = v[1]; }

	explicit operator tuples::templates::Tuple2<T>() noexcept { return tuples::templates::Tuple2<T>(x, y); }
	template<typename U> explicit operator tuples::templates::Tuple2<U>() noexcept { return tuples::templates::Tuple2<U>(U(x), U(y)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) { return (&x)[i]; }
	const T& operator[](int i) const { return (&x)[i]; }

	IntVector2 operator+() const noexcept { return *this; }
	IntVector2 operator-() const noexcept { return IntVector2(-x, -y); }
	IntVector2& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; return *this; }
	IntVector2& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; return *this; }
	IntVector2& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; return *this; }
	IntVector2& operator*=(const T f) noexcept { x *= f; y *= f; return *this; }
	IntVector2& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; return *this; }
	IntVector2& operator/=(const T f) noexcept { x /= f; y /= f; return *this; }
	friend IntVector2 operator+(ConstArg v1, ConstArg v2) noexcept { return IntVector2(v1.x + v2.x, v1.y + v2.y); }
	friend IntVector2 operator-(ConstArg v1, ConstArg v2) noexcept { return IntVector2(v1.x - v2.x, v1.y - v2.y); }
	friend IntVector2 operator*(ConstArg v1, ConstArg v2) noexcept { return IntVector2(v1.x*v2.x, v1.y*v2.y); }
	friend IntVector2 operator*(const T f, ConstArg v) noexcept { return IntVector2(f*v.x, f*v.y); }
	friend IntVector2 operator*(ConstArg v, const T f) noexcept { return IntVector2(v.x*f, v.y*f); }
	friend IntVector2 operator/(ConstArg v1, ConstArg v2) noexcept { return IntVector2(v1.x/v2.x, v1.y/v2.y); }
	friend IntVector2 operator/(const T f, ConstArg v) noexcept { return IntVector2(f/v.x, f/v.y); }
	friend IntVector2 operator/(ConstArg v, const T f) noexcept { return IntVector2(v.x/f, v.y/f); }
	bool operator==(ConstArg v) const noexcept { return (x == v.x) && (y == v.y); }
	bool operator!=(ConstArg v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, IntVector2& v) { return s >> v.x >> std::skipws >> v.y; }
	friend std::ostream& operator<<(std::ostream& s, const IntVector2& v) { return s << v.x << ' ' << v.y; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	bool isZero() const noexcept { return (x == T()) && (y == T()); }
	bool allLessThan(ConstArg v) const noexcept { return (x < v.x) && (y < v.y); }
	bool allLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) && (y <= v.y); }
	bool allGreaterThan(ConstArg v) const noexcept { return (x > v.x) && (y > v.y); }
	bool allGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) && (y >= v.y); }
	bool anyLessThan(ConstArg v) const noexcept { return (x < v.x) || (y < v.y); }
	bool anyLessThanEqual(ConstArg v) const noexcept { return (x <= v.x) || (y <= v.y); }
	bool anyGreaterThan(ConstArg v) const noexcept { return (x > v.x) || (y > v.y); }
	bool anyGreaterThanEqual(ConstArg v) const noexcept { return (x >= v.x) || (y >= v.y); }
	T getMinComponent() const noexcept { return std::min(x, y); }
	T getMaxComponent() const noexcept { return std::max(x, y); }
	IntVector2& setZero() noexcept { x = T(); y = T(); return *this; }
	IntVector2& set(const T x, const T y) noexcept { this->x = x; this->y = y; return *this; }
	IntVector2& setMinimum(ConstArg v1, ConstArg v2) noexcept;
	IntVector2& setMaximum(ConstArg v1, ConstArg v2) noexcept;
	IntVector2& negate() noexcept { x = -x; y = -y; return *this; }
	IntVector2& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; return *this; }
	static const IntVector2&/*IntVector2::ConstResult*/ getZero() noexcept { return ZERO; }

	static const IntVector2 ZERO;

	T x, y;
};

template<typename T>
inline IntVector2<T>& IntVector2<T>::setMinimum(ConstArg v1, ConstArg v2)
{
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y); 
	return *this; 
}

template<typename T>
inline IntVector2<T>& IntVector2<T>::setMaximum(ConstArg v1, ConstArg v2)
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

} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_INT_VECTOR2_HPP */

/*
 *	Name: IntVector4
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cstddef>
#include <istream>
#include <ostream>
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <Tuples/Tuple4.hpp>
#include "IntVector2.hpp"
#include "IntVector3.hpp"
#include "Vector4.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct IntVector4
{
	using Real = T;
	using ConstArg = const IntVector4&;
	using ConstResult = const IntVector4&;

	static constexpr int NUM_COMPONENTS = 4;

	constexpr IntVector4() noexcept : x(), y(), z(), w() {}
	constexpr explicit IntVector4(const T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr IntVector4(const T x, const T y, const T z, const T w) noexcept : x(x), y(y), z(z), w(w) {}
	constexpr IntVector4(IntVector2<T>::ConstArg v) noexcept : x(v.x), y(v.y), z(), w(T(1)) {}
	constexpr IntVector4(IntVector2<T>::ConstArg v, const T z, const T w) noexcept : x(v.x), y(v.y), z(z), w(w) {}
	constexpr IntVector4(IntVector2<T>::ConstArg xy, IntVector2<T>::ConstArg zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	constexpr IntVector4(IntVector3<T>::ConstArg v) noexcept : x(v.x), y(v.y), z(v.z), w(T(1)) {}
	constexpr IntVector4(IntVector3<T>::ConstArg v, const T w) noexcept : x(v.x), y(v.y), z(v.z), w(w) {}
	template<typename U> explicit IntVector4(Vector4<U>::ConstArg v) noexcept : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}
	explicit IntVector4(const tuples::templates::Tuple4<T>& t) noexcept : x(t.x), y(t.y), z(t.z), w(t.w) {}
	template<typename U> explicit IntVector4(const tuples::templates::Tuple4<U>& t) noexcept : x(T(t.x)), y(T(t.y)), z(T(t.z)), w(T(t.w)) {}
	explicit IntVector4(const std::tuple<T, T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)), w(std::get<3>(t)) {}
	template<typename U> explicit IntVector4(const std::tuple<U, U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))), w(T(std::get<3>(t))) {}
	explicit IntVector4(const T* const v) noexcept { x = v[0]; y = v[1]; z = v[2]; w = v[3]; }

	explicit operator tuples::templates::Tuple4<T>() noexcept { return tuples::templates::Tuple4<T>(x, y, z, w); }
	template<typename U> explicit operator tuples::templates::Tuple4<U>() noexcept { return tuples::templates::Tuple4<U>(U(x), U(y), U(z), U(w)); }
	explicit operator std::tuple<T, T, T, T>() { return std::tuple<T, T, T, T>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }
	T& operator[](int i) noexcept { return (&x)[i]; }
	const T& operator[](int i) const noexcept { return (&x)[i]; }

	IntVector4 operator+() const noexcept { return *this; }
	IntVector4 operator-() const noexcept { return IntVector4(-x, -y, -z, -w); }
	IntVector4& operator+=(ConstArg v) noexcept { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	IntVector4& operator-=(ConstArg v) noexcept { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	IntVector4& operator*=(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	IntVector4& operator*=(const T f) noexcept { x *= f; y *= f; z *= f; w *= f; return *this; }
	IntVector4& operator/=(ConstArg v) noexcept { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	IntVector4& operator/=(const T f) noexcept { x /= f; y /= f; z /= f; w /= f; return *this; }
	friend IntVector4 operator+(ConstArg v1, ConstArg v2) noexcept { return IntVector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }
	friend IntVector4 operator-(ConstArg v1, ConstArg v2) noexcept { return IntVector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }
	friend IntVector4 operator*(ConstArg v1, ConstArg v2) noexcept { return IntVector4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w); }
	friend IntVector4 operator*(const T f, ConstArg v) noexcept { return IntVector4(f*v.x, f*v.y, f*v.z, f*v.w); }
	friend IntVector4 operator*(ConstArg v, const T f) noexcept { return IntVector4(v.x*f, v.y*f, v.z*f, v.w*f); }
	friend IntVector4 operator/(ConstArg v1, ConstArg v2) noexcept { return IntVector4(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w); }
	friend IntVector4 operator/(const T f, ConstArg v) noexcept { return IntVector4(f/v.x, f/v.y, f/v.z, f/v.w); }
	friend IntVector4 operator/(ConstArg v, const T f) noexcept { return IntVector4(v.x/f, v.y/f, v.z/f, v.w/f); }
	bool operator==(const IntVector4& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const IntVector4& v) const noexcept { return !(*this == v); }
	friend std::istream& operator>>(std::istream& s, IntVector4& v);
	friend std::ostream& operator<<(std::ostream& s, const IntVector4& v) { return s << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w; }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	IntVector2<T>::ConstResult xy/*getXY*/() const noexcept { return reinterpret_cast<const IntVector2<T>&>(*this); }
	IntVector3<T>::ConstResult xyz/*getXYZ*/() const noexcept { return reinterpret_cast<const IntVector3<T>&>(*this); }
	//void setXY(IntVector2<T>::ConstArg v) noexcept { x = v.x; y = v.y; }
	//void setXYZ(IntVector3<T>::ConstArg v) noexcept { x = v.x; y = v.y; z = v.z; }
	bool isZero() const noexcept { return (x == T()) && (y == T()) && (z == T()) && (w == T()); }
	bool allLessThan(const IntVector4& v) const noexcept { return (x < v.x) && (y < v.y) && (z < v.z) && (w < v.w); }
	bool allLessThanEqual(const IntVector4& v) const noexcept { return (x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w); }
	bool allGreaterThan(const IntVector4& v) const noexcept { return (x > v.x) && (y > v.y) && (z > v.z) && (w > v.w); }
	bool allGreaterThanEqual(const IntVector4& v) const noexcept { return (x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w); }
	bool anyLessThan(const IntVector4& v) const noexcept { return (x < v.x) || (y < v.y) || (z < v.z) || (w < v.w); }
	bool anyLessThanEqual(const IntVector4& v) const noexcept { return (x <= v.x) || (y <= v.y) || (z <= v.z) || (w <= v.w); }
	bool anyGreaterThan(const IntVector4& v) const noexcept { return (x > v.x) || (y > v.y) || (z > v.z) || (w > v.w); }
	bool anyGreaterThanEqual(const IntVector4& v) const noexcept { return (x >= v.x) || (y >= v.y) || (z >= v.z) || (w >= v.w); }
	T getMinComponent() const { return std::min(std::min(std::min(x, y), z), w); }
	T getMaxComponent() const { return std::max(std::max(std::max(x, y), z), w); }
	IntVector4& setZero() noexcept { x = T(); y = T(); z = T(); w = T(); return *this; }
	IntVector4& set(const T x, const T y, const T z, const T w) noexcept { this->x = x; this->y = y; this->z = z; this->w = w; return *this; }
	IntVector4& setMinimum(const IntVector4& v1, const IntVector4& v2);
	IntVector4& setMaximum(const IntVector4& v1, const IntVector4& v2);
	IntVector4& negate() noexcept { x = -x; y = -y; z = -z; w = -w; return *this; }
	//IntVector4& scale(ConstArg v) noexcept { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }

	//static const IntVector4&/*IntVector4::ConstResult*/ getZero() noexcept { return ZERO; }

	static const IntVector4 ZERO;

	T x, y, z, w;
};

template<typename T>
inline IntVector4<T> minimum(const IntVector4<T>& v1, const IntVector4<T>& v2)
{
	return IntVector4<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z), std::min(v1.w, v2.w));
}

template<typename T>
inline IntVector4<T> maximum(const IntVector4<T>& v1, const IntVector4<T>& v2)
{
	return IntVector4<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z), std::max(v1.w, v2.w));
}

template<typename T>
inline std::istream& operator>>(std::istream& s, IntVector4<T>& v) 
{ 
	return s >> v.x >> std::skipws >> v.y >> std::skipws >> v.z >> std::skipws >> v.w; 
}

template<typename T>
inline IntVector4<T>& IntVector4<T>::setMinimum(const IntVector4<T>& v1, const IntVector4<T>& v2)
{
	x = std::min(v1.x, v2.x); 
	y = std::min(v1.y, v2.y);
	z = std::min(v1.z, v2.z); 
	w = std::min(v1.w, v2.w); 
	return *this; 
}

template<typename T>
inline IntVector4<T>& IntVector4<T>::setMaximum(const IntVector4<T>& v1, const IntVector4<T>& v2)
{
	x = std::max(v1.x, v2.x); 
	y = std::max(v1.y, v2.y);
	z = std::max(v1.z, v2.z); 
	w = std::max(v1.w, v2.w); 
	return *this; 
}

template<typename T> const IntVector4<T> IntVector4<T>::ZERO{};

} // namespace templates

using IntVector4 = templates::IntVector4<int>;
using IntVector4Arg = templates::IntVector4<int>::ConstArg;
using IntVector4Result = templates::IntVector4<int>::ConstResult;

} // namespace core::mathematics

namespace std
{

template<std::size_t I, typename T>
struct tuple_element;

template<typename T>
struct tuple_size;

template<std::size_t I, typename T>
struct tuple_element<I, core::mathematics::templates::IntVector4<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size<core::mathematics::templates::IntVector4<T>> : std::integral_constant<std::size_t, 4> 
{
};

template<std::size_t I, typename T>
inline T& get(core::mathematics::templates::IntVector4<T>& v) noexcept
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
inline const T& get(const core::mathematics::templates::IntVector4<T>& v) noexcept
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
inline T&& get(core::mathematics::templates::IntVector4<T>&& v) noexcept
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
inline const T&& get(const core::mathematics::templates::IntVector4<T>&& v) noexcept
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

} // namespace std

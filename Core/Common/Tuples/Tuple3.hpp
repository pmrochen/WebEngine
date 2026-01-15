/*
 *	Name: Tuple3
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cstddef>
#include <type_traits>
#include <tuple>

namespace core {
namespace tuples {
namespace templates {

template<typename T>
struct Tuple3
{
	static constexpr int NUM_COMPONENTS = 3;

	constexpr Tuple3() = default; //: x(), y(), z() {}
	constexpr explicit Tuple3(const T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
	constexpr Tuple3(const T x, const T y, const T z) noexcept : x(x), y(y), z(z) {}
	explicit Tuple3(const std::tuple<T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)) {}
	template<typename U> explicit Tuple3(const std::tuple<U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))) {}
	explicit Tuple3(const T* const v) noexcept : x(v[0]), y(v[1]), z(v[2]) {}

	explicit operator std::tuple<T, T, T>() noexcept { return std::tuple<T, T, T>(x, y, z); }
	template<typename U> explicit operator std::tuple<U, U, U>() noexcept { return std::tuple<U, U, U>(U(x), U(y), U(z)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }

	bool operator==(const Tuple3& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const Tuple3& v) const noexcept { return !(*this == v); }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	T x, y, z;

	//union
	//{
	//	struct { T x, y, z; };
	//	struct { T r, g, b; };
	//};
};

} // namespace templates

using UChar3/*Byte3*/ = templates::Tuple3<unsigned char>;
using Int3 = templates::Tuple3<int>;
using UInt3 = templates::Tuple3<unsigned int>;
using LongLong3 = templates::Tuple3<long long>;
using Float3 = templates::Tuple3<float>;
using Double3 = templates::Tuple3<double>;

} // namespace tuples
} // namespace core

namespace std
{

template<typename T>
struct tuple_size<core::tuples::templates::Tuple3<T>> : std::integral_constant<std::size_t, 3> {};

template<std::size_t I, typename T>
inline T& get(core::tuples::templates::Tuple3<T>& v) noexcept
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
inline const T& get(const core::tuples::templates::Tuple3<T>& v) noexcept
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
inline T&& get(core::tuples::templates::Tuple3<T>&& v) noexcept
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
inline const T&& get(const core::tuples::templates::Tuple3<T>&& v) noexcept
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

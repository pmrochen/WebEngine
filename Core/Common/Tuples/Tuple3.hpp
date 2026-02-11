/*
 *	Name: Tuple3
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <initializer_list>
#include <tuple>
#include <cstddef>

namespace half_float {
class half;
} // namespace half_float

namespace core::tuples {
namespace templates {

template<typename T>
struct Tuple3
{
	static constexpr int NUM_COMPONENTS = 3;

	constexpr Tuple3() = default; //: x(), y(), z() {}
	constexpr explicit Tuple3(const T scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
	constexpr Tuple3(const T x, const T y, const T z) noexcept : x(x), y(y), z(z) {}
	explicit Tuple3(std::initializer_list<T> t) noexcept : x(t.data()[0]), y(t.data()[1]), z(t.data()[2]) {}
	template<typename U> explicit Tuple3(std::initializer_list<U> t) noexcept : x(T(t.data()[0])), y(T(t.data()[1])), z(T(t.data()[2])) {}
	explicit Tuple3(const std::tuple<T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)) {}
	template<typename U> explicit Tuple3(const std::tuple<U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))) {}
	explicit Tuple3(const T* const v) noexcept : x(v[0]), y(v[1]), z(v[2]) {}

	//explicit operator std::tuple<T, T, T>() noexcept { return std::tuple<T, T, T>(x, y, z); }
	//template<typename U> explicit operator std::tuple<U, U, U>() noexcept { return std::tuple<U, U, U>(U(x), U(y), U(z)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }

	bool operator==(const Tuple3& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
	bool operator!=(const Tuple3& v) const noexcept { return !(*this == v); }
	
	template<typename A> void serialize(A& ar)
	{ 
		if constexpr (std::is_same_v<T, ::half_float::half>)
			ar(reinterpret_cast<short&>(x), reinterpret_cast<short&>(y), reinterpret_cast<short&>(z));
		else
			ar(x, y, z); 
	}

	template<std::size_t I> T& get() noexcept
	{
		if constexpr (I == 0)
			return x;
		else if constexpr (I == 1)
			return y;
		else if constexpr (I == 2)
			return z;
		static_assert(false);
	}

	template<std::size_t I> const T& get() const noexcept
	{
		if constexpr (I == 0)
			return x;
		else if constexpr (I == 1)
			return y;
		else if constexpr (I == 2)
			return z;
		static_assert(false);
	}

	T x, y, z;

	//union
	//{
	//	struct { T x, y, z; };
	//	struct { T r, g, b; };
	//};
};

template<std::size_t I, typename T>
inline T& get(Tuple3<T>& v) noexcept
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
inline const T& get(const Tuple3<T>& v) noexcept
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
inline T&& get(Tuple3<T>&& v) noexcept
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
inline const T&& get(const Tuple3<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	else if constexpr (I == 2)
		return v.z;
	static_assert(false);
}

} // namespace templates

using UChar3/*Byte3*/ = templates::Tuple3<unsigned char>;
using Int3 = templates::Tuple3<int>;
using UInt3 = templates::Tuple3<unsigned int>;
using LongLong3 = templates::Tuple3<long long>;
using Float3 = templates::Tuple3<float>;
using Double3 = templates::Tuple3<double>;

} // namespace core::tuples

namespace std {

template<size_t I, typename T>
struct tuple_element;

template<size_t I, typename T>
struct tuple_element<I, ::core::tuples::templates::Tuple3<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size;

template<typename T>
struct tuple_size<::core::tuples::templates::Tuple3<T>> : integral_constant<size_t, 3> 
{
};

template<typename T>
struct hash;

template<typename T>
struct hash<::core::tuples::templates::Tuple3<T>>
{
	std::size_t operator()(const ::core::tuples::templates::Tuple3<T>& v) const noexcept
	{
		std::hash<T> hasher;
		std::size_t seed = hasher(v.x) + 0x9e3779b9;
		seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

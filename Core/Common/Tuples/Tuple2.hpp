/*
 *	Name: Tuple2
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>
#include <tuple>

namespace core::tuples {
namespace templates {

template<typename T>
struct Tuple2
{
	static constexpr int NUM_COMPONENTS = 2;

	constexpr Tuple2() = default; //: x(), y() {}
	constexpr explicit Tuple2(const T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr Tuple2(const T x, const T y) noexcept : x(x), y(y) {}
	explicit Tuple2(const std::pair<T, T>& t) noexcept : x(t.first), y(t.second) {}
	template<typename U> explicit Tuple2(const std::pair<U, U>& t) noexcept : x(T(t.first)), y(T(t.second)) {}
	explicit Tuple2(const std::tuple<T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)) {}
	template<typename U> explicit Tuple2(const std::tuple<U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))) {}
	explicit Tuple2(const T* const v) noexcept : x(v[0]), y(v[1]) {}

	explicit operator std::pair<T, T>() noexcept { return std::pair<T, T>(x, y); }
	template<typename U> explicit operator std::pair<U, U>() noexcept { return std::pair<U, U>(U(x), U(y)); }
	explicit operator std::tuple<T, T>() noexcept { return std::tuple<T, T>(x, y); }
	template<typename U> explicit operator std::tuple<U, U>() noexcept { return std::tuple<U, U>(U(x), U(y)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }

	bool operator==(const Tuple2& v) const noexcept { return (x == v.x) && (y == v.y); }
	bool operator!=(const Tuple2& v) const noexcept { return !(*this == v); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	T x, y;

	//union
	//{
	//	struct { T x, y; };
	//	struct { T r, g; }; 
	//};
};

} // namespace templates

using UChar2/*Byte2*/ = templates::Tuple2<unsigned char>;
using Short2 = templates::Tuple2<short>;
using UShort2 = templates::Tuple2<unsigned short>;
using Int2 = templates::Tuple2<int>;
using UInt2 = templates::Tuple2<unsigned int>;
using LongLong2 = templates::Tuple2<long long>;
using Float2 = templates::Tuple2<float>;
using Double2 = templates::Tuple2<double>;

#ifdef HALF_HALF_HPP
using Half2 = templates::Tuple2<half_float::half>;
#endif // HALF_HALF_HPP

} // namespace core::tuples

#ifdef HALF_HALF_HPP
namespace ::core::serialization { 
template<class A> 
inline void serialize(A& ar, core::tuples::templates::Tuple2<half_float::half>& t, 
    const unsigned int version) 
{ 
    ar & reinterpret_cast<short&>(t.x) & reinterpret_cast<short&>(t.y); 
}
}
#endif // HALF_HALF_HPP

namespace std
{

template<std::size_t I, typename T>
struct tuple_element;

template<typename T>
struct tuple_size;

template<std::size_t I, typename T>
struct tuple_element<I, core::tuples::templates::Tuple2<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size<core::tuples::templates::Tuple2<T>> : std::integral_constant<std::size_t, 2> 
{
};

template<std::size_t I, typename T>
inline T& get(core::tuples::templates::Tuple2<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const core::tuples::templates::Tuple2<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(core::tuples::templates::Tuple2<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const core::tuples::templates::Tuple2<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

} // namespace std

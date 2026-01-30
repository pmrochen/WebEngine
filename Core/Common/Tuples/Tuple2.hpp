/*
 *	Name: Tuple2
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <initializer_list>
#include <utility>
#include <tuple>
#include <cstddef>

namespace core::tuples 
{
namespace templates 
{

template<typename T>
struct Tuple2
{
	static constexpr int NUM_COMPONENTS = 2;

	constexpr Tuple2() = default; //: x(), y() {}
	constexpr explicit Tuple2(const T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr Tuple2(const T x, const T y) noexcept : x(x), y(y) {}
	explicit Tuple2(const std::initializer_list<T>& t) noexcept : x(t.data()[0]), y(t.data()[1]) {}
	template<typename U> explicit Tuple2(const std::initializer_list<U>& t) noexcept : x(T(t.data()[0])), y(T(t.data()[1])) {}
	explicit Tuple2(const std::pair<T, T>& t) noexcept : x(t.first), y(t.second) {}
	template<typename U> explicit Tuple2(const std::pair<U, U>& t) noexcept : x(T(t.first)), y(T(t.second)) {}
	explicit Tuple2(const std::tuple<T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)) {}
	template<typename U> explicit Tuple2(const std::tuple<U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))) {}
	explicit Tuple2(const T* const v) noexcept : x(v[0]), y(v[1]) {}

	//explicit operator std::pair<T, T>() noexcept { return std::pair<T, T>(x, y); }
	//template<typename U> explicit operator std::pair<U, U>() noexcept { return std::pair<U, U>(U(x), U(y)); }
	//explicit operator std::tuple<T, T>() noexcept { return std::tuple<T, T>(x, y); }
	//template<typename U> explicit operator std::tuple<U, U>() noexcept { return std::tuple<U, U>(U(x), U(y)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }

	bool operator==(const Tuple2& v) const noexcept { return (x == v.x) && (y == v.y); }
	bool operator!=(const Tuple2& v) const noexcept { return !(*this == v); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	T x, y;

	//union
	//{
	//	struct { T x, y; };
	//	struct { T r, g; }; 
	//};
};

template<std::size_t I, typename T>
inline T& get(Tuple2<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const Tuple2<T>& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(Tuple2<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const Tuple2<T>&& v) noexcept
{
	if constexpr (I == 0)
		return v.x;
	else if constexpr (I == 1)
		return v.y;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline T& Tuple2<T>::get()
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& Tuple2<T>::get() const
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	static_assert(false);
}

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
namespace core::serialization 
{ 
template<class A> 
inline void serialize(A& ar, ::core::tuples::templates::Tuple2<half_float::half>& t, const unsigned int version) 
{ 
    ar & reinterpret_cast<short&>(t.x) & reinterpret_cast<short&>(t.y); 
}
}
#endif // HALF_HALF_HPP

namespace std
{

template<size_t I, typename T>
struct tuple_element;

template<size_t I, typename T>
struct tuple_element<I, ::core::tuples::templates::Tuple2<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size;

template<typename T>
struct tuple_size<::core::tuples::templates::Tuple2<T>> : integral_constant<size_t, 2> 
{
};

template<typename T>
struct hash;

template<typename T>
struct hash<::core::tuples::templates::Tuple2<T>>
{
	std::size_t operator()(const ::core::tuples::templates::Tuple2<T>& v) const noexcept
	{
		std::hash<T> hasher;
		std::size_t seed = hasher(v.x) + 0x9e3779b9;
		seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

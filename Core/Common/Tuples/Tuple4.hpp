/*
 *	Name: Tuple4
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <tuple>
#include <cstddef>

namespace core::tuples {
namespace templates {

template<typename T>
struct Tuple4
{
	static constexpr int NUM_COMPONENTS = 4;

	constexpr Tuple4() = default; //: x(), y(), z(), w() {}
	constexpr explicit Tuple4(const T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr Tuple4(const T x, const T y, const T z, const T w) noexcept : x(x), y(y), z(z), w(w) {}
	explicit Tuple4(const std::tuple<T, T, T, T>& t) noexcept : x(std::get<0>(t)), y(std::get<1>(t)), z(std::get<2>(t)), w(std::get<3>(t)) {}
	template<typename U> explicit Tuple4(const std::tuple<U, U, U, U>& t) noexcept : x(T(std::get<0>(t))), y(T(std::get<1>(t))), z(T(std::get<2>(t))), w(T(std::get<3>(t))) {}
	explicit Tuple4(const T* const v) noexcept : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

	explicit operator std::tuple<T, T, T, T>() noexcept { return std::tuple<T, T, T, T>(x, y, z, w); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() noexcept { return std::tuple<U, U, U, U>(U(x), U(y), U(z), U(w)); }
	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }

	bool operator==(const Tuple4& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const Tuple4& v) const noexcept { return !(*this == v); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

	T x, y, z, w;

	//union
	//{
	//	struct { T x, y, z, w; };
	//	struct { T r, g, b, a; };
	//};
};

template<std::size_t I, typename T>
inline T& get(Tuple4<T>& v) noexcept
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
inline const T& get(const Tuple4<T>& v) noexcept
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
inline T&& get(Tuple4<T>&& v) noexcept
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
inline const T&& get(const Tuple4<T>&& v) noexcept
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

template<typename T>
template<std::size_t I>
inline T& Tuple4<T>::get()
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	else if constexpr (I == 3)
		return w;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& Tuple4<T>::get() const
{
	if constexpr (I == 0)
		return x;
	else if constexpr (I == 1)
		return y;
	else if constexpr (I == 2)
		return z;
	else if constexpr (I == 3)
		return w;
	static_assert(false);
}

} // namespace templates

using UChar4/*Byte4*/ = templates::Tuple4<unsigned char>;
using Short4 = templates::Tuple4<short>;
using UShort4 = templates::Tuple4<unsigned short>;
using Int4 = templates::Tuple4<int>;
using UInt4 = templates::Tuple4<unsigned int>;
using LongLong4 = templates::Tuple4<long long>;
using Float4 = templates::Tuple4<float>;
using Double4 = templates::Tuple4<double>;

#ifdef HALF_HALF_HPP
using Half4 = templates::Tuple4<half_float::half>;
#endif // HALF_HALF_HPP

} // namespace core::tuples

#ifdef HALF_HALF_HPP
namespace ::core::serialization { 
template<class A> 
inline void serialize(A& ar, core::tuples::templates::Tuple4<half_float::half>& t, const unsigned int version) 
{ 
    ar & reinterpret_cast<short&>(t.x) & reinterpret_cast<short&>(t.y) & 
        reinterpret_cast<short&>(t.z) & reinterpret_cast<short&>(t.w); 
}
}
#endif // HALF_HALF_HPP

namespace std
{

template<size_t I, typename T>
struct tuple_element;

template<typename T>
struct tuple_size;

template<size_t I, typename T>
struct tuple_element<I, core::tuples::templates::Tuple4<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size<core::tuples::templates::Tuple4<T>> : integral_constant<size_t, 4> 
{
};

} // namespace std

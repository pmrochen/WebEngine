/*
 *	Name: Tuple2
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_TUPLES_TUPLE2_HPP
#define CORE_TUPLES_TUPLE2_HPP

namespace core {
namespace tuples {
namespace templates {

template<typename T>
struct Tuple2
{
	static constexpr int NUM_COMPONENTS = 2;

	constexpr Tuple2() = default; //: x(), y() {}
	constexpr explicit Tuple2(const T scalar) noexcept : x(scalar), y(scalar) {}
	constexpr Tuple2(const T x, const T y) noexcept : x(x), y(y) {}
	explicit Tuple2(const T* const v) noexcept : x(v[0]), y(v[1]) {}

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

} // namespace tuples
} // namespace core

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

#endif /* CORE_TUPLES_TUPLE2_HPP */

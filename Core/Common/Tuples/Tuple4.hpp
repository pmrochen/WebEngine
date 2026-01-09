/*
 *	Name: Tuple4
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_TUPLES_TUPLE4_HPP
#define CORE_TUPLES_TUPLE4_HPP

namespace core {
namespace tuples {
namespace templates {

template<typename T>
struct Tuple4
{
	static constexpr int NUM_COMPONENTS = 4;

	constexpr Tuple4() = default; //: x(), y(), z(), w() {}
	constexpr explicit Tuple4(const T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr Tuple4(const T x, const T y, const T z, const T w) noexcept : x(x), y(y), z(z), w(w) {}
	explicit Tuple4(const T* const v) noexcept : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

	explicit operator T*() noexcept { return &x; }
	explicit operator const T*() const noexcept { return &x; }

	bool operator==(const Tuple4& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
	bool operator!=(const Tuple4& v) const noexcept { return !(*this == v); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	T x, y, z, w;

	//union
	//{
	//	struct { T x, y, z, w; };
	//	struct { T r, g, b, a; };
	//};
};

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

} // namespace tuples
} // namespace core

#ifdef HALF_HALF_HPP
namespace ::core::serialization { 
template<class A> 
inline void serialize(A& ar, core::tuples::templates::Tuple4<half_float::half>& t, 
    const unsigned int version) 
{ 
    ar & reinterpret_cast<short&>(t.x) & reinterpret_cast<short&>(t.y) & 
        reinterpret_cast<short&>(t.z) & reinterpret_cast<short&>(t.w); 
}
}
#endif // HALF_HALF_HPP

#endif /* CORE_TUPLES_TUPLE4_HPP */

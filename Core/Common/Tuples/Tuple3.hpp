/*
 *	Name: Tuple3
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_TUPLES_TUPLE3_HPP
#define CORE_TUPLES_TUPLE3_HPP

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
	explicit Tuple3(const T* const v) noexcept : x(v[0]), y(v[1]), z(v[2]) {}

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

#endif /* CORE_TUPLES_TUPLE3_HPP */

/*
 *	Name: Tuples
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_TUPLES_HPP
#define CORE_TUPLES_HPP

namespace core {
namespace tuples {
namespace templates {


/*
 *	Tuple2
 */

template<typename T>
struct Tuple2
{
	constexpr Tuple2() = default; //: x(), y() {}
	constexpr explicit Tuple2(T scalar) : x(scalar), y(scalar) {}
	constexpr Tuple2(T x, T y) : x(x), y(y) {}
	explicit Tuple2(const T* v) : x(v[0]), y(v[1]) {}

	/*explicit*/ operator T*() { return &x; }
	/*explicit*/ operator const T*() const { return &x; }

	bool operator==(const Tuple2& v) const { return ((x == v.x) && (y == v.y)); }
	bool operator!=(const Tuple2& v) const { return !(*this == v); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y; }

	T x, y;
};


/*
 *	Tuple3
 */

template<typename T>
struct Tuple3
{
	constexpr Tuple3() = default; //: x(), y(), z() {}
	constexpr explicit Tuple3(T scalar) : x(scalar), y(scalar), z(scalar) {}
	constexpr Tuple3(T x, T y, T z) : x(x), y(y), z(z) {}
	explicit Tuple3(const T* v) : x(v[0]), y(v[1]), z(v[2]) {}

	/*explicit*/ operator T*() { return &x; }
	/*explicit*/ operator const T*() const { return &x; }

	bool operator==(const Tuple3& v) const { return ((x == v.x) && (y == v.y) && (z == v.z)); }
	bool operator!=(const Tuple3& v) const { return !(*this == v); }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z; }

	T x, y, z;
};


/*
 *	Tuple4
 */

template<typename T>
struct Tuple4
{
	constexpr Tuple4() = default; //: x(), y(), z(), w() {}
	constexpr explicit Tuple4(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr Tuple4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
	explicit Tuple4(const T* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

	/*explicit*/ operator T*() { return &x; }
	/*explicit*/ operator const T*() const { return &x; }

	bool operator==(const Tuple4& v) const { return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)); }
	bool operator!=(const Tuple4& v) const { return !(*this == v); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & x & y & z & w; }

	T x, y, z, w;
};


/*
 *	Tuple2x2
 */

template<typename T>
struct Tuple2x2
{
	constexpr Tuple2x2() = default; //: m00(), m01(), m10(), m11() {}
	constexpr Tuple2x2(T m00, T m01, T m10, T m11) : m00(m00), m01(m01), m10(m10), m11(m11) {}
	constexpr Tuple2x2(const Tuple2<T>& row0, const Tuple2<T>& row1) : m00(row0.x), m01(row0.y), m10(row1.x), m11(row1.y) {}
	explicit Tuple2x2(const T* m) : m00(m[0]), m01(m[1]), m10(m[2]), m11(m[3]) {}

	/*explicit*/ operator T*() { return &m00; }
	/*explicit*/ operator const T*() const { return &m00; }
	Tuple2<T>& operator[](int i) { return reinterpret_cast<Tuple2<T>*>(&m00)[i]; }
	const Tuple2<T>& operator[](int i) const { return reinterpret_cast<const Tuple2<T>*>(&m00)[i]; }

	bool operator==(const Tuple2x2& m) const { return ((m00 == m.m00) && (m01 == m.m01) && (m10 == m.m10) && (m11 == m.m11)); }
	bool operator!=(const Tuple2x2& m) const { return !(*this == m); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & m00 & m01 & m10 & m11; }

	T m00, m01;
	T m10, m11;
};


/*
 *	Tuple3x3
 */

template<typename T>
struct Tuple3x3
{
	constexpr Tuple3x3() = default; //: m00(), m01(), m02(), m10(), m11(), m12(), m20(), m21(), m22() {}
	constexpr Tuple3x3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) :
		m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22) {}
	constexpr Tuple3x3(const Tuple3<T>& row0, const Tuple3<T>& row1, const Tuple3<T>& row2) : m00(row0.x), m01(row0.y), m02(row0.z),
		m10(row1.x), m11(row1.y), m12(row1.z), m20(row2.x), m21(row2.y), m22(row2.z) {}
	explicit Tuple3x3(const T* m) : m00(m[0]), m01(m[1]), m02(m[2]), m10(m[3]), m11(m[4]), m12(m[5]), m20(m[6]), m21(m[7]), 
		m22(m[8]) {}

	/*explicit*/ operator T*() { return &m00; }
	/*explicit*/ operator const T*() const { return &m00; }
	Tuple3<T>& operator[](int i) { return reinterpret_cast<Tuple3<T>*>(&m00)[i]; }
	const Tuple3<T>& operator[](int i) const { return reinterpret_cast<const Tuple3<T>*>(&m00)[i]; }

	bool operator==(const Tuple3x3& m) const { return ((m00 == m.m00) && (m01 == m.m01) && (m02 == m.m02) && (m10 == m.m10) && 
		(m11 == m.m11) && (m12 == m.m12) && (m20 == m.m20) && (m21 == m.m21) && (m22 == m.m22)); }
	bool operator!=(const Tuple3x3& m) const { return !(*this == m); }
	
	template<class A> void serialize(A& ar, const unsigned int version) { ar & m00 & m01 & m02 & m10 & m11 & m12 & m20 & m21 & m22; }

	T m00, m01, m02;
	T m10, m11, m12;
	T m20, m21, m22;
};


/*
 *	Tuple4x3
 */

template<typename T>
struct Tuple4x3
{
	constexpr Tuple4x3() = default; //: m00(), m01(), m02(), m10(), m11(), m12(), m20(), m21(), m22(), m30(), m31(), m32() {}
	constexpr Tuple4x3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22, 
		T m30, T m31, T m32) : m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22), 
		m30(m30), m31(m31), m32(m32) {}
	constexpr Tuple4x3(const Tuple3<T>& row0, const Tuple3<T>& row1, const Tuple3<T>& row2, const Tuple3<T>& row3) :
		m00(row0.x), m01(row0.y), m02(row0.z), m10(row1.x), m11(row1.y), m12(row1.z), m20(row2.x), m21(row2.y), m22(row2.z),
		m30(row3.x), m31(row3.y), m32(row3.z) {}
	explicit Tuple4x3(const T* m) : m00(m[0]), m01(m[1]), m02(m[2]), m10(m[3]), m11(m[4]), m12(m[5]), m20(m[6]), m21(m[7]), 
		m22(m[8]), m30(m[9]), m31(m[10]), m32(m[11]) {}

	/*explicit*/ operator T*() { return &m00; }
	/*explicit*/ operator const T*() const { return &m00; }
	Tuple3<T>& operator[](int i) { return reinterpret_cast<Tuple3<T>*>(&m00)[i]; }
	const Tuple3<T>& operator[](int i) const { return reinterpret_cast<const Tuple3<T>*>(&m00)[i]; }

	bool operator==(const Tuple4x3& m) const { return ((m00 == m.m00) && (m01 == m.m01) && (m02 == m.m02) && (m10 == m.m10) && 
		(m11 == m.m11) && (m12 == m.m12) && (m20 == m.m20) && (m21 == m.m21) && (m22 == m.m22) && (m30 == m.m30) && 
		(m31 == m.m31) && (m32 == m.m32)); }
	bool operator!=(const Tuple4x3& m) const { return !(*this == m); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & m00 & m01 & m02 & m10 & m11 & m12 & m20 & m21 & m22 &
		m30 & m31 & m32; }

	T m00, m01, m02;
	T m10, m11, m12;
	T m20, m21, m22;
	T m30, m31, m32;
};


/*
 *	Tuple4x4
 */

template<typename T>
struct Tuple4x4
{
	constexpr Tuple4x4() = default; //: m00(), m01(), m02(), m03(), m10(), m11(), m12(), m13(), m20(), m21(), m22(), m23(), m30(), m31(), m32(), m33() {}
	constexpr Tuple4x4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, 
		T m22, T m23, T m30, T m31, T m32, T m33) : m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11),
		m12(m12), m13(m13), m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31), m32(m32), m33(m33) {}
	constexpr Tuple4x4(const Tuple4<T>& row0, const Tuple4<T>& row1, const Tuple4<T>& row2, const Tuple4<T>& row3) :
		m00(row0.x), m01(row0.y), m02(row0.z), m03(row0.w), m10(row1.x), m11(row1.y), m12(row1.z), m13(row1.w), 
		m20(row2.x), m21(row2.y), m22(row2.z), m23(row2.w), m30(row3.x), m31(row3.y), m32(row3.z), m33(row3.w) {}
	explicit Tuple4x4(const T* m) : m00(m[0]), m01(m[1]), m02(m[2]), m03(m[3]), m10(m[4]), m11(m[5]), m12(m[6]), m13(m[7]), 
		m20(m[8]), m21(m[9]), m22(m[10]), m23(m[11]), m30(m[12]), m31(m[13]), m32(m[14]), m33(m[15]) {}

	/*explicit*/ operator T*() { return &m00; }
	/*explicit*/ operator const T*() const { return &m00; }
	Tuple4<T>& operator[](int i) { return reinterpret_cast<Tuple4<T>*>(&m00)[i]; }
	const Tuple4<T>& operator[](int i) const { return reinterpret_cast<const Tuple4<T>*>(&m00)[i]; }

	bool operator==(const Tuple4x4& m) const { return ((m00 == m.m00) && (m01 == m.m01) && (m02 == m.m02) && (m03 == m.m03) && 
		(m10 == m.m10) && (m11 == m.m11) && (m12 == m.m12) && (m13 == m.m13) && (m20 == m.m20) && (m21 == m.m21) && (m22 == m.m22) && 
		(m23 == m.m23) && (m30 == m.m30) && (m31 == m.m31) && (m32 == m.m32) && (m33 == m.m33)); }
	bool operator!=(const Tuple4x4& m) const { return !(*this == m); }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & m00 & m01 & m02 & m03 & m10 & m11 & m12 & m13 & m20 &
		m21 & m22 & m23 & m30 & m31 & m32 & m33; }

	T m00, m01, m02, m03;
	T m10, m11, m12, m13;
	T m20, m21, m22, m23;
	T m30, m31, m32, m33;
};


} // namespace templates


/*
 *	UChar
 */

using UChar2/*Byte2*/ = templates::Tuple2<unsigned char>;
using UChar3/*Byte3*/ = templates::Tuple3<unsigned char>;
using UChar4/*Byte4*/ = templates::Tuple4<unsigned char>;


/*
 *	Short
 */

using Short2 = templates::Tuple2<short>;
//using Short3 = templates::Tuple3<short>;
using Short4 = templates::Tuple4<short>;


/*
 *	UShort
 */

using UShort2 = templates::Tuple2<unsigned short>;
//using UShort3 = templates::Tuple3<unsigned short>;
using UShort4 = templates::Tuple4<unsigned short>;


/*
 *	Int
 */

using Int2 = templates::Tuple2<int>;
using Int3 = templates::Tuple3<int>;
using Int4 = templates::Tuple4<int>;


/*
 *	UInt
 */

using UInt2 = templates::Tuple2<unsigned int>;
using UInt3 = templates::Tuple3<unsigned int>;
using UInt4 = templates::Tuple4<unsigned int>;


/*
 *	LongLong
 */

using LongLong2 = templates::Tuple2<long long>;
using LongLong3 = templates::Tuple3<long long>;
using LongLong4 = templates::Tuple4<long long>;


/*
 *	Half
 */

#ifdef HALF_HALF_HPP
using Half2 = templates::Tuple2<half_float::half>;
using Half4 = templates::Tuple4<half_float::half>;
namespace ::core::serialization { 
template<class A> void serialize(A& ar, Half2& t, const unsigned int version) { ar & reinterpret_cast<short&>(t.x) & 
	reinterpret_cast<short&>(t.y); }
template<class A> void serialize(A& ar, Half4& t, const unsigned int version) { ar & reinterpret_cast<short&>(t.x) & 
	reinterpret_cast<short&>(t.y) & reinterpret_cast<short&>(t.z) & reinterpret_cast<short&>(t.w); }
}
#endif // HALF_HALF_HPP


/*
 *	Float
 */

using Float2 = templates::Tuple2<float>;
using Float3 = templates::Tuple3<float>;
using Float4 = templates::Tuple4<float>;
using Float2x2 = templates::Tuple2x2<float>;
using Float3x3 = templates::Tuple3x3<float>;
//using Float4x3 = templates::Tuple4x3<float>;
using Float4x4 = templates::Tuple4x4<float>;


/*
 *	Double
 */

using Double2 = templates::Tuple2<double>;
using Double3 = templates::Tuple3<double>;
using Double4 = templates::Tuple4<double>;
using Double2x2 = templates::Tuple2x2<double>;
using Double3x3 = templates::Tuple3x3<double>;
//using Double4x3 = templates::Tuple4x3<double>;
using Double4x4 = templates::Tuple4x4<double>;


} // namespace tuples
} // namespace core

#endif /* CORE_TUPLES_HPP */

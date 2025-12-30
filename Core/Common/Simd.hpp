/*
 *	Name: Simd
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_SIMD_HPP
#define CORE_SIMD_HPP

#if defined(__SSE4_1__) || defined(__AVX__)
#define SIMD_SSE 4
#elif defined(__SSE3__) || defined(__AVX__)
#define SIMD_SSE 3
#elif (_M_IX86_FP >= 2) || defined(_M_X64) || defined(__SSE2__)
#define SIMD_SSE 2
#elif (_M_IX86_FP >= 1) || defined(_M_X64) || defined(__SSE__)
#define SIMD_SSE 1
#endif

#if defined(__AVX2__)
#define SIMD_AVX 2
#elif defined(__AVX__)
#define SIMD_AVX 1
#endif

#ifdef SIMD_SSE
#define SIMD_HAS_FLOAT4 1
#else
#define SIMD_HAS_FLOAT4 0
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
#define SIMD_CALL __vectorcall
#else
#define SIMD_CALL
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM64))
#include <intrin.h> // for _tzcnt_u32, __popcnt
#elif defined(__cpp_lib_bitops)
#include <bit> // C++20
#endif

//#if defined(SIMD_SSE) || defined(SIMD_AVX)
//#include <intrin.h> // for __cpuid
//#endif

#ifdef SIMD_SSE
#include <xmmintrin.h>
#if (SIMD_SSE >= 2)
#include <emmintrin.h>
#endif
#if (SIMD_SSE >= 3)
#include <pmmintrin.h>
#endif
#if (SIMD_SSE >= 4)
#include <smmintrin.h> // SSE 4.1
#endif
#endif

#ifdef SIMD_AVX
#include <immintrin.h>
#endif

#include <cstdint>
#include <cstring> // for std::memcpy

namespace core {
namespace simd {

namespace bitops {

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
inline unsigned int ctz(unsigned int x) { return _tzcnt_u32(x); }
#elif defined(_MSC_VER) && defined(_M_ARM64)
inline unsigned int ctz(unsigned int x) { return __popcnt((x & -x) - 1); }
#elif defined(__GNUC__) || defined(__clang__)
inline unsigned int ctz(unsigned int x) { return __builtin_ctz(x); }
#elif defined(__cpp_lib_bitops)
inline unsigned int ctz(unsigned int x) { return std::countr_zero(x); }
#else
inline unsigned int ctz(unsigned int x) { x = (x & -x) - 1; x = x - ((x >> 1) & 0x55555555); x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F; x = x + (x >> 8); x = x + (x >> 16); return x & 0x3F; }
#endif

} // namespace bitops

namespace alignment {

template<int A, typename T>
inline bool isAligned(const T& t) noexcept { return !(reinterpret_cast<size_t>(&t) & (A - 1)); }

template<int A, typename T>
inline T align(const T& t) noexcept { /*_MM_ALIGN16*/ T a; std::memcpy/*_inline*/(&a, &t, sizeof(T)); return a; }

} // namespace alignment

#if SIMD_HAS_FLOAT4

//using Float4 = __m128;
//using Bool4 = __m128;

/*inline*/ namespace float4 {

using Type = __m128;
using BoolType = __m128;
constexpr int ALIGNMENT = 16;

template<typename T>
inline bool isAligned(const T& t) noexcept { return alignment::isAligned<16>(t); }

template<typename T>
inline T align(const T& t) noexcept { return alignment::align<16>(t); }

namespace detail {

_MM_ALIGN16 struct UInt32M128
{
	inline operator __m128() const noexcept { return v; }
	//inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	//inline operator __m128d() const noexcept { return _mm_castps_pd(v); }

	union { std::uint32_t u[4]; __m128 v; };
};

const UInt32M128 ZERO4 = { { { 0x00000000, 0x00000000, 0x00000000, 0x00000000 } } };
const UInt32M128 ONE1 = { { { 0x3F800000, 0x00000000, 0x00000000, 0x00000000 } } };
const UInt32M128 ONE4 = { { { 0x3F800000, 0x3F800000, 0x3F800000, 0x3F800000 } } };
const UInt32M128 HALF4 = { { { 0x3F000000, 0x3F000000, 0x3F000000, 0x3F000000 } } };
const UInt32M128 THREE4 = { { { 0x40400000, 0x40400000, 0x40400000, 0x40400000 } } };
const UInt32M128 ZERO3_ONE1 = { { { 0x00000000, 0x00000000, 0x00000000, 0x3F800000 } } };
const UInt32M128 MASK1 = { { { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } } };
const UInt32M128 MASK2 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
const UInt32M128 MASK3 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
const UInt32M128 MASK4 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF } } };
const UInt32M128 ADD_SUB_SIGN = { { { 0x80000000, 0x00000000, 0x80000000, 0x00000000 } } };
const UInt32M128 SIGN2 = { { { 0x80000000, 0x80000000, 0x00000000, 0x00000000 } } };
const UInt32M128 SIGN3 = { { { 0x80000000, 0x80000000, 0x80000000, 0x00000000 } } };
const UInt32M128 SIGN4 = { { { 0x80000000, 0x80000000, 0x80000000, 0x80000000 } } };
const UInt32M128 COMPONENT_MASKS[] = { { { { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } } },
	{ { { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 } } },
	{ { { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 } } },
	{ { { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF } } } };

//template<typename T>
//inline bool isAligned16(const T& t) noexcept { return !((size_t)(&t) & 0xF); }

} // namespace detail 

constexpr int X = 0;
constexpr int Y = 1;
constexpr int Z = 2;
constexpr int W = 3;

constexpr int XYYY = _MM_SHUFFLE(1, 1, 1, 0);
constexpr int XZZZ = _MM_SHUFFLE(2, 2, 2, 0);
//constexpr int XWWW = _MM_SHUFFLE(3, 3, 3, 0);
constexpr int XYZZ = _MM_SHUFFLE(2, 2, 1, 0);
//constexpr int XYWW = _MM_SHUFFLE(3, 3, 1, 0);
constexpr int ZYYY = _MM_SHUFFLE(1, 1, 1, 2);

/*template<int I0, int I1, int I2, int I3>
static inline __m128i constant4i() 
{
    static const union 
	{
        int i[4];
        __m128i xmm;
    } u = { { I0, I1, I2, I3 } };
    return u.xmm;
}*/

inline __m128 zero()
{
	return _mm_setzero_ps();
}

inline __m128 set1(float s)
{
	return _mm_set_ss(s);
}

inline __m128 set2(float s)
{ 
	const __m128 t = _mm_set_ss(s);
	return _mm_unpacklo_ps(t, t); 
}

inline __m128 set2(float x, float y)
{ 
	return _mm_unpacklo_ps(_mm_set_ss(x), _mm_set_ss(y)); 
}

//inline __m128 set2(__m128 xy) // Use cutoff2 instead
//{
//	return _mm_and_ps(xy, detail::MASK2);
//}

inline __m128 set3(float s)
{
	const __m128 t = _mm_set_ss(s);
	return _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 0, 0, 0));
}

inline __m128 set3(float x, float y, float z)
{
	return _mm_movelh_ps(_mm_unpacklo_ps(_mm_set_ss(x), _mm_set_ss(y)), _mm_set_ss(z));
}

//inline __m128 set3(__m128 xyz) // Use cutoff3 instead
//{
//	return _mm_and_ps(xyz, detail::MASK3);
//}

inline __m128 set4(float s)
{
	return _mm_set_ps1(s);
}

inline __m128 set4(float x, float y, float z, float w)
{ 
	return _mm_setr_ps(x, y, z, w); 
}

//inline __m128 set4(__m128 xy, __m128 zw) // Use combine2 instead
//{
//	return _mm_movelh_ps(xy, zw);
//}

//inline __m128 set4(__m128 xyz, float w) // Use insert instead
//{
//	const __m128 t = _mm_set_ss(w);
//	return _mm_or_ps(_mm_and_ps(xyz, detail::MASK3), _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 1, 1, 1)));
//}

inline __m128 load2(const float* v) 
{
	return _mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])); 
}

//inline __m128 load2Broadcast(const float* v) 
//{
//	__m128 v0 = _mm_load_ss(&v[0]);
//	__m128 v1 = _mm_load_ss(&v[1]);
//	return _mm_or_ps(v0, _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(0, 0, 0, 1))); 
//}
//
//inline __m128 load2ZeroOne(const float* v) 
//{
//	return _mm_or_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), detail::ZERO3_ONE1);
//}
//
//inline __m128 unpack2(const float* v, __m128 remainder)
//{
//	return _mm_or_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), remainder/*_mm_andnot_ps(detail::MASK2, remainder)*/);
//}

inline __m128 load3(const float* v)
{
	return _mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), _mm_load_ss(&v[2]));
}

//inline __m128 load3Broadcast(const float* v)
//{
//	__m128 v01 = _mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1]));
//	__m128 v2 = _mm_load_ss(&v[2]);
//	return _mm_or_ps(v01, _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(0, 0, 1, 1))); 
//}
//
//inline __m128 load3One(const float* v) 
//{ 
//	return _mm_or_ps(_mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), _mm_load_ss(&v[2])), detail::ZERO3_ONE1);
//}
//
//inline __m128 unpack3(const float* v, __m128 remainder)
//{
//	return _mm_or_ps(_mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), _mm_load_ss(&v[2])), 
//		remainder/*_mm_andnot_ps(detail::MASK3, remainder)*/);
//}

inline __m128 load4(const float* v) 
{ 
	return _mm_loadu_ps(v); 
}

//inline void load(const Matrix2& m, __m128& row0, __m128& row1)
//{
//	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::MASK2);
//	row1 = _mm_unpacklo_ps(_mm_load_ss(&m.m10), _mm_load_ss(&m.m11));
//}
//
//inline void load(const Matrix2& m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
//{
//	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::MASK2);
//	row1 = _mm_unpacklo_ps(_mm_load_ss(&m.m10), _mm_load_ss(&m.m11));
//	row2 = _mm_setzero_ps();
//	row3 = detail::ZERO3_ONE1;
//}
//
//inline void load(const Matrix3& m, __m128& row0, __m128& row1, __m128& row2)
//{
//	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::MASK3);
//	row1 = _mm_and_ps(_mm_loadu_ps(&m.m10), detail::MASK3);
//	row2 = _mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&m.m20), _mm_load_ss(&m.m21)), _mm_load_ss(&m.m22));
//}
//
//inline void load(const Matrix3& m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
//{
//	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::MASK3);
//	row1 = _mm_and_ps(_mm_loadu_ps(&m.m10), detail::MASK3);
//	row2 = _mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&m.m20), _mm_load_ss(&m.m21)), _mm_load_ss(&m.m22));
//	row3 = detail::ZERO3_ONE1;
//}
//
//inline void load(const AffineTransform& m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
//{
//	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::MASK3);
//	row1 = _mm_and_ps(_mm_loadu_ps(&m.m10), detail::MASK3);
//	row2 = _mm_and_ps(_mm_loadu_ps(&m.m20), detail::MASK3);
//	row3 = _mm_or_ps(_mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&m.x), _mm_load_ss(&m.y)), _mm_load_ss(&m.z)), detail::ZERO3_ONE1);
//}
//
//inline void load4x4(const float* m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
//{
//	row0 = _mm_loadu_ps(&m[0]);
//	row1 = _mm_loadu_ps(&m[4]);
//	row2 = _mm_loadu_ps(&m[8]);
//	row3 = _mm_loadu_ps(&m[12]);
//}

inline void store2(__m128 u, float* v) 
{ 
	//_mm_storel_pi((__m64*)&v[0], u); 
	_mm_store_ss(&v[0], u);
	_mm_store_ss(&v[1], _mm_shuffle_ps(u, u, _MM_SHUFFLE(1, 1, 1, 1)));
}

inline void store3(__m128 u, float* v) 
{ 
	//_mm_storel_pi((__m64*)&v[0], u);
	//_mm_store_ss(&v[2], _mm_shuffle_ps(u, u, _MM_SHUFFLE(2, 2, 2, 2)));
	_mm_store_ss(&v[0], u);
	_mm_store_ss(&v[1], _mm_shuffle_ps(u, u, _MM_SHUFFLE(1, 1, 1, 1)));
	_mm_store_ss(&v[2], _mm_shuffle_ps(u, u, _MM_SHUFFLE(2, 2, 2, 2)));
}

inline void store4(__m128 u, float* v) 
{ 
	_mm_storeu_ps(v, u); 
}

inline void pack2x2(__m128 row0, __m128 row1, float* m)
{
	_mm_storeu_ps(m, _mm_movelh_ps(row0, row1));
}

inline void pack3x3(__m128 row0, __m128 row1, __m128 row2, float* m)
{
	row0 = _mm_shuffle_ps(row0, _mm_shuffle_ps(row0, row1, _MM_SHUFFLE(0, 0, 2, 2)), _MM_SHUFFLE(2, 0, 1, 0));
	row1 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(1, 0, 2, 1));
	row2 = _mm_shuffle_ps(row2, row2, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_storeu_ps(&m[0], row0);
	_mm_storeu_ps(&m[4], row1);
	_mm_store_ss(&m[8], row2);
}

inline void pack4x3(__m128 row0, __m128 row1, __m128 row2, __m128 row3, float* m)
{
	__m128 row1x = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(1, 0, 2, 1));
	row1 = _mm_shuffle_ps(row1, row0, _MM_SHUFFLE(2, 2, 0, 0));
	row0 = _mm_shuffle_ps(row0, row1, _MM_SHUFFLE(0, 2, 1, 0));
	row2 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(0, 0, 2, 2));
	row2 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(2, 1, 2, 0));
	_mm_storeu_ps(&m[0], row0);
	_mm_storeu_ps(&m[4], row1x);
	_mm_storeu_ps(&m[8], row2);
}

//inline void store4x4(__m128 row0, __m128 row1, __m128 row2, __m128 row3, float* m) 
//{ 
//	_mm_storeu_ps(&m[0], row0); 
//	_mm_storeu_ps(&m[4], row1);
//	_mm_storeu_ps(&m[8], row2);
//	_mm_storeu_ps(&m[12], row3);
//}

inline float toFloat/*extract*/(__m128 s)
{
	return _mm_cvtss_f32(s);
}

template<int I>
inline float extract(__m128 v)
{
	static_assert((I & ~3) == 0);
	return _mm_cvtss_f32((I == 0) ? v : _mm_shuffle_ps(v, v, _MM_SHUFFLE(I, I, I, I)));
}

inline __m128 cutoff2(__m128 xy)
{
	return _mm_and_ps(xy, detail::MASK2);
}

inline __m128 cutoff3(__m128 xyz)
{
	return _mm_and_ps(xyz, detail::MASK3);
}

template<int I>
inline __m128 insert(float s, __m128 v)
{
	static_assert((I & ~3) == 0);
#if (SIMD_SSE >= 4)
	return _mm_insert_ps(v, _mm_set_ss(s), I << 4); // SSE 4.1
#else
	const __m128 t = _mm_set_ss(s);
	//static const int32_t mask[] = { 0, 0, 0, -1, 0, 0, 0 };
	//return _mm_or_ps(_mm_andnot_ps(_mm_loadu_ps((const float*)(mask + 3 - (I & 3))), v),
	//	_mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 1, 1, 1) ^ (1 << (I + I))));
	return _mm_or_ps(_mm_andnot_ps(detail::COMPONENT_MASKS[I], v),
		(I == 0) ? t : _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 1, 1, 1) ^ (1 << (I + I))));
#endif
}

inline __m128 insert2(__m128 xy, __m128 v)
{
	return _mm_shuffle_ps(xy, v, _MM_SHUFFLE(3, 2, 1, 0));
}

inline __m128 insert3(__m128 xyz, __m128 v)
{
#if (SIMD_SSE >= 4)
	return _mm_blendv_ps(v, xyz, detail::MASK3); // SSE 4.1
#else
	return _mm_or_ps(_mm_and_ps(detail::MASK3, xyz), _mm_andnot_ps(detail::MASK3, v));
#endif
}

inline __m128 combine2(__m128 xy, __m128 zw)
{
	return _mm_movelh_ps(xy, zw);
}

inline __m128 bool4(bool s)
{
	return _mm_castsi128_ps(_mm_set1_epi32(-(int)s));
}

inline __m128 bool4(bool x, bool y, bool z, bool w)
{ 
	return _mm_castsi128_ps(_mm_setr_epi32(-(int)x, -(int)y, -(int)z, -(int)w));
}

inline __m128 and4(__m128 v1, __m128 v2)
{
	return _mm_and_ps(v1, v2);
}

inline __m128 or4(__m128 v1, __m128 v2)
{
	return _mm_or_ps(v1, v2);
}

inline __m128 andNot4(__m128 v1, __m128 v2)
{
	return _mm_andnot_ps(v2, v1);
}

inline __m128 not4(__m128 v)
{
	return _mm_xor_ps(v, _mm_castsi128_ps(_mm_set1_epi32(-1)));
}

inline __m128 select(__m128 b, __m128 v1, __m128 v2) // b ? v1 : v2
{
#if (SIMD_SSE >= 4)
	return _mm_blendv_ps(v2, v1, b); // SSE 4.1
#else
	return _mm_or_ps(_mm_and_ps(b, v1), _mm_andnot_ps(b, v2));
#endif
}

//inline __m128d select(__m128d b, __m128d v1, __m128d v2) // b ? v1 : v2
//{
//#if (SIMD_SSE >= 4)
//	return _mm_blendv_pd(v2, v1, b); // SSE 4.1
//#else
//	return _mm_or_pd(_mm_and_pd(b, v1), _mm_andnot_pd(b, v2)); // SSE 2
//#endif
//}

template<int I>
inline __m128 broadcast(__m128 v)
{
	static_assert((I & ~3) == 0);
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(I, I, I, I));
}

//inline __m128 broadcast(__m128 v, int index)
//{
//	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(index, index, index, index));
//}

template<int M>
inline __m128 swizzle(__m128 v)
{
	static_assert((M & ~0xFF) == 0);
	return _mm_shuffle_ps(v, v, M);
}

//inline __m128 swizzle(__m128 v, int mask)
//{
//	return _mm_shuffle_ps(v, v, mask);
//}

//inline __m128 reverse(__m128 v)
//{
//	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 2, 3));
//}

inline bool all2(__m128 b)
{
	return ((_mm_movemask_ps(b) & 3) == 3);
}

inline bool all3(__m128 b)
{
	return ((_mm_movemask_ps(b) & 7) == 7);
}

inline bool all4(__m128 b)
{
	return ((_mm_movemask_ps(b) /*& 0xF*/) == 0xF);
}

inline bool any2(__m128 b)
{
	return (_mm_movemask_ps(b) & 3);
}

inline bool any3(__m128 b)
{
	return (_mm_movemask_ps(b) & 7);
}

inline bool any4(__m128 b)
{
	return (_mm_movemask_ps(b) /*& 0xF*/);
}

inline int asIndex(__m128 b)
{
	return /*std::countr_zero*/bitops::ctz(_mm_movemask_ps(b));
}

inline __m128 equal(__m128 v1, __m128 v2)
{
	return _mm_cmpeq_ps(v1, v2);
}

inline __m128 lessThan(__m128 v1, __m128 v2)
{
	return _mm_cmplt_ps(v1, v2);
}

inline __m128 lessThanEqual(__m128 v1, __m128 v2)
{
	return _mm_cmple_ps(v1, v2);
}

inline __m128 greaterThan(__m128 v1, __m128 v2)
{
	return _mm_cmpgt_ps(v1, v2);
}

inline __m128 greaterThanEqual(__m128 v1, __m128 v2)
{
	return _mm_cmpge_ps(v1, v2);
}

inline __m128 min4(__m128 v1, __m128 v2)
{
	return _mm_min_ps(v1, v2);
}

inline __m128 max4(__m128 v1, __m128 v2)
{
	return _mm_max_ps(v1, v2);
}

inline __m128 neg2(__m128 v)
{
	return _mm_xor_ps(v, detail::SIGN2);
}

inline __m128 neg3(__m128 v)
{
	return _mm_xor_ps(v, detail::SIGN3);
}

inline __m128 neg4(__m128 v)
{
	return _mm_xor_ps(v, detail::SIGN4/*_mm_castsi128_ps(_mm_set1_epi32(0x80000000))*/);
}

inline __m128 abs4(__m128 v)
{
	return _mm_andnot_ps(detail::SIGN4, v);
}

inline __m128 add4(__m128 v1, __m128 v2)
{
	return _mm_add_ps(v1, v2);
}

inline __m128 sub4(__m128 v1, __m128 v2)
{
	return _mm_sub_ps(v1, v2);
}

inline __m128 addSub4(__m128 v1, __m128 v2)
{
#if (SIMD_SSE >= 3)
	return _mm_addsub_ps(v1, v2);
#else
	return _mm_add_ps(v1, _mm_xor_ps(v2, detail::ADD_SUB_SIGN));
#endif
}

inline __m128 mul4(__m128 v1, __m128 v2)
{
	return _mm_mul_ps(v1, v2);
}

inline __m128 div2(__m128 v1, __m128 v2)
{
	__m128 t = _mm_movelh_ps(v2, detail::ONE4);
	return _mm_div_ps(v1, t);
}

inline __m128 div3(__m128 v1, __m128 v2)
{
#if (SIMD_SSE >= 4)
	__m128 t = _mm_blendv_ps(detail::ZERO3_ONE1, v2, detail::MASK3); // SSE 4.1
#else
	__m128 t = _mm_or_ps(_mm_and_ps(detail::MASK3, v2), detail::ZERO3_ONE1);
#endif
	return _mm_div_ps(v1, t);
}

inline __m128 div4(__m128 v1, __m128 v2)
{
	return _mm_div_ps(v1, v2);
}

inline __m128 mulAdd4(__m128 v1, __m128 v2, __m128 v3)
{
	return _mm_add_ps(_mm_mul_ps(v1, v2), v3);
}

inline __m128 sqrt1(__m128 s)
{
	return _mm_sqrt_ss(s);
}

inline __m128 rcpSqrtApprox1(__m128 s)
{
	__m128 b = _mm_rsqrt_ss(s);
#if (SIMD_SSE >= 2)
	if ((_mm_extract_epi16(_mm_castps_si128(b), 1) & 0x7F80) == 0x7F80) // NaN or infinity
#else
	const __m128 zero = _mm_setzero_ps();
	if (!(_mm_movemask_ps(_mm_cmpeq_ss(_mm_mul_ss(b, zero), zero)) & 1)) // NaN or infinity
#endif
		return b; // preserve NaN/infinity result
	//static const __m128 half = _mm_set_ps1(0.5f);
	//static const __m128 three = _mm_set_ps1(3.f);
	return _mm_mul_ss(_mm_mul_ss(/*half*/detail::HALF4, b), _mm_sub_ss(/*three*/detail::THREE4, _mm_mul_ss(_mm_mul_ss(s, b), b))); // Newton-Raphson step
}

inline __m128 hMin2(__m128 v)
{
	return _mm_min_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 2, 0, 1)));
}

inline __m128 hMin3(__m128 v)
{
	__m128 t = _mm_min_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)));
	return _mm_min_ps(t, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)));
}

inline __m128 hMin4(__m128 v)
{
	__m128 t = _mm_min_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3)));
	return _mm_min_ps(t, _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 0, 3, 2)));
}

inline __m128 hMax2(__m128 v)
{
	return _mm_max_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 2, 0, 1)));
}

inline __m128 hMax3(__m128 v)
{
	__m128 t = _mm_max_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)));
	return _mm_max_ps(t, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)));
}

inline __m128 hMax4(__m128 v)
{
	__m128 t = _mm_max_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3)));
	return _mm_max_ps(t, _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 0, 3, 2)));
}

inline __m128 hAdd2(__m128 v)
{
	return _mm_add_ss(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
}

inline __m128 hAdd3(__m128 v)
{
	__m128 t = _mm_add_ps(v, _mm_movehl_ps(v, v));
	return _mm_add_ss(t, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
}

inline __m128 hAdd4(__m128 v)
{
	__m128 t = _mm_add_ps(v, _mm_movehl_ps(v, v));
	return _mm_add_ss(t, _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 1, 1, 1)));
}

inline __m128 dot2(__m128 v1, __m128 v2)
{
#if (SIMD_SSE >= 4)
	return _mm_dp_ps(v1, v2, 0x31); // SSE 4.1
#else
	__m128 v = _mm_mul_ps(v1, v2);
	return _mm_add_ss(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
#endif
}

inline __m128 dot3(__m128 v1, __m128 v2)
{
#if (SIMD_SSE >= 4)
	return _mm_dp_ps(v1, v2, 0x71/*0x77*/); // SSE 4.1
#else
	__m128 v = _mm_mul_ps(v1, v2);
	__m128 t = _mm_add_ps(v, _mm_movehl_ps(v, v));
	return _mm_add_ss(t, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
#endif
}

inline __m128 dot4(__m128 v1, __m128 v2)
{
#if (SIMD_SSE >= 4)
	return _mm_dp_ps(v1, v2, 0xF1/*0xFF*/); // SSE 4.1
#else
	__m128 v = _mm_mul_ps(v1, v2);
	__m128 t = _mm_add_ps(v, _mm_movehl_ps(v, v));
	return _mm_add_ss(t, _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 1, 1, 1)));
#endif
}

//inline float floor(float x) // pre SSE 4.1
//{
//	__m128 f = _mm_set_ss(x);
//	//static const __m128 one = _mm_set_ss(1.0f);
//	__m128 t = _mm_cvtepi32_ps(_mm_cvttps_epi32(f));
//	__m128 r = _mm_sub_ps(t, _mm_and_ps(_mm_cmplt_ps(f, t), /*one*/detail::ONE1));
//	return _mm_cvtss_f32(r);
//}

inline __m128 floor(__m128 v)
{
#if (SIMD_SSE >= 4)
	return _mm_floor_ps(v); // SSE 4.1
#else
	//static const __m128 one = _mm_set_ps1(1.0f);
	__m128 t = _mm_cvtepi32_ps(_mm_cvttps_epi32(v));
	return _mm_sub_ps(t, _mm_and_ps(_mm_cmplt_ps(v, t), /*one*/detail::ONE4));
#endif
}

inline __m128 round(__m128 v)
{
#if (SIMD_SSE >= 4)
	return _mm_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC); // SSE 4.1
#else
	#error // #TODO
#endif
}

inline __m128 isFinite(__m128 v)
{
#if (SIMD_SSE >= 2)
	const __m128i t = _mm_sll_epi32(_mm_castps_si128(v), _mm_cvtsi32_si128(1)); // SSE 2
	const __m128i m = _mm_set1_epi32(0xFF000000);
	return _mm_castsi128_ps(_mm_xor_si128(_mm_cmpeq_epi32(_mm_and_si128(t, m), m), _mm_set1_epi32(-1)));
#else
	#error // #TODO
#endif
}

inline __m128 isInf(__m128 v)
{
#if (SIMD_SSE >= 2)
	const __m128i t = _mm_sll_epi32(_mm_castps_si128(v), _mm_cvtsi32_si128(1)); // SSE 2
    return _mm_castsi128_ps(_mm_cmpeq_epi32(t, _mm_set1_epi32(0xFF000000)));
#else
	#error // #TODO
#endif
}

} // namespace float4

using Float4 = float4::Type;

#endif /* SIMD_HAS_FLOAT4 */

} // namespace simd
} // namespace core

#endif /* CORE_SIMD_HPP */

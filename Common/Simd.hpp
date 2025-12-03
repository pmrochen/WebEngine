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

#if defined(SIMD_SSE) || defined(SIMD_AVX)
#define SIMD_ENABLED
//#include <intrin.h> // for __cpuid
#endif

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

namespace core {
namespace simd {

#ifdef SIMD_SSE

using Float4 = __m128;

namespace detail {

_MM_ALIGN16 struct UInt32M128
{
	union
	{
		std::uint32_t u[4];
		__m128 v;
	};

	inline operator __m128() const noexcept { return v; }
	//inline operator __m128i() const noexcept { return _mm_castps_si128(v); }
	//inline operator __m128d() const noexcept { return _mm_castps_pd(v); }
};

const UInt32M128 zeroZeroZeroOne = { { { 0x00000000, 0x00000000, 0x00000000, 0x3F800000 } } };
const UInt32M128 maskXY = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
const UInt32M128 maskXYZ = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
const UInt32M128 addSubMask = { { { 0x80000000, 0x00000000, 0x80000000, 0x00000000 } } };

//template<typename T>
//inline bool isAligned16(const T& t) noexcept { return !((size_t)(&t) & 0xF); }

} // namespace detail 

//inline bool isSse3Supported()
//{
//	int info[4] = { -1 };
//	__cpuid(info, 0);
//	if (info[0] < 1)
//		return false;
//	__cpuid(info, 1);
//	return ((info[2] & 0x1) != 0);
//}
//
//inline bool isSse41Supported()
//{
//	int info[4] = { -1 };
//	__cpuid(info, 0);
//	if (info[0] < 1)
//		return false;
//	__cpuid(info, 1);
//	return ((info[2] & 0x80000) == 0x80000);
//}
//
//inline bool isAvxSupported()
//{
//	int info[4] = { -1 };
//	__cpuid(info, 0);
//	if (info[0] < 1)
//		return false;
//	__cpuid(info, 1);
//	return ((info[2] & 0x18080001) == 0x18080001);
//}
//
//inline bool isAvx2Supported()
//{
//	int info[4] = { -1 };
//	__cpuid(info, 0);
//	if (info[0] < 7)
//		return false;
//	__cpuid(info, 1);
//	if ((info[2] & 0x38081001) != 0x38081001)
//		return false;
//	__cpuidex(info, 7, 0);
//	return ((info[1] & 0x20) == 0x20);
//}

//inline float floor(float x) // pre SSE 4.1
//{
//	__m128 f = _mm_set_ss(x);
//	static const __m128 one = _mm_set_ss(1.0f);
//	__m128 t = _mm_cvtepi32_ps(_mm_cvttps_epi32(f));
//	__m128 r = _mm_sub_ps(t, _mm_and_ps(_mm_cmplt_ps(f, t), one));
//	return _mm_cvtss_f32(r);
//}

inline __m128 set4(float s)
{
	return _mm_set_ps1(s);
}

inline __m128 load2(const float* v) 
{
	return _mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])); 
}

inline __m128 load2ZeroOne(const float* v) 
{
	return _mm_or_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), detail::zeroZeroZeroOne);
}

inline __m128 load3(const float* v)
{
	return _mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), _mm_load_ss(&v[2]));
}

inline __m128 load3One(const float* v) 
{ 
	return _mm_or_ps(_mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&v[0]), _mm_load_ss(&v[1])), _mm_load_ss(&v[2])), detail::zeroZeroZeroOne);
}

inline __m128 load4(const float* v) 
{ 
	return _mm_loadu_ps(v); 
}
/*
inline void load(const Matrix2& m, __m128& row0, __m128& row1)
{
	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::maskXY);
	row1 = _mm_unpacklo_ps(_mm_load_ss(&m.m10), _mm_load_ss(&m.m11));
}

inline void load(const Matrix2& m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
{
	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::maskXY);
	row1 = _mm_unpacklo_ps(_mm_load_ss(&m.m10), _mm_load_ss(&m.m11));
	row2 = _mm_setzero_ps();
	row3 = detail::zeroZeroZeroOne;
}

inline void load(const Matrix3& m, __m128& row0, __m128& row1, __m128& row2)
{
	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::maskXYZ);
	row1 = _mm_and_ps(_mm_loadu_ps(&m.m10), detail::maskXYZ);
	row2 = _mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&m.m20), _mm_load_ss(&m.m21)), _mm_load_ss(&m.m22));
}

inline void load(const Matrix3& m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
{
	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::maskXYZ);
	row1 = _mm_and_ps(_mm_loadu_ps(&m.m10), detail::maskXYZ);
	row2 = _mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&m.m20), _mm_load_ss(&m.m21)), _mm_load_ss(&m.m22));
	row3 = detail::zeroZeroZeroOne;
}

inline void load(const AffineTransform& m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
{
	row0 = _mm_and_ps(_mm_loadu_ps(&m.m00), detail::maskXYZ);
	row1 = _mm_and_ps(_mm_loadu_ps(&m.m10), detail::maskXYZ);
	row2 = _mm_and_ps(_mm_loadu_ps(&m.m20), detail::maskXYZ);
	row3 = _mm_or_ps(_mm_movelh_ps(_mm_unpacklo_ps(_mm_load_ss(&m.x), _mm_load_ss(&m.y)), _mm_load_ss(&m.z)), detail::zeroZeroZeroOne);
}
*/
inline void load4x4(const float* m, __m128& row0, __m128& row1, __m128& row2, __m128& row3)
{
	row0 = _mm_loadu_ps(&m[0]);
	row1 = _mm_loadu_ps(&m[4]);
	row2 = _mm_loadu_ps(&m[8]);
	row3 = _mm_loadu_ps(&m[12]);
}

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

inline void store2x2(__m128 row0, __m128 row1, float* m)
{
	_mm_storeu_ps(m, _mm_movelh_ps(row0, row1));
}

inline void store3x3(__m128 row0, __m128 row1, __m128 row2, float* m)
{
	row0 = _mm_shuffle_ps(row0, _mm_shuffle_ps(row0, row1, _MM_SHUFFLE(0, 0, 2, 2)), _MM_SHUFFLE(2, 0, 1, 0));
	row1 = _mm_shuffle_ps(row1, row2, _MM_SHUFFLE(1, 0, 2, 1));
	row2 = _mm_shuffle_ps(row2, row2, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_storeu_ps(&m[0], row0);
	_mm_storeu_ps(&m[4], row1);
	_mm_store_ss(&m[8], row2);
}

inline void store4x3(__m128 row0, __m128 row1, __m128 row2, __m128 row3, float* m)
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

inline void store4x4(__m128 row0, __m128 row1, __m128 row2, __m128 row3, float* m) 
{ 
	_mm_storeu_ps(&m[0], row0); 
	_mm_storeu_ps(&m[4], row1);
	_mm_storeu_ps(&m[8], row2);
	_mm_storeu_ps(&m[12], row3);
}

inline float asFloat(__m128 s)
{
	return _mm_cvtss_f32(s);
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
	return _mm_add_ps(v1, _mm_xor_ps(v2, detail::addSubMask));
#endif
}

inline __m128 mul4(__m128 v1, __m128 v2)
{
	return _mm_mul_ps(v1, v2);
}

inline __m128 sqrt1(__m128 s)
{
	return _mm_sqrt_ss(s);
}

inline __m128 hadd2(__m128 v)
{
	return _mm_add_ss(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
}

inline __m128 hadd3(__m128 v)
{
	__m128 t = _mm_add_ps(v, _mm_movehl_ps(v, v));
	return _mm_add_ss(t, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
}

inline __m128 hadd4(__m128 v)
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

//inline __m128 length2(__m128 v)
//{
//	return _mm_sqrt_ss(dot2(v, v));
//}

//inline __m128 length3(__m128 v)
//{
//	return _mm_sqrt_ss(dot3(v, v));
//}

//inline __m128 length4(__m128 v)
//{
//	return _mm_sqrt_ss(dot4(v, v));
//}

#endif /* SIMD_SSE */

} // namespace simd
} // namespace core

#endif /* CORE_SIMD_HPP */

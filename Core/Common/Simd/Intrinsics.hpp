/*
 *	Name: Intrinsics
 *	Author: Pawel Mrochen
 */

#pragma once

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
#define SIMD_CALL __vectorcall
#else
#define SIMD_CALL
#endif

#include "SseIntrinsics.hpp"
#include "AvxIntrinsics.hpp"

#ifndef SIMD_HAS_FLOAT4
#define SIMD_HAS_FLOAT4 0
#endif

namespace core::simd {

#ifdef SIMD_SSE
using namespace sse;
#endif

#ifdef SIMD_AVX
using namespace avx;
#endif

#if defined(SIMD_SSE) /*|| defined(SIMD_AVX)*/

template<typename T> inline T xxxx(T v) { return broadcast<X>(v); }
template<typename T> inline T yyyy(T v) { return broadcast<Y>(v); }
template<typename T> inline T zzzz(T v) { return broadcast<Z>(v); }
template<typename T> inline T wwww(T v) { return broadcast<W>(v); }
template<typename T> inline T xyyy(T v) { return swizzle<X, Y, Y, Y>(v); }
template<typename T> inline T xyxy(T v) { return swizzle<X, Y, X, Y>(v); }
template<typename T> inline T xxyy(T v) { return swizzle<X, X, Y, Y>(v); }
template<typename T> inline T xzzz(T v) { return swizzle<X, Z, Z, Z>(v); }
template<typename T> inline T xwww(T v) { return swizzle<X, W, W, W>(v); }
template<typename T> inline T xyzz(T v) { return swizzle<X, Y, Z, Z>(v); }
template<typename T> inline T xyww(T v) { return swizzle<X, Y, W, W>(v); }
template<typename T> inline T xzww(T v) { return swizzle<X, Z, W, W>(v); }
template<typename T> inline T yxxx(T v) { return swizzle<Y, X, X, X>(v); }
template<typename T> inline T yzzz(T v) { return swizzle<Y, Z, Z, Z>(v); }
template<typename T> inline T yzxx(T v) { return swizzle<Y, Z, X, X>(v); }
template<typename T> inline T yzxw(T v) { return swizzle<Y, Z, X, W>(v); }
template<typename T> inline T yzww(T v) { return swizzle<Y, Z, W, W>(v); }
template<typename T> inline T ywww(T v) { return swizzle<Y, W, W, W>(v); }
template<typename T> inline T yxzw(T v) { return swizzle<Y, X, Z, W>(v); }
template<typename T> inline T yxwz(T v) { return swizzle<Y, X, W, Z>(v); }
template<typename T> inline T zxxx(T v) { return swizzle<Z, X, X, X>(v); }
template<typename T> inline T zxyy(T v) { return swizzle<Z, X, Y, Y>(v); }
template<typename T> inline T zxyw(T v) { return swizzle<Z, X, Y, W>(v); }
template<typename T> inline T zyyy(T v) { return swizzle<Z, Y, Y, Y>(v); }
template<typename T> inline T zyww(T v) { return swizzle<Z, Y, W, W>(v); }
template<typename T> inline T zzww(T v) { return swizzle<Z, Z, W, W>(v); }
template<typename T> inline T zwww(T v) { return swizzle<Z, W, W, W>(v); }
template<typename T> inline T zwxy(T v) { return swizzle<Z, W, X, Y>(v); }
template<typename T> inline T zwzw(T v) { return swizzle<Z, W, Z, W>(v); }
template<typename T> inline T wxxx(T v) { return swizzle<W, X, X, X>(v); }
template<typename T> inline T wyyy(T v) { return swizzle<W, Y, Y, Y>(v); }
template<typename T> inline T wzzz(T v) { return swizzle<W, Z, Z, Z>(v); }
template<typename T> inline T wzyx(T v) { return swizzle<W, Z, Y, X>(v); }

#endif

} // namespace core::simd

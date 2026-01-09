/*
 *	Name: Intrinsics
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_SIMD_INTRINSICS_HPP
#define CORE_SIMD_INTRINSICS_HPP

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

#if defined(SIMD_SSE) || defined(SIMD_AVX)

template<typename T> inline T xxxx(T v) { return broadcast<X>(v); }
template<typename T> inline T yyyy(T v) { return broadcast<Y>(v); }
template<typename T> inline T zzzz(T v) { return broadcast<Z>(v); }
template<typename T> inline T wwww(T v) { return broadcast<W>(v); }
template<typename T> inline T xyyy(T v) { return swizzle4<XYYY>(v); }
template<typename T> inline T xzzz(T v) { return swizzle4<XZZZ>(v); }
template<typename T> inline T xwww(T v) { return swizzle4<XWWW>(v); }
template<typename T> inline T xyzz(T v) { return swizzle4<XYZZ>(v); }
template<typename T> inline T xyww(T v) { return swizzle4<XYWW>(v); }
template<typename T> inline T xzww(T v) { return swizzle4<XZWW>(v); }
template<typename T> inline T yxxx(T v) { return swizzle4<YXXX>(v); }
template<typename T> inline T yzzz(T v) { return swizzle4<YZZZ>(v); }
template<typename T> inline T yzxx(T v) { return swizzle4<YZXX>(v); }
template<typename T> inline T yzww(T v) { return swizzle4<YZWW>(v); }
template<typename T> inline T ywww(T v) { return swizzle4<YWWW>(v); }
template<typename T> inline T yxzw(T v) { return swizzle4<YXZW>(v); }
template<typename T> inline T yxwz(T v) { return swizzle4<YXWZ>(v); }
template<typename T> inline T zxxx(T v) { return swizzle4<ZXXX>(v); }
template<typename T> inline T zxyy(T v) { return swizzle4<ZXYY>(v); }
template<typename T> inline T zyyy(T v) { return swizzle4<ZYYY>(v); }
template<typename T> inline T zyww(T v) { return swizzle4<ZYWW>(v); }
template<typename T> inline T zwww(T v) { return swizzle4<ZWWW>(v); }
template<typename T> inline T zwxy(T v) { return swizzle4<ZWXY>(v); }
template<typename T> inline T wxxx(T v) { return swizzle4<WXXX>(v); }
template<typename T> inline T wyyy(T v) { return swizzle4<WYYY>(v); }
template<typename T> inline T wzzz(T v) { return swizzle4<WZZZ>(v); }
template<typename T> inline T wzyx(T v) { return swizzle4<WZYX>(v); }

#endif

} // namespace core::simd

#endif /* CORE_SIMD_INTRINSICS_HPP */

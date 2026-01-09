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

#ifdef SIMD_SSE
#define SIMD_HAS_FLOAT4 1
#else
#define SIMD_HAS_FLOAT4 0
#endif

namespace core::simd {

#ifdef SIMD_SSE
//using namespace sse;
namespace float4 = sse::float4;
#endif

} // namespace core::simd

#endif /* CORE_SIMD_INTRINSICS_HPP */

/*
 *	Name: AvxIntrinsics
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_SIMD_AVX_INTRINSICS_HPP
#define CORE_SIMD_AVX_INTRINSICS_HPP

#if defined(__AVX2__)
#define SIMD_AVX 2
#elif defined(__AVX__)
#define SIMD_AVX 1
#endif

#ifdef SIMD_AVX

#include <immintrin.h>

namespace core::simd::avx {
} // namespace core::simd::avx

#endif /* SIMD_AVX */
#endif /* CORE_SIMD_AVX_INTRINSICS_HPP */

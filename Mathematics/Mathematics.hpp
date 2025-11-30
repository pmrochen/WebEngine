/*
 *	Name: Mathematics
 *	Description: 
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_HPP
#define CORE_MATHEMATICS_HPP
/*
#ifndef MATHEMATICS_FAST_NORMALIZE
#define MATHEMATICS_FAST_NORMALIZE 1
#endif

#ifndef MATHEMATICS_USE_SSE
#if defined(__SSE4_1__) || defined(__AVX__)
#define MATHEMATICS_USE_SSE 4
#elif defined(__SSE3__) || defined(__AVX__)
#define MATHEMATICS_USE_SSE 3
#elif (_M_IX86_FP >= 2) || defined(_M_X64) || defined(__SSE2__)
#define MATHEMATICS_USE_SSE 2
#elif (_M_IX86_FP >= 1) || defined(_M_X64) || defined(__SSE__)
#define MATHEMATICS_USE_SSE 1
#endif
#endif

#ifndef MATHEMATICS_USE_AVX
#if defined(__AVX2__)
#define MATHEMATICS_USE_AVX 2
#elif defined(__AVX__)
#define MATHEMATICS_USE_AVX 1
#endif
#endif

#ifndef MATHEMATICS_API
#if defined(MATHEMATICS_DLL) || defined(CORE_DLL)
#if defined(MATHEMATICS_EXPORTS) || defined(CORE_EXPORTS)
#define MATHEMATICS_API __declspec(dllexport)
#else
#define MATHEMATICS_API __declspec(dllimport)
#endif
#else
#define MATHEMATICS_API
#endif
#endif

#ifdef MATHEMATICS_USE_SSE
#include <xmmintrin.h>
#if (MATHEMATICS_USE_SSE >= 2)
#include <emmintrin.h>
#endif
#if (MATHEMATICS_USE_SSE >= 3)
#include <pmmintrin.h>
#endif
#if (MATHEMATICS_USE_SSE >= 4)
#include <smmintrin.h> // SSE 4.1
#endif
#endif

#ifdef MATHEMATICS_USE_AVX
#include <immintrin.h>
#endif

#include <stdexcept>
#include <atomic>
#include <algorithm>*/
#include <limits>
/*#include <vector>
#include <iosfwd>
#include <cmath>
#include <cfloat>
#include <cstddef>
#include <cstdint>
*/
namespace core {
namespace mathematics {


/*
 *	Constants
 */

template<typename T>
struct Constants
{
	static constexpr T /*zero*/tolerance() noexcept { return T(); }
	static constexpr T infinity() noexcept { return T(); }
	static constexpr T e() noexcept { return T(); }
	static constexpr T ln2() noexcept { return T(); }
	static constexpr T pi() noexcept { return T(); }
	static constexpr T piSquare() noexcept { return T(); }
	static constexpr T halfPi() noexcept { return T(); }
	static constexpr T twoPi() noexcept { return T(); }
	static constexpr T inversePi() noexcept { return T(); }
	static constexpr T sqrt2() noexcept { return T(); }
	static constexpr T sqrt3() noexcept { return T(); }
	static constexpr T degToRad() noexcept { return T(); }
	static constexpr T radToDeg() noexcept { return T(); }
};

template<>
struct Constants<float>
{
	static constexpr float /*zero*/tolerance() noexcept { return 1e-6f; }
	static constexpr float infinity() noexcept { return std::numeric_limits<float>::infinity(); }
	static constexpr float e() noexcept { return 2.71828182845904553488f; }
	static constexpr float ln2() noexcept { return 0.693147180559945309417f; }
	static constexpr float pi() noexcept { return 3.14159265358979323846f; }
	static constexpr float piSquare() noexcept { return 9.86960440108935861883f; }
	static constexpr float halfPi() noexcept { return 1.57079632679489661923f; }
	static constexpr float twoPi() noexcept { return 6.28318530717958647693f; }
	static constexpr float inversePi() noexcept { return 0.31830988618379069122f; }
	static constexpr float sqrt2() noexcept { return 1.41421356237309514547f; }
	static constexpr float sqrt3() noexcept { return 1.73205080756887719318f; }
	static constexpr float degToRad() noexcept { return 0.01745329251994329547f; }
	static constexpr float radToDeg() noexcept { return 57.29577951308232286465f; }
};

template<>
struct Constants<double>
{
	static constexpr double /*zero*/tolerance() noexcept { return 1e-15; }
	static constexpr double infinity() noexcept { return std::numeric_limits<double>::infinity(); }
	static constexpr double e() noexcept { return 2.71828182845904553488; }
	static constexpr double ln2() noexcept { return 0.693147180559945309417; }
	static constexpr double pi() noexcept { return 3.14159265358979323846; }
	static constexpr double piSquare() noexcept { return 9.86960440108935861883; }
	static constexpr double halfPi() noexcept { return 1.57079632679489661923; }
	static constexpr double twoPi() noexcept { return 6.28318530717958647693; }
	static constexpr double inversePi() noexcept { return 0.31830988618379069122; }
	static constexpr double sqrt2() noexcept { return 1.41421356237309514547; }
	static constexpr double sqrt3() noexcept { return 1.73205080756887719318; }
	static constexpr double degToRad() noexcept { return 0.01745329251994329547; }
	static constexpr double radToDeg() noexcept { return 57.29577951308232286465; }
};


namespace templates {
} // namespace templates
} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_HPP */

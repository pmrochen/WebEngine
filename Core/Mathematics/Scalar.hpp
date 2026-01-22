/*
 *	Name: Scalar
 *	Author: Pawel Mrochen
 */

#pragma once

#include <utility>
#include <type_traits>
#include <cmath>
#include "Constants.hpp"

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

#ifndef MATHEMATICS_FAST_NORMALIZE
#define MATHEMATICS_FAST_NORMALIZE 1
#endif

namespace core::mathematics {

//extern MATHEMATICS_API std::pair<int, int> /*makeRational*/rational(float x, int maxDenominator);
//extern MATHEMATICS_API std::pair<int, int> /*makeRational*/rational(double x, int maxDenominator);
//extern MATHEMATICS_API float rcpSqrtApprox(float x);
//extern MATHEMATICS_API float sinApprox(float x);
//extern MATHEMATICS_API float cosApprox(float x);
//extern MATHEMATICS_API float atan2Approx(float y, float x);

template<typename T>
inline T sign(T x) 
{ 
	return ((x > T(0)) ? T(1) : ((x < T(0)) ? T(-1) : T(0))); 
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T frac(T x)
{ 
	return (x - std::floor(x)); 
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool approxEquals(T a, T b) 
{ 
	return (std::fabs(b - a) < Constants<T>::TOLERANCE); 
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool approxEquals(T a, T b, T tolerance) 
{ 
	return (std::fabs(b - a) < tolerance); 
}

//template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
//inline bool isNan(T x) 
//{ 
//	return !(x == x); 
//}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T log(T x, T b) // logarithm for a base b
{ 
	return std::log(x)/std::log(b);
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T log2(T x) // base-2 logarithm
{ 
	return std::log(x)/Constants<T>::LN2; 
}

//inline std::pair<float, float> sinCos(float x) { return std::pair<float, float>(std::sin(x), std::cos(x)); }
//inline std::pair<double, double> sinCos(double x) { return std::pair<double, double>(std::sin(x), std::cos(x)); }
//inline void sinCos(float x, float& sine, float& cosine) { sine = std::sin(x); cosine = std::cos(x); }
//inline void sinCos(double x, double& sine, double& cosine) { sine = std::sin(x); cosine = std::cos(x); }

//template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
//inline std::pair<T, T> sinCos(T x) { return std::pair<T, T>(std::sin(x), std::cos(x)); }

//template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
//inline void sinCos(T x, T& sine, T& cosine) { sine = std::sin(x); cosine = std::cos(x); }

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T radians(T x) 
{ 
	return x*Constants<T>::DEG_TO_RAD; 
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T degrees(T x) 
{ 
	return x*Constants<T>::RAD_TO_DEG; 
}

template<typename T>
inline T step(T a, T t) 
{ 
	return (t >= a) ? T(1) : T(0); 
}

template<typename T>
inline T pulse(T a, T b, T t) 
{ 
	return step(a, t) - step(b, t); 
}

template<typename T>
inline T boxStep(T a, T b, T t)
{
	if (t <= a)
		return T(0);
	if (t >= b)
		return T(1);
	return (t - a)/(b - a);
}

template<typename T>
inline T smoothStep(T a, T b, T t)
{
	if (t <= a)
		return T(0);
	if (t >= b)
		return T(1);
	t = (t - a)/(b - a);
	return t*t*(T(3) - T(2)*t);
}

template<typename T>
inline T smootherStep(T a, T b, T t)
{
	if (t <= a)
		return T(0);
	if (t >= b)
		return T(1);
	t = (t - a)/(b - a);
	return t*t*t*(t*(t*T(6) - T(15)) + T(10));
}

} // namespace core::mathematics

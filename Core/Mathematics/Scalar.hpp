/*
 *	Name: Scalar
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_SCALAR_HPP
#define CORE_MATHEMATICS_SCALAR_HPP

#include <cmath>
#include <utility>
#include <type_traits>
#include "Constants.hpp"

namespace core {
namespace mathematics {

template<typename T>
inline T sign(T x) { return ((x > T(0)) ? T(1) : ((x < T(0)) ? T(-1) : T(0))); } // signum

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T frac(T x) { return (x - std::floor(x)); } // fractional part

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool approxEquals(T a, T b) { return (std::fabs(b - a) < Constants<T>::TOLERANCE); } // equality approximation

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline bool approxEquals(T a, T b, T tolerance) { return (std::fabs(b - a) < tolerance); }

//template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
//inline bool isNan(T x) { return !(x == x); }

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T log(T x, T b) { return std::log(x)/std::log(b); } // logarithm for a base b

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T log2(T x) { return std::log(x)/Constants<T>::LN2; } // base-2 logarithm

//inline std::pair<float, float> sinCos(float x) { return std::pair<float, float>(std::sin(x), std::cos(x)); }
//inline std::pair<double, double> sinCos(double x) { return std::pair<double, double>(std::sin(x), std::cos(x)); }
//inline void sinCos(float x, float& sine, float& cosine) { sine = std::sin(x); cosine = std::cos(x); }
//inline void sinCos(double x, double& sine, double& cosine) { sine = std::sin(x); cosine = std::cos(x); }

//template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
//inline std::pair<T, T> sinCos(T x) { return std::pair<T, T>(std::sin(x), std::cos(x)); }

//template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
//inline void sinCos(T x, T& sine, T& cosine) { sine = std::sin(x); cosine = std::cos(x); }

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T radians(T x) { return x*Constants<T>::DEG_TO_RAD; } // degree/radian conversion

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
inline T degrees(T x) { return x*Constants<T>::RAD_TO_DEG; }

template<typename T>
inline T step(T a, T t) { return (t >= a) ? T(1) : T(0); }

template<typename T>
inline T pulse(T a, T b, T t) { return (step(a, t) - step(b, t)); }

} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_SCALAR_HPP */

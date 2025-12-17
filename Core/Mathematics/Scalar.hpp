/*
 *	Name: Scalar
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_MATHEMATICS_SCALAR_HPP
#define CORE_MATHEMATICS_SCALAR_HPP

#include <cmath>
#include <utility>

namespace core {
namespace mathematics {

inline std::pair<float, float> sinCos(float x) { return std::pair<float, float>(std::sin(x), std::cos(x)); }
inline std::pair<double, double> sinCos(double x) { return std::pair<double, double>(std::sin(x), std::cos(x)); }
inline void sinCos(float x, float& sine, float& cosine) { sine = std::sin(x); cosine = std::cos(x); }
inline void sinCos(double x, double& sine, double& cosine) { sine = std::sin(x); cosine = std::cos(x); }

} // namespace mathematics
} // namespace core

#endif /* CORE_MATHEMATICS_SCALAR_HPP */

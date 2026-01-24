/*
 *	Name: YawPitchRoll
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <cstddef>
#include <cmath>
#include <Simd/Intrinsics.hpp>
#include "Constants.hpp"
#include "Scalar.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct Euler;

template<typename T>
struct Matrix3;

template<typename T>
struct YawPitchRoll
{
	using Real = T;

	constexpr YawPitchRoll() noexcept : yaw(), pitch(), roll() {}
	constexpr YawPitchRoll(T yaw, T pitch, T roll) noexcept : yaw(yaw), pitch(pitch), roll(roll) {}
	explicit YawPitchRoll(const Euler<T>& e) noexcept;
	explicit YawPitchRoll(const Quaternion<T>& q) noexcept;
	explicit YawPitchRoll(const Matrix3<T>& m) noexcept;
	//explicit YawPitchRoll(const Vector3<T>& direction) noexcept; // -> fromForward
	//YawPitchRoll(const Vector3<T>& direction, T roll) noexcept;
	explicit YawPitchRoll(const std::tuple<T, T, T>& t) noexcept : yaw(std::get<0>(t)), pitch(std::get<1>(t)), roll(std::get<2>(t)) {}
	template<typename U> explicit YawPitchRoll(const std::tuple<U, U, U>& t) noexcept : yaw(T(std::get<0>(t))), pitch(T(std::get<1>(t))), roll(T(std::get<2>(t))) {}
	//YawPitchRoll(const T* r) noexcept : yaw(r[0]), pitch(r[1]), roll(r[2]) {}

	//explicit operator std::tuple<T, T, T>() { return std::tuple<T, T, T>(yaw, pitch, roll); }
	//template<typename U> explicit operator std::tuple<U, U, U>() { return std::tuple<U, U, U>(U(yaw), U(pitch), U(roll)); }
	//explicit operator T*() noexcept { return &yaw; }
	//explicit operator const T*() const noexcept { return &yaw; }

	YawPitchRoll operator+() const noexcept { return *this; }
	YawPitchRoll operator-() const noexcept { return YawPitchRoll(-yaw, -pitch, -roll); }
	YawPitchRoll& operator+=(const YawPitchRoll& r) noexcept { yaw += r.yaw; pitch += r.pitch; roll += r.roll; return *this; }
	YawPitchRoll& operator-=(const YawPitchRoll& r) noexcept { yaw -= r.yaw; pitch -= r.pitch; roll -= r.roll; return *this; }
	YawPitchRoll& operator*=(T f) noexcept { yaw *= f; pitch *= f; roll *= f; return *this; }
	YawPitchRoll& operator/=(T f) noexcept { T s = T(1)/f; yaw *= s; pitch *= s; roll *= s; return *this; }
	friend YawPitchRoll operator+(const YawPitchRoll& r1, const YawPitchRoll& r2) noexcept;
	friend YawPitchRoll operator-(const YawPitchRoll& r1, const YawPitchRoll& r2) noexcept;
	friend YawPitchRoll operator*(T f, const YawPitchRoll& r) noexcept { return YawPitchRoll(f*r.yaw, f*r.pitch, f*r.roll); }
	friend YawPitchRoll operator*(const YawPitchRoll& r, T f) noexcept { return YawPitchRoll(r.yaw*f, r.pitch*f, r.roll*f); }
	friend YawPitchRoll operator/(T f, const YawPitchRoll& r) noexcept { return YawPitchRoll(f/r.yaw, f/r.pitch, f/r.roll); }
	friend YawPitchRoll operator/(const YawPitchRoll& r, T f) noexcept { T s = T(1)/f; return YawPitchRoll(r.yaw*s, r.pitch*s, r.roll*s); }
	bool operator==(const YawPitchRoll& r) const noexcept { return (yaw == r.yaw) && (pitch == r.pitch) && (roll == r.roll); }
	bool operator!=(const YawPitchRoll& r) const noexcept { return !(*this == r); }
	friend std::istream& operator>>(std::istream& s, YawPitchRoll& r);
	friend std::ostream& operator<<(std::ostream& s, const YawPitchRoll& r) { return s << r.yaw << ' ' << r.pitch << ' ' << r.roll; }

	template<class A> void serialize(A& ar, unsigned int version) { ar & yaw & pitch & roll; }

	template<std::size_t I> T& get() noexcept;
	template<std::size_t I> const T& get() const noexcept;

    static YawPitchRoll fromForward(const Vector3<T>& forward, T roll = T()) noexcept;
	Vector3 toForward/*toDirection*/() const noexcept;

    bool isZero() const noexcept { return (yaw == T()) && (pitch == T()) && (roll == T()); }
	bool isApproxZero() const noexcept;
	bool isApproxEqual(const YawPitchRoll& r) const noexcept;
	bool isApproxEqual(const YawPitchRoll& r, T tolerance) const noexcept;
	bool isFinite() const noexcept { return std::isfinite(yaw) && std::isfinite(pitch) && std::isfinite(roll); }
	YawPitchRoll& setZero/*zero*/() noexcept { yaw = T(); pitch = T(); roll = T(); return *this; }
	YawPitchRoll& set(T yaw, T pitch, T roll) noexcept { this->yaw = yaw; this->pitch = pitch; this->roll = roll; return *this; }
	YawPitchRoll& negate() noexcept { yaw = -yaw; pitch = -pitch; roll = -roll; return *this; }
	//YawPitchRoll& clamp(T low, T high);

	//static const YawPitchRoll& getZero() noexcept { return ZERO; }

	static const YawPitchRoll ZERO;

    T yaw, pitch, roll;
};

template<typename T> const YawPitchRoll<T> YawPitchRoll<T>::ZERO{};

template<std::size_t I, typename T>
inline T& get(YawPitchRoll<T>& r) noexcept
{
	if constexpr (I == 0)
		return r.yaw;
	else if constexpr (I == 1)
		return r.pitch;
	else if constexpr (I == 2)
		return r.roll;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const YawPitchRoll<T>& r) noexcept
{
	if constexpr (I == 0)
		return r.yaw;
	else if constexpr (I == 1)
		return r.pitch;
	else if constexpr (I == 2)
		return r.roll;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(YawPitchRoll<T>&& r) noexcept
{
	if constexpr (I == 0)
		return r.yaw;
	else if constexpr (I == 1)
		return r.pitch;
	else if constexpr (I == 2)
		return r.roll;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const YawPitchRoll<T>&& r) noexcept
{
	if constexpr (I == 0)
		return r.yaw;
	else if constexpr (I == 1)
		return r.pitch;
	else if constexpr (I == 2)
		return r.roll;
	static_assert(false);
}

template<typename T>
inline YawPitchRoll<T> operator+(const YawPitchRoll<T>& r1, const YawPitchRoll<T>& r2)
{
	return YawPitchRoll<T>(r1.yaw + r2.yaw, r1.pitch + r2.pitch, r1.roll + r2.roll);
}

template<typename T>
inline YawPitchRoll<T> operator-(const YawPitchRoll<T>& r1, const YawPitchRoll<T>& r2)
{
	return YawPitchRoll<T>(r1.yaw - r2.yaw, r1.pitch - r2.pitch, r1.roll - r2.roll);
}

template<typename T>
inline std::istream& operator>>(std::istream& s, YawPitchRoll<T>& r) 
{ 
	return s >> r.yaw >> std::skipws >> r.pitch >> std::skipws >> r.roll; 
}

template<typename T>
template<std::size_t I>
inline T& YawPitchRoll<T>::get()
{
	if constexpr (I == 0)
		return yaw;
	else if constexpr (I == 1)
		return pitch;
	else if constexpr (I == 2)
		return roll;
	static_assert(false);
}

template<typename T>
template<std::size_t I>
inline const T& YawPitchRoll<T>::get() const
{
	if constexpr (I == 0)
		return yaw;
	else if constexpr (I == 1)
		return pitch;
	else if constexpr (I == 2)
		return roll;
	static_assert(false);
}

template<typename T>
inline YawPitchRoll<T> YawPitchRoll<T>::fromForward(const Vector3<T>& forward, T roll)
{
    T h = T(0);
    T p = T(0);
    T m = direction.getMagnitude();
    if (m > T(0))
    {
        Vector3<T> direction(forward/m);
        p = std::asin(std::clamp(-direction.y, T(-1), T(1)));
        if ((T(1) - std::fabs(direction.y)) >= Constants<T>::TOLERANCE)
        {
            T cosP = std::cos(p);
            h = std::acos(std::clamp(direction.z/cosP, T(-1), T(1)));
            if (direction.x < T(0)) 
                h = -h; // h = Constants<T>::TWO_PI - h;
        }
	}

    return YawPitchRoll<T>(h, p, roll);
}

template<typename T>
inline Vector3<T> YawPitchRoll<T>::toForward() const
{
	T sh = std::sin(yaw);
    T ch = std::cos(yaw);
    T sp = std::sin(pitch);
    T cp = std::cos(pitch);
	return Vector3<T>(sh*cp, -sp, ch*cp);
}

template<typename T>
inline bool YawPitchRoll<T>::isApproxZero() const
{ 
	return (std::fabs(yaw) < Constants<T>::TOLERANCE) && (std::fabs(pitch) < Constants<T>::TOLERANCE) && 
		(std::fabs(roll) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool YawPitchRoll<T>::isApproxEqual(const YawPitchRoll<T>& r) const
{ 
	return (std::fabs(r.yaw - yaw) < Constants<T>::TOLERANCE) && (std::fabs(r.pitch - pitch) < Constants<T>::TOLERANCE) &&
		(std::fabs(r.roll - roll) < Constants<T>::TOLERANCE);
}

template<typename T>
inline bool YawPitchRoll<T>::isApproxEqual(const YawPitchRoll<T>& r, T tolerance) const
{ 
	return (std::fabs(r.yaw - yaw) < tolerance) && (std::fabs(r.pitch - pitch) < tolerance) &&
		(std::fabs(r.roll - roll) < tolerance);
}

//template<typename T>
//inline YawPitchRoll<T>& YawPitchRoll<T>::clamp(T low, T high)
//{
//	yaw = std::clamp(yaw, low, high);
//	pitch = std::clamp(pitch, low, high);
//	roll = std::clamp(roll, low, high);
//	return *this;
//}

} // namespace templates

#if MATHEMATICS_DOUBLE
using YawPitchRoll = templates::YawPitchRoll<double>;
#else
using YawPitchRoll = templates::YawPitchRoll<float>;
#endif

} // namespace core::mathematics

namespace std
{

template<size_t I, typename T>
struct tuple_element;

template<typename T>
struct tuple_size;

template<size_t I, typename T>
struct tuple_element<I, core::mathematics::templates::YawPitchRoll<T>>
{
	using type = T;
};

template<typename T>
struct tuple_size<core::mathematics::templates::YawPitchRoll<T>> : integral_constant<size_t, 3> 
{
};

} // namespace std

#include "Euler.hpp"
#include "Matrix3.hpp"

namespace core::mathematics::templates {

template<typename T>
inline YawPitchRoll<T>::YawPitchRoll(const Euler<T>& e)
{
	if (e.order == EulerOrder::ZXY)
	{
        yaw = e.y;
		pitch = e.x;
		roll = e.z;
	}
	else if (e.order != EulerOrder::UNSPECIFIED)
	{
		Euler<T> euler(Matrix3<T>(e), EulerOrder::ZXY);
		yaw = euler.y;
		pitch = euler.x;
		roll = euler.z;
	}
	else
	{
		yaw = T();
		pitch = T();
		roll = T();
	}
}

template<typename T>
inline YawPitchRoll<T>::YawPitchRoll(const Quaternion<T>& q) : YawPitchRoll<T>(Matrix3<T>::makeRotation(q))
{
}

template<typename T>
inline YawPitchRoll<T>::YawPitchRoll(const Matrix3<T>& m) : YawPitchRoll<T>(Euler<T>(m, EulerOrder::ZXY))
{
}

} // namespace core::mathematics::templates

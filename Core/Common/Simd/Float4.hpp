/*
 *	Name: Float4
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_SIMD_FLOAT4_HPP
#define CORE_SIMD_FLOAT4_HPP

#include "Intrinsics.hpp"

#if SIMD_HAS_FLOAT4

namespace core::simd {

struct Float4
{
	struct Bool4
	{
		Bool4() = default;
        explicit Bool4(const bool s) noexcept { value = float4::set4(s); }
        Bool4(const bool x, const bool y, const bool z, const bool w) noexcept { value = float4::set4(x, y, z, w); }
		Bool4(const float4::Type b) noexcept : value(b) {}

		operator float4::Type() const noexcept { return value; }
		Bool4 operator!() const noexcept { return Bool4(float4::not4(value)); }
        //friend Bool4 operator&&(const Bool4 b1, const Bool4 b2) noexcept { return Bool4(float4::and4(b1, b2)); }
        //friend Bool4 operator||(const Bool4 b1, const Bool4 b2) noexcept { return Bool4(float4::or4(b1, b2)); }

		float4::Type value;
	};

	Float4() = default;
	explicit Float4(const float s) noexcept { value = float4::set4(s); }
	Float4(const float x, const float y, const float z, const float w) noexcept { value = float4::set4(x, y, z, w); }
	Float4(const float4::Type v) noexcept : value(v) {}

	operator float4::Type() const noexcept { return value; }
	//float operator[](int i) const noexcept { /*TODO*/ }
	Float4 operator+() const noexcept { return *this; }
	Float4 operator-() const noexcept { return Float4(float4::neg4(value)); }
	Float4& operator+=(const Float4 v) noexcept { value = float4::add4(value, v); return *this; }
	Float4& operator-=(const Float4 v) noexcept { value = float4::sub4(value, v); return *this; }
	Float4& operator*=(const Float4 v) noexcept { value = float4::mul4(value, v); return *this; }
	Float4& operator*=(const float f) noexcept { value = float4::mul4(value, float4::set4(f)); return *this; }
	Float4& operator/=(const Float4 v) noexcept { value = float4::div4(value, v); return *this; }
	Float4& operator/=(const float f) noexcept { value = float4::div4(value, float4::set4(f)); return *this; }
	//friend Float4 operator+(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::add4(v1, v2)); }
	//friend Float4 operator-(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::sub4(v1, v2)); }
	//friend Float4 operator*(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::mul4(v1, v2)); }
	//friend Float4 operator*(const float f, const Float4 v) noexcept { return Float4(float4::mul4(float4::set4(f), v)); }
	//friend Float4 operator*(const Float4 v, const float f) noexcept { return Float4(float4::mul4(v, float4::set4(f))); }
	//friend Float4 operator/(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::div4(v1, v2)); }
	//friend Float4 operator/(const float f, const Float4 v) noexcept { return Float4(float4::div4(float4::set4(f), v)); }
	//friend Float4 operator/(const Float4 v, const float f) noexcept { return Float4(float4::div4(v, float4::set4(f))); }
	Bool4 operator<(const Float4& v) const noexcept { return Bool4(float4::less(value, v)); }
	Bool4 operator<=(const Float4& v) const noexcept { return Bool4(float4::lessEqual(value, v)); }
	Bool4 operator>(const Float4& v) const noexcept { return Bool4(float4::greater(value, v)); }
	Bool4 operator>=(const Float4& v) const noexcept { return Bool4(float4::greaterEqual(value, v)); }
	Bool4 operator==(const Float4& v) const noexcept { return Bool4(float4::equal(value, v)); }
	Bool4 operator!=(const Float4& v) const noexcept { return Bool4(float4::not4(float4::equal(value, v))); }

	float x() const noexcept { return float4::extract<float4::X>(value); }
	float y() const noexcept { return float4::extract<float4::Y>(value); }
	float z() const noexcept { return float4::extract<float4::Z>(value); }
	float w() const noexcept { return float4::extract<float4::W>(value); }
	Float4 xxxx() const noexcept { return Float4(float4::broadcast<float4::X>(value)); }
	Float4 yyyy() const noexcept { return Float4(float4::broadcast<float4::Y>(value)); }
	Float4 zzzz() const noexcept { return Float4(float4::broadcast<float4::Z>(value)); }
	Float4 wwww() const noexcept { return Float4(float4::broadcast<float4::W>(value)); }
	Float4 xyyy() const noexcept { return Float4(float4::swizzle<float4::XYYY>(value)); }
	Float4 xzzz() const noexcept { return Float4(float4::swizzle<float4::XZZZ>(value)); }
	Float4 xwww() const noexcept { return Float4(float4::swizzle<float4::XWWW>(value)); }
	Float4 xyzz() const noexcept { return Float4(float4::swizzle<float4::XYZZ>(value)); }
	Float4 xyww() const noexcept { return Float4(float4::swizzle<float4::XYWW>(value)); }
	Float4 xzww() const noexcept { return Float4(float4::swizzle<float4::XZWW>(value)); }
	Float4 yxxx() const noexcept { return Float4(float4::swizzle<float4::YXXX>(value)); }
	Float4 yzzz() const noexcept { return Float4(float4::swizzle<float4::YZZZ>(value)); }
	Float4 yzxx() const noexcept { return Float4(float4::swizzle<float4::YZXX>(value)); }
	Float4 yzww() const noexcept { return Float4(float4::swizzle<float4::YZWW>(value)); }
	Float4 ywww() const noexcept { return Float4(float4::swizzle<float4::YWWW>(value)); }
	Float4 yxzw() const noexcept { return Float4(float4::swizzle<float4::YXZW>(value)); }
	Float4 yxwz() const noexcept { return Float4(float4::swizzle<float4::YXWZ>(value)); }
	Float4 zxxx() const noexcept { return Float4(float4::swizzle<float4::ZXXX>(value)); }
	Float4 zxyy() const noexcept { return Float4(float4::swizzle<float4::ZXYY>(value)); }
	Float4 zyyy() const noexcept { return Float4(float4::swizzle<float4::ZYYY>(value)); }
	Float4 zyww() const noexcept { return Float4(float4::swizzle<float4::ZYWW>(value)); }
	Float4 zwww() const noexcept { return Float4(float4::swizzle<float4::ZWWW>(value)); }
	Float4 zwxy() const noexcept { return Float4(float4::swizzle<float4::ZWXY>(value)); }
	Float4 wxxx() const noexcept { return Float4(float4::swizzle<float4::WXXX>(value)); }
	Float4 wyyy() const noexcept { return Float4(float4::swizzle<float4::WYYY>(value)); }
	Float4 wzzz() const noexcept { return Float4(float4::swizzle<float4::WZZZ>(value)); }
	Float4 wzyx() const noexcept { return Float4(float4::swizzle<float4::WZYX>(value)); }

	float4::Type value;
};

inline Float4::Bool4 operator&&(const Float4::Bool4 b1, const Float4::Bool4 b2) noexcept { return Float4::Bool4(float4::and4(b1, b2)); }
inline Float4::Bool4 operator||(const Float4::Bool4 b1, const Float4::Bool4 b2) noexcept { return Float4::Bool4(float4::or4(b1, b2)); }

inline bool any(const Float4::Bool4 b) noexcept { return float4::any(b.value); }
inline bool all(const Float4::Bool4 b) noexcept { return float4::all(b.value); }

inline Float4 operator+(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::add4(v1, v2)); }
inline Float4 operator-(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::sub4(v1, v2)); }
inline Float4 operator*(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::mul4(v1, v2)); }
inline Float4 operator*(const float f, const Float4 v) noexcept { return Float4(float4::mul4(float4::set4(f), v)); }
inline Float4 operator*(const Float4 v, const float f) noexcept { return Float4(float4::mul4(v, float4::set4(f))); }
inline Float4 operator/(const Float4 v1, const Float4 v2) noexcept { return Float4(float4::div4(v1, v2)); }
inline Float4 operator/(const float f, const Float4 v) noexcept { return Float4(float4::div4(float4::set4(f), v)); }
inline Float4 operator/(const Float4 v, const float f) noexcept { return Float4(float4::div4(v, float4::set4(f))); }

} // namespace core::simd

#endif /* SIMD_HAS_FLOAT4 */
#endif /* CORE_SIMD_FLOAT4_HPP */

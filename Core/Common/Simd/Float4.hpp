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
        explicit Bool4(bool s) noexcept { value = set4(s); }
        Bool4(bool x, bool y, bool z, bool w) noexcept { value = set4(x, y, z, w); }
		Bool4(float4 b) noexcept : value(b) {}

		operator float4() const noexcept { return value; }
		Bool4 operator!() const noexcept { return Bool4(not4(value)); }
        //friend Bool4 operator&&(const Bool4 b1, const Bool4 b2) noexcept { return Bool4(and4(b1, b2)); }
        //friend Bool4 operator||(const Bool4 b1, const Bool4 b2) noexcept { return Bool4(or4(b1, b2)); }

		float4 value;
	};

	Float4() = default;
	explicit Float4(float s) noexcept { value = set4(s); }
	Float4(float x, float y, float z, float w) noexcept { value = set4(x, y, z, w); }
	Float4(float4 v) noexcept : value(v) {}

	operator float4() const noexcept { return value; }
	//float operator[](int i) const noexcept { /*TODO*/ }
	Float4 operator+() const noexcept { return *this; }
	Float4 operator-() const noexcept { return Float4(neg4(value)); }
	Float4& operator+=(const Float4 v) noexcept { value = add4(value, v); return *this; }
	Float4& operator-=(const Float4 v) noexcept { value = sub4(value, v); return *this; }
	Float4& operator*=(const Float4 v) noexcept { value = mul4(value, v); return *this; }
	Float4& operator*=(float s) noexcept { value = mul4(value, set4(s)); return *this; }
	Float4& operator/=(const Float4 v) noexcept { value = div4(value, v); return *this; }
	Float4& operator/=(float s) noexcept { value = div4(value, set4(s)); return *this; }
	//friend Float4 operator+(const Float4 v1, const Float4 v2) noexcept { return Float4(add4(v1, v2)); }
	//friend Float4 operator-(const Float4 v1, const Float4 v2) noexcept { return Float4(sub4(v1, v2)); }
	//friend Float4 operator*(const Float4 v1, const Float4 v2) noexcept { return Float4(mul4(v1, v2)); }
	//friend Float4 operator*(float s, const Float4 v) noexcept { return Float4(mul4(set4(s), v)); }
	//friend Float4 operator*(const Float4 v, float s) noexcept { return Float4(mul4(v, set4(s))); }
	//friend Float4 operator/(const Float4 v1, const Float4 v2) noexcept { return Float4(div4(v1, v2)); }
	//friend Float4 operator/(float s, const Float4 v) noexcept { return Float4(div4(set4(s), v)); }
	//friend Float4 operator/(const Float4 v, float s) noexcept { return Float4(div4(v, set4(s))); }
	Bool4 operator<(const Float4& v) const noexcept { return Bool4(less(value, v)); }
	Bool4 operator<=(const Float4& v) const noexcept { return Bool4(lessEqual(value, v)); }
	Bool4 operator>(const Float4& v) const noexcept { return Bool4(greater(value, v)); }
	Bool4 operator>=(const Float4& v) const noexcept { return Bool4(greaterEqual(value, v)); }
	Bool4 operator==(const Float4& v) const noexcept { return Bool4(equal(value, v)); }
	Bool4 operator!=(const Float4& v) const noexcept { return Bool4(not4(equal(value, v))); }

	float x() const noexcept { return extract<X>(value); }
	float y() const noexcept { return extract<Y>(value); }
	float z() const noexcept { return extract<Z>(value); }
	float w() const noexcept { return extract<W>(value); }
	Float4 xxxx() const noexcept { return Float4(broadcast<X>(value)); }
	Float4 yyyy() const noexcept { return Float4(broadcast<Y>(value)); }
 	Float4 zzzz() const noexcept { return Float4(broadcast<Z>(value)); }
	Float4 wwww() const noexcept { return Float4(broadcast<W>(value)); }
	Float4 xyyy() const noexcept { return Float4(swizzle4<XYYY>(value)); }
	Float4 xzzz() const noexcept { return Float4(swizzle4<XZZZ>(value)); }
	Float4 xwww() const noexcept { return Float4(swizzle4<XWWW>(value)); }
	Float4 xyzz() const noexcept { return Float4(swizzle4<XYZZ>(value)); }
	Float4 xyww() const noexcept { return Float4(swizzle4<XYWW>(value)); }
	Float4 xzww() const noexcept { return Float4(swizzle4<XZWW>(value)); }
	Float4 yxxx() const noexcept { return Float4(swizzle4<YXXX>(value)); }
	Float4 yzzz() const noexcept { return Float4(swizzle4<YZZZ>(value)); }
	Float4 yzxx() const noexcept { return Float4(swizzle4<YZXX>(value)); }
	Float4 yzww() const noexcept { return Float4(swizzle4<YZWW>(value)); }
	Float4 ywww() const noexcept { return Float4(swizzle4<YWWW>(value)); }
	Float4 yxzw() const noexcept { return Float4(swizzle4<YXZW>(value)); }
	Float4 yxwz() const noexcept { return Float4(swizzle4<YXWZ>(value)); }
	Float4 zxxx() const noexcept { return Float4(swizzle4<ZXXX>(value)); }
	Float4 zxyy() const noexcept { return Float4(swizzle4<ZXYY>(value)); }
	Float4 zyyy() const noexcept { return Float4(swizzle4<ZYYY>(value)); }
	Float4 zyww() const noexcept { return Float4(swizzle4<ZYWW>(value)); }
	Float4 zwww() const noexcept { return Float4(swizzle4<ZWWW>(value)); }
	Float4 zwxy() const noexcept { return Float4(swizzle4<ZWXY>(value)); }
	Float4 wxxx() const noexcept { return Float4(swizzle4<WXXX>(value)); }
	Float4 wyyy() const noexcept { return Float4(swizzle4<WYYY>(value)); }
	Float4 wzzz() const noexcept { return Float4(swizzle4<WZZZ>(value)); }
	Float4 wzyx() const noexcept { return Float4(swizzle4<WZYX>(value)); }

	float4 value;
};

inline Float4::Bool4 operator&&(const Float4::Bool4 b1, const Float4::Bool4 b2) noexcept { return Float4::Bool4(and4(b1, b2)); }
inline Float4::Bool4 operator||(const Float4::Bool4 b1, const Float4::Bool4 b2) noexcept { return Float4::Bool4(or4(b1, b2)); }

inline bool any(const Float4::Bool4 b) noexcept { return any(b.value); }
inline bool all(const Float4::Bool4 b) noexcept { return all(b.value); }

inline Float4 operator+(const Float4 v1, const Float4 v2) noexcept { return Float4(add4(v1, v2)); }
inline Float4 operator-(const Float4 v1, const Float4 v2) noexcept { return Float4(sub4(v1, v2)); }
inline Float4 operator*(const Float4 v1, const Float4 v2) noexcept { return Float4(mul4(v1, v2)); }
inline Float4 operator*(float s, const Float4 v) noexcept { return Float4(mul4(set4(s), v)); }
inline Float4 operator*(const Float4 v, float s) noexcept { return Float4(mul4(v, set4(s))); }
inline Float4 operator/(const Float4 v1, const Float4 v2) noexcept { return Float4(div4(v1, v2)); }
inline Float4 operator/(float s, const Float4 v) noexcept { return Float4(div4(set4(s), v)); }
inline Float4 operator/(const Float4 v, float s) noexcept { return Float4(div4(v, set4(s))); }

} // namespace core::simd

#endif /* SIMD_HAS_FLOAT4 */
#endif /* CORE_SIMD_FLOAT4_HPP */

/*
 *	Name: Float4
 *	Author: Pawel Mrochen
 */

#pragma once

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
	Float4 xyyy() const noexcept { return Float4(swizzle<X, Y, Y, Y>(value)); }
	Float4 xyxy() const noexcept { return Float4(swizzle<X, Y, X, Y>(value)); }
	Float4 xxyy() const noexcept { return Float4(swizzle<X, X, Y, Y>(value)); }
	Float4 xzzz() const noexcept { return Float4(swizzle<X, Z, Z, Z>(value)); }
	Float4 xwww() const noexcept { return Float4(swizzle<X, W, W, W>(value)); }
	Float4 xyzz() const noexcept { return Float4(swizzle<X, Y, Z, Z>(value)); }
	Float4 xyww() const noexcept { return Float4(swizzle<X, Y, W, W>(value)); }
	Float4 xzww() const noexcept { return Float4(swizzle<X, Z, W, W>(value)); }
	Float4 yxxx() const noexcept { return Float4(swizzle<Y, X, X, X>(value)); }
	Float4 yzzz() const noexcept { return Float4(swizzle<Y, Z, Z, Z>(value)); }
	Float4 yzxx() const noexcept { return Float4(swizzle<Y, Z, X, X>(value)); }
	Float4 yzxw() const noexcept { return Float4(swizzle<Y, Z, X, W>(value)); }
	Float4 yzww() const noexcept { return Float4(swizzle<Y, Z, W, W>(value)); }
	Float4 ywww() const noexcept { return Float4(swizzle<Y, W, W, W>(value)); }
	Float4 yxzz() const noexcept { return Float4(swizzle<Y, X, Z, Z>(value)); }
	Float4 yxzw() const noexcept { return Float4(swizzle<Y, X, Z, W>(value)); }
	Float4 yxwz() const noexcept { return Float4(swizzle<Y, X, W, Z>(value)); }
	Float4 zxxx() const noexcept { return Float4(swizzle<Z, X, X, X>(value)); }
	Float4 zxyy() const noexcept { return Float4(swizzle<Z, X, Y, Y>(value)); }
	Float4 zxyw() const noexcept { return Float4(swizzle<Z, X, Y, W>(value)); }
	Float4 zyyy() const noexcept { return Float4(swizzle<Z, Y, Y, Y>(value)); }
	Float4 zyww() const noexcept { return Float4(swizzle<Z, Y, W, W>(value)); }
	Float4 zzww() const noexcept { return Float4(swizzle<Z, Z, W, W>(value)); }
	Float4 zwww() const noexcept { return Float4(swizzle<Z, W, W, W>(value)); }
	Float4 zwxy() const noexcept { return Float4(swizzle<Z, W, X, Y>(value)); }
	Float4 zwzw() const noexcept { return Float4(swizzle<Z, W, Z, W>(value)); }
	Float4 wxxx() const noexcept { return Float4(swizzle<W, X, X, X>(value)); }
	Float4 wyyy() const noexcept { return Float4(swizzle<W, Y, Y, Y>(value)); }
	Float4 wzzz() const noexcept { return Float4(swizzle<W, Z, Z, Z>(value)); }
	Float4 wzyx() const noexcept { return Float4(swizzle<W, Z, Y, X>(value)); }

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

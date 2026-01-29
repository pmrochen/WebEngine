/*
 *	Name: Matrix2
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <cstddef>
#include <cmath>
#include <Simd/Intrinsics.hpp>
#include "Constants.hpp"
#include "Scalar.hpp"
#include "Vector2.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct Matrix2
{
	using Real = T;
	using ConstArg = const Matrix2&;
	using ConstResult = const Matrix2&;

	//static constexpr int NUM_COMPONENTS = 4;

	constexpr Matrix2() noexcept : m00(), m01(), m10(), m11() {}
	explicit Matrix2(Uninitialized) noexcept {}
	//explicit Matrix2(Identity) noexcept {}
	constexpr Matrix2(T m00, T m01, T m10, T m11) noexcept : m00(m00), m01(m01), m10(m10), m11(m11) {}
	constexpr Matrix2(const Vector2<T>& row0, const Vector2<T>& row1) noexcept : m00(row0.x), m01(row0.y), m10(row1.x), m11(row1.y) {}
	explicit Matrix2(const T* m) noexcept : m00(m[0]), m01(m[1]), m10(m[2]), m11(m[3]) {}

	//explicit operator T*() noexcept { return &m00; }
	//explicit operator const T*() const noexcept { return &m00; }
	Vector2<T>& operator[](int i) noexcept { return reinterpret_cast<Vector2<T>*>(&m00)[i]; }
	const Vector2<T>& operator[](int i) const noexcept { return reinterpret_cast<const Vector2<T>*>(&m00)[i]; }

	Matrix2 operator+() const noexcept { return *this; }
	Matrix2 operator-() const noexcept { return Matrix2(-m00, -m01, -m10, -m11); }
	Matrix2& operator+=(const Matrix2& m) noexcept;
	Matrix2& operator-=(const Matrix2& m) noexcept;
	Matrix2& operator*=(T f) noexcept;
	Matrix2& operator/=(T f) noexcept { return operator*=(T(1)/f); }
	Matrix2& operator*=(const Matrix2& m) noexcept;
	friend Matrix2 operator+(const Matrix2& m1, const Matrix2& m2) noexcept;
	friend Matrix2 operator-(const Matrix2& m1, const Matrix2& m2) noexcept;
	friend Matrix2 operator*(T f, const Matrix2& m) noexcept;
	friend Matrix2 operator*(const Matrix2& m, T f) noexcept;
	friend Matrix2 operator/(const Matrix2& m, T f) noexcept { return operator*(m, T(1)/f); }
	friend Matrix2 operator*(const Matrix2& m1, const Matrix2& m2) noexcept;
	bool operator==(const Matrix2& m) const noexcept { return (m00 == m.m00) && (m01 == m.m01) && (m10 == m.m10) && (m11 == m.m11); }
	bool operator!=(const Matrix2& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, Matrix2& m);
	friend std::ostream& operator<<(std::ostream& s, const Matrix2& m);

	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m10 & m11; }

	// #TODO rename get...() to make...()
	static Matrix2 getScaling(const Vector2<T>& v) noexcept { return Matrix2(Uninitialized()).setScaling(v); }
	static Matrix2 getScaling(T f) noexcept { return Matrix2(Uninitialized()).setScaling(f); }
	static Matrix2 getRotation(T angle) noexcept { return Matrix2(Uninitialized()).setRotation(angle); }
	static Matrix2 getScalingRotation(const Vector2<T>& s, T angle) noexcept { return Matrix2(Uninitialized()).setScalingRotation(s, angle); }
	static Matrix2 getShearing(T xy, T yx) noexcept { return Matrix2(Uninitialized()).setShearing(xy, yx); }

	bool isZero() const noexcept { return (m00 == T()) && (m01 == T()) && (m10 == T()) && (m11 == T()); }
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept { return (m00 == T(1)) && (m01 == T()) && (m10 == T()) && (m11 == T(1)); }
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Matrix2& m) const noexcept;
	bool isApproxEqual(const Matrix2& m, T tolerance) const noexcept;
	bool isApproxOrthogonal() const noexcept;
	bool hasApproxUniformScaling() const noexcept;
	bool isFinite() const noexcept { return std::isfinite(m00) && std::isfinite(m01) && std::isfinite(m10) && std::isfinite(m11); }
	const Vector2<T>& getRow(int i) const noexcept { return reinterpret_cast<const Vector2<T>*>(&m00)[i]; }
	Vector2<T> getColumn(int i) const noexcept;
	Vector2<T> getScaleComponent() const noexcept { return Vector2<T>((*this)[0].getMagnitude(), (*this)[1].getMagnitude()); }
	T getTrace() const noexcept { return m00 + m11; }
	T getDeterminant() const noexcept;
	bool isSingular() const noexcept { return (getDeterminant() == T(0)); }
	Matrix2& setZero/*zero*/() noexcept { m00 = T(); m01 = T(); m10 = T(); m11 = T(); return *this; }
	Matrix2& setIdentity/*makeIdentity*/() noexcept { m00 = T(1); m01 = T(); m10 = T(); m11 = T(1); return *this; }
	Matrix2& set(const Vector2<T>& row0, const Vector2<T>& row1) noexcept { m00 = row0.x; m01 = row0.y; m10 = row1.x; m11 = row1.y; return *this; }
	Matrix2& set(T m00, T m01, T m10, T m11) noexcept { this->m00 = m00; this->m01 = m01; this->m10 = m10; this->m11 = m11; return *this; }
	Matrix2& setScaling/*makeScaling*/(const Vector2<T>& v) noexcept;
	Matrix2& setScaling/*makeScaling*/(T f) noexcept;
	Matrix2& setRotation(T angle) noexcept;
	Matrix2& setScalingRotation(const Vector2<T>& s, T angle) noexcept;
	Matrix2& setShearing(T xy, T yx) noexcept;
	Matrix2& setTranspose/*transposeOf*/(const Matrix2& m) noexcept;
	Matrix2& setInverse/*inverseOf*/(const Matrix2& m) noexcept;
	Matrix2& setInverseTranspose/*inverseTransposeOf*/(const Matrix2& m) noexcept;
	Matrix2& preConcatenate(const Matrix2& m) noexcept;
	Matrix2& concatenate(const Matrix2& m) noexcept;
	Matrix2& preScale(const Vector2<T>& v) noexcept;
	Matrix2& preScale(T f) noexcept { (*this) *= f; return *this; }
	Matrix2& scale(const Vector2<T>& v) noexcept;
	Matrix2& scale(T f) noexcept { (*this) *= f; return *this; }
	Matrix2& rotate(T angle) noexcept { concatenate(Matrix2(Uninitialized()).setRotation(angle)); return *this; }
	Matrix2& shear(T xy, T yx) noexcept { concatenate(Matrix2(Uninitialized()).setShearing(xy, yx)); return *this; }
	Matrix2& negate() noexcept;
	Matrix2& transpose() noexcept;
	Matrix2& invert() noexcept;
	Matrix2& orthonormalize() noexcept;

	//static const Matrix2& getZero() noexcept { return ZERO; }
	//static const Matrix2& getIdentity() noexcept { return IDENTITY; }

	static const Matrix2 ZERO;
	static const Matrix2 IDENTITY;

	T m00, m01;
	T m10, m11;
};

template<typename T> const Matrix2<T> Matrix2<T>::ZERO{};
template<typename T> const Matrix2<T> Matrix2<T>::IDENTITY{ T(1), T(0), T(0), T(1) };

#if SIMD_HAS_FLOAT4

template<>
struct Matrix2<float>
{
	using Real = float;
	using ConstArg = const Matrix2;
	using ConstResult = const Matrix2;

	//static constexpr int NUM_COMPONENTS = 4;

	/*constexpr*/ Matrix2() noexcept : row0(simd::zero<simd::float4>()), row1(simd::zero<simd::float4>()) {}
	explicit Matrix2(Uninitialized) noexcept {}
	//explicit Matrix2(Identity) noexcept {}
	/*constexpr*/ Matrix2(float m00, float m01, float m10, float m11) noexcept;
	/*constexpr*/ Matrix2(const Vector2<float>& row0, const Vector2<float>& row1) noexcept : row0(row0), row1(row1) {}
	explicit Matrix2(const float* m) noexcept;
	explicit Matrix2(const simd::float4* m) noexcept : row0(m[0]), row1(m[1]) {}
	Matrix2(simd::float4 row0, simd::float4 row1) noexcept : row0(row0), row1(row1) {}
	Matrix2(const Matrix2& m) noexcept : row0(m.row0), row1(m.row1) {}
	Matrix2& operator=(const Matrix2& m) noexcept { row0 = m.row0; row1 = m.row1; return *this; }

	//explicit operator float*() noexcept { return &m00; }
	//explicit operator const float*() const noexcept { return &m00; }
	explicit operator simd::float4* () noexcept { return &row0; }
	explicit operator const simd::float4* () const noexcept { return &row0; }
	Vector2<float>& operator[](int i) noexcept { return reinterpret_cast<Vector2<float>&>((&row0)[i]); }
	const Vector2<float>& operator[](int i) const noexcept { return reinterpret_cast<const Vector2<float>&>((&row0)[i]); }

	Matrix2 operator+() const noexcept { return *this; }
	Matrix2 operator-() const noexcept;
	Matrix2& operator+=(const Matrix2& m) noexcept;
	Matrix2& operator-=(const Matrix2& m) noexcept;
	Matrix2& operator*=(float f) noexcept;
	Matrix2& operator/=(float f) noexcept { return operator*=(1.f/f); }
	Matrix2& operator*=(const Matrix2& m) noexcept;
	friend Matrix2 operator+(const Matrix2& m1, const Matrix2& m2) noexcept;
	friend Matrix2 operator-(const Matrix2& m1, const Matrix2& m2) noexcept;
	friend Matrix2 operator*(float f, const Matrix2& m) noexcept;
	friend Matrix2 operator*(const Matrix2& m, float f) noexcept;
	friend Matrix2 operator/(const Matrix2& m, float f) noexcept { return operator*(m, 1.f/f); }
	friend Matrix2 operator*(const Matrix2& m1, const Matrix2& m2) noexcept;
	bool operator==(const Matrix2& m) const noexcept;
	bool operator!=(const Matrix2& m) const noexcept { return !(*this == m); }
	friend std::istream& operator>>(std::istream& s, Matrix2& m);
	friend std::ostream& operator<<(std::ostream& s, const Matrix2& m);

	// #FIXME use simd::set()
	template<class A> void serialize(A& ar, unsigned int version) { ar & m00 & m01 & m10 & m11; }

	// #TODO rename get...() to make...()
	static Matrix2 getScaling(const Vector2<float>& v) noexcept { return Matrix2(Uninitialized()).setScaling(v); }
	static Matrix2 getScaling(float f) noexcept { return Matrix2(Uninitialized()).setScaling(f); }
	static Matrix2 getRotation(float angle) noexcept { return Matrix2(Uninitialized()).setRotation(angle); }
	static Matrix2 getScalingRotation(const Vector2<float>& s, float angle) noexcept { return Matrix2(Uninitialized()).setScalingRotation(s, angle); }
	static Matrix2 getShearing(float xy, float yx) noexcept { return Matrix2(Uninitialized()).setShearing(xy, yx); }

	bool isZero() const noexcept { return simd::all4(simd::equal(simd::pack2x2(row0, row1), simd::zero<simd::float4>())); }
	bool isApproxZero() const noexcept;
	bool isIdentity() const noexcept { return simd::all4(simd::equal(simd::pack2x2(row0, row1), simd::constant4<simd::float4, 1, 0, 0, 1>())); }
	bool isApproxIdentity() const noexcept;
	bool isApproxEqual(const Matrix2& m) const noexcept;
	bool isApproxEqual(const Matrix2& m, float tolerance) const noexcept;
	bool isApproxOrthogonal() const noexcept;
	bool hasApproxUniformScaling() const noexcept;
	bool isFinite() const noexcept { return simd::all4(simd::isFinite(simd::pack2x2(row0, row1))); }
	const Vector2<float>& getRow(int i) const noexcept { return reinterpret_cast<const Vector2<float>&>((&row0)[i]); }
	Vector2<float> getColumn(int i) const noexcept;
	Vector2<float> getScaleComponent() const noexcept { return Vector2<float>((*this)[0].getMagnitude(), (*this)[1].getMagnitude()); }
	float getTrace() const noexcept { return m00 + m11; }
	float getDeterminant() const noexcept;
	bool isSingular() const noexcept { return (getDeterminant() == 0.f); }
	Matrix2& setZero/*zero*/() noexcept { row0 = simd::zero<simd::float4>(); row1 = simd::zero<simd::float4>(); return *this; }
	Matrix2& setIdentity/*makeIdentity*/() noexcept { row0 = Vector2<float>::UNIT_X; row1 = Vector2<float>::UNIT_Y; return *this; }
	Matrix2& set(const Vector2<float>& row0, const Vector2<float>& row1) noexcept { this->row0 = row0; this->row1 = row1; return *this; }
	Matrix2& set(float m00, float m01, float m10, float m11) noexcept;
	Matrix2& setScaling/*makeScaling*/(const Vector2<float>& v) noexcept;
	Matrix2& setScaling/*makeScaling*/(float f) noexcept;
	Matrix2& setRotation(float angle) noexcept;
	Matrix2& setScalingRotation(const Vector2<float>& s, float angle) noexcept;
	Matrix2& setShearing(float xy, float yx) noexcept;
	Matrix2& setTranspose/*transposeOf*/(const Matrix2& m) noexcept;
	Matrix2& setInverse/*inverseOf*/(const Matrix2& m) noexcept;
	Matrix2& setInverseTranspose/*inverseTransposeOf*/(const Matrix2& m) noexcept;
	Matrix2& preConcatenate(const Matrix2& m) noexcept;
	Matrix2& concatenate(const Matrix2& m) noexcept;
	Matrix2& preScale(const Vector2<float>& v) noexcept;
	Matrix2& preScale(float f) noexcept { (*this) *= f; return *this; }
	Matrix2& scale(const Vector2<float>& v) noexcept;
	Matrix2& scale(float f) noexcept { (*this) *= f; return *this; }
	Matrix2& rotate(float angle) noexcept { concatenate(Matrix2(Uninitialized()).setRotation(angle)); return *this; }
	Matrix2& shear(float xy, float yx) noexcept { concatenate(Matrix2(Uninitialized()).setShearing(xy, yx)); return *this; }
	Matrix2& negate() noexcept;
	Matrix2& transpose() noexcept;
	Matrix2& invert() noexcept;
	Matrix2& orthonormalize() noexcept;

	//static const Matrix2& getZero() noexcept { return ZERO; }
	//static const Matrix2& getIdentity() noexcept { return IDENTITY; }

	static const Matrix2 ZERO;
	static const Matrix2 IDENTITY;

	union
	{
		simd::float4 row0;
		struct { float m00, m01; };
	};

	union
	{
		simd::float4 row1;
		struct { float m10, m11; };
	};
};

const Matrix2<float> Matrix2<float>::ZERO{};
const Matrix2<float> Matrix2<float>::IDENTITY{ 1.f, 0.f, 0.f, 1.f };

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Matrix2<T>& Matrix2<T>::operator+=(const Matrix2<T>& m)
{
	m00 += m.m00; m01 += m.m01;
	m10 += m.m10; m11 += m.m11;
	return *this;
}

template<typename T>
inline Matrix2<T>& Matrix2<T>::operator-=(const Matrix2<T>& m)
{
	m00 -= m.m00; m01 -= m.m01;
	m10 -= m.m10; m11 -= m.m11;
	return *this;
}

template<typename T>
inline Matrix2<T>& Matrix2<T>::operator*=(T f)
{
	m00 *= f; m01 *= f;
	m10 *= f; m11 *= f;
	return *this;
}

template<typename T>
inline Matrix2<T>& Matrix2<T>::operator*=(const Matrix2<T>& m)
{
	set(m00*m.m00 + m01*m.m10, m00*m.m01 + m01*m.m11,
		m10*m.m00 + m11*m.m10, m10*m.m01 + m11*m.m11);
	return *this;
}

template<typename T>
inline Matrix2<T> operator+(const Matrix2<T>& m1, const Matrix2<T>& m2)
{
	return Matrix2<T>(m1.m00 + m2.m00, m1.m01 + m2.m01, m1.m10 + m2.m10, m1.m11 + m2.m11);
}

template<typename T>
inline Matrix2<T> operator-(const Matrix2<T>& m1, const Matrix2<T>& m2)
{
	return Matrix2<T>(m1.m00 - m2.m00, m1.m01 - m2.m01, m1.m10 - m2.m10, m1.m11 - m2.m11);
}

template<typename T>
inline Matrix2<T> operator*(T f, const Matrix2<T>& m)
{
	return Matrix2<T>(f*m.m00, f*m.m01, f*m.m10, f*m.m11);
}

template<typename T>
inline Matrix2<T> operator*(const Matrix2<T>& m, T f)
{
	return Matrix2<T>(m.m00*f, m.m01*f, m.m10*f, m.m11*f);
}

template<typename T>
inline Matrix2<T> operator*(const Matrix2<T>& m1, const Matrix2<T>& m2)
{
	return Matrix2<T>(m1.m00*m2.m00 + m1.m01*m2.m10, m1.m00*m2.m01 + m1.m01*m2.m11,
		m1.m10*m2.m00 + m1.m11*m2.m10, m1.m10*m2.m01 + m1.m11*m2.m11);
}

template<typename T>
inline std::istream& operator>>(std::istream& s, Matrix2<T>& m)
{
	return s >> m.m00 >> std::skipws >> m.m01 >> std::skipws >> m.m10 >> std::skipws >> m.m11;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& s, const Matrix2<T>& m)
{
	return s << m.m00 << ' ' << m.m01 << ' ' << m.m10 << ' ' << m.m11;
}

#if SIMD_HAS_FLOAT4

inline Matrix2<float>::Matrix2(float m00, float m01, float m10, float m11) : 
#if MATHEMATICS_SIMD_EXPAND_LAST
	row0(simd::set4(m00, m01, m01, m01),
	row1(simd::set4(m10, m11, m11, m11)
#else
	row0(simd::set2(m00, m01)),
	row1(simd::set2(m10, m11))
#endif
{
}

#if MATHEMATICS_SIMD_EXPAND_LAST
inline Matrix2<float>::Matrix2(const float* m) :
	Matrix2(m[0], m[1], m[2], m[3])
{
}
#else
inline Matrix2<float>::Matrix2(const float* m)
{
	simd::unpack2x2(m, row0, row1);
}
#endif

inline Matrix2<float> Matrix2<float>::operator-() const
{
#if MATHEMATICS_SIMD_EXPAND_LAST
	return Matrix2<float>(simd::neg4(row0), simd::neg4(row1));
#else
	return Matrix2<float>(simd::neg2(row0), simd::neg2(row1));
#endif
}

inline Matrix2<float>& Matrix2<float>::operator+=(const Matrix2<float>& m)
{
	row0 = simd::add4(row0, m.row0);
	row1 = simd::add4(row1, m.row1);
	return *this;
}

inline Matrix2<float>& Matrix2<float>::operator-=(const Matrix2<float>& m)
{
	row0 = simd::sub4(row0, m.row0);
	row1 = simd::sub4(row1, m.row1);
	return *this;
}

inline Matrix2<float>& Matrix2<float>::operator*=(float f)
{
	auto t = simd::set4(f);
	row0 = simd::mul4(row0, t);
	row1 = simd::mul4(row1, t);
	return *this;
}

inline Matrix2<float>& Matrix2<float>::operator*=(const Matrix2<float>& m)
{
	auto a = simd::pack2x2(row0, row1);
	auto b = simd::pack2x2(m.row0, m.row1);
	auto a1 = simd::swizzle<0, 0, 2, 2>(a);
	auto b1 = simd::swizzle<0, 1, 0, 1>(b);
	auto a2 = simd::swizzle<1, 1, 3, 3>(a);
	auto b2 = simd::swizzle<2, 3, 2, 3>(b);
	auto result = simd::add4(simd::mul4(a1, b1), simd::mul4(a2, b2));
	simd::unpack2x2(result, row0, row1);
	return *this;
}

template<>
inline Matrix2<float> operator+(const Matrix2<float>& m1, const Matrix2<float>& m2)
{
	return Matrix2<float>(simd::add4(m1.row0, m2.row0),
		simd::add4(m1.row1, m2.row1));
}

template<>
inline Matrix2<float> operator-(const Matrix2<float>& m1, const Matrix2<float>& m2)
{
	return Matrix2<float>(simd::sub4(m1.row0, m2.row0),
		simd::sub4(m1.row1, m2.row1));
}

template<>
inline Matrix2<float> operator*(float f, const Matrix2<float>& m)
{
	auto t = simd::set4(f);
	return Matrix2<float>(simd::mul4(t, m.row0),
		simd::mul4(t, m.row1));
}

template<>
inline Matrix2<float> operator*(const Matrix2<float>& m, float f)
{
	auto t = simd::set4(f);
	return Matrix2<float>(simd::mul4(m.row0, t),
		simd::mul4(m.row1, t));
}

template<>
inline Matrix2<float> operator*(const Matrix2<float>& m1, const Matrix2<float>& m2)
{
	auto a = simd::pack2x2(m1.row0, m1.row1);
	auto b = simd::pack2x2(m2.row0, m2.row1);
	auto a1 = simd::swizzle<0, 0, 2, 2>(a);
	auto b1 = simd::swizzle<0, 1, 0, 1>(b);
	auto a2 = simd::swizzle<1, 1, 3, 3>(a);
	auto b2 = simd::swizzle<2, 3, 2, 3>(b);
	auto result = simd::add4(simd::mul4(a1, b1), simd::mul4(a2, b2));
	Matrix2<float> m(Uninitialized());
	simd::unpack2x2(result, m.row0, m.row1);
	return m;
}

inline bool Matrix2<float>::operator==(const Matrix2<float>& m) const
{
	return simd::all4(simd::equal(simd::pack2x2(row0, row1), simd::pack2x2(m.row0, m.row1)));
}

template<>
inline std::istream& operator>>(std::istream& s, Matrix2<float>& m)
{
	float m00, m01;
	float m10, m11;
	s >> m00 >> std::skipws >> m01 >> std::skipws >> m10 >> std::skipws >> m11;
	m.set(m00, m01, m10, m11);
	return s;
}

template<>
inline std::ostream& operator<<(std::ostream& s, const Matrix2<float>& m)
{
	return s << m.m00 << ' ' << m.m01 << ' ' << m.m10 << ' ' << m.m11;
}

inline Matrix2<float>& Matrix2<float>::set(float m00, float m01, float m10, float m11)
{ 
#if MATHEMATICS_SIMD_EXPAND_LAST
	row0 = simd::set4(m00, m01, m01, m01);
	row1 = simd::set4(m10, m11, m11, m11);
#else
	row0 = simd::set2(m00, m01);
	row1 = simd::set2(m10, m11);
#endif
	return *this;
}

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline Matrix2<T> concatenate(const Matrix2<T>& m1, const Matrix2<T>& m2) noexcept
{
	return m1*m2;
}

template<typename T>
inline Matrix2<T> concatenate(const Matrix2<T>& m1, const Matrix2<T>& m2, const Matrix2<T>& m3) noexcept
{
	return concatenate(concatenate(m1, m2), m3);
}

template<typename T>
inline Matrix2<T> concatenate(const Matrix2<T>& m1, const Matrix2<T>& m2, const Matrix2<T>& m3, const Matrix2<T>& m4) noexcept
{
	return concatenate(concatenate(concatenate(m1, m2), m3), m4);
}

template<typename T>
inline Matrix2<T> transpose(const Matrix2<T>& m) noexcept
{
	return Matrix2<T>(m.m00, m.m10, m.m01, m.m11);
}

template<typename T>
inline Matrix2<T> inverse(const Matrix2<T>& m) noexcept
{
	return Matrix2<T>(Uninitialized()).setInverse(m);
}

template<typename T>
inline Matrix2<T> adjoint(const Matrix2<T>& m) noexcept
{
	return Matrix2<T>(m.m11, -m.m10, -m.m01, m.m00);
}

#if SIMD_HAS_FLOAT4

template<>
inline Matrix2<float> transpose(const Matrix2<float>& m) noexcept
{
	Matrix2<float> n(m);
	simd::transpose2x2(n.row0, n.row1);
	return n;
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

using Matrix2f = templates::Matrix2<float>;
using Matrix2d = templates::Matrix2<double>;

#if MATHEMATICS_DOUBLE
using Matrix2 = templates::Matrix2<double>;
using Matrix2Arg = templates::Matrix2<double>::ConstArg;
using Matrix2Result = templates::Matrix2<double>::ConstResult;
#else
using Matrix2 = templates::Matrix2<float>;
using Matrix2Arg = templates::Matrix2<float>::ConstArg;
using Matrix2Result = templates::Matrix2<float>::ConstResult;
#endif

} // namespace core::mathematics

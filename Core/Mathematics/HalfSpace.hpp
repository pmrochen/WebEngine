/*
 *	Name: HalfSpace
 *	Author: Pawel Mrochen
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <istream>
#include <ostream>
#include <limits>
#include <type_traits>
#include <tuple>
#include <algorithm>
#include <Simd/Intrinsics.hpp>
#include "Constants.hpp"
#include "Scalar.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "AffineTransform.hpp"
#include "Segment3.hpp"
#include "Triangle3.hpp"

namespace core::mathematics {
namespace templates {

template<typename T>
struct Plane;

template<typename T>
struct HalfSpace
{
	using Real = T;
	using ConstArg = const HalfSpace&;
	using ConstResult = const HalfSpace&;

	constexpr HalfSpace() noexcept : a(), b(), c(), d() {}
	constexpr HalfSpace(T a, T b, T c, T d) noexcept : a(a), b(b), c(c), d(d) {}
	constexpr HalfSpace(Vector3<T>::ConstArg normal, T constant) noexcept : x(normal.x), y(normal.y), z(normal.z), w(constant) {}
	HalfSpace(Vector3<T>::ConstArg normal, Vector3<T>::ConstArg point) noexcept : a(normal.x), b(normal.y), c(normal.z), d(-dot(normal, point)) {}
	HalfSpace(Vector3<T>::ConstArg p0, Vector3<T>::ConstArg p1, Vector3<T>::ConstArg p2) noexcept;
	HalfSpace(const Plane<T>& p) noexcept;
	explicit HalfSpace(const std::tuple<T, T, T, T>& t) noexcept : a(std::get<0>(t)), b(std::get<1>(t)), c(std::get<2>(t)), d(std::get<3>(t)) {}
	template<typename U> explicit HalfSpace(const std::tuple<U, U, U, U>& t) noexcept : a(T(std::get<0>(t))), b(T(std::get<1>(t))), c(T(std::get<2>(t))), d(T(std::get<3>(t))) {}
	//explicit HalfSpace(const T* h) noexcept { a = h[0]; b = h[1]; c = h[2]; d = h[3]; }

	explicit operator std::tuple<T, T, T, T>() noexcept { return std::tuple<T, T, T, T>(a, b, c, d); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() noexcept { return std::tuple<U, U, U, U>(U(a), U(b), U(c), U(d)); }
	//explicit operator T*() noexcept { return &a; }
	//explicit operator const T*() const noexcept { return &a; }
	//T& operator[](int i) noexcept { return (&a)[i]; }
	//const T& operator[](int i) const noexcept { return (&a)[i]; }

	bool operator==(const HalfSpace& h) const noexcept { return (a == h.a) && (b == h.b) && (c == h.c) && (d == h.d); }
	bool operator!=(const HalfSpace& h) const noexcept { return !(*this == h); }
	friend std::istream& operator>>(std::istream& s, HalfSpace& h);
	friend std::ostream& operator<<(std::ostream& s, const HalfSpace& h) { return s << h.a << ' ' << h.b << ' ' << h.c << ' ' << h.d; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & a & b & c & d; }

	// Properties
	bool isEmpty() const noexcept { return (a == T()) && (b == T()) && (c == T()) && (d == T()); }
	bool isApproxEqual(const HalfSpace& h) const noexcept;
	bool isApproxEqual(const HalfSpace& h, T tolerance) const noexcept;
	bool isFinite() const noexcept { return std::isfinite(a) && std::isfinite(b) && std::isfinite(c) && std::isfinite(d); }
	Vector3<T>::ConstResult getNormal() const noexcept { return reinterpret_cast<const Vector3&>(*this); }
	void setNormal(Vector3<T>::ConstArg normal) noexcept { a = normal.x; b = normal.y; c = normal.z; }
	T getConstant() const noexcept { return d; }
	void setConstant(T constant) noexcept { d = constant; }
	HalfSpace& makeEmpty() noexcept { a = T(); b = T(); c = T(); d = T(); return *this; }
	HalfSpace& set(Vector3<T>::ConstArg normal, T constant) noexcept { a = normal.x; b = normal.y; c = normal.z; d = constant; return *this; }
	HalfSpace& set(T a, T b, T c, T d) noexcept { this->a = a; this->b = b; this->c = c; this->d = d; return *this; }

	// Transformation
	HalfSpace& translate(Vector3<T>::ConstArg offset) noexcept;
	HalfSpace& transform(const Matrix3<T>& matrix, bool orthogonal = false) noexcept;
	HalfSpace& transform(const AffineTransform<T>& transformation, bool orthogonal = false) noexcept;
	HalfSpace& flip() noexcept { a = -a; b = -b; c = -c; d = -d; return *this; }
	HalfSpace& normalize() noexcept;

	// Distances
	T getDistanceTo(Vector3<T>::ConstArg point) const { return std::max(dot(getNormal(), point) + d, 0.f); } // plane must be normalized
	template<bool NORMALIZED> T getDistanceTo(Vector3<T>::ConstArg point) const;
	T getSignedDistanceTo(Vector3<T>::ConstArg point) const noexcept { return (dot(getNormal(), point) + d); } // plane must be normalized
	template<bool NORMALIZED> T getSignedDistanceTo(Vector3<T>::ConstArg point) const noexcept;

	// Intersection
	bool contains(Vector3<T>::ConstArg point) const noexcept { return (dot(getNormal(), point) <= -d); }
	bool testIntersection(const Segment3<T>& segment) const noexcept;
	bool testIntersection(const Triangle3<T>& triangle) const noexcept;

	//static const HalfSpace& getEmpty() noexcept { return EMPTY; }

	static const HalfSpace EMPTY;

	T a, b, c, d;
};

template<typename T> const HalfSpace<T> HalfSpace<T>::EMPTY{};

#if SIMD_HAS_FLOAT4

template<>
struct HalfSpace<float>
{
	using Real = float;
	using ConstArg = const HalfSpace;
	using ConstResult = const HalfSpace;

	/*constexpr*/ HalfSpace() noexcept { abcd = simd::zero<simd::float4>(); }
	/*constexpr*/ HalfSpace(float a, float b, float c, float d) noexcept { abcd = simd::set4(a, b, c, d); }
	/*constexpr*/ HalfSpace(Vector3<float>::ConstArg normal, float constant) noexcept { abcd = simd::insert<simd::W>(constant, normal); }
	HalfSpace(Vector3<float>::ConstArg normal, Vector3<float>::ConstArg point) noexcept { abcd = simd::insert<simd::W>(-dot(normal, point), normal); }
	HalfSpace(Vector3<float>::ConstArg p0, Vector3<float>::ConstArg p1, Vector3<float>::ConstArg p2) noexcept;
	HalfSpace(const Plane<float>& p) noexcept;
	explicit HalfSpace(const std::tuple<float, float, float, float>& t) noexcept { abcd = simd::set4(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t)); }
	template<typename U> explicit HalfSpace(const std::tuple<U, U, U, U>& t) noexcept { abcd = simd::set4((float)std::get<0>(t), (float)std::get<1>(t), (float)std::get<2>(t), (float)std::get<3>(t)); }
	//explicit HalfSpace(const float* h) noexcept { abcd = simd::load4(h); }
	explicit HalfSpace(simd::float4 h) noexcept : abcd(h) {}

	operator simd::float4() const noexcept { return abcd; }
	explicit operator std::tuple<float, float, float, float>() noexcept { return std::tuple<float, float, float, float>(a, b, c, d); }
	template<typename U> explicit operator std::tuple<U, U, U, U>() noexcept { return std::tuple<U, U, U, U>(U(a), U(b), U(c), U(d)); }
	//explicit operator float* () noexcept { return &a; }
	//explicit operator const float* () const noexcept { return &a; }
	//float& operator[](int i) noexcept { return (&a)[i]; }
	//const float& operator[](int i) const noexcept { return (&a)[i]; }

	bool operator==(const HalfSpace& h) const noexcept { return simd::all4(simd::equal(abcd, h)); }
	bool operator!=(const HalfSpace& h) const noexcept { return !(*this == h); }
	friend std::istream& operator>>(std::istream& s, HalfSpace& h);
	friend std::ostream& operator<<(std::ostream& s, const HalfSpace& h) { return s << h.a << ' ' << h.b << ' ' << h.c << ' ' << h.d; }

	template<class A> void serialize(A& ar, const unsigned int version) { ar & a & b & c & d; } // #FIXME use simd::set(a, b, c, d)

	// Properties
	bool isEmpty() const noexcept { return simd::all4(simd::equal(abcd, simd::zero<simd::float4>())); }
	bool isApproxEqual(const HalfSpace& h) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(abcd, h)), simd::set4(Constants<float>::TOLERANCE))); }
	bool isApproxEqual(const HalfSpace& h, float tolerance) const noexcept { simd::all4(simd::lessThan(simd::abs4(simd::sub4(abcd, h)), simd::set4(tolerance))); }
	bool isFinite() const noexcept { return simd::all4(simd::isFinite(abcd)); }
#if MATHEMATICS_SIMD_EXPAND_LAST
	Vector3<float> getNormal() const noexcept { return Vector3<float>(simd::swizzle<simd::XYZZ>(abcd)); }
#else
	Vector3<float> getNormal() const noexcept { return Vector3<float>(simd::cutoff3(abcd)); }
#endif
	void setNormal(Vector3<float>::ConstArg normal) noexcept { abcd = simd::insert3(normal, abcd); }
	float getConstant() const noexcept { return simd::extract<simd::W>(abcd); }
	void setConstant(float constant) noexcept { abcd = simd::insert<simd::W>(constant, abcd); }
	HalfSpace& makeEmpty() noexcept { abcd = simd::zero<simd::float4>(); return *this; }
	HalfSpace& set(Vector3<float>::ConstArg normal, float constant) noexcept { abcd = simd::insert<simd::W>(constant, normal); return *this; }
	HalfSpace& set(float a, float b, float c, float d) noexcept { abcd = simd::set4(a, b, c, d); return *this; }

	// Transformation
	HalfSpace& translate(Vector3<float>::ConstArg offset) noexcept;
	HalfSpace& transform(const Matrix3<float>& matrix, bool orthogonal = false) noexcept;
	HalfSpace& transform(const AffineTransform<float>& transformation, bool orthogonal = false) noexcept;
	HalfSpace& flip() noexcept { abcd = simd::neg4(abcd); return *this; }
	HalfSpace& normalize() noexcept;

	// Distances
	float getDistanceTo(Vector3<float>::ConstArg point) const { return std::max(dot(getNormal(), point) + d, 0.f); } // plane must be normalized
	template<bool NORMALIZED> float getDistanceTo<(Vector3<float>::ConstArg point) const;
	float getSignedDistanceTo(Vector3<float>::ConstArg point) const noexcept { return (dot(getNormal(), point) + d); } // plane must be normalized
	template<bool NORMALIZED> float getSignedDistanceTo(Vector3<float>::ConstArg point) const noexcept;

	// Intersection
	bool contains(Vector3<float>::ConstArg point) const noexcept { return (dot(getNormal(), point) <= -d); }
	bool testIntersection(const Segment3<float>& segment) const noexcept;
	bool testIntersection(const Triangle3<float>& triangle) const noexcept;

	//static const HalfSpace& getEmpty() noexcept { return EMPTY; }

	static const HalfSpace EMPTY;

	union
	{
		simd::float4 abcd;
		struct { float a, b, c, d; };
	};
};

const HalfSpace<float> HalfSpace<float>::EMPTY{};

#endif /* SIMD_HAS_FLOAT4 */

template<typename T>
inline HalfSpace<T>::HalfSpace<T>(Vector3<T>::ConstArg p0, Vector3<T>::ConstArg p1, Vector3<T>::ConstArg p2)
{
	Vector3<T> normal(cross(p1 - p0, p2 - p0));
	normal.normalize();
	set(normal, -dot(normal, p0));
}

template<typename T>
inline std::istream& operator>>(std::istream& s, HalfSpace<T>& h) 
{ 
	return s >> h.a >> std::skipws >> h.b >> std::skipws >> h.c >> std::skipws >> h.d; 
}

template<typename T>
inline bool HalfSpace<T>::isApproxEqual(const HalfSpace<T>& h) const
{ 
	return (std::fabs(h.a - a) < Constants<T>::TOLERANCE) && (std::fabs(h.b - b) < Constants<T>::TOLERANCE) && 
		(std::fabs(h.c - c) < Constants<T>::TOLERANCE) && (std::fabs(h.d - d) < Constants<T>::TOLERANCE); 
}

template<typename T>
inline bool HalfSpace<T>::isApproxEqual(const HalfSpace<T>& h, T tolerance) const
{
	return (std::fabs(h.a - a) < tolerance) && (std::fabs(h.b - b) < tolerance) && 
		(std::fabs(h.c - c) < tolerance) && (std::fabs(h.d - d) < tolerance); 
}

template<typename T>
inline HalfSpace<T>& HalfSpace<T>::translate(Vector3<T>::ConstArg offset)
{
	Vector3 p(-getConstant()*getNormal());
	setConstant(-dot(getNormal(), p + offset));
	return *this;
}

template<typename T>
inline HalfSpace<T>& HalfSpace<T>::transform(const Matrix3<T>& matrix, bool orthogonal)
{
	if (orthogonal)
	{
		Vector3 p(-getConstant()*getNormal());
		set(getNormal()*matrix, -dot(getNormal(), p*matrix);
	}
	else
	{
		Matrix3 normalMatrix;
		normalMatrix.inverseTransposeOf(matrix);
		Vector3 p(-getConstant()*getNormal());
		set(::normalize(getNormal()*normalMatrix), -dot(getNormal(), p*matrix);
	}
	return *this;
}

template<typename T>
inline HalfSpace<T>& HalfSpace<T>::transform(const AffineTransform<T>& transformation, bool orthogonal)
{
	if (orthogonal)
	{
		Vector3 p(-getConstant()*getNormal());
		set(getNormal()*transformation.getBasis(), -dot(getNormal(), ::transform(p, transformation));
	}
	else
	{
		Matrix3 normalMatrix;
		normalMatrix.inverseTransposeOf(transformation.getBasis());
		Vector3 p(-getConstant()*getNormal());
		set(::normalize(getNormal()*normalMatrix), -dot(getNormal(), ::transform(p, transformation));
	}
	return *this;
}

template<typename T>
inline HalfSpace<T>& HalfSpace<T>::normalize()
{
//#if MATHEMATICS_FAST_NORMALIZE
//	if costexpr(std::is_same_v<T, float>)
//	{
//		T m = rcpSqrtApprox(getNormal().getMagnitudeSquared());
//		if (m <= std::numeric_limits<T>::max())
//			*this *= m;
//	}
//	else
//#endif
	{
		T m = getNormal().getMagnitude();
		if (m > T(0))
		{
			m = T(1)/m;
			a *= m;
			b *= m;
			c *= m;
			d *= m;
		}
	}
	return *this;
}

template<typename T>
template<bool NORMALIZED> 
inline T HalfSpace<T>::getDistanceTo<(Vector3<T>::ConstArg point) const
{
	if constexpr (NORMALIZED)
		return std::max(dot(getNormal(), point) + d, T(0));
	else
		return std::max((dot(getNormal(), point) + d)/getNormal().getMagnitude(), T(0));
}

template<typename T>
template<bool NORMALIZED> 
inline T HalfSpace<T>::getSignedDistanceTo(Vector3<T>::ConstArg point) const
{
	if constexpr (NORMALIZED)
		return dot(getNormal(), point) + d;
	else
		return (dot(getNormal(), point) + d)/getNormal().getMagnitude(), T(0);
}

template<typename T>
inline bool HalfSpace<T>::testIntersection(const Segment3<T>& segment) const
{
	return contains(segment.start) || contains(segment.end);
}

template<typename T>
inline bool HalfSpace<T>::testIntersection(const Triangle3<T>& triangle) const
{
	return contains(triangle.vertices[0]) || contains(triangle.vertices[1]) || contains(triangle.vertices[2]);
}

#if SIMD_HAS_FLOAT4

inline HalfSpace<float>::HalfSpace<float>(Vector3<float>::ConstArg p0, Vector3<float>::ConstArg p1, Vector3<float>::ConstArg p2)
{
	Vector3<float> normal(cross(p1 - p0, p2 - p0));
	normal.normalize();
	set(normal, -dot(normal, p0));
}

template<>
inline std::istream& operator>>(std::istream& s, HalfSpace<float>& h)
{ 
	float a, b, c, d; 
	s >> a >> std::skipws >> b >> std::skipws >> c >> std::skipws >> d;
	h.set(a, b, c, d);
	return s; 
}

inline HalfSpace<float>& HalfSpace<float>::translate(Vector3<float>::ConstArg offset)
{
	Vector3 p(-getConstant()*getNormal());
	setConstant(-dot(getNormal(), p + offset));
	return *this;
}

inline HalfSpace<float>& HalfSpace<float>::transform(const Matrix3<float>& matrix, bool orthogonal)
{
	if (orthogonal)
	{
		Vector3 p(-getConstant()*getNormal());
		set(getNormal()*matrix, -dot(getNormal(), p*matrix);
	}
	else
	{
		Matrix3 normalMatrix;
		normalMatrix.inverseTransposeOf(matrix);
		Vector3 p(-getConstant()*getNormal());
		set(::normalize(getNormal()*normalMatrix), -dot(getNormal(), p*matrix);
	}
	return *this;
}

inline HalfSpace<float>& HalfSpace<float>::transform(const AffineTransform<float>& transformation, bool orthogonal)
{
	if (orthogonal)
	{
		Vector3 p(-getConstant()*getNormal());
		set(getNormal()*transformation.getBasis(), -dot(getNormal(), ::transform(p, transformation));
	}
	else
	{
		Matrix3 normalMatrix;
		normalMatrix.inverseTransposeOf(transformation.getBasis());
		Vector3 p(-getConstant()*getNormal());
		set(::normalize(getNormal()*normalMatrix), -dot(getNormal(), ::transform(p, transformation));
	}
	return *this;
}

inline HalfSpace<float>& HalfSpace<float>::normalize()
{
#if MATHEMATICS_FAST_NORMALIZE
	float m = simd::toFloat(simd::rcpSqrtApprox1(simd::dot3(abcd, abcd)));
	if (m <= std::numeric_limits<float>::max()) 
		abcd = simd::mul4(abcd, simd::set4(m));
#else
	float m = getNormal().getMagnitude();
	if (m > 0.f) 
		abcd = simd::div4(abcd, simd::set4(m));
#endif
	return *this;
}

template<bool NORMALIZED>
inline T HalfSpace<float>::getDistanceTo<(Vector3<float>::ConstArg point) const
{
	if constexpr (NORMALIZED)
		return std::max(dot(getNormal(), point) + d, T(0));
	else
		return std::max((dot(getNormal(), point) + d)/getNormal().getMagnitude(), T(0));
}

template<bool NORMALIZED>
inline T HalfSpace<float>::getSignedDistanceTo(Vector3<float>::ConstArg point) const
{
	if constexpr (NORMALIZED)
		return dot(getNormal(), point) + d;
	else
		return (dot(getNormal(), point) + d)/getNormal().getMagnitude(), T(0);
}

inline bool HalfSpace<float>::testIntersection(const Segment3<float>& segment) const
{
	return contains(segment.start) || contains(segment.end);
}

inline bool HalfSpace<float>::testIntersection(const Triangle3<float>& triangle) const
{
	return contains(triangle.vertices[0]) || contains(triangle.vertices[1]) || contains(triangle.vertices[2]);
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace templates

#if MATHEMATICS_DOUBLE
using HalfSpace = templates::HalfSpace<double>;
using HalfSpaceArg = templates::HalfSpace<double>::ConstArg;
using HalfSpaceResult = templates::HalfSpace<double>::ConstResult;
#else
using HalfSpace = templates::HalfSpace<float>;
using HalfSpaceArg = templates::HalfSpace<float>::ConstArg;
using HalfSpaceResult = templates::HalfSpace<float>::ConstResult;
#endif

} // namespace core::mathematics

#include "Plane.hpp"

namespace core::mathematics::templates {

template<typename T>
template<typename U>
inline HalfSpace<T>::HalfSpace(const Plane<U>& p) : a(p.a), b(p.b), c(p.c), d(p.d)
{
}

#if SIMD_HAS_FLOAT4

template<typename U> 
inline HalfSpace<float>::HalfSpace(const Plane<U>& p)
{
	abcd = p.abcd;
}

#endif /* SIMD_HAS_FLOAT4 */

} // namespace core::mathematics::templates

namespace std
{

template<typename T>
struct tuple_size<core::mathematics::templates::HalfSpace<T>> : std::integral_constant<std::size_t, 4> {};

template<std::size_t I, typename T>
inline T& get(core::mathematics::templates::HalfSpace<T>& h) noexcept
{
	if constexpr (I == 0)
		return h.a;
	else if constexpr (I == 1)
		return h.b;
	else if constexpr (I == 2)
		return h.c;
	else if constexpr (I == 3)
		return h.d;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T& get(const core::mathematics::templates::HalfSpace<T>& h) noexcept
{
	if constexpr (I == 0)
		return h.a;
	else if constexpr (I == 1)
		return h.b;
	else if constexpr (I == 2)
		return h.c;
	else if constexpr (I == 3)
		return h.d;
	static_assert(false);
}

template<std::size_t I, typename T>
inline T&& get(core::mathematics::templates::HalfSpace<T>&& h) noexcept
{
	if constexpr (I == 0)
		return h.a;
	else if constexpr (I == 1)
		return h.b;
	else if constexpr (I == 2)
		return h.c;
	else if constexpr (I == 3)
		return h.d;
	static_assert(false);
}

template<std::size_t I, typename T>
inline const T&& get(const core::mathematics::templates::HalfSpace<T>&& h) noexcept
{
	if constexpr (I == 0)
		return h.a;
	else if constexpr (I == 1)
		return h.b;
	else if constexpr (I == 2)
		return h.c;
	else if constexpr (I == 3)
		return h.d;
	static_assert(false);
}

} // namespace std

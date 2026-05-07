/*
 *	Name: Viewport
 *	Author: Pawel Mrochen
 */

#pragma once

#include <istream>
#include <ostream>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <functional>
#include <utility>
#include <cstddef>
#include <cmath>
#include <Vector2.hpp>
#include <AffineTransform.hpp>
#include <Interval.hpp>
#include <Size.hpp>
#include <Point.hpp>
#include <Rectangle.hpp>

namespace graphics {
namespace templates {

//using namespace ::mathematics::templates;
//using namespace ::imaging::templates;

using ::mathematics::Arithmetic;
using ::mathematics::templates::Vector2;
using ::mathematics::templates::AffineTransform;
using ::mathematics::templates::Interval;
using ::imaging::templates::Size;
using ::imaging::templates::Point;
using ::imaging::templates::Rectangle;

template<typename T, typename U>
	requires ((std::floating_point<T> || std::integral<T>) && std::floating_point<U>)
struct Viewport
{
	using ConstArg = const Viewport&;
	using ConstResult = const Viewport&;

	Viewport() noexcept : bounds(), depthRange(U(0), U(1)) {}
	explicit Viewport(Uninitialized) noexcept : bounds(Uninitialized()), depthRange(Uninitialized()) {}
	Viewport(const Rectangle<T>& bounds, const Interval<U>& depthRange) noexcept : bounds(bounds), depthRange(depthRange) {}
	Viewport(const Point<T>& location, const Size<T>& size, const Interval<U>& depthRange) noexcept : bounds(location, size), depthRange(depthRange) {}
	Viewport(const Vector2<T>& location, const Vector2<T>& size, const Interval<U>& depthRange) noexcept : bounds(location.x, location.y, size.x, size.y), depthRange(depthRange) {}
	explicit Viewport(const Rectangle<T>& bounds) noexcept : bounds(bounds), depthRange(U(0), U(1)) {}
	Viewport(const Point<T>& location, const Size<T>& size) noexcept : bounds(location, size), depthRange(U(0), U(1)) {}
	Viewport(const Vector2<T>& location, const Vector2<T>& size) noexcept : bounds(location.x, location.y, size.x, size.y), depthRange(U(0), U(1)) {}
	explicit Viewport(const Size<T>& size) noexcept : bounds(size), depthRange(U(0), U(1)) {}
	explicit Viewport(const Vector2<T>& size) noexcept : bounds(size.x, size.y), depthRange(U(0), U(1)) {}
	Viewport(T x, T y, T width, T height, U minDepth, U maxDepth) noexcept : bounds(x, y, width, height), depthRange(minDepth, maxDepth) {}
	Viewport(T x, T y, T width, T height) noexcept : bounds(x, y, width, height), depthRange(U(0), U(1)) {}
	Viewport(T width, T height) noexcept : bounds(width, height), depthRange(U(0), U(1)) {}
	template<Arithmetic V> explicit Viewport(const Viewport<V, U>& viewport) noexcept : bounds(viewport.bounds), depthRange(viewport.depthRange) {}

	bool operator==(const Viewport& viewport) const noexcept { return (bounds == viewport.bounds) && (depthRange == viewport.depthRange); }
	bool operator!=(const Viewport& viewport) const noexcept { return !(*this == viewport); }

	template<typename A> void serialize(A& ar) { ar(bounds, depthRange); }

	static Viewport fromMinimumMaximum(const Point<T>& minimum, const Point<T>& maximum, const Interval<U>& depthRange) noexcept
	{
		return Viewport<T>(minimum, maximum - minimum, depthRange);
	}

	static Viewport fromMinimumMaximum(const Point<T>& minimum, const Point<T>& maximum) noexcept
	{
		return Viewport<T>(minimum, maximum - minimum);
	}

	static Viewport fromMinimumMaximum(const Vector2<T>& minimum, const Vector2<T>& maximum, const Interval<U>& depthRange) noexcept
	{
		return Viewport<T>(minimum, maximum - minimum, depthRange);
	}

	static Viewport fromMinimumMaximum(const Vector2<T>& minimum, const Vector2<T>& maximum) noexcept
	{
		return Viewport<T>(minimum, maximum - minimum);
	}

	static Viewport fromLeftTopRightBottom(T left, T top, T right, T bottom, U minDepth, U maxDepth) noexcept
	{
		return Viewport<T>(left, top, right - left, bottom - top, minDepth, maxDepth);
	}

	static Viewport fromLeftTopRightBottom(T left, T top, T right, T bottom) noexcept
	{
		return Viewport<T>(left, top, right - left, bottom - top);
	}

	bool isEmpty() const noexcept
	{
		return bounds.size.anyLessThanEqual(Size<T>::ZERO) || depthRange.isEmpty();
	}

	bool approxEquals(const Viewport& viewport) const noexcept 
	{
		if constexpr (std::is_floating_point_v<T>)
			return bounds.approxEquals(viewport.bounds) && depthRange.approxEquals(viewport.depthRange);
		else
			return (bounds == viewport.bounds) && depthRange.approxEquals(viewport.depthRange);
	}

	bool approxEquals(const Viewport& viewport, T tolerance) const noexcept 
	{
		if constexpr (std::is_floating_point_v<T>)
			return bounds.approxEquals(viewport.bounds, tolerance) && depthRange.approxEquals(viewport.depthRange, tolerance);
		else
			return (bounds == viewport.bounds) && depthRange.approxEquals(viewport.depthRange, tolerance);
	}

	bool isFinite() const noexcept 
	{ 
		if constexpr (std::is_floating_point_v<T>)
			return bounds.isFinite() && depthRange.isFinite();
		else
			return depthRange.isFinite();
	}
	
	Viewport& set(const Rectangle<T>& bounds, const Interval<U>& depthRange) noexcept 
	{ 
		this->bounds = bounds; 
		this->depthRange = depthRange; 
		return *this; 
	}
	
	const Rectangle<T>& getBounds() const noexcept { return bounds; }
	void setBounds(const Rectangle<T>& bounds) noexcept { this->bounds = bounds; }
	Vector2<T> getLocation() const noexcept { return Vector2<T>(bounds.location.x, bounds.location.y); }
	void setLocation(const Vector2<T>& location) noexcept { bounds.location.set(location.x, location.y); }
	Vector2<T> getSize() const noexcept { return Vector2<T>(bounds.size.width, bounds.size.height); }
	void setSize(const Vector2<T>& size) noexcept { bounds.size.set(size.x, size.y); }
	Vector2<T> getMinimum() const noexcept { return Vector2<T>(bounds.location.x, bounds.location.y); }
	Vector2<T> getMaximum() const noexcept { return Vector2<T>(bounds.location.x + bounds.size.width, bounds.location.y + bounds.size.height); }
	T getX() const noexcept { return location.x; }
	void setX(T x) noexcept { location.x = x; }
	T getY() const noexcept { return location.y; }
	void setY(T y) noexcept { location.y = y; }
	T getWidth() const noexcept { return size.width; }
	void setWidth(T width) noexcept { size.width = width; }
	T getHeight() const noexcept { return size.height; }
	void setHeight(T height) noexcept { size.height = height; }
	T getLeft() const noexcept { return location.x; }
	T getTop() const noexcept { return location.y; }
	T getRight() const noexcept { return location.x + size.width; }
	T getBottom() const noexcept { return location.y + size.height; }
	U getAspectRatio() const noexcept { return size.getAspectRatio<U>(); }
	const Interval<U>& getDepthRange() const noexcept { return depthRange; }
	void setDepthRange(const Interval<U>& depthRange) noexcept { this->depthRange = depthRange; }
	U getMinDepth() const noexcept { return depthRange.minimum; }
	void setMinDepth(U minDepth) noexcept { depthRange.minimum = minDepth; }
	U getMaxDepth() const noexcept { return depthRange.maximum; }
	void setMaxDepth(U maxDepth) noexcept { depthRange.maximum = maxDepth; }

	AffineTransform<U> getTransformation() const noexcept
	{
		U halfW = U(size.width)*U(0.5);
		U halfH = U(size.height)*U(0.5);
		return AffineTransform<U>(halfW, U(0), U(0),
			U(0), -halfH, U(0),
			U(0), U(0), depthRange.maximum - depthRange.minimum,
			/*location.x +*/ halfW, /*location.y +*/ halfH, depthRange.minimum);
	}

	AffineTransform<U> getInverseTransformation() const noexcept
	{
		return AffineTransform<U>(U(2)/U(size.width), U(0), U(0),
			U(0), -U(2)/U(size.height), U(0),
			U(0), U(0), U(1)/(depthRange.maximum - depthRange.minimum),
			U(-1), U(1), -depthRange.minimum/(depthRange.maximum - depthRange.minimum));
	}

	Rectangle<T> bounds;
	Interval<U> depthRange;
};

template<typename C, typename T, typename U, typename V>
	requires ((std::floating_point<U> || std::integral<U>) && std::floating_point<V>)
inline std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& s, Viewport<U, V>& viewport)
{ 
	return s >> viewport.bounds >> std::ws >> viewport.depthRange;
}

template<typename C, typename T, typename U, typename V>
	requires ((std::floating_point<U> || std::integral<U>) && std::floating_point<V>)
inline std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& s, const Viewport<U, V>& viewport)
{ 
	constexpr C WS(0x20);
	return s << viewport.bounds << WS << viewport.depthRange;
}

} // namespace templates

using Viewport = templates::Viewport<int, float>;
using ViewportArg = templates::Viewport<int, float>::ConstArg;
using ViewportResult = templates::Viewport<int, float>::ConstResult;

using ViewportF = templates::Viewport<float, float>;
using ViewportFArg = templates::Viewport<float, float>::ConstArg;
using ViewportFResult = templates::Viewport<float, float>::ConstResult;

} // namespace graphics

namespace std {

template<typename T>
struct hash;

template<typename T, typename U>
struct hash<::graphics::templates::Viewport<T, U>>
{
	size_t operator()(const ::graphics::templates::Viewport<T, U>& viewport) const noexcept
	{
		size_t seed = hash<typename ::imaging::templates::Rectangle<T>>()(viewport.bounds) + 0x9e3779b9;
		seed ^= hash<typename ::mathematics::templates::Interval<U>>()(viewport.depthRange) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

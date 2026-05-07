/*
 *	Name: ViewVolume
 *	Author: Pawel Mrochen
 */

#pragma once

#include <stdexcept>
#include <limits>
#include <type_traits>
#include <concepts>
#include <functional>
#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <Constants.hpp>
#include <Scalar.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Matrix3.hpp>
#include <AffineTransform.hpp>
#include <Matrix4.hpp>
#include <Ray3.hpp>
#include <LineSegment3.hpp>
#include <HalfSpace.hpp>
#include <Quadrilateral3.hpp>
#include <AxisAlignedBox.hpp>
#include <OrientedBox.hpp>
#include <Sphere.hpp>
#include <SymmetricFrustum.hpp>
#include <Polyhedron.hpp>
#include <ConvexPolyhedron.hpp>
#include "Projection.hpp"

namespace graphics {
namespace templates {

using ::mathematics::Identity;
using ::mathematics::templates::Constants;
using ::mathematics::templates::Vector2;
using ::mathematics::templates::Vector3;
using ::mathematics::templates::Matrix3;
using ::mathematics::templates::AffineTransform;
using ::mathematics::templates::Matrix4;
using ::mathematics::templates::Ray3;
using ::mathematics::templates::LineSegment3;
using ::mathematics::templates::HalfSpace;
using ::mathematics::templates::Quadrilateral3;
using ::mathematics::templates::AxisAlignedBox;
using ::mathematics::templates::OrientedBox;
using ::mathematics::templates::Sphere;
using ::mathematics::templates::SymmetricFrustum;
using ::mathematics::templates::Polyhedron;
using ::mathematics::templates::ConvexPolyhedron;

template<typename T>
	requires std::floating_point<T>
struct ViewVolume
{
	using ConstArg = const ViewVolume&;
	using ConstResult = const ViewVolume&;

	ViewVolume() noexcept :
		origin(),
		basis(Identity()),
		minimum(),
		maximum(),
		projection(Projection::PERSPECTIVE)
	{
	}

	ViewVolume(const Vector3<T>& origin, const Matrix3<T>& basis, const Vector3<T>& minimum, const Vector3<T>& maximum,
		Projection projection) noexcept :
		origin(origin),
		basis(basis),
		minimum(minimum),
		maximum(maximum),
		projection(projection)
	{
	}

	ViewVolume(const Vector3<T>& origin, const Matrix3<T>& basis, const Vector2<T>& fov, T depthMin, T depthMax, Projection projection) noexcept :
		origin(origin),
		basis(basis),
		minimum((projection == Projection::PARALLEL) ? -fov.x*T(0.5) : -depthMin*std::tan(fov.x*T(0.5)),
			(projection == Projection::PARALLEL) ? -fov.y*T(0.5) : -depthMin*std::tan(fov.y*T(0.5)),
			depthMin),
		maximum((projection == Projection::PARALLEL) ? fov.x*T(0.5) : depthMin*std::tan(fov.x*T(0.5)),
			(projection == Projection::PARALLEL) ? fov.y*T(0.5) : depthMin*std::tan(fov.y*T(0.5)),
			depthMax),
		projection(projection)
	{
	}

	ViewVolume(const Vector3<T>& origin, const Matrix3<T>& basis, T zoom, T aspect, T depthMin, T depthMax, Projection projection) noexcept :
		origin(origin),
		basis(basis),
		minimum((projection == Projection::PARALLEL) ? -aspect/zoom : -aspect*depthMin/zoom,
			(projection == Projection::PARALLEL) ? T(-1)/zoom : -depthMin/zoom,
			depthMin),
		maximum((projection == Projection::PARALLEL) ? aspect/zoom : aspect*depthMin/zoom,
			(projection == Projection::PARALLEL) ? T(1)/zoom : depthMin/zoom,
			depthMax),
		projection(projection)
	{
	}

	ViewVolume(const AxisAlignedBox<T>& box) noexcept :
		ViewVolume(OrientedBox<T>(box))
	{
	}

	ViewVolume(const OrientedBox<T>& box) noexcept :
		origin(box.center),
		basis(box.basis),
		minimum(-box.halfDims),
		maximum(box.halfDims),
		projection(Projection::PARALLEL)
	{
	}

	ViewVolume(const SymmetricFrustum<T>& frustum) noexcept :
		origin(frustum.origin),
		basis(frustum.basis),
		minimum(-frustum.halfDims, frustum.depthMin),
		maximum(frustum.halfDims, frustum.depthMax),
		projection(Projection::PERSPECTIVE)
	{
	}

	//explicit ViewVolume(const Matrix4<T>& matrix) noexcept; // #TODO

	bool operator==(const ViewVolume& volume) const noexcept
	{
		return (origin == volume.origin) && (basis == volume.basis) && (minimum == volume.minimum) && (maximum == volume.maximum) &&
			(projection == volume.projection);
	}

	bool operator!=(const ViewVolume& volume) const noexcept
	{
		return !(*this == volume);
	}

	template<typename A> void serialize(A& ar)
	{
		ar(origin, basis, minimum, maximum, projection);
	}

	OrientedBox<T> asOrientedBox() const
	{
		if (projection != Projection::PARALLEL)
			throw std::runtime_error("ViewVolume::asOrientedBox()");

		Vector3<T> center = (minimum + maximum)*T(0.5);
		Vector3<T> halfDims = (maximum - minimum)*T(0.5);
		return OrientedBox<T>(center*basis + origin, basis, halfDims);
	}

	SymmetricFrustum<T> asSymmetricFrustum() const
	{
		if ((projection != Projection::PERSPECTIVE) || !isSymmetric())
			throw std::runtime_error("ViewVolume::asSymmetricFrustum()");

		//return reinterpret_cast<const SymmetricFrustum<T>&>(*this);
		return SymmetricFrustum<T>(origin, basis, maximum.xy(), minimum.z, maximum.z);
	}

	template<std::integral U>
	Polyhedron<T>* toPolyhedron() const
	{
		Polyhedron<T, U>* polyhedron = new Polyhedron<T, U>(getVertices());
		auto [first, last] = getPrimitives<U>(3);
		polyhedron->indices.assign(first, last);
		return polyhedron;
	}

	ConvexPolyhedron<T>* toConvexPolyhedron() const
	{
		return new ConvexPolyhedron<T>(getHalfSpaces());
	}

	bool isEmpty() const noexcept
	{
		return minimum.anyGreaterThanEqual(maximum);
	}

	bool approxEquals(const ViewVolume& volume) const noexcept
	{
		return origin.approxEquals(volume.origin) && basis.approxEquals(volume.basis) &&
			minimum.approxEquals(volume.minimum) && maximum.approxEquals(volume.maximum) &&
			(projection == volume.projection);
	}

	bool approxEquals(const ViewVolume& volume, T tolerance) const noexcept
	{
		return origin.approxEquals(volume.origin, tolerance) && basis.approxEquals(volume.basis, tolerance) &&
			minimum.approxEquals(volume.minimum, tolerance) && maximum.approxEquals(volume.maximum, tolerance) &&
			(projection == volume.projection);
	}

	bool isFinite() const noexcept
	{
		return (maximum.z < std::numeric_limits<T>::max());
	}

	bool isSymmetric() const noexcept
	{
		return (-minimum.xy() == maximum.xy());
	}

	//bool isOrthographic() const noexcept
	//{
	//	return (projection == Projection::PARALLEL) && basis.isApproxOrthogonal();
	//}

	const Vector3<T>& getOrigin() const noexcept { return origin; }
	void setOrigin(const Vector3<T>& origin) noexcept { this->origin = origin; }
	const Matrix3<T>& getBasis() const noexcept { return basis; }
	void setBasis(const Matrix3<T>& basis) noexcept { this->basis = basis; }
	const Vector3<T>& getMinimum() const noexcept { return minimum; }
	void setMinimum(const Vector3<T>& minimum) noexcept { this->minimum = minimum; }
	const Vector3<T>& getMaximum() const noexcept { return maximum; }
	void setMaximum(const Vector3<T>& maximum) noexcept { this->maximum = maximum; }
	Vector3<T> getDimensions() const noexcept { return (maximum - minimum); }
	void setDimensions(const Vector3<T>& dimensions) noexcept { minimum = -dimensions*T(0.5); maximum = dimensions*T(0.5); }
	Vector3<T> getHalfDimensions() const noexcept { return (maximum - minimum)*T(0.5); }
	void setHalfDimensions(const Vector3<T>& halfDims) noexcept { minimum = -halfDims; maximum = halfDims; }
	Vector2<T> getBaseDimensions() const noexcept { return (maximum.xy() - minimum.xy())*(maximum.z/minimum.z); }
	void setBaseDimensions(const Vector2<T>& dimensions) noexcept { setHalfDimensions(dimensions*(T(0.5)*depthRange.minimum/depthRange.maximum)); }
	Vector2<T> getBaseHalfDimensions() const noexcept { return (maximum.xy() - minimum.xy())*(T(0.5)*maximum.z/minimum.z); }
	void setBaseHalfDimensions(const Vector2<T>& halfDims) noexcept { setHalfDimensions(halfDims*(depthRange.minimum/depthRange.maximum)); }
	T getLeft() const noexcept { return minimum.x; }
	void setLeft(T left) noexcept { minimum.x = left; }
	T getRight() const noexcept { return maximum.x; }
	void setRight(T right) noexcept { maximum.x = right; }
	T getWidth() const noexcept { return (maximum.x - minimum.x); }
	void setWidth(T width) noexcept { minimum.x = -width*T(0.5); maximum.x = width*T(0.5); }
	T getBottom() const noexcept { return minimum.y; }
	void setBottom(T bottom) noexcept { minimum.y = bottom; }
	T getTop() const noexcept { return maximum.y; }
	void setTop(T top) noexcept { maximum.y = top; }
	T getHeight() const noexcept { return (maximum.y - minimum.y); }
	void setHeight(T height) noexcept { minimum.y = -height*T(0.5); maximum.y = height*T(0.5); }
	Interval<T> getDepthRange() const noexcept { return { minimum.z, maximum.z }; }
	void setDepthRange(const Interval<T>& depthRange) noexcept { minimum.z = depthRange.minimum; maximum.z = depthRange.maximum; }
	T getMinDepth() const noexcept { return minimum.z; }
	void setMinDepth(T depthMin) noexcept { minimum.z = depthMin; }
	T getMaxDepth() const noexcept { return maximum.z; }
	void setMaxDepth(T depthMax) noexcept { maximum.z = depthMax; }
	T getDepthRatio() const noexcept { return maximum.z/minimum.z; }
	Projection getProjection() const noexcept { return projection; }
	void setProjection(Projection projection) noexcept { this->projection = projection; }

	Vector2<T> getFieldOfView() const noexcept
	{
		return (projection == Projection::PARALLEL) ?
			(maximum.xy() - minimum.xy()) :
			Vector2<T>(std::atan2(maximum.x, minimum.z) - std::atan2(minimum.x, minimum.z), std::atan2(maximum.y, minimum.z) - std::atan2(minimum.y, minimum.z));
	}

	void setFieldOfView(const Vector2<T>& fov) noexcept
	{
		minimum.x = (projection == Projection::PARALLEL) ? -fov.x*T(0.5) : -minimum.z*std::tan(fov.x*T(0.5));
		minimum.y = (projection == Projection::PARALLEL) ? -fov.y*T(0.5) : -minimum.z*std::tan(fov.y*T(0.5));
		maximum.x = (projection == Projection::PARALLEL) ? fov.x*T(0.5) : minimum.z*std::tan(fov.x*T(0.5));
		maximum.y = (projection == Projection::PARALLEL) ? fov.y*T(0.5) : minimum.z*std::tan(fov.y*T(0.5));
	}

	T getHorizontalFieldOfView() const noexcept
	{
		return (projection == Projection::PARALLEL) ? 
			(maximum.x - minimum.x) : 
			(std::atan2(maximum.x, minimum.z) - std::atan2(minimum.x, minimum.z));
	}

	void setHorizontalFieldOfView(T fovX) noexcept
	{
		minimum.x = (projection == Projection::PARALLEL) ? -fovX*T(0.5) : -minimum.z*std::tan(fovX*T(0.5));
		maximum.x = (projection == Projection::PARALLEL) ? fovX*T(0.5) : minimum.z*std::tan(fovX*T(0.5));
	}

	T getVerticalFieldOfView() const noexcept
	{
		return (projection == Projection::PARALLEL) ? 
			(maximum.y - minimum.y) : 
			(std::atan2(maximum.y, minimum.z) - std::atan2(minimum.y, minimum.z));
	}

	void setVerticalFieldOfView(T fovY) noexcept
	{
		minimum.y = (projection == Projection::PARALLEL) ? -fovY*T(0.5) : -minimum.z*tan(fovY*T(0.5));
		maximum.y = (projection == Projection::PARALLEL) ? fovY*T(0.5) : minimum.z*tan(fovY*T(0.5));
	}

	T getZoomFactor() const noexcept
	{
		return (projection == Projection::PARALLEL) ? T(2)/(maximum.y - minimum.y) : T(2)*minimum.z/(maximum.y - minimum.y);
	}

	T getAspectRatio() const noexcept 
	{
		Vector2<T> dims = maximum.xy() - minimum.xy();
		return dims.x/dims.y;
	}

	AffineTransform<T> getViewTransformation() const noexcept
	{
		Matrix3<T> bt = transpose(basis);
		return AffineTransform<T>(bt, -(origin*bt));
	}

	AffineTransform<T> getInverseViewTransformation() const noexcept
	{
		return AffineTransform<T>(basis, origin);
	}

	Matrix4<T> getProjectionMatrix(DepthRange depthRange) const noexcept
	{
		auto [l, b, n] = minimum;
		auto [r, t, f] = maximum;
		bool symmetric = isSymmetric();
		bool positiveZ = (depthRange != DepthRange::NEGATIVE_ONE_TO_ONE);
		bool reverseZ = (depthRange == DepthRange::ONE_TO_ZERO);

		if (projection == Projection::PARALLEL)
		{
			T k = positiveZ ? T(1)/(f - n) : T(2)/(f - n);
			T z = positiveZ ? n/(n - f) : (n + f)/(n - f);

			return { T(2)/(r - l), T(), T(), T(),
				T(), T(2)/(t - b), T(), T(),
				T(), T(), reverseZ ? -k : k, T(),
				symmetric ? T() : (l + r)/(l - r), symmetric ? T() : (b + t)/(b - t), reverseZ ? (z + T(1)) : z, T(1) };
		}
		else
		{
			bool finite = (f < std::numeric_limits<T>::max());
			T k = finite ?
				(positiveZ ? f/(f - n) : (f + n)/(f - n)) :
				T(1);
			T z = finite ?
				(positiveZ ? n*f/(n - f) : T(2)*n*f/(n - f)) :
				(positiveZ ? -n : -T(2)*n);

			return { T(2)*n/(r - l), T(), T(), T(),
				T(), T(2)*n/(t - b), T(), T(),
				symmetric ? T() : (l + r)/(l - r), symmetric ? T() : (b + t)/(b - t), reverseZ ? -k : k, T(1),
				T(), T(), reverseZ ? (z + T(1)) : z, T() };
		}
	}

	Matrix4<T> getViewProjectionMatrix(DepthRange depthRange) const noexcept
	{
		return Matrix4<T>(getViewTransformation())*getProjectionMatrix(depthRange);
	}

	template<std::output_iterator<Vector3<T>> O> O copyVertices(O target) const
	{
		AffineTransform<T> m(basis, origin);
		*target++ = transform(minimum, m);
		*target++ = transform(Vector3<T>(maximum.x, minimum.y, minimum.z), m);
		*target++ = transform(Vector3<T>(minimum.x, maximum.y, minimum.z), m);
		*target++ = transform(Vector3<T>(maximum.x, maximum.y, minimum.z), m);

		if (projection == Projection::PARALLEL)
		{
			*target++ = transform(Vector3<T>(minimum.x, minimum.y, maximum.z), m);
			*target++ = transform(Vector3<T>(maximum.x, minimum.y, maximum.z), m);
			*target++ = transform(Vector3<T>(minimum.x, maximum.y, maximum.z), m);
			*target++ = transform(maximum, m);
		}
		else
		{
			T depthRatio = maximum.z/minimum.z;
			*target++ = transform(Vector3<T>(minimum.x*depthRatio, minimum.y*depthRatio, maximum.z), m);
			*target++ = transform(Vector3<T>(maximum.x*depthRatio, minimum.y*depthRatio, maximum.z), m);
			*target++ = transform(Vector3<T>(minimum.x*depthRatio, maximum.y*depthRatio, maximum.z), m);
			*target++ = transform(Vector3<T>(maximum.x*depthRatio, maximum.y*depthRatio, maximum.z), m);
		}

		return target;
	}

	std::vector<Vector3<T>> getVertices() const
	{
		AffineTransform<T> m(basis, origin);

		if (projection == Projection::PARALLEL)
		{
			return { transform(minimum, m), transform(Vector3<T>(maximum.x, minimum.y, minimum.z), m),
				transform(Vector3<T>(minimum.x, maximum.y, minimum.z), m), transform(Vector3<T>(maximum.x, maximum.y, minimum.z), m),
				transform(Vector3<T>(minimum.x, minimum.y, maximum.z), m), transform(Vector3<T>(maximum.x, minimum.y, maximum.z), m),
				transform(Vector3<T>(minimum.x, maximum.y, maximum.z), m), transform(maximum, m) };
		}
		else
		{
			T depthRatio = maximum.z/minimum.z;
			return { transform(minimum, m), transform(Vector3<T>(maximum.x, minimum.y, minimum.z), m),
				transform(Vector3<T>(minimum.x, maximum.y, minimum.z), m), transform(Vector3<T>(maximum.x, maximum.y, minimum.z), m),
				transform(Vector3<T>(minimum.x*depthRatio, minimum.y*depthRatio, maximum.z), m),
				transform(Vector3<T>(maximum.x*depthRatio, minimum.y*depthRatio, maximum.z), m),
				transform(Vector3<T>(minimum.x*depthRatio, maximum.y*depthRatio, maximum.z), m),
				transform(Vector3<T>(maximum.x*depthRatio, maximum.y*depthRatio, maximum.z), m) };
		}
	}

	Quadrilateral3<T> getProjectionPlaneQuad() const noexcept
	{
		AffineTransform<T> m(basis, origin);
		return { transform(Vector3<T>(minimum.x, maximum.y, minimum.z), m),
			transform(Vector3<T>(maximum.x, maximum.y, minimum.z), m),
			transform(Vector3<T>(maximum.x, minimum.y, minimum.z), m),
			transform(minimum, m) };
	}

	Quadrilateral3<T> getProjectionPlaneQuad(T depth) const noexcept
	{
		AffineTransform<T> m(basis, origin);

		if (projection == Projection::PARALLEL)
		{
			return { transform(Vector3<T>(minimum.x, maximum.y, depth), m),
				transform(Vector3<T>(maximum.x, maximum.y, depth), m),
				transform(Vector3<T>(maximum.x, minimum.y, depth), m),
				transform(Vector3<T>(minimum.x, minimum.y, depth), m) };
		}
		else
		{
			T depthRatio = maximum.z/minimum.z;
			T t = (depth - minimum.z)/(maximum.z - minimum.z);
			Vector2<T> lb = minimum.xy();
			Vector2<T> rb(maximum.x, minimum.y);
			Vector2<T> lt(minimum.x, maximum.y);
			Vector2<T> rt = maximum.xy();

			return { transform(Vector3<T>(lerp(lt, lt*depthRatio, t), depth), m),
				transform(Vector3<T>(lerp(rt, rt*depthRatio, t), depth), m),
				transform(Vector3<T>(lerp(rb, rb*depthRatio, t), depth), m),
				transform(Vector3<T>(lerp(lb, lb*depthRatio, t), depth), m) };
		}
	}

	template<std::integral U> std::pair<const U*, const U*> getPrimitives(int nVerticesPerPrimitive) const noexcept // #TODO return range
	{
		static const U edges[24] = { 0, 2, 2, 3, 3, 1, 1, 0, 3, 7, 5, 1, 6, 2, 0, 4, 5, 7, 7, 6, 6, 4, 4, 5 };
		static const U triangles[36] = { 0, 2, 1, 3, 1, 2, 1, 3, 5, 7, 5, 3, 5, 7, 4, 6, 4, 7, 4, 6, 0, 2, 0, 6, 2, 6, 3, 7, 3, 6, 4, 0, 5, 1, 5, 0 };
		static const U quads[24] = { 0, 2, 3, 1, 1, 3, 7, 5, 5, 7, 6, 4, 4, 6, 2, 0, 2, 6, 7, 3, 4, 0, 1, 5 };

		switch (nVerticesPerPrimitive)
		{
			case 2:
				return { edges, edges + 24 };
			case 3:
				return { triangles, triangles + 36 };
			case 4:
				return { quads, quads + 24 };
			default:
				return { nullptr, nullptr };
		}
	}

	std::size_t getPrimitiveCount(int nVerticesPerPrimitive) const noexcept
	{
		switch (nVerticesPerPrimitive)
		{
			case 2:
				return 12;
			case 3:
				return 12;
			case 4:
				return 6;
			default:
				return 0;
		}
	}

	template<std::output_iterator<HalfSpace<T>> O> O copyHalfSpaces(O target) const	// 0-left, 1-right, 2-bottom, 3-top, 4-near, 5-far
	{																				// normals point outwards
		if (projection == Projection::PARALLEL)
		{
			*target++ = HalfSpace<T>(-basis[0], minimum.x*basis[0] + origin);
			*target++ = HalfSpace<T>(basis[0], maximum.x*basis[0] + origin);
			*target++ = HalfSpace<T>(-basis[1], minimum.y*basis[1] + origin);
			*target++ = HalfSpace<T>(basis[1], maximum.y*basis[1] + origin);
			*target++ = HalfSpace<T>(-basis[2], minimum.z*basis[2] + origin);
			*target++ = HalfSpace<T>(basis[2], maximum.z*basis[2] + origin);
		}
		else
		{
			AffineTransform<T> m(basis, origin);
			Vector3<T> lb = transform(minimum, m);
			Vector3<T> rb = transform(Vector3<T>(maximum.x, minimum.y, minimum.z), m);
			Vector3<T> lt = transform(Vector3<T>(minimum.x, maximum.y, minimum.z), m);
			Vector3<T> rt = transform(Vector3<T>(maximum.x, maximum.y, minimum.z), m);

			bool flip = (basis.getDeterminant() < T(0));
			*target++ = flip ? HalfSpace<T>(origin, lt, lb) : HalfSpace<T>(origin, lb, lt);
			*target++ = flip ? HalfSpace<T>(origin, rb, rt) : HalfSpace<T>(origin, rt, rb);
			*target++ = flip ? HalfSpace<T>(origin, lb, rb) : HalfSpace<T>(origin, rb, lb);
			*target++ = flip ? HalfSpace<T>(origin, rt, lt) : HalfSpace<T>(origin, lt, rt);
			*target++ = HalfSpace<T>(-basis[2], minimum.z*basis[2] + origin);
			if (maximum.z < std::numeric_limits<T>::max())
				*target++ = HalfSpace<T>(basis[2], maximum.z*basis[2] + origin);

			// Another way to get half spaces is from a 4x4 projection matrix M:
			// Left		= -M0 - M3
			// Right	=  M0 - M3
			// Bottom	= -M1 - M3
			// Top		=  M1 - M3
			// Near		= -M2 - M3
			// Far		=  M2 - M3
			// where M0, M1, ... are the i-th column of the matrix M
		}

		return target;
	}

	std::vector<HalfSpace<T>> getHalfSpaces() const
	{
		if (projection == Projection::PARALLEL)
		{
			return { HalfSpace<T>(-basis[0], minimum.x*basis[0] + origin), HalfSpace<T>(basis[0], maximum.x*basis[0] + origin),
				HalfSpace<T>(-basis[1], minimum.y*basis[1] + origin), HalfSpace<T>(basis[1], maximum.y*basis[1] + origin),
				HalfSpace<T>(-basis[2], minimum.z*basis[2] + origin), HalfSpace<T>(basis[2], maximum.z*basis[2] + origin) };
		}
		else
		{
			AffineTransform<T> m(basis, origin);
			Vector3<T> lb = transform(minimum, m);
			Vector3<T> rb = transform(Vector3<T>(maximum.x, minimum.y, minimum.z), m);
			Vector3<T> lt = transform(Vector3<T>(minimum.x, maximum.y, minimum.z), m);
			Vector3<T> rt = transform(Vector3<T>(maximum.x, maximum.y, minimum.z), m);

			bool flip = (basis.getDeterminant() < T(0));
			if (maximum.z < std::numeric_limits<T>::max())
			{
				return { flip ? HalfSpace<T>(origin, lt, lb) : HalfSpace<T>(origin, lb, lt),
					flip ? HalfSpace<T>(origin, rb, rt) : HalfSpace<T>(origin, rt, rb),
					flip ? HalfSpace<T>(origin, lb, rb) : HalfSpace<T>(origin, rb, lb),
					flip ? HalfSpace<T>(origin, rt, lt) : HalfSpace<T>(origin, lt, rt),
					HalfSpace<T>(-basis[2], minimum.z*basis[2] + origin),
					HalfSpace<T>(basis[2], maximum.z*basis[2] + origin) };
			}
			else
			{
				return { flip ? HalfSpace<T>(origin, lt, lb) : HalfSpace<T>(origin, lb, lt),
					flip ? HalfSpace<T>(origin, rb, rt) : HalfSpace<T>(origin, rt, rb),
					flip ? HalfSpace<T>(origin, lb, rb) : HalfSpace<T>(origin, rb, lb),
					flip ? HalfSpace<T>(origin, rt, lt) : HalfSpace<T>(origin, lt, rt),
					HalfSpace<T>(-basis[2], minimum.z*basis[2] + origin) };
			}
		}
	}

	template<typename F> bool enumerateHalfSpaces(F&& f) const noexcept
	{
		if (projection == Projection::PARALLEL)
		{
			if (!f(HalfSpace<T>(-basis[0], minimum.x*basis[0] + origin)))
				return false;
			if (!f(HalfSpace<T>(basis[0], maximum.x*basis[0] + origin)))
				return false;
			if (!f(HalfSpace<T>(-basis[1], minimum.y*basis[1] + origin)))
				return false;
			if (!f(HalfSpace<T>(basis[1], maximum.y*basis[1] + origin)))
				return false;
			if (!f(HalfSpace<T>(-basis[2], minimum.z*basis[2] + origin)))
				return false;
			if (!f(HalfSpace<T>(basis[2], maximum.z*basis[2] + origin)))
				return false;
		}
		else
		{
			AffineTransform<T> m(basis, origin);
			Vector3<T> lb = transform(minimum, m);
			Vector3<T> rb = transform(Vector3<T>(maximum.x, minimum.y, minimum.z), m);
			Vector3<T> lt = transform(Vector3<T>(minimum.x, maximum.y, minimum.z), m);
			Vector3<T> rt = transform(Vector3<T>(maximum.x, maximum.y, minimum.z), m);

			bool flip = (basis.getDeterminant() < T(0));
			if (!f(flip ? HalfSpace<T>(origin, lt, lb) : HalfSpace<T>(origin, lb, lt)))
				return false;
			if (!f(flip ? HalfSpace<T>(origin, rb, rt) : HalfSpace<T>(origin, rt, rb)))
				return false;
			if (!f(flip ? HalfSpace<T>(origin, lb, rb) : HalfSpace<T>(origin, rb, lb)))
				return false;
			if (!f(flip ? HalfSpace<T>(origin, rt, lt) : HalfSpace<T>(origin, lt, rt)))
				return false;
			if (!f(HalfSpace<T>(-basis[2], minimum.z*basis[2] + origin)))
				return false;
			if ((maximum.z < std::numeric_limits<T>::max()) && !f(HalfSpace<T>(basis[2], maximum.z*basis[2] + origin)))
				return false;
		}

		return true;
	}

	template<std::integral U> std::pair<const U*, const U*> getEdges() const noexcept
	{
		static const U edges[24] = { 0, 2, 2, 3, 3, 1, 1, 0, 3, 7, 5, 1, 6, 2, 0, 4, 5, 7, 7, 6, 6, 4, 4, 5 };
		//static const U hsEdges[24] = { 4, 0, 4, 3, 4, 1, 4, 2, 1, 3, 1, 2, 0, 3, 0, 2, 5, 1, 5, 3, 5, 0, 5, 2 };

		if ((projection == Projection::PARALLEL) || (maximum.z < std::numeric_limits<T>::max()))
			return { edges, edges + 24 };
		else
			return { edges, edges + 16 };
	}

	OrientedBox<T> getCircumscribedBox() const noexcept
	{
		if (projection == Projection::PARALLEL)
		{
			Vector3<T> center = (minimum + maximum)*T(0.5);
			Vector3<T> halfDims = (maximum - minimum)*T(0.5);
			return OrientedBox<T>(center*basis + origin, basis, halfDims);
		}
		else if (isSymmetric())
		{
			Vector2<T> baseHalfDims = maximum.xy()*(maximum.z/minimum.z);
			return OrientedBox<T>(origin + ((minimum.z + maximum.z)*T(0.5))*basis[2], basis,
				Vector3<T>(baseHalfDims, (maximum.z - minimum.z)*T(0.5)));
		}
		else
		{
			T depthRatio = maximum.z/minimum.z;
			Vector3<T> lbRatio(minimum.xy()*depthRatio, minimum.z);
			Vector3<T> rtRatio(maximum.xy()*depthRatio, maximum.z);
			Vector3<T> center = (lbRatio + rtRatio)*T(0.5);
			Vector3<T> halfDims = (rtRatio - lbRatio)*T(0.5);
			return OrientedBox<T>(center*basis + origin, basis, halfDims);
		}
	}

	Sphere<T> getCircumscribedSphere() const /*noexcept*/
	{
		if (projection == Projection::PARALLEL)
		{
			Vector3<T> center = (minimum + maximum)*T(0.5);
			Vector3<T> halfDims = (maximum - minimum)*T(0.5);
			return Sphere<T>(center, halfDims.getMagnitude());
		}
		else if (isSymmetric())
		{
			Vector2<T> baseHalfDims = maximum.xy()*(maximum.z/minimum.z);
			T coneRadiusSq = baseHalfDims.getMagnitudeSquared();
			T depthMaxSq = square(maximum.z);
			if (depthMaxSq > coneRadiusSq)
			{
				T sphereRadius = (coneRadiusSq + depthMaxSq)/(T(2)*maximum.z);
				return Sphere<T>(origin + sphereRadius*basis[2], sphereRadius);
			}
			else
			{
				return Sphere<T>(origin + maximum.z*basis[2], std::sqrt(coneRadiusSq));
			}
		}
		else
		{
			// #TODO Get 8 vertices and found minimum bounding sphere
			throw std::runtime_error("ViewVolume::getCircumscribedSphere() : not implemented");
		}
	}

	Cone<T> getCircumscribedCone() const /*noexcept*/
	{
		if (projection == Projection::PARALLEL)
		{
			// #TODO
			throw std::runtime_error("ViewVolume::getCircumscribedCone() : not implemented");
		}
		else if (isSymmetric())
		{
			Vector2<T> baseHalfDims = maximum.xy()*(maximum.z/minimum.z);
			return Cone<T>(origin, basis[2], maximum.z, baseHalfDims.getMagnitude());
		}
		else
		{
			// #TODO
			throw std::runtime_error("ViewVolume::getCircumscribedCone() : not implemented");
		}
	}

	Ray3<T> getRayTo(const Vector2<T>& ndcPoint) const noexcept			// ndcPoint.xy = -1..1
	{
		Vector2<T> p = fromNormalized(ndcPoint);

		if (projection == Projection::PARALLEL)
			return Ray3<T>(Vector3<T>(p)*basis + origin, basis[2]);
		else
			return Ray3<T>(origin, normalize(Vector3<T>(p, minimum.z))*basis);
	}

	LineSegment3<T> getLineSegmentTo(const Vector2<T>& ndcPoint, T depth) const noexcept
	{
		Vector2<T> p = fromNormalized(ndcPoint);

		if (projection == Projection::PARALLEL)
		{
			Vector3<T> start = Vector3<T>(p)*basis + origin;
			return LineSegment3<T>(start, start + basis[2]*depth);
		}
		else
		{
			Vector3<T> dir = normalize(Vector3<T>(p, minimum.z));
			return LineSegment3<T>(origin, origin + (dir*basis)*(depth/dir.z));
		}
	}

	Vector2<T> project(const Vector3<T>& point) const noexcept			// returns normalized coordinates (-1..1)
	{
		Vector3<T> ptLocal = toViewSpace(point);
		if (projection == Projection::PARALLEL)
			return toNormalized(ptLocal.xy());
		else
			return toNormalized(ptLocal.xy()*(minimum.z/ptLocal.z));
	}

	Vector2<T> projectLocal(const Vector3<T>& point) const noexcept		// project view space point
	{
		if (projection == Projection::PARALLEL)
			return toNormalized(point.xy());
		else
			return toNormalized(point.xy()*(minimum.z/point.z));
	}

	Vector3<T> unproject(const Vector2<T>& ndcPoint) const noexcept
	{
		Vector2<T> p = fromNormalized(ndcPoint);
		return fromViewSpace(Vector3<T>(p, minimum.z));
	}

	Vector3<T> unproject(const Vector2<T>& ndcPoint, T depth) const noexcept
	{
		Vector2<T> p = fromNormalized(ndcPoint);
		if (projection == Projection::PARALLEL)
			return fromViewSpace(Vector3<T>(p, depth));
		else
			return fromViewSpace(Vector3<T>(p*(depth/minimum.z), depth));
	}

	//T getProjectedArea(const AxisAlignedBox<T>& box) const noexcept; // #TODO
	//T getProjectedArea(const OrientedBox<T>& box) const noexcept; // #TODO
	
	T getProjectedArea(const Sphere<T>& sphere) const noexcept
	{
		if (projection == Projection::PARALLEL)
		{
			return Constants<T>::PI*sphere.radius*sphere.radius;
		}
		else
		{
			//T r = sphere.radius*minimum.z/sphere.center.z; // for sphere in view space
			T r = sphere.radius*minimum.z/dot(/*normalize*/(basis[2]), sphere.center - origin);
			return Constants<T>::PI*r*r;
		}
	}

	ViewVolume& crop(const Vector2<T>& ndcMinimum, const Vector2<T>& ndcMaximum) noexcept // ndcMinimum.xy, ndcMaximum.xy = -1..1
	{
		Vector2<T> lb = fromNormalized(ndcMinimum);
		Vector2<T> rt = fromNormalized(ndcMaximum);
		minimum = Vector3<T>(lb, minimum.z);
		maximum = Vector3<T>(rt, maximum.z);
		return *this;
	}

	ViewVolume& translate(const Vector3<T>& offset) noexcept 
	{ 
		origin += offset; 
		return *this; 
	}

	bool contains(const Vector3<T>& point) const noexcept
	{
		if (projection == Projection::PARALLEL)
			return asOrientedBox().contains(point);
		//else if (isSymmetric())
		//	return asSymmetricFrustum().contains(point);
		else
			return enumerateHalfSpaces([&point](const HalfSpace<T>& h) { return h.contains(point); });
	}

	bool intersects(const AxisAlignedBox<T>& box) const noexcept
	{
		if (projection == Projection::PARALLEL)
			return asOrientedBox().intersects(box);
		else if (isSymmetric())
			return asSymmetricFrustum().intersects(box);
		else
			return enumerateHalfSpaces([&box](const HalfSpace<T>& h) { return h.intersects(box); });
	}

	bool intersects(const OrientedBox<T>& box) const noexcept
	{
		if (projection == Projection::PARALLEL)
			return asOrientedBox().intersects(box);
		else if (isSymmetric())
			return asSymmetricFrustum().intersects(box);
		else
			return enumerateHalfSpaces([&box](const HalfSpace<T>& h) { return h.intersects(box); });
	}

	bool intersects(const Sphere<T>& sphere) const noexcept
	{
		if (projection == Projection::PARALLEL)
			return asOrientedBox().intersects(sphere);
		else if (isSymmetric())
			return asSymmetricFrustum().intersects(sphere);
		else
			return enumerateHalfSpaces([&sphere](const HalfSpace<T>& h) { return h.intersects(sphere); });
	}

	Vector3<T> origin;
	Matrix3<T> basis;
	Vector3<T> minimum;		// left, bottom, near
	Vector3<T> maximum;		// right, top, far
	Projection projection;

private:
	//Vector2<T> getLeftBottom() const noexcept { return minimum.xy(); }
	//Vector2<T> getRightTop() const noexcept { return maximum.xy(); }
	//const Vector3<T>& getLeftBottomNear() const noexcept { return minimum; }
	//Vector3<T> getLeftBottomFar() const noexcept { return Vector3<T>(minimum.xy(), maximum.z); }
	//Vector3<T> getRightTopNear() const noexcept { return Vector3<T>(maximum.xy(), minimum.z); }
	//const Vector3<T>& getRightTopFar() const noexcept { return maximum; }

	Vector3<T> fromViewSpace(const Vector3<T>& point) const noexcept 
	{ 
		return point*basis + origin; 
	}

	Vector3<T> toViewSpace(const Vector3<T>& point) const noexcept 
	{ 
		return basis*(point - origin)/*(point - origin)*transpose(basis)*/; 
	}

	Vector2<T> fromNormalized(const Vector2<T>& point) const noexcept
	{
		Vector2<T> lb = minimum.xy();
		Vector2<T> rt = maximum.xy();
		return lb + (rt - lb)*(Vector2<T>::ONE + point)*T(0.5);
	}

	Vector2<T> toNormalized(const Vector2<T>& point) const noexcept
	{
		Vector2<T> lb = minimum.xy();
		Vector2<T> rt = maximum.xy();
		return (T(2)*point - (rt + lb))/(rt - lb);
	}
};

} // namespace templates

#if MATHEMATICS_DOUBLE
using ViewVolume = templates::ViewVolume<double>;
using ViewVolumeArg = templates::ViewVolume<double>::ConstArg;
using ViewVolumeResult = templates::ViewVolume<double>::ConstResult;
#else
using ViewVolume = templates::ViewVolume<float>;
using ViewVolumeArg = templates::ViewVolume<float>::ConstArg;
using ViewVolumeResult = templates::ViewVolume<float>::ConstResult;
#endif

} // namespace graphics

namespace std {

template<typename T>
struct hash;

template<typename T>
struct hash<::graphics::templates::ViewVolume<T>>
{
	size_t operator()(const ::graphics::templates::ViewVolume<T>& volume) const noexcept
	{
		hash<typename ::mathematics::templates::Vector3<T>> hasher;
		size_t seed = hasher(volume.origin) + 0x9e3779b9;
		seed ^= hash<typename ::mathematics::templates::Matrix3<U>>()(volume.basis) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(volume.minimum) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(volume.maximum) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash<int/*Projection*/>()((int)volume.projection) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

/*
 *	Name: ConstraintRow
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
//#include <functional>
#include <cstddef>
#include <Mathematics/Constants.hpp>
#include <Mathematics/Algebra/Vector3.hpp>
#include <Mathematics/Interval.hpp>

namespace physics {
namespace templates {

using mathematics::Constants;
using mathematics::templates::Vector3;
using mathematics::templates::Interval;

template<std::floating_point T>
struct ConstraintRow
{
	bool operator==(const ConstraintRow& constraintRow) const noexcept
	{
		return (linearAxis == constraintRow.linearAxis) && (angularAxis == constraintRow.angularAxis) &&
			(referenceBodyLinearAxis == constraintRow.referenceBodyLinearAxis) &&
			(referenceBodyAngularAxis == constraintRow.referenceBodyAngularAxis) &&
			(constraintError == constraintRow.constraintError) && (limits == constraintRow.limits);
	}

	bool operator!=(const ConstraintRow& constraintRow) const noexcept { return !(*this == constraintRow); }

	template<typename A> void serialize(A& ar)
	{
		ar(linearAxis, angularAxis, referenceBodyLinearAxis, referenceBodyAngularAxis, constraintError, limits);
	}

	// Axes
	const Vector3<T>& getLinearAxis() const noexcept { return linearAxis; }
	void setLinearAxis(const Vector3<T>& axis) noexcept { linearAxis = axis; }
	const Vector3<T>& getAngularAxis() const noexcept { return angularAxis; }
	void setAngularAxis(const Vector3<T>& axis) noexcept { angularAxis = axis; }
	const Vector3<T>& getReferenceBodyLinearAxis() const noexcept { return referenceBodyLinearAxis; }
	void setReferenceBodyLinearAxis(const Vector3<T>& axis) noexcept { referenceBodyLinearAxis = axis; }
	const Vector3<T>& getReferenceBodyAngularAxis() const noexcept { return referenceBodyAngularAxis; }
	void setReferenceBodyAngularAxis(const Vector3<T>& axis) noexcept { referenceBodyAngularAxis = axis; }

	// Constraint error
	T getConstraintError() const noexcept { return constraintError; }
	void setConstraintError(T constraintError) noexcept { this->constraintError = constraintError; }

	// Limits
	const Interval<T>& getLimits() const noexcept { return limits; }
	void setLimits(const Interval<T>& limits) noexcept { this->limits = limits; }
	T getLowerLimit() const noexcept { return limits.minimum; }
	void setLowerLimit(T lowerLimit) noexcept { limits.minimum = lowerLimit; }
	T getUpperLimit() const noexcept { return limits.maximum; }
	void setUpperLimit(T upperLimit) noexcept { limits.maximum = upperLimit; }

	Vector3<T> linearAxis;
	Vector3<T> angularAxis;
	Vector3<T> referenceBodyLinearAxis;
	Vector3<T> referenceBodyAngularAxis;
	T constraintError = T();
	Interval<T> limits = { Constants<T>::MINUS_INF, Constants<T>::INF };
};

} // namespace templates

#if PHYSICS_DOUBLE_PRECISION
using ConstraintRow = templates::ConstraintRow<double>;
#else
using ConstraintRow = templates::ConstraintRow<float>;
#endif

} // namespace physics

namespace std {

//template<typename T>
//struct hash;
//
//template<typename T>
//struct hash<::physics::templates::ConstraintRow<T>> // #TODO
//{
//	size_t operator()(const ::physics::templates::ConstraintRow<T>& constraintRow) const noexcept
//	{
//	}
//};

} // namespace std

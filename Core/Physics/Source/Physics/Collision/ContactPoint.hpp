/*
 *	Name: ContactPoint
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
#include <algorithm>
//#include <functional>
#include <array>
#include <cstddef>
#include <Mathematics/Constants.hpp>
#include <Mathematics/Algebra/Vector3.hpp>
#include "ContactPointFlags.hpp"

namespace physics {
namespace templates {

using mathematics::Constants;
using mathematics::templates::Vector3;

template<std::floating_point T>
struct ContactPoint
{
	bool operator==(const ContactPoint& contactPoint) const noexcept
	{
		return (worldPositions[0] == contactPoint.worldPositions[0]) && (worldPositions[1] == contactPoint.worldPositions[1]) &&
			(localPositions[0] == contactPoint.localPositions[0]) && (localPositions[1] == contactPoint.localPositions[1]) &&
			(worldNormal == contactPoint.worldNormal) && (distance == contactPoint.distance) &&
			(shapeIndices[0] == contactPoint.shapeIndices[0]) && (shapeIndices[1] == contactPoint.shapeIndices[1]) &&
			/*(materialIndices[0] == contactPoint.materialIndices[0]) && (materialIndices[1] == contactPoint.materialIndices[1]) &&*/
			(frictionCoefficient == contactPoint.frictionCoefficient) && (frictionDirections[0] == contactPoint.frictionDirections[0]) &&
			(frictionDirections[1] == contactPoint.frictionDirections[1]) && (frictionCfm == contactPoint.frictionCfm) &&
			(rollingResistanceCoefficient == contactPoint.rollingResistanceCoefficient) &&
			(spinningResistanceCoefficient == contactPoint.spinningResistanceCoefficient) &&
			(restitutionCoefficient == contactPoint.restitutionCoefficient) && (contactStiffness == contactPoint.contactStiffness) &&
			(contactDamping == contactPoint.contactDamping) && (contactErp == contactPoint.contactErp) &&
			(contactCfm == contactPoint.contactCfm) && (appliedImpulse == contactPoint.appliedImpulse) &&
			(lifetime == contactPoint.lifetime) && (lifetimeInTicks == contactPoint.lifetimeInTicks) && (flags == contactPoint.flags);
	}

	bool operator!=(const ContactPoint& contactPoint) const noexcept { return !(*this == contactPoint); }

	// Location
	const Vector3<T>& getWorldPosition() const noexcept { return worldPositions[1]; }
	const Vector3<T>& getWorldPosition(int index) const noexcept { return ((unsigned int)index < 2u) ? worldPositions[index] : Vector3<T>::ZERO; }
	const Vector3<T>& getLocalPosition(int index) const noexcept { return ((unsigned int)index < 2u) ? localPositions[index] : Vector3<T>::ZERO; }
	const Vector3<T>& getWorldNormal() const noexcept { return worldNormal; }
	T getDistance() const noexcept { return distance; }
	T getPenetrationDepth() const noexcept { return std::max(-distance, T(0))/*penetrationDepth*/; }

	// Shape & material
	std::ptrdiff_t getShapeIndex(int index) const noexcept { return (std::ptrdiff_t)(((unsigned int)index < 2u) ? shapeIndices[index] : -1); }
	//std::ptrdiff_t getMaterialIndex(int index) const noexcept { return (std::ptrdiff_t)(((unsigned int)index < 2u) ? materialIndices[index] : -1); } // #TODO

	// Friction
	T getFrictionCoefficient() const noexcept { return frictionCoefficient; }
	void setFrictionCoefficient(T friction) noexcept { frictionCoefficient = friction; }
	const Vector3<T>& getFrictionDirection1() const noexcept { return frictionDirections[0]; }
	void setFrictionDirection1(const Vector3<T>& direction) noexcept { frictionDirections[0] = direction; }
	const Vector3<T>& getFrictionDirection2() const noexcept { return frictionDirections[1]; }
	void setFrictionDirection2(const Vector3<T>& direction) noexcept { frictionDirections[1] = direction; }
	T getFrictionCfm() const noexcept { return frictionCfm; }
	void setFrictionCfm(T cfm) noexcept { frictionCfm = cfm; }
	T getRollingResistanceCoefficient() const noexcept { return rollingResistanceCoefficient; }
	void setRollingResistanceCoefficient(T resistance) noexcept { rollingResistanceCoefficient = resistance; }
	T getSpinningResistanceCoefficient() const noexcept { return spinningResistanceCoefficient; }
	void setSpinningResistanceCoefficient(T resistance) noexcept { spinningResistanceCoefficient = resistance; }

	// Restitution
	T getRestitutionCoefficient() const noexcept { return restitutionCoefficient; }
	void setRestitutionCoefficient(T restitution) noexcept { restitutionCoefficient = restitution; }

	// Soft contact
	T getContactStiffness() const noexcept { return contactStiffness; }
	void setContactStiffness(T stiffness) noexcept { contactStiffness = stiffness; }
	T getContactDamping() const noexcept { return contactDamping; }
	void setContactDamping(T damping) noexcept { contactDamping = damping; }
	T getContactErp() const noexcept { return contactErp; }
	void setContactErp(T erp) noexcept { contactErp = erp; }
	T getContactCfm() const noexcept { return contactCfm; }
	void setContactCfm(T cfm) noexcept { contactCfm = cfm; }

	// Impulse
	T getAppliedImpulse() const noexcept { return appliedImpulse; }

	// Lifetime
	T getLifetime() const noexcept { return lifetime; }
	int getLifetimeInTicks() const noexcept { return lifetimeInTicks; }

	// Flags
	ContactPointFlags getContactPointFlags() const noexcept { return flags; }
	void setContactPointFlags(ContactPointFlags flags) noexcept { this->flags = flags; }

	std::array<Vector3<T>, 2> worldPositions;
	std::array<Vector3<T>, 2> localPositions;
	Vector3<T> worldNormal;
	T distance/*penetrationDepth*/ = Constants<T>::INF;
	std::array<int, 2> shapeIndices = { -1, -1 };
	//std::array<int, 2> materialIndices = { -1, -1 }; // #TODO
	T frictionCoefficient = T();
	std::array<Vector3<T>, 2> /*lateral*/frictionDirections[2];
	T frictionCfm = T();
	T rollingResistanceCoefficient = T();
	T spinningResistanceCoefficient = T();
	T restitutionCoefficient = T();
	T contactStiffness = Constants<T>::INF;
	T contactDamping = T();
	T contactErp = T();
	T contactCfm = T();
	T appliedImpulse = T();
	T lifetime = T();
	int	lifetimeInTicks = 0;
	ContactPointFlags flags = {};
};

} // namespace templates

#if PHYSICS_DOUBLE_PRECISION
using ContactPoint = templates::ContactPoint<double>;
#else
using ContactPoint = templates::ContactPoint<float>;
#endif

} // namespace physics

namespace std {

//template<typename T>
//struct hash;
//
//template<typename T>
//struct hash<::physics::templates::ContactPoint<T>> // #TODO
//{
//	size_t operator()(const ::physics::templates::ContactPoint<T>& contactPoint) const noexcept
//	{
//	}
//};

} // namespace std

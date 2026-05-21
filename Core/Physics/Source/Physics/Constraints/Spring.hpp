/*
 *	Name: Spring
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
//#include <functional>
#include <cstddef>

namespace physics {
namespace templates {

template<std::floating_point T>
struct Spring
{
	Spring() noexcept : equilibriumPoint(), stiffness(), compressionDamping(), relaxationDamping(), enabled() {}
	
	Spring(T stiffness, T damping) noexcept :
		equilibriumPoint(),
		stiffness(stiffness),
		compressionDamping(damping),
		relaxationDamping(damping),
		enabled(true)
	{
	}
	
	Spring(T stiffness, T compressionDamping, T relaxationDamping) noexcept :
		equilibriumPoint(),
		stiffness(stiffness),
		compressionDamping(compressionDamping),
		relaxationDamping(relaxationDamping),
		enabled(true)
	{
	}

	bool operator==(const Spring& spring) const noexcept
	{
		return (equilibriumPoint == spring.equilibriumPoint) && (stiffness == spring.stiffness) &&
			(compressionDamping == spring.compressionDamping) && (relaxationDamping == spring.relaxationDamping) &&
			(enabled == spring.enabled);
	}

	bool operator!=(const Spring& spring) const noexcept { return !(*this == spring); }

	template<typename A> void serialize(A& ar) { ar(equilibriumPoint, stiffness, compressionDamping, relaxationDamping, enabled); }

	// Enable/disable
	bool isEnabled() const noexcept { return enabled; }
	void setEnabled(bool enabled) noexcept { this->enabled = enabled; }
	void enable() noexcept { enabled = true; }
	void disable() noexcept { enabled = false; }

	// Properties
	T getEquilibriumPoint() const noexcept { return equilibriumPoint; }
	void setEquilibriumPoint(T point) noexcept { equilibriumPoint = point; }
	T getStiffness() const noexcept { return stiffness; }
	void setStiffness(T stiffness) noexcept { this->stiffness = stiffness; }
	T getCompressionDamping() const noexcept { return compressionDamping; }
	void setCompressionDamping(T damping) noexcept { compressionDamping = damping; }
	T getRelaxationDamping() const noexcept { return relaxationDamping; }
	void setRelaxationDamping(T damping) noexcept { relaxationDamping = damping; }

	static const Spring DISABLED;

	T equilibriumPoint;
	T stiffness;
	T compressionDamping;
	T relaxationDamping;
	bool enabled;
};

template<std::floating_point T> const Spring<T> Spring<T>::DISABLED{};

} // namespace templates

#if PHYSICS_DOUBLE_PRECISION
using Spring = templates::Spring<double>;
#else
using Spring = templates::Spring<float>;
#endif

} // namespace physics

namespace std {

//template<typename T>
//struct hash;
//
//template<typename T>
//struct hash<::physics::templates::Spring<T>> // #TODO
//{
//	size_t operator()(const ::physics::templates::Spring<T>& spring) const noexcept
//	{
//	}
//};

} // namespace std

/*
 *	Name: Motor
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
//#include <functional>
#include <cstddef>
#include "MotorMode.hpp"

namespace physics {
namespace templates {

template<std::floating_point T>
struct Motor
{
	Motor() noexcept : destination(), velocity(), maxForce(), mode(), enabled() {}
	Motor(T velocity, T maxForce) noexcept : destination(), velocity(velocity), maxForce(maxForce), mode(), enabled(true) {}

	bool operator==(const Motor& motor) const noexcept 
	{
		return (destination == motor.destination) && (velocity == motor.velocity) &&
			(maxForce == motor.maxForce) && (mode == motor.mode) && (enabled == motor.enabled);
	}

	bool operator!=(const Motor& motor) const noexcept { return !(*this == motor); }

	template<typename A> void serialize(A& ar) { ar(destination, velocity, maxForce, mode, enabled); }

	// Enable/disable
	bool isEnabled() const noexcept { return enabled; }
	void setEnabled(bool enabled) noexcept { this->enabled = enabled; }
	void enable() noexcept { enabled = true; }
	void disable() noexcept { enabled = false; }

	// Properties
	MotorMode getMotorMode() const noexcept { return mode; }
	void setMotorMode(MotorMode mode) noexcept { this->mode = mode; }
	T getDestination() const noexcept { return destination; }
	void setDestination(T destination) noexcept { this->destination = destination; }
	T getVelocity() const noexcept { return velocity; }
	void setVelocity(T velocity) noexcept { this->velocity = velocity; }
	T getMaxForce() const noexcept { return maxForce; }
	void setMaxForce(T force) noexcept { maxForce = force; }

	static const Motor DISABLED;

	T destination;
	T velocity;
	T maxForce;
	MotorMode mode;
	bool enabled;
};

template<std::floating_point T> const Motor<T> Motor<T>::DISABLED{};

} // namespace templates

#if PHYSICS_DOUBLE_PRECISION
using Motor = templates::Motor<double>;
#else
using Motor = templates::Motor<float>;
#endif

} // namespace physics

namespace std {

//template<typename T>
//struct hash;
//
//template<typename T>
//struct hash<::physics::templates::Motor<T>> // #TODO
//{
//	size_t operator()(const ::physics::templates::Motor<T>& motor) const noexcept
//	{
//	}
//};

} // namespace std

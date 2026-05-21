/*
 *	Name: Limit
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
//#include <functional>
#include <cstddef>
#include <Mathematics/Interval.hpp>

namespace physics {
namespace templates {

using mathematics::templates::Interval;

template<std::floating_point T>
struct Limit
{
	Limit() noexcept : range(T()), softness(), bounciness(), enabled() {}
	explicit Limit(T value) noexcept : range(value), softness(), bounciness(), enabled(true) {}
	Limit(T minimum, T maximum) noexcept : range(minimum, maximum), softness(), bounciness(), enabled(true) {}
	Limit(const Interval<T>& range) noexcept : range(range), softness(), bounciness(), enabled(true) {}

	bool operator==(const Limit& limit) const noexcept
	{
		return (range == limit.range) && (softness == limit.softness) && (bounciness == limit.bounciness) && (enabled == limit.enabled);
	}

	bool operator!=(const Limit& limit) const noexcept { return !(*this == limit); }

	template<typename A> void serialize(A& ar) { ar(range, softness, bounciness, enabled); }

	// Enable/disable
	bool isEnabled() const noexcept { return enabled; }
	void setEnabled(bool enabled) noexcept { this->enabled = enabled; }
	void enable() noexcept { enabled = true; }
	void disable() noexcept { enabled = false; }

	// Properties
	const Interval<T>& getRange() const noexcept { return range; }
	void setRange(const Interval<T>& range) noexcept { this->range = range; }
	T getMinimum() const noexcept { return range.minimum; }
	void setMinimum(T minimum) noexcept { range.minimum = minimum; }
	T getMaximum() const noexcept { return range.maximum; }
	void setMaximum(T maximum) noexcept { range.maximum = maximum; }
	//T getSpan() const noexcept { return range.getLength(); }
	//void setSpan(T span) noexcept { range.set(-span*T(0.5), span*T(0.5)); }
	T getSoftness() const noexcept { return softness; }
	void setSoftness(T softness) noexcept { this->softness = softness; }
	T getBounciness() const noexcept { return bounciness; }
	void setBounciness(T bounciness) noexcept { this->bounciness = bounciness; }

	static const Limit DISABLED;
	static const Limit ZERO;

	Interval<T> range;
	T softness;		// constraint becomes more "soft" when this factor is close to 1
	T bounciness;	// bounce on limits: 1 means full bounce, 0 means no bounce
	bool enabled;
};

template<std::floating_point T> const Limit<T> Limit<T>::DISABLED{};
template<std::floating_point T> const Limit<T> Limit<T>::ZERO{ T(0) };

} // namespace templates

#if PHYSICS_DOUBLE_PRECISION
using Limit = templates::Limit<double>;
#else
using Limit = templates::Limit<float>;
#endif

} // namespace physics

namespace std {

//template<typename T>
//struct hash;
//
//template<typename T>
//struct hash<::physics::templates::Limit<T>> // #TODO
//{
//	size_t operator()(const ::physics::templates::Limit<T>& limit) const noexcept
//	{
//	}
//};

} // namespace std

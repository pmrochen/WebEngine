/*
 *	Name: Clonable
 *	Author: Pawel Mrochen
 */

#pragma once

#include <algorithm>
#include <iterator>
#include "Concepts.hpp"

namespace common::clonable {

template<typename T>
	requires (PointerLike<T> || Container<T>)
inline T clone(const T& x);

template<PointerLike T>
inline T clone(const T& x)
{
	if (x)
		return { x->clone() };
	else
		return {};
}

template<Container T>
	//requires Clonable<typename T::value_type>
inline T clone(const T& x)
{
	T copy;
	copy.reserve(x.size()); // #FIXME Only supporting containers
	std::transform(x.begin(), x.end(), std::back_inserter(copy),
        [](const auto& e) /*-> T::value_type* */ { return e ? e->clone() : {}; });
	return copy;
}

} // namespace common::clonable

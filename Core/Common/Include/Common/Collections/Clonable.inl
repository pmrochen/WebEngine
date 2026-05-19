/*
 *	Name: Clonable
 *	Author: Pawel Mrochen
 */

#pragma once

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
	T copy(x);
	for (auto& v : copy)
		v = clone(v);
	return copy;
}

} // namespace common::clonable

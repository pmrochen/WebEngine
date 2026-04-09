/*
 *	Name: Hash
 *	Author: Pawel Mrochen
 */

#pragma once

#include <functional>
#include <iterator>
#include <cstddef>

namespace graphics::hash {

template<typename I>
inline std::size_t range(I first, I last) noexcept
{
	std::hash<typename std::iterator_traits<I>::value_type> hasher;
	std::size_t seed = 0;
	for (; first != last; ++first)
		seed ^= hasher(*first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

} // namespace graphics::hash

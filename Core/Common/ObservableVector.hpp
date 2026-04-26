/*
 *	Name: ObservableVector
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <vector>
#include "ObservableSequenceCollection.hpp"

namespace common {

template<typename T, typename Alloc = std::allocator<T>>
using ObservableVector = ObservableSequenceContainer<std::vector>;

} // namespace common

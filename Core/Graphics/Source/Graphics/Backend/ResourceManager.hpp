/*
 *	Name: ResourceManager
 *	Author: Pawel Mrochen
 */

#pragma once

#include <oup/observable_unique_ptr.hpp>

namespace graphics {

class ResourceManager : public oup::enable_observer_from_this_unique<ResourceManager>
{
};

} // namespace graphics

/*
 *	Name: Renderer
 *	Author: Pawel Mrochen
 */

#pragma once

#include <oup/observable_unique_ptr.hpp>

namespace graphics {

class Renderer : public oup::enable_observer_from_this_unique<Renderer>
{
};

} // namespace graphics

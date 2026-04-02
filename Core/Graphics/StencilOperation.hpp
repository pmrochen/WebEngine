/*
 *	Name: StencilOperation
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class StencilOperation
{
    KEEP,
    ZERO,
    REPLACE,
    INCREMENT_CLAMP,
    INCREMENT_WRAP,
    DECREMENT_CLAMP,
    DECREMENT_WRAP,
    INVERT
};

} // namespace graphics

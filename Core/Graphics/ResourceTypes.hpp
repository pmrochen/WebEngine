/*
 *	Name: ResourceTypes
 *	Author: Pawel Mrochen
 */

#pragma once

#include <bitwise_enum.hpp>

namespace graphics {

enum class ResourceTypes
{
	NONE = 0,
	STRUCTURED_BUFFER = 0x01,
	CONSTANT_BUFFER = 0x02,
	VERTEX_BUFFER = 0x04,
	INDEX_BUFFER = 0x08,
	TEXTURE = 0x10,
	SAMPLER = 0x20,
	MUTABLE_IMAGE = 0x40,
	SHADER = 0x80,
	ALL = 0xFF
};

ENABLE_ENUM_BITWISE_OPERATORS(ResourceTypes);

} // namespace graphics

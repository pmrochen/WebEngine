/*
 *	Name: ShaderStage
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class ShaderStage
{
	UNSPECIFIED,
	VERTEX,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY,
	FRAGMENT,
	COMPUTE
};

} // namespace graphics

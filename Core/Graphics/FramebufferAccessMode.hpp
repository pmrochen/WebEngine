/*
 *	Name: FramebufferAccessMode
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class FramebufferAccessMode
{
	//NONE,
	DISCARD,
	PRESERVE,
	//RESOLVE,
	CLEAR
};

} // namespace graphics

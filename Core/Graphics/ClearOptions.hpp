/*
 *	Name: ClearOptions
 *	Author: Pawel Mrochen
 */

#pragma once

#include <stdexcept>
#include <algorithm>
#include <Color4.hpp>
#include "AttributeMask.hpp"

namespace graphics {

using imaging::Color4;

struct ClearOptions
{
	static constexpr unsigned int MAX_COLOR_COUNT = 8u;
	
	ClearOptions() noexcept : 
		colors{ Color4::UNIT_A }, 
		depthValue(1.f), 
		stencilValue(), 
		attributeMask()
	{
	}

	ClearOptions(ColorMask colorMask, const Color4& color, bool depthMask = true, float depthValue = 1.f,
		bool stencilMask = true, int stencilValue = 0) noexcept :
		colors{ color },
		depthValue(depthValue),
		stencilValue(stencilValue),
		attributeMask((AttributeMask)(colorMask & ColorMask::ALL) | (depthMask ? AttributeMask::DEPTH : AttributeMask::NONE) |
			(stencilMask ? AttributeMask::STENCIL : AttributeMask::NONE))
	{
	}

	bool operator==(const ClearOptions& options) const noexcept
	{
		return std::equal(&colors[0], &colors[MAX_COLOR_COUNT], &options.colors[0]) && (depthValue == options.depthValue) &&
			(stencilValue == options.stencilValue) && (attributeMask == options.attributeMask);
	}

	bool operator!=(const ClearOptions& options) const noexcept 
	{ 
		return !(*this == options); 
	}

	const Color4& getColor() const noexcept 
	{ 
		return colors[0]; 
	}

	void setColor(const Color4& color) noexcept 
	{ 
		colors[0] = color; 
	}

	const Color4& getColor(int index) const noexcept 
	{ 
		return (((unsigned int)index < MAX_COLOR_COUNT) ? colors[index] : Color4::ZERO); 
	}

	void setColor(int index, const Color4& color) // throw (std::out_of_range)
	{
		if ((unsigned int)index >= MAX_COLOR_COUNT)
			throw std::out_of_range("ClearOptions::setColor() : index");

		colors[index] = color;
	}

	float getDepthValue() const noexcept { return depthValue; }
	void setDepthValue(float depthValue) noexcept { this->depthValue = depthValue; }
	int getStencilValue() const noexcept { return stencilValue; }
	void setStencilValue(int stencilValue) noexcept { this->stencilValue = stencilValue; }
	AttributeMask getAttributeMask() const noexcept { return attributeMask; }
	void setAttributeMask(AttributeMask attributeMask) noexcept { this->attributeMask = attributeMask; }

	Color4 colors[MAX_COLOR_COUNT];
	float depthValue;
	int stencilValue;
	AttributeMask attributeMask;
};

} // namespace graphics

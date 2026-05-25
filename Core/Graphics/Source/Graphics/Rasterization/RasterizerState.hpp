/*
 *	Name: RasterizerState
 *	Author: Pawel Mrochen
 */

#pragma once

#include <algorithm>
#include <functional>
#include <cstddef>
#include <cstdint>
#include "ComparisonFunction.hpp"
#include "BlendFunction.hpp"
#include "BlendEquation.hpp"
#include "RasterizerFlags.hpp"

namespace graphics {

struct RasterizerState
{
	bool operator==(const RasterizerState& state) const noexcept
	{
		// return (srcBlendFunction == state.srcBlendFunction) && (destBlendFunction == state.destBlendFunction) &&
		// 	(blendEquation == state.blendEquation) && (depthFunction == state.depthFunction) && 
		// 	(constantDepthBias == state.constantDepthBias) && (slopeScaleDepthBias == state.slopeScaleDepthBias) && 
		// 	(rasterizerFlags == state.rasterizerFlags);
		static_assert((sizeof(RasterizerState) & (sizeof(std::size_t) - 1)) == 0);
		return std::equal((const std::size_t*)this, (const std::size_t*)this + sizeof(RasterizerState)/sizeof(std::size_t),
			(const std::size_t*)&state);
	}

	bool operator!=(const RasterizerState& state) const noexcept 
	{ 
		return !(*this == state);
	}

	template<typename A> void serialize(A& ar) 
	{ 
		ar(srcBlendFunction, destBlendFunction, blendEquation, depthFunction, constantDepthBias, slopeScaleDepthBias, 
			rasterizerFlags); 
	}

	static const RasterizerState& getDefault()
	{
		static const RasterizerState state;
		return state;
	}

	// Properties
	BlendFunction getSourceBlendFunction() const noexcept { return (BlendFunction)srcBlendFunction; }
	void setSourceBlendFunction(BlendFunction function) noexcept { srcBlendFunction = (std::uint8_t)function; }
	BlendFunction getDestinationBlendFunction() const noexcept { return (BlendFunction)destBlendFunction; }
	void setDestinationBlendFunction(BlendFunction function) noexcept { destBlendFunction = (std::uint8_t)function; }
	BlendEquation getBlendEquation() const noexcept { return (BlendEquation)blendEquation; }
	void setBlendEquation(BlendEquation equation) noexcept { blendEquation = (std::uint8_t)equation; }
	ComparisonFunction getDepthComparisonFunction() const noexcept { return (ComparisonFunction)depthFunction; }
	void setDepthComparisonFunction(ComparisonFunction function) noexcept { depthFunction = (std::uint8_t)function; }
	float getConstantDepthBias() const noexcept { return constantDepthBias; }
	void setConstantDepthBias(float depthBias) noexcept { constantDepthBias = depthBias; }
	float getSlopeScaleDepthBias() const noexcept { return slopeScaleDepthBias; }
	void setSlopeScaleDepthBias(float depthBias) noexcept { slopeScaleDepthBias = depthBias; }
	//float getDepthBoundsTestMinimum() const noexcept { return depthBoundsTestMin; }
	//void setDepthBoundsTestMinimum(float depthMin) noexcept { depthBoundsTestMin = depthMin; }
	//float getDepthBoundsTestMaximum() const noexcept { return depthBoundsTestMax; }
	//void setDepthBoundsTestMaximum(float depthMax) noexcept { depthBoundsTestMax = depthMax; }
	//float getCoverage() const noexcept { return coverage; }
	//void setCoverage(float coverage) noexcept { this->coverage = coverage; }
	RasterizerFlags getRasterizerFlags() const noexcept { return rasterizerFlags; }
	void setRasterizerFlags(RasterizerFlags flags) noexcept { rasterizerFlags = flags; }

	std::uint8_t/*BlendFunction*/ srcBlendFunction = (std::uint8_t)BlendFunction::ONE;
	std::uint8_t/*BlendFunction*/ destBlendFunction = (std::uint8_t)BlendFunction::ZERO;
	std::uint8_t/*BlendEquation*/ blendEquation = (std::uint8_t)BlendEquation::ADD;
	std::uint8_t/*ComparisonFunction*/ depthFunction = (std::uint8_t)ComparisonFunction::LESS;
	float constantDepthBias = {};
	float slopeScaleDepthBias = {};
	//float depthBoundsTestMin;
	//float depthBoundsTestMax;
	//float coverage = 1.f;
	RasterizerFlags rasterizerFlags = RasterizerFlags::DEFAULT;
};

} // namespace graphics

#include <Common/Collections/Hash.inl>

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::RasterizerState>
{
	size_t operator()(const ::graphics::RasterizerState& state) const noexcept
	{
		// hash<float> hasher;
		// size_t seed = hash<ComparisonFunction>()(state.depthFunction) + 0x9e3779b9;
		// seed ^= hasher(state.constantDepthBias) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		// seed ^= hasher(state.slopeScaleDepthBias) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		// seed ^= hash<RasterizerFlags>()(state.rasterizerFlags) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		// return seed;
		static_assert((sizeof(::graphics::RasterizerState) & (sizeof(size_t) - 1)) == 0);
		return ::common::hash::range((const size_t*)&state, 
			(const size_t*)&state + sizeof(::graphics::RasterizerState)/sizeof(size_t));
	}
};

} // namespace std

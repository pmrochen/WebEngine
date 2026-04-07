/*
 *	Name: RasterizerState
 *	Author: Pawel Mrochen
 */

#pragma once

#include <functional>
#include <cstddef>
#include "ComparisonFunction.hpp"
#include "RasterizerFlags.hpp"

namespace graphics {

struct RasterizerState
{
	bool operator==(const RasterizerState& state) const noexcept
	{
		return (depthFunction == state.depthFunction) && (constantDepthBias == state.constantDepthBias) &&
			(slopeScaleDepthBias == state.slopeScaleDepthBias) && (rasterizerFlags == state.rasterizerFlags);
	}

	bool operator!=(const RasterizerState& state) const noexcept { return !(*this == state); }

	template<typename A> void serialize(A& ar) { ar(depthFunction, constantDepthBias, slopeScaleDepthBias, rasterizerFlags); }

	// Properties
	ComparisonFunction getDepthComparisonFunction() const noexcept { return depthFunction; }
	void setDepthComparisonFunction(ComparisonFunction function) noexcept { depthFunction = function; }
	float getConstantDepthBias() const noexcept { return constantDepthBias; }
	void setConstantDepthBias(float depthBias) noexcept { constantDepthBias = depthBias; }
	float getSlopeScaleDepthBias() const noexcept { return slopeScaleDepthBias; }
	void setSlopeScaleDepthBias(float depthBias) noexcept { slopeScaleDepthBias = depthBias; }
	//float getDepthBoundsTestMinimum() const noexcept { return depthBoundsTestMin; }
	//void setDepthBoundsTestMinimum(float depthMin) noexcept { depthBoundsTestMin = depthMin; }
	//float getDepthBoundsTestMaximum() const noexcept { return depthBoundsTestMax; }
	//void setDepthBoundsTestMaximum(float depthMax) noexcept { depthBoundsTestMax = depthMax; }
	//int getMaxAnisotropy() const noexcept { return maxAnisotropy; }
	//void setMaxAnisotropy(int maxAnisotropy) noexcept { this->maxAnisotropy = maxAnisotropy; }
	//float getMipmapLodBias() const noexcept { return mipLodBias; }
	//void setMipmapLodBias(float mipLodBias) noexcept { this->mipLodBias = mipLodBias; }
	RasterizerFlags getRasterizerFlags() const noexcept { return rasterizerFlags; }
	void setRasterizerFlags(RasterizerFlags flags) noexcept { rasterizerFlags = flags; }

	ComparisonFunction depthFunction = ComparisonFunction::LESS;
	float constantDepthBias = {};
	float slopeScaleDepthBias = {};
	//float depthBoundsTestMin;
	//float depthBoundsTestMax;
	//int maxAnisotropy = 16;
	//float mipLodBias = {};
	RasterizerFlags rasterizerFlags = RasterizerFlags::DEFAULT;
};

} // namespace graphics

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::RasterizerState>
{
	size_t operator()(const ::graphics::RasterizerState& state) const noexcept
	{
		hash<float> hasher;
		size_t seed = hash<ComparisonFunction>()(state.depthFunction) + 0x9e3779b9;
		seed ^= hasher(state.constantDepthBias) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(state.slopeScaleDepthBias) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash<RasterizerFlags>()(state.rasterizerFlags) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

} // namespace std

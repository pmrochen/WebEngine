/*
 *	Name: SamplerState
 *	Author: Pawel Mrochen
 */

#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <cstddef>
#include "ComparisonFunction.hpp"
#include "TextureWrapMode.hpp"
#include "TextureFilter.hpp"

namespace graphics {
namespace detail {

template<typename I>
inline std::size_t hashRange(I first, I last) noexcept
{
	std::hash<typename std::iterator_traits<I>::value_type> hasher;
	std::size_t seed = 0;
	for (; first != last; ++first)
		seed ^= hasher(*first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

} // namespace detail

struct SamplerState
{
	SamplerState() noexcept;
	explicit SamplerState(TextureWrapMode wrapMode) noexcept;
	SamplerState(TextureWrapMode wrapMode, TextureFilter filter) noexcept;
	SamplerState(TextureWrapMode wrapMode, TextureFilter filter, int maxAnisotropy) noexcept;

	bool operator==(const SamplerState& state) const noexcept
	{
		static_assert((sizeof(SamplerState) & (sizeof(std::size_t) - 1)) == 0);
		return std::equal((const std::size_t*)this, (const std::size_t*)this + sizeof(SamplerState)/sizeof(std::size_t),
			(const std::size_t*)&state);
	}

	bool operator!=(const SamplerState& state) const noexcept
	{
		return !(*this == state);
	}

	template<typename A> void serialize(A& ar)
	{
		ar(wrapModes, filters, maxAnisotropy, depthFunctionSrgbDecoding, minMipLevel, maxMipLevel, mipLodBias);
	}

	// Default
	static const SamplerState& getDefault() noexcept;

	// Wrap mode
	TextureWrapMode getWrapModeU() const noexcept { return (TextureWrapMode)(wrapModes & 3); }
	TextureWrapMode getWrapModeV() const noexcept { return (TextureWrapMode)((wrapModes >> 2) & 3); }
	TextureWrapMode getWrapModeW() const noexcept { return (TextureWrapMode)((wrapModes >> 4) & 3); }
	void setWrapModeU(TextureWrapMode wrapU) noexcept { wrapModes = (wrapModes & ~3) | ((int)wrapU & 3); }
	void setWrapModeV(TextureWrapMode wrapV) noexcept { wrapModes = (wrapModes & ~(3 << 2)) | (((int)wrapV & 3) << 2); }
	void setWrapModeW(TextureWrapMode wrapW) noexcept { wrapModes = (wrapModes & ~(3 << 4)) | (((int)wrapW & 3) << 4); }
	void setWrapModes(TextureWrapMode wrapMode) noexcept { setWrapModes(wrapMode, wrapMode, wrapMode); }
	void setWrapModes(TextureWrapMode wrapU, TextureWrapMode wrapVW) noexcept { setWrapModes(wrapU, wrapVW, wrapVW); }
	void setWrapModes(TextureWrapMode wrapU, TextureWrapMode wrapV, TextureWrapMode wrapW) noexcept;

	// Texture filter
	TextureFilter getMinificationFilter() const noexcept { return (TextureFilter)(filters & 3); }
	TextureFilter getMagnificationFilter() const noexcept { return (TextureFilter)((filters >> 2) & 3); }
	TextureFilter getMipmapFilter() const noexcept { return (TextureFilter)((filters >> 4) & 3); }
	void setMinificationFilter(TextureFilter minFilter) noexcept { filters = (filters & ~3) | ((int)minFilter & 3); }
	void setMagnificationFilter(TextureFilter magFilter) noexcept { filters = (filters & ~(3 << 2)) | (((int)magFilter & 3) << 2); }
	void setMipmapFilter(TextureFilter mipFilter) noexcept { filters = (filters & ~(3 << 4)) | (((int)mipFilter & 3) << 4); }
	void setFilters(TextureFilter filter) noexcept { setFilters(filter, filter, filter); }
	void setFilters(TextureFilter minFilter, TextureFilter magFilter, TextureFilter mipFilter) noexcept;
	int getMaxAnisotropy() const noexcept { return (int)maxAnisotropy; }
	void setMaxAnisotropy(int maxAnisotropy) noexcept { this->maxAnisotropy = (std::int8_t)maxAnisotropy; }
	ComparisonFunction getDepthComparisonFunction() const noexcept { return (ComparisonFunction)(depthFunctionSrgbDecoding & 0x7F); }
	void setDepthComparisonFunction(ComparisonFunction function) noexcept;

	// Mipmaps
	float getMinMipmapLevel() const noexcept { return minMipLevel; }
	void setMinMipmapLevel(float minMipLevel) noexcept { this->minMipLevel = minMipLevel; }
	float getMaxMipmapLevel() const noexcept { return maxMipLevel; }
	void setMaxMipmapLevel(float maxMipLevel) noexcept { this->maxMipLevel = maxMipLevel; }
	float getMipmapLodBias() const noexcept { return mipLodBias; }
	void setMipmapLodBias(float mipLodBias) noexcept { this->mipLodBias = mipLodBias; }

	// sRGB-linear conversion
	bool isSrgbDecodingEnabled() const noexcept { return (depthFunctionSrgbDecoding & 0x80); }
	void setSrgbDecodingEnabled(bool enabled) noexcept;

	// Simplification
	void simplify(int maxAnisotropy, bool mipmapped) noexcept;

	std::uint8_t wrapModes;					// 3*2 bits
	std::uint8_t filters;					// 3*2 bits
	std::int8_t maxAnisotropy;				// 4 bits
	std::uint8_t depthFunctionSrgbDecoding;	// 3+1 bits
	float minMipLevel;
	float maxMipLevel;
	float mipLodBias;						// 4*uint8_t + 3*float = 128 bits
};

} // namespace graphics

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::SamplerState>
{
	size_t operator()(const ::graphics::SamplerState& state) const noexcept
	{
		static_assert((sizeof(::graphics::SamplerState) & (sizeof(size_t) - 1)) == 0);
		return ::graphics::detail::hashRange/*boost::hash_range*/((const size_t*)&state, 
			(const size_t*)&state + sizeof(::graphics::SamplerState)/sizeof(size_t));
	}
};

} // namespace std

/*
 *	Name: SamplerState
 *	Author: Pawel Mrochen
 */

#pragma once

#include <limits>
#include <algorithm>
#include <functional>
#include <cstddef>
#include "ComparisonFunction.hpp"
#include "TextureWrapMode.hpp"
#include "TextureFilter.hpp"

namespace graphics {

struct SamplerState
{
	SamplerState() noexcept :
		wrapModes((int)TextureWrapMode::REPEAT | ((int)TextureWrapMode::REPEAT << 2) | ((int)TextureWrapMode::REPEAT << 4)),
		filters((int)TextureFilter::LINEAR | ((int)TextureFilter::LINEAR << 2) | ((int)TextureFilter::LINEAR << 4)),
		maxAnisotropy(16),
		depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
		minMipLevel(-std::numeric_limits<float>::infinity()),
		maxMipLevel(std::numeric_limits<float>::infinity()),
		mipLodBias()
	{
	}

	explicit SamplerState(TextureWrapMode wrapMode) noexcept :
		wrapModes(((int)wrapMode & 3) | (((int)wrapMode & 3) << 2) | (((int)wrapMode & 3) << 4)),
		filters((int)TextureFilter::LINEAR | ((int)TextureFilter::LINEAR << 2) | ((int)TextureFilter::LINEAR << 4)),
		maxAnisotropy(16),
		depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
		minMipLevel(-std::numeric_limits<float>::infinity()),
		maxMipLevel(std::numeric_limits<float>::infinity()),
		mipLodBias()
	{
	}

	SamplerState(TextureWrapMode wrapMode, TextureFilter filter) noexcept :
		wrapModes(((int)wrapMode & 3) | (((int)wrapMode & 3) << 2) | (((int)wrapMode & 3) << 4)),
		filters(((int)filter & 3) | (((int)filter & 3) << 2) | (((int)filter & 3) << 4)),
		maxAnisotropy(16),
		depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
		minMipLevel(-std::numeric_limits<float>::infinity()),
		maxMipLevel(std::numeric_limits<float>::infinity()),
		mipLodBias()
	{
	}

	SamplerState(TextureWrapMode wrapMode, TextureFilter filter, int maxAnisotropy) noexcept :
		wrapModes(((int)wrapMode & 3) | (((int)wrapMode & 3) << 2) | (((int)wrapMode & 3) << 4)),
		filters(((int)filter & 3) | (((int)filter & 3) << 2) | (((int)filter & 3) << 4)),
		maxAnisotropy((int8_t)maxAnisotropy),
		depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
		minMipLevel(-std::numeric_limits<float>::infinity()),
		maxMipLevel(std::numeric_limits<float>::infinity()),
		mipLodBias()
	{
	}

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

	static const SamplerState& getDefault()
	{
		static const SamplerState state;
		return state;
	}

	// Wrap mode
	TextureWrapMode getWrapModeU() const noexcept { return (TextureWrapMode)(wrapModes & 3); }
	TextureWrapMode getWrapModeV() const noexcept { return (TextureWrapMode)((wrapModes >> 2) & 3); }
	TextureWrapMode getWrapModeW() const noexcept { return (TextureWrapMode)((wrapModes >> 4) & 3); }
	void setWrapModeU(TextureWrapMode wrapU) noexcept { wrapModes = (wrapModes & ~3) | ((int)wrapU & 3); }
	void setWrapModeV(TextureWrapMode wrapV) noexcept { wrapModes = (wrapModes & ~(3 << 2)) | (((int)wrapV & 3) << 2); }
	void setWrapModeW(TextureWrapMode wrapW) noexcept { wrapModes = (wrapModes & ~(3 << 4)) | (((int)wrapW & 3) << 4); }
	void setWrapModes(TextureWrapMode wrapMode) noexcept { setWrapModes(wrapMode, wrapMode, wrapMode); }
	void setWrapModes(TextureWrapMode wrapU, TextureWrapMode wrapVW) noexcept { setWrapModes(wrapU, wrapVW, wrapVW); }
	void setWrapModes(TextureWrapMode wrapU, TextureWrapMode wrapV, TextureWrapMode wrapW) noexcept
	{
		wrapModes = ((int)wrapU & 3) | (((int)wrapV & 3) << 2) | (((int)wrapW & 3) << 4);
	}

	// Texture filter
	TextureFilter getMinificationFilter() const noexcept { return (TextureFilter)(filters & 3); }
	TextureFilter getMagnificationFilter() const noexcept { return (TextureFilter)((filters >> 2) & 3); }
	TextureFilter getMipmapFilter() const noexcept { return (TextureFilter)((filters >> 4) & 3); }
	void setMinificationFilter(TextureFilter minFilter) noexcept { filters = (filters & ~3) | ((int)minFilter & 3); }
	void setMagnificationFilter(TextureFilter magFilter) noexcept { filters = (filters & ~(3 << 2)) | (((int)magFilter & 3) << 2); }
	void setMipmapFilter(TextureFilter mipFilter) noexcept { filters = (filters & ~(3 << 4)) | (((int)mipFilter & 3) << 4); }
	void setFilters(TextureFilter filter) noexcept { setFilters(filter, filter, filter); }
	void setFilters(TextureFilter minFilter, TextureFilter magFilter, TextureFilter mipFilter) noexcept
	{
		filters = ((int)minFilter & 3) | (((int)magFilter & 3) << 2) | (((int)mipFilter & 3) << 4);
	}

	// Anisotropy	
	int getMaxAnisotropy() const noexcept { return (int)maxAnisotropy; }
	void setMaxAnisotropy(int maxAnisotropy) noexcept { this->maxAnisotropy = (std::int8_t)maxAnisotropy; }

	// Depth comparison
	ComparisonFunction getDepthComparisonFunction() const noexcept { return (ComparisonFunction)(depthFunctionSrgbDecoding & 0x7F); }
	void setDepthComparisonFunction(ComparisonFunction function) noexcept
	{
		depthFunctionSrgbDecoding = (depthFunctionSrgbDecoding & ~0x7F) | ((int)function & 0x7F);
	}

	// Mipmaps
	float getMinMipmapLevel() const noexcept { return minMipLevel; }
	void setMinMipmapLevel(float minMipLevel) noexcept { this->minMipLevel = minMipLevel; }
	float getMaxMipmapLevel() const noexcept { return maxMipLevel; }
	void setMaxMipmapLevel(float maxMipLevel) noexcept { this->maxMipLevel = maxMipLevel; }
	float getMipmapLodBias() const noexcept { return mipLodBias; }
	void setMipmapLodBias(float mipLodBias) noexcept { this->mipLodBias = mipLodBias; }

	// sRGB-linear conversion
	bool isSrgbDecodingEnabled() const noexcept { return (depthFunctionSrgbDecoding & 0x80); }
	void setSrgbDecodingEnabled(bool enabled) noexcept
	{
		if (enabled)
			depthFunctionSrgbDecoding |= 0x80;
		else
			depthFunctionSrgbDecoding &= ~0x80;
	}

	// Simplification
	void simplify(int maxAnisotropy, bool mipmapped) noexcept
	{
		if (!mipmapped)
		{
			filters &= ~(3 << 4); // set mipFilter to TextureFilter::NONE
			//minMipLevel = 0.0f;
			//maxMipLevel = 0.0f;
			//mipLodBias = 0.0f;
		}

		if (((filters & 3) <= 1) && (((filters >> 2) & 3) <= 1) && (((filters >> 4) & 3) <= 1))
			this->maxAnisotropy = 1;
		else if (maxAnisotropy < (int)this->maxAnisotropy)
			this->maxAnisotropy = (std::int8_t)maxAnisotropy;
	}

	//static const SamplerState DEFAULT;

	std::uint8_t wrapModes;					// 3*2 bits
	std::uint8_t filters;					// 3*2 bits
	std::int8_t maxAnisotropy;				// 4 bits
	std::uint8_t depthFunctionSrgbDecoding;	// 3+1 bits
	float minMipLevel;
	float maxMipLevel;
	float mipLodBias;						// 4*uint8_t + 3*float = 128 bits
};

} // namespace graphics

#include "Hash.inl"

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::SamplerState>
{
	size_t operator()(const ::graphics::SamplerState& state) const noexcept
	{
		static_assert((sizeof(::graphics::SamplerState) & (sizeof(size_t) - 1)) == 0);
		return ::graphics::hash::range/*boost::hash_range*/((const size_t*)&state, 
			(const size_t*)&state + sizeof(::graphics::SamplerState)/sizeof(size_t));
	}
};

} // namespace std

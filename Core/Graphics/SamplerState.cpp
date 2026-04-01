/*
 *	Name: SamplerState
 *	Author: Pawel Mrochen
 */

#include <limits>
#include "SamplerState.hpp"

namespace graphics {

SamplerState::SamplerState() noexcept :
	wrapModes((int)TextureWrapMode::REPEAT | ((int)TextureWrapMode::REPEAT << 2) | ((int)TextureWrapMode::REPEAT << 4)),
	filters((int)TextureFilter::LINEAR | ((int)TextureFilter::LINEAR << 2) | ((int)TextureFilter::LINEAR << 4)),
	maxAnisotropy(16),
	depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
	minMipLevel(-std::numeric_limits<float>::infinity()),
	maxMipLevel(std::numeric_limits<float>::infinity()),
	mipLodBias()
{
}

SamplerState::SamplerState(TextureWrapMode wrapMode) noexcept :
	wrapModes(((int)wrapMode & 3) | (((int)wrapMode & 3) << 2) | (((int)wrapMode & 3) << 4)),
	filters((int)TextureFilter::LINEAR | ((int)TextureFilter::LINEAR << 2) | ((int)TextureFilter::LINEAR << 4)),
	maxAnisotropy(16),
	depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
	minMipLevel(-std::numeric_limits<float>::infinity()),
	maxMipLevel(std::numeric_limits<float>::infinity()),
	mipLodBias()
{
}

SamplerState::SamplerState(TextureWrapMode wrapMode, TextureFilter filter) noexcept :
	wrapModes(((int)wrapMode & 3) | (((int)wrapMode & 3) << 2) | (((int)wrapMode & 3) << 4)),
	filters(((int)filter & 3) | (((int)filter & 3) << 2) | (((int)filter & 3) << 4)),
	maxAnisotropy(16),
	depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
	minMipLevel(-std::numeric_limits<float>::infinity()),
	maxMipLevel(std::numeric_limits<float>::infinity()),
	mipLodBias()
{
}

SamplerState::SamplerState(TextureWrapMode wrapMode, TextureFilter filter, int maxAnisotropy) noexcept :
	wrapModes(((int)wrapMode & 3) | (((int)wrapMode & 3) << 2) | (((int)wrapMode & 3) << 4)),
	filters(((int)filter & 3) | (((int)filter & 3) << 2) | (((int)filter & 3) << 4)),
	maxAnisotropy((int8_t)maxAnisotropy),
	depthFunctionSrgbDecoding((int)ComparisonFunction::NEVER | 0x80),
	minMipLevel(-std::numeric_limits<float>::infinity()),
	maxMipLevel(std::numeric_limits<float>::infinity()),
	mipLodBias()
{
}

/*static*/ const SamplerState& SamplerState::getDefault() noexcept
{
	static const SamplerState defaultState;
	return defaultState;
}

void SamplerState::setWrapModes(TextureWrapMode wrapU, TextureWrapMode wrapV, TextureWrapMode wrapW) noexcept
{ 
	wrapModes = ((int)wrapU & 3) | (((int)wrapV & 3) << 2) | (((int)wrapW & 3) << 4);
}

void SamplerState::setFilters(TextureFilter minFilter, TextureFilter magFilter, TextureFilter mipFilter) noexcept
{ 
	filters = ((int)minFilter & 3) | (((int)magFilter & 3) << 2) | (((int)mipFilter & 3) << 4);
}

void SamplerState::setDepthComparisonFunction(ComparisonFunction function) noexcept
{
	depthFunctionSrgbDecoding = (depthFunctionSrgbDecoding & ~0x7F) | ((int)function & 0x7F);
}

void SamplerState::setSrgbDecodingEnabled(bool enabled) noexcept
{
	if (enabled)
		depthFunctionSrgbDecoding |= 0x80;
	else
		depthFunctionSrgbDecoding &= ~0x80;
}

void SamplerState::simplify(int maxAnisotropy, bool mipmapped) noexcept
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

} // namespace graphics

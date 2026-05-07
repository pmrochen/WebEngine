/*
 *	Name: MultisampleMode
 *	Author: Pawel Mrochen
 */

#pragma once

namespace graphics {

enum class MultisampleMode
{
	NONE = 0,
	MSAA_2X = 2,
	MSAA_4X = 4,
	//MSAA_6X = 6,
	MSAA_8X = 8,
	MSAA_16X = 16
	//CSAA_8X = 0x0408,
	//CSAA_8XQ = 0x0808,
	//CSAA_16X = 0x0410,
	//CSAA_16XQ = 0x0810
};

//namespace detail {
//constexpr int COVERAGE_SAMPLES_MASK = 0x00FF;
//constexpr int COLOR_SAMPLES_MASK = 0xFF00;
//constexpr int COLOR_SAMPLES_SHIFT = 8;
//} // namespace detail

//inline MultisampleMode pack(int nCoverageSamples, int nColorSamples) noexcept
//{
//	return (MultisampleMode)((nCoverageSamples & detail::COVERAGE_SAMPLES_MASK) | ((nColorSamples << detail::COLOR_SAMPLES_SHIFT) & detail::COLOR_SAMPLES_MASK));
//}

//inline int getNumberOfCoverageSamples(MultisampleMode mode) noexcept 
//{
//	return (mode & detail::COVERAGE_SAMPLES_MASK);
//}

//inline int getNumberOfColorSamples(MultisampleMode mode) noexcept 
//{ 
//	return ((mode & detail::COLOR_SAMPLES_MASK) >> detail::COLOR_SAMPLES_SHIFT);
//}

} // namespace graphics

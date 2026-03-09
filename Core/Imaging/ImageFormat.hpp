/*
 *	Name: ImageFormat
 *	Author: Pawel Mrochen
 */

#pragma once

namespace core::imaging {

enum class ImageFormat
{
    UNDETERMINED = 0,
	NATIVE = 1,
	BMP = 2,
	TGA/*TRUEVISION_TGA*/ = 3,
	PSD = 4,
//#ifdef IMAGING_HAS_LIBPNG
	PNG = 5,
//#endif
//#ifdef IMAGING_HAS_LIBJPEG
	JPEG = 6,
//#endif
	DDS = 9,
	HDR/*RADIANCE_RGBE*/ = 10,
//#ifdef IMAGING_HAS_OPENEXR
	OPENEXR = 11,
//#endif
//#ifdef IMAGING_HAS_LIBWEBP
	WEBP = 12,
//#endif
};

} // namespace core::imaging

/*
 *	Name: Endianness
 *	Author: Pawel Mrochen
 */

#pragma once

namespace filesystem {

enum class Endianness
{
	NATIVE,
	LITTLE_ENDIAN,
	BIG_ENDIAN
};

inline Endianness endianness() noexcept 
{ 
	static const short test = (short)Endianness::LITTLE_ENDIAN | ((short)Endianness::BIG_ENDIAN << 8); 
	return (Endianness)*(const char*)&test; 
}

} // namespace filesystem

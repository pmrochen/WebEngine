/*
 *	Name: Endianness
 *	Author: Pawel Mrochen
 */

#pragma once

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM64))
#include <intrin.h> // for _byteswap_ushort/ulong/uint64
#endif

#include <concepts>
#include <type_traits>
#include <bit>
#include <cstdint>

namespace common {
namespace detail {

#if defined(__clang__) || defined(__GNUC__)
inline std::uint16_t bswap16(std::uint16_t x) noexcept { return __builtin_bswap16(x); }
inline std::uint32_t bswap32(std::uint32_t x) noexcept { return __builtin_bswap32(x); }
inline std::uint64_t bswap64(std::uint64_t x) noexcept { return __builtin_bswap64(x); }
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM64))
inline std::uint16_t bswap16(std::uint16_t x) noexcept { return _byteswap_ushort(x); }
inline std::uint32_t bswap32(std::uint32_t x) noexcept { return _byteswap_ulong(x); }
inline std::uint64_t bswap32(std::uint64_t x) noexcept { return _byteswap_uint64(x); }
#else
inline std::uint16_t bswap16(std::uint16_t x) noexcept { return (x << 8) | (x >> 8); }
inline std::uint32_t bswap32(std::uint32_t x) noexcept { return (x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24); }
inline std::uint64_t bswap64(std::uint64_t x) noexcept; // #TODO
#endif

} // namespace detail

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

template<typename T>
	requires ((std::integral<T> || std::floating_point<T>) &&
		((sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8)))
inline T bswap(T x) noexcept
{
	if constexpr (std::is_floating_point_v<T>)
	{
		if constexpr (sizeof(T) == 8)
			return std::bit_cast<T>(detail::bswap64(std::bit_cast<std::uint64_t>(x)));
		else if constexpr (sizeof(T) == 4)
			return std::bit_cast<T>(detail::bswap32(std::bit_cast<std::uint32_t>(x)));
		else if constexpr (sizeof(T) == 2)
			return std::bit_cast<T>(detail::bswap16(std::bit_cast<std::uint16_t>(x)));
		else // 1
			return x;
	}
	else // integral
	{
		if constexpr (sizeof(T) == 8)
			return T(detail::bswap64((std::uint64_t)x));
		else if constexpr (sizeof(T) == 4)
			return T(detail::bswap32((std::uint32_t)x));
		else if constexpr (sizeof(T) == 2)
			return T(detail::bswap16((std::uint16_t)x));
		else // 1
			return x;
	}
}

} // namespace common

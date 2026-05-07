/*
 *	Name: VertexAttributeInfo
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
#include <functional>
#include <cstddef>
#include <cstdint>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include "VertexDataType.hpp"

namespace graphics {
namespace detail {

template<typename T, typename ... U>
concept Either = (std::same_as<T, U> || ...);

} // namespace detail

template<typename T>
concept AnyVertexAttribute = detail::Either<T, float, mathematics::templates::Vector2<float>, mathematics::templates::Vector3<float>, 
	mathematics::templates::Vector4<float>, std::uint8_t, mathematics::templates::Vector2<uint8_t>, 
	mathematics::templates::Vector3<uint8_t>, mathematics::templates::Vector4<uint8_t>, std::uint32_t>;

struct VertexAttributeInfo
{
	constexpr VertexAttributeInfo() noexcept :
		dataTypeAndOffset(0) 
	{
	}

	//constexpr explicit VertexAttributeInfo(VertexDataType dataType) noexcept : 
	//	dataTypeAndOffset((unsigned int)dataType & 0x000FFFFF)
	//{
	//}
	
	constexpr VertexAttributeInfo(VertexDataType dataType, unsigned int offset) noexcept :
		dataTypeAndOffset(((unsigned int)dataType & 0x000FFFFF) | (offset << 20))
	{
	}

	bool operator==(const VertexAttributeInfo& attribute) const noexcept
	{ 
		return (dataTypeAndOffset == attribute.dataTypeAndOffset); 
	}
	
	bool operator!=(const VertexAttributeInfo& attribute) const noexcept
	{ 
		return (dataTypeAndOffset != attribute.dataTypeAndOffset); 
	}

	template<typename A> void serialize(A& ar) 
	{ 
		ar(dataTypeAndOffset);
	}

	static const VertexAttributeInfo& getEmpty() noexcept
	{ 
		static const VertexAttributeInfo empty;
		return empty; 
	}

	bool isEmpty() const noexcept 
	{ 
		return !dataTypeAndOffset; 
	}

	void clear() noexcept
	{ 
		dataTypeAndOffset = 0; 
	}

	void set(VertexDataType dataType, unsigned int offset) noexcept 
	{ 
		dataTypeAndOffset = ((unsigned int)dataType & 0x000FFFFF) | (offset << 20); 
	}

	VertexDataType getDataType() const noexcept 
	{ 
		return (VertexDataType)(dataTypeAndOffset & 0x000FFFFF); 
	}
	
	void setDataType(VertexDataType dataType) noexcept 
	{ 
		dataTypeAndOffset = (dataTypeAndOffset & 0xFFF00000) | ((unsigned int)dataType & 0x000FFFFF); 
	}
	
	unsigned int getOffset() const noexcept 
	{ 
		return (dataTypeAndOffset >> 20); 
	}
	
	void setOffset(unsigned int offset) noexcept 
	{ 
		dataTypeAndOffset = (dataTypeAndOffset & 0x000FFFFF) | (offset << 20); 
	}

	unsigned int dataTypeAndOffset;		// 20+12 bits
};

} // namespace graphics

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::VertexAttributeInfo>
{
	size_t operator()(const ::graphics::VertexAttributeInfo& attribute) const noexcept
	{
		return hash<unsigned int>()(attribute.dataTypeAndOffset);
	}
};

} // namespace std

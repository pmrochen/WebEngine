/*
 *	Name: VertexAttribute
 *	Author: Pawel Mrochen
 */

#pragma once

#include <functional>
#include <cstddef>
#include "VertexDataType.hpp"

namespace graphics {

struct VertexAttribute
{
	constexpr VertexAttribute() noexcept : 
		dataTypeAndOffset(0) 
	{
	}

	//constexpr explicit VertexAttribute(VertexDataType dataType) noexcept : 
	//	dataTypeAndOffset((unsigned int)dataType & 0x000FFFFF)
	//{
	//}
	
	constexpr VertexAttribute(VertexDataType dataType, unsigned int offset) noexcept : 
		dataTypeAndOffset(((unsigned int)dataType & 0x000FFFFF) | (offset << 20))
	{
	}

	bool operator==(const VertexAttribute& attribute) const noexcept 
	{ 
		return (dataTypeAndOffset == attribute.dataTypeAndOffset); 
	}
	
	bool operator!=(const VertexAttribute& attribute) const noexcept 
	{ 
		return (dataTypeAndOffset != attribute.dataTypeAndOffset); 
	}

	template<typename A> void serialize(A& ar) 
	{ 
		ar(dataTypeAndOffset);
	}

	static const VertexAttribute& getEmpty() noexcept 
	{ 
		static const VertexAttribute empty; 
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
struct hash<::graphics::VertexAttribute>
{
	size_t operator()(const ::graphics::VertexAttribute& attribute) const noexcept
	{
		return hash<unsigned int>()(attribute.dataTypeAndOffset);
	}
};

} // namespace std

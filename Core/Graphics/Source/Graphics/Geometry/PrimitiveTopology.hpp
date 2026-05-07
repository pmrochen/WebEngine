/*
 *	Name: PrimitiveTopology
 *	Author: Pawel Mrochen
 */

#pragma once

#include <functional>
#include <cstddef>
#include "PrimitiveType.hpp"

namespace graphics {

struct PrimitiveTopology
{
	constexpr PrimitiveTopology() noexcept : 
		primitiveTypeAndControlPoints() 
	{
	}

	//constexpr explicit PrimitiveTopology(PrimitiveType primitiveType) noexcept : 
	//	primitiveTypeAndControlPoints(primitiveType & 0xF) 
	//{
	//}

	constexpr PrimitiveTopology(PrimitiveType primitiveType, int nControlPoints) noexcept : 
		primitiveTypeAndControlPoints(((int)primitiveType & 0xF) | (nControlPoints << 4)) 
	{
	}

	bool operator==(const PrimitiveTopology& topology) const noexcept 
	{ 
		return (primitiveTypeAndControlPoints == topology.primitiveTypeAndControlPoints); 
	}
	
	bool operator!=(const PrimitiveTopology& topology) const noexcept 
	{ 
		return (primitiveTypeAndControlPoints != topology.primitiveTypeAndControlPoints); 
	}

	template<typename A> void serialize(A& ar) 
	{ 
		ar(primitiveTypeAndControlPoints); 
	}

	static const PrimitiveTopology& getEmpty() noexcept 
	{ 
		static const PrimitiveTopology empty; 
		return empty; 
	}

	bool isEmpty() const noexcept 
	{ 
		return !primitiveTypeAndControlPoints; 
	}

	void clear() noexcept
	{ 
		primitiveTypeAndControlPoints = 0; 
	}

	void set(PrimitiveType primitiveType, int nControlPoints) noexcept 
	{ 
		primitiveTypeAndControlPoints = (((int)primitiveType & 0xF) | (nControlPoints << 4)); 
	}
	
	PrimitiveType getPrimitiveType() const noexcept 
	{ 
		return (PrimitiveType)(primitiveTypeAndControlPoints & 0xF); 
	}

	void setPrimitiveType(PrimitiveType primitiveType) noexcept 
	{ 
		primitiveTypeAndControlPoints = ((primitiveTypeAndControlPoints & ~0xF) | ((int)primitiveType & 0xF)); 
	}

	int getNumberOfControlPoints() const noexcept 
	{ 
		return (primitiveTypeAndControlPoints >> 4); 
	}

	void setNumberOfControlPoints(int nControlPoints) noexcept 
	{ 
		primitiveTypeAndControlPoints = ((primitiveTypeAndControlPoints & 0xF) | (nControlPoints << 4)); 
	}

	int getNumberOfVerticesPerPrimitive() const noexcept 
	{ 
		return ((primitiveTypeAndControlPoints & 0xF) >= PrimitiveType::PATCHES) ? 
			(primitiveTypeAndControlPoints >> 4) : 
			((primitiveTypeAndControlPoints & 0xF) ? (((primitiveTypeAndControlPoints & 0xF) >> 1) + 1) : 0); 
	}

	int primitiveTypeAndControlPoints;
};

} // namespace graphics

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::PrimitiveTopology>
{
	size_t operator()(const ::graphics::PrimitiveTopology& topology) const noexcept
	{
		return hash<int>()(topology.primitiveTypeAndControlPoints);
	}
};

} // namespace std

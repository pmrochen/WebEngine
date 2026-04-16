/*
 *	Name: VertexLayout
 *	Author: Pawel Mrochen
 */

#pragma once

#include <stdexcept>
#include <functional>
#include <array>
#include <algorithm>
#include <cstddef>
#include "VertexDataType.hpp"
#include "VertexAttribute.hpp"

namespace graphics {

struct VertexLayout
{
	static constexpr int MAX_ATTRIBUTE_COUNT = 16;

	bool operator==(const VertexLayout& layout) const noexcept
	{
		static_assert((sizeof(VertexLayout) & (sizeof(std::size_t) - 1)) == 0);
		return std::equal((const std::size_t*)this, (const std::size_t*)this + sizeof(VertexLayout)/sizeof(std::size_t), 
			(const std::size_t*)&layout);
	}

	bool operator!=(const VertexLayout& layout) const noexcept
	{
		return !(*this == layout);
	}

	template<typename A> void serialize(A& ar)
	{
		for (int i = 0; i < MAX_ATTRIBUTE_COUNT; i++)
			ar(attributes[i]);

		ar(divisor, stride);
	}

	// #TODO bool isEmpty() const noexcept;

	void clear() noexcept
	{
		for (int i = 0; i < MAX_ATTRIBUTE_COUNT; i++)
			attributes[i].clear();

		divisor = 0;
		stride = 0;
	}

	const VertexAttribute& getAttribute(int index) const noexcept 
	{ 
		return ((std::size_t)index < (std::size_t)MAX_ATTRIBUTE_COUNT) ? attributes[index] : VertexAttribute::getEmpty();
	}

	void setAttribute(int index, const VertexAttribute& attribute) // throw (std::out_of_range)
	{
		if ((std::size_t)index >= (std::size_t)MAX_ATTRIBUTE_COUNT)
			throw std::out_of_range("VertexLayout::setAttribute() : index");

		attributes[index] = attribute;
	}

	//VertexStepFunction getStepFunction() const noexcept
	//{
	//	return stepRate ? VertexStepFunction::PER_INSTANCE/*1*/ : VertexStepFunction::PER_VERTEX/*0*/;
	//}

	unsigned int getDivisor/*getStepRate*/() const noexcept { return divisor; }
	void setDivisor/*setStepRate*/(unsigned int divisor) noexcept { this->divisor = divisor; }
	std::size_t getStride() const noexcept { return stride; }
	void setStride(std::size_t stride) noexcept { this->stride = (unsigned int)stride; }

	void computeOffsetsAndStride() noexcept
	{
		unsigned int offset = 0;
		for (int i = 0; i < MAX_ATTRIBUTE_COUNT; i++)
		{
			if (attributes[i].getDataType() != VertexDataType::UNSPECIFIED)
			{
				attributes[i].setOffset(offset);
				offset += (unsigned int)getSize(attributes[i].getDataType());
			}
			else
			{
				attributes[i].setOffset(0);
			}
		}

		stride = offset;
	}

	std::array<VertexAttribute, MAX_ATTRIBUTE_COUNT> attributes;
	unsigned int divisor/*stepRate*/ = 0;	// 0 = per vertex
	unsigned int stride/*size*/ = 0;		// vertex size
};

} // namespace graphics

#include "Hash.inl"

namespace std {

template<typename T>
struct hash;

template<>
struct hash<::graphics::VertexLayout>
{
	size_t operator()(const ::graphics::VertexLayout& layout) const noexcept
	{
		static_assert((sizeof(::graphics::VertexLayout) & (sizeof(size_t) - 1)) == 0);
		return ::graphics::hash::range/*boost::hash_range*/((const size_t*)&layout,
			(const size_t*)&layout + sizeof(::graphics::VertexLayout)/sizeof(size_t));
	}
};

} // namespace std

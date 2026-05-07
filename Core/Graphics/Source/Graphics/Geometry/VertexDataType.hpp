/*
 *	Name: VertexDataType
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>

namespace graphics {

enum class VertexDataType
{
	UNSPECIFIED = 0,
	BYTE = 0x001131,
	BYTE2 = 0x001231,
	BYTE3 = 0x001331,
	BYTE4 = 0x001431,
	BYTE_NORMALIZED = 0x011131,
	BYTE2_NORMALIZED = 0x011231,
	BYTE3_NORMALIZED = 0x011331,
	BYTE4_NORMALIZED = 0x011431,
	//RGBA8_UNSIGNED_NORMALIZED // #TODO
	BGRA8_UNSIGNED_NORMALIZED = 0x031431,
	RGB10_A2_SIGNED_NORMALIZED = 0x051421,
	RGB10_A2_UNSIGNED_NORMALIZED = 0x051431,
	//INT = 0x001124,
	//INT2 = 0x001224,
	//INT3 = 0x001324,
	//INT4 = 0x001424,
	UINT = 0x001134,
	//UINT2 = 0x001234,
	//UINT3 = 0x001334,
	//UINT4 = 0x001434,
	FLOAT = 0x001144,
	FLOAT2 = 0x001244,
	FLOAT3 = 0x001344,
	FLOAT4 = 0x001444
};

namespace detail {

constexpr int COMPONENT_TYPE_MASK = 0x0000FF;
constexpr int COMPONENT_SIZE_MASK = 0x00000F;
constexpr int UNSIGNED_MASK = 0x000010;
constexpr int H_COMPONENTS_MASK = 0x000F00;
constexpr int H_COMPONENTS_SHIFT = 8;
//constexpr int V_COMPONENTS_MASK = 0x00F000;
//constexpr int V_COMPONENTS_SHIFT = 12;
constexpr int NORMALIZED_MASK = 0x010000;
constexpr int BGRA8_MASK = 0x020000;
constexpr int RGB10_A2_MASK = 0x040000;

} // namespace detail

template<typename T>
struct VertexDataTypeOf;

template<>
struct VertexDataTypeOf<float> : std::integral_constant<VertexDataType, VertexDataType::FLOAT> {};

template<>
struct VertexDataTypeOf<mathematics::templates::Vector2<float>> : std::integral_constant<VertexDataType, VertexDataType::FLOAT2> {};

template<>
struct VertexDataTypeOf<mathematics::templates::Vector3<float>> : std::integral_constant<VertexDataType, VertexDataType::FLOAT3> {};

template<>
struct VertexDataTypeOf<mathematics::templates::Vector4<float>> : std::integral_constant<VertexDataType, VertexDataType::FLOAT4> {};

template<>
struct VertexDataTypeOf<std::uint8_t> : std::integral_constant<VertexDataType, VertexDataType::BYTE> {};

template<>
struct VertexDataTypeOf<mathematics::templates::Vector2<std::uint8_t>> : std::integral_constant<VertexDataType, VertexDataType::BYTE2> {};

template<>
struct VertexDataTypeOf<mathematics::templates::Vector3<std::uint8_t>> : std::integral_constant<VertexDataType, VertexDataType::BYTE3> {};

template<>
struct VertexDataTypeOf<mathematics::templates::Vector4<std::uint8_t>> : std::integral_constant<VertexDataType, VertexDataType::BYTE4> {};

template<>
struct VertexDataTypeOf<std::uint32_t> : std::integral_constant<VertexDataType, VertexDataType::UINT> {};

inline VertexDataType pack(VertexDataType componentType, int nComponents) noexcept 
{ 
	return (VertexDataType)(((int)componentType & detail::COMPONENT_TYPE_MASK) | 0x001000 | 
		((nComponents << detail::H_COMPONENTS_SHIFT) & detail::H_COMPONENTS_MASK));
}

//inline VertexDataType pack(VertexDataType componentType, int nComponents, bool normalized) noexcept // #TODO
//{
//}

inline VertexDataType getComponentType(VertexDataType dataType) noexcept 
{ 
	return (VertexDataType)(((int)dataType & detail::COMPONENT_TYPE_MASK) | 0x001100); 
}

inline int getComponentSize(VertexDataType dataType) noexcept 
{ 
	return (int)dataType & detail::COMPONENT_SIZE_MASK; 
}

inline bool isUnsigned(VertexDataType dataType) noexcept 
{ 
	return (int)dataType & detail::UNSIGNED_MASK;
}

inline int getNumberOfComponents(VertexDataType dataType) noexcept 
{ 
	return ((int)dataType & detail::H_COMPONENTS_MASK) >> detail::H_COMPONENTS_SHIFT; 
}

inline std::size_t getSize/*InBytes*/(VertexDataType dataType) noexcept 
{ 
	return (std::size_t)getComponentSize()*getNumberOfComponents(); 
}

inline bool isNormalized(VertexDataType dataType) noexcept 
{ 
	return (int)dataType & detail::NORMALIZED_MASK;
}

} // namespace graphics

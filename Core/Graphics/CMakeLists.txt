# Graphics

cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CACHE{GRAPHICS_LIB_VERSION})
	string(TIMESTAMP VER_YEAR "%y")
	string(TIMESTAMP VER_MONTH "%m")
	math(EXPR VER_MONTH "${VER_MONTH} + 0")
    set(GRAPHICS_LIB_VERSION "${VER_YEAR}.${VER_MONTH}" CACHE STRING "Project version (auto-set at first configure)")
endif()

project(Graphics
	VERSION ${GRAPHICS_LIB_VERSION}
	LANGUAGES CXX
)

set(GRAPHICS_LIB_NAME Graphics)

find_package(OpenMP)
if(MSVC)
	set(OpenMP_RUNTIME_MSVC "llvm")
endif()

add_library(${GRAPHICS_LIB_NAME} STATIC)

target_include_directories(${GRAPHICS_LIB_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Mathematics>
		$<INSTALL_INTERFACE:../Mathematics>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Imaging>
		$<INSTALL_INTERFACE:../Imaging>
)

target_sources(${GRAPHICS_LIB_NAME}
	PRIVATE
		#Source/Graphics/Backend/ComputeEngine.cpp
		#Source/Graphics/Backend/Renderer.cpp
		#Source/Graphics/Backend/ResourceManager.cpp
		#Source/Graphics/Geometry/Mesh.cpp
		Source/Graphics/Rasterization/Framebuffer.cpp
		#Source/Graphics/SceneGraph/Locator.cpp
		#Source/Graphics/Shading/Material.cpp
		MikkTSpace/mikktspace.c
				
	PRIVATE
    	FILE_SET privateHeaders
	    TYPE HEADERS
    	FILES
			MikkTSpace/mikktspace.h
				
	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
		BASE_DIRS Source
    	FILES
			#Source/Graphics/Animation/FCurve.hpp
			Source/Graphics/Animation/FCurveAttributes.hpp
			Source/Graphics/Animation/FCurveChannel.hpp
			Source/Graphics/Animation/FCurveExtrapolationMode.hpp
			Source/Graphics/Animation/FCurveInterpolationMode.hpp
			Source/Graphics/Backend/ComputeEngine.hpp
			Source/Graphics/Backend/Renderer.hpp
			Source/Graphics/Backend/ResourceManager.hpp
			Source/Graphics/Backend/ResourceStorageFlags.hpp
			Source/Graphics/Backend/ResourceTypes.hpp
			Source/Graphics/Backend/SwapChain.hpp
			Source/Graphics/Backend/VertexStorageFlags.hpp
			Source/Graphics/Geometry/GeometryType.hpp
			#Source/Graphics/Geometry/Mesh.hpp
			Source/Graphics/Geometry/PrimitiveTopology.hpp
			Source/Graphics/Geometry/PrimitiveType.hpp
			Source/Graphics/Geometry/Sidedness.hpp
			Source/Graphics/Geometry/VertexAttributeIndex.hpp
			Source/Graphics/Geometry/VertexAttributeInfo.hpp
			Source/Graphics/Geometry/VertexAttributeTypes.hpp
			Source/Graphics/Geometry/VertexDataType.hpp
			Source/Graphics/Geometry/VertexLayout.hpp
			Source/Graphics/Rasterization/AttributeMask.hpp
			Source/Graphics/Rasterization/BlendEquation.hpp
			Source/Graphics/Rasterization/BlendFunction.hpp
			Source/Graphics/Rasterization/ClearOptions.hpp
			Source/Graphics/Rasterization/ClipOrigin.hpp
			Source/Graphics/Rasterization/ColorFormat.hpp
			Source/Graphics/Rasterization/ComparisonFunction.hpp
			Source/Graphics/Rasterization/CullFaceMode.hpp
			Source/Graphics/Rasterization/DepthMask.hpp
			Source/Graphics/Rasterization/DepthStencilFormat.hpp
			Source/Graphics/Rasterization/Framebuffer.hpp
			Source/Graphics/Rasterization/FramebufferAccessMode.hpp
			Source/Graphics/Rasterization/FramebufferAttributes.hpp
			Source/Graphics/Rasterization/FramebufferOptions.hpp
			Source/Graphics/Rasterization/FrontFace.hpp
			Source/Graphics/Rasterization/MultisampleMode.hpp
			Source/Graphics/Rasterization/NdcDepthRange.hpp
			Source/Graphics/Rasterization/PolygonFillMode.hpp
			Source/Graphics/Rasterization/RasterizerFlags.hpp
			Source/Graphics/Rasterization/RasterizerState.hpp
			Source/Graphics/Rasterization/StencilOperation.hpp
			Source/Graphics/Rasterization/Viewport.hpp
			Source/Graphics/SceneGraph/AppearanceFlags.hpp
			Source/Graphics/SceneGraph/AppearanceOptions.hpp
			#Source/Graphics/SceneGraph/Locator.hpp
			Source/Graphics/SceneGraph/LightTypes.hpp
			Source/Graphics/SceneGraph/Projection.hpp
			Source/Graphics/SceneGraph/ViewVolume.hpp
			Source/Graphics/Shading/Material.hpp
			Source/Graphics/Shading/SamplerState.hpp
			Source/Graphics/Shading/TextureFilter.hpp
			Source/Graphics/Shading/TextureWrapMode.hpp
			Source/Graphics/Graphics.hpp
)

target_compile_features(${GRAPHICS_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

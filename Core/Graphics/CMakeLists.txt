cmake_minimum_required(VERSION 3.25)

project(Graphics LANGUAGES CXX)
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
		ViewVolume.cpp
		MikkTSpace/mikktspace.c
				
	PRIVATE
    	FILE_SET privateHeaders
	    TYPE HEADERS
    	FILES
			MikkTSpace/mikktspace.h
				
	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
    	FILES
			AppearanceFlags.hpp
			AppearanceOptions.hpp
			AttributeMask.hpp
			BlendEquation.hpp
			BlendFunction.hpp
			ClearOptions.hpp
			ClipOrigin.hpp
			ColorFormat.hpp
			ComparisonFunction.hpp
			CullFaceMode.hpp
			DepthMask.hpp
			DepthRange.hpp
			DepthStencilFormat.hpp
			FramebufferAccessMode.hpp
			FramebufferOptions.hpp
			FrontFace.hpp
			GeometryType.hpp
			Graphics.hpp
			LightType.hpp
			MultisampleMode.hpp
			PolygonFillMode.hpp
			PrimitiveTopology.hpp
			PrimitiveType.hpp
			Projection.hpp
			RasterizerFlags.hpp
			RasterizerState.hpp
			SamplerState.hpp
			SkinningFlags.hpp
			StencilOperation.hpp
			StorageFlags.hpp
			TextureFilter.hpp
			TextureWrapMode.hpp
			VertexAttributeFlags.hpp
			Viewport.hpp
			ViewVolume.hpp
)

target_compile_features(${GRAPHICS_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

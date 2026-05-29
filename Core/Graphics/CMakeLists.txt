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

if(DEFINED MATHEMATICS_DOUBLE_PRECISION)
	set(GRAPHICS_DOUBLE_PRECISION "${MATHEMATICS_DOUBLE_PRECISION}")
endif()

if(DEFINED MATHEMATICS_HAS_SIMD)
	set(GRAPHICS_HAS_SIMD "${MATHEMATICS_HAS_SIMD}")
endif()

find_package(OpenMP)
if(MSVC)
	set(OpenMP_RUNTIME_MSVC "llvm")
endif()

add_library(${GRAPHICS_LIB_NAME} STATIC)
add_library(Core::${GRAPHICS_LIB_NAME} ALIAS ${GRAPHICS_LIB_NAME})
#set_target_properties(${GRAPHICS_LIB_NAME} PROPERTIES EXPORT_NAME Core::${GRAPHICS_LIB_NAME})

target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC oup::oup)
target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC isptr::isptr)
target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC Pal::Sigslot)
target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC Core::Common)
target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC Core::Filesystem)
target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC Core::Mathematics)
target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC Core::Imaging)

if(GRAPHICS_HAS_SIMD)
	target_link_libraries(${GRAPHICS_LIB_NAME} PUBLIC Core::Simd)
endif()

target_sources(${GRAPHICS_LIB_NAME}
	PRIVATE
		#Source/Graphics/Backend/ComputeEngine.cpp
		#Source/Graphics/Backend/Renderer.cpp
		#Source/Graphics/Backend/ResourceManager.cpp
		#Source/Graphics/Geometry/Heightfield.cpp
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
			Source/Graphics/Animation/AnimationPlayMode.hpp
			Source/Graphics/Animation/FCurve.hpp
			Source/Graphics/Animation/FCurveAttributes.hpp
			Source/Graphics/Animation/FCurveChannel.hpp
			Source/Graphics/Animation/FCurveExtrapolationMode.hpp
			Source/Graphics/Animation/FCurveInterpolationMode.hpp
			Source/Graphics/Backend/BufferCache.hpp
			Source/Graphics/Backend/ComputeEngine.hpp
			Source/Graphics/Backend/DispatchSizeMode.hpp
			Source/Graphics/Backend/DisplaySynchronizationMode.hpp
			Source/Graphics/Backend/GeometryCache.hpp
			Source/Graphics/Backend/Renderer.hpp
			Source/Graphics/Backend/ResourceCache.hpp
			Source/Graphics/Backend/ResourceManager.hpp
			Source/Graphics/Backend/ResourceStorageFlags.hpp
			Source/Graphics/Backend/ResourceTypes.hpp
			Source/Graphics/Backend/ShaderCache.hpp
			Source/Graphics/Backend/SwapChain.hpp
			Source/Graphics/Backend/TechniqueCache.hpp
			Source/Graphics/Backend/TextureCache.hpp
			Source/Graphics/Backend/VertexStorageFlags.hpp
			Source/Graphics/Geometry/GeometryType.hpp
			Source/Graphics/Geometry/GeometryViewTarget.hpp
			Source/Graphics/Geometry/Heightfield.hpp
			Source/Graphics/Geometry/Mesh.hpp
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
			Source/Graphics/SceneGraph/AttenuationType.hpp
			Source/Graphics/SceneGraph/LightTypes.hpp
			Source/Graphics/SceneGraph/Locator.hpp
			Source/Graphics/SceneGraph/Projection.hpp
			Source/Graphics/SceneGraph/ReferenceFrame.hpp
			Source/Graphics/SceneGraph/RotationType.hpp
			Source/Graphics/SceneGraph/TransformMode.hpp
			Source/Graphics/SceneGraph/ViewVolume.hpp
			Source/Graphics/Shading/ComputeTechnique.hpp
			Source/Graphics/Shading/Material.hpp
			Source/Graphics/Shading/RenderTechnique.hpp
			Source/Graphics/Shading/SamplerState.hpp
			Source/Graphics/Shading/Shader.hpp
			#Source/Graphics/Shading/ShaderDataSource.hpp
			Source/Graphics/Shading/ShaderStage.hpp
			Source/Graphics/Shading/ShaderVariableQualifier.hpp
			Source/Graphics/Shading/Technique.hpp
			Source/Graphics/Shading/TextureFilter.hpp
			Source/Graphics/Shading/TextureWrapMode.hpp
			Source/Graphics/Graphics.hpp
)

target_compile_features(${GRAPHICS_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

target_compile_definitions(${GRAPHICS_LIB_NAME}
	PUBLIC 
		GRAPHICS_DOUBLE_PRECISION=$<BOOL:${GRAPHICS_DOUBLE_PRECISION}>
        GRAPHICS_HAS_SIMD=$<BOOL:${GRAPHICS_HAS_SIMD}>
)

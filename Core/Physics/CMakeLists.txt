# Physics

cmake_minimum_required(VERSION 3.25)

include(FetchContent)

FetchContent_Declare(bitwise-enum
		GIT_REPOSITORY https://github.com/jonesinator/bitwise-enum.git
		GIT_TAG cb1afd271eef0d9fcf43d9a2a55157c7f807f4c8
)

FetchContent_Declare(intrusive_shared_ptr
		GIT_REPOSITORY https://github.com/gershnik/intrusive_shared_ptr.git
		GIT_TAG v1.13
)

FetchContent_Declare(json
		GIT_REPOSITORY https://github.com/nlohmann/json.git
		GIT_TAG v3.12.0
)

#FetchContent_Declare(bullet3
#		GIT_REPOSITORY https://github.com/pmrochen/bullet3.git
#		GIT_TAG 3b0c9de03049d21d9618cb5e5cee59f4d6a987f4
#)

if(NOT DEFINED CACHE{PHYSICS_LIB_VERSION})
	string(TIMESTAMP VER_YEAR "%y")
	string(TIMESTAMP VER_MONTH "%m")
	math(EXPR VER_MONTH "${VER_MONTH} + 0")
    set(PHYSICS_LIB_VERSION "${VER_YEAR}.${VER_MONTH}" CACHE STRING "Project version (auto-set at first configure)")
endif()

project(Physics
	VERSION ${PHYSICS_LIB_VERSION}
	LANGUAGES CXX
)

set(PHYSICS_LIB_NAME Physics)

if(DEFINED MATHEMATICS_DOUBLE_PRECISION)
	set(PHYSICS_DOUBLE_PRECISION "${MATHEMATICS_DOUBLE_PRECISION}")
endif()

if(DEFINED MATHEMATICS_HAS_SIMD)
	set(PHYSICS_HAS_SIMD "${MATHEMATICS_HAS_SIMD}")
endif()

#FetchContent_GetProperties(bitwise-enum)
#if(NOT bitwise-enum_POPULATED)
	FetchContent_MakeAvailable(bitwise-enum)
#endif()
if(NOT TARGET bitwise-enum::bitwise-enum)
	add_library(bitwise-enum::bitwise-enum INTERFACE)
	target_include_directories(bitwise-enum::bitwise-enum INTERFACE ${bitwise-enum_SOURCE_DIR})
endif()

FetchContent_MakeAvailable(intrusive_shared_ptr json)

add_library(${PHYSICS_LIB_NAME} STATIC)
add_library(Core::${PHYSICS_LIB_NAME} ALIAS ${PHYSICS_LIB_NAME})
#set_target_properties(${PHYSICS_LIB_NAME} PROPERTIES EXPORT_NAME Core::${PHYSICS_LIB_NAME})

#target_include_directories(${PHYSICS_LIB_NAME} PUBLIC ${bitwise-enum_SOURCE_DIR})

target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC bitwise-enum::bitwise-enum)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC isptr::isptr)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Common)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Filesystem)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Mathematics)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Graphics)

if(PHYSICS_HAS_SIMD)
	target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Simd)
endif()

target_sources(${PHYSICS_LIB_NAME}
	PRIVATE
		#Source/Physics/Collision/CollisionShape.cpp
		#Source/Physics/Constraints/Constraint.cpp
				
	PRIVATE
    	FILE_SET privateHeaders
	    TYPE HEADERS
    	FILES
			Source/Physics/Extensions/Conversion.inl
			Source/Physics/Extensions/ConvexHullShape.hpp
			Source/Physics/Extensions/FilteredBvhTriangleMeshShape.hpp
			Source/Physics/Extensions/FilteredHeightfieldTerrainShape.hpp
			Source/Physics/Extensions/FilteredTriangleCallback.hpp
			Source/Physics/Extensions/Generic6DofSpringConstraint.hpp
			Source/Physics/Extensions/HeightfieldTerrainShape.hpp
			Source/Physics/Extensions/MotionState.hpp
			Source/Physics/Extensions/TorusShape.hpp

	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
		BASE_DIRS Source
    	FILES
			#Source/Physics/Collision/CollisionShape.hpp
			Source/Physics/Collision/CollisionShapeType.hpp
			Source/Physics/Collision/ContactPoint.hpp
			Source/Physics/Collision/ContactPointFlags.hpp
			Source/Physics/Collision/PhysicalMaterial.hpp
			#Source/Physics/Constraints/Constraint.hpp
			Source/Physics/Constraints/ConstraintRow.hpp
			Source/Physics/Constraints/Limit.hpp
			Source/Physics/Constraints/Motor.hpp
			Source/Physics/Constraints/MotorMode.hpp
			Source/Physics/Constraints/Spring.hpp
			Source/Physics/Physics.hpp
)

target_compile_features(${PHYSICS_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

target_compile_definitions(${PHYSICS_LIB_NAME}
	PUBLIC 
		PHYSICS_DOUBLE_PRECISION=$<BOOL:${PHYSICS_DOUBLE_PRECISION}>
        PHYSICS_HAS_SIMD=$<BOOL:${PHYSICS_HAS_SIMD}>
)

# Physics

cmake_minimum_required(VERSION 3.25)

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

add_library(${PHYSICS_LIB_NAME} STATIC)
add_library(Core::${PHYSICS_LIB_NAME} ALIAS ${PHYSICS_LIB_NAME})
#set_target_properties(${PHYSICS_LIB_NAME} PROPERTIES EXPORT_NAME Core::${PHYSICS_LIB_NAME})

target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC isptr::isptr)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Common)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Filesystem)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Mathematics)
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Imaging) # for bitwise-enum
target_link_libraries(${PHYSICS_LIB_NAME} PUBLIC Core::Graphics)

target_sources(${PHYSICS_LIB_NAME}
	PRIVATE
		#Source/Physics/Collision/CollisionShape.cpp
		#Source/Physics/Constraints/Constraint.cpp

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

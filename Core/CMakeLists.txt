# Core

cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CACHE{CORE_LIB_VERSION})
	string(TIMESTAMP VER_YEAR "%y")
	string(TIMESTAMP VER_MONTH "%m")
	math(EXPR VER_MONTH "${VER_MONTH} + 0")
    set(CORE_LIB_VERSION "${VER_YEAR}.${VER_MONTH}" CACHE STRING "Project version (auto-set at first configure)")
endif()

project(Core
	VERSION ${CORE_LIB_VERSION}
	LANGUAGES CXX
)

set(CORE_LIB_NAME Core)

add_subdirectory(Common)
add_subdirectory(Mathematics)
add_subdirectory(Imaging)
add_subdirectory(Graphics)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Null.cpp "")

add_library(${CORE_LIB_NAME}
    SHARED
        ${CMAKE_CURRENT_BINARY_DIR}/Null.cpp
)

target_link_libraries(${CORE_LIB_NAME}
    PRIVATE
        -Wl,--whole-archive
        ${IMAGING_LIB_NAME}
        ${GRAPHICS_LIB_NAME}
        -Wl,--no-whole-archive
)

##add_library(${IMAGING_LIB_NAME} OBJECT "${sourcesA}")
##add_library(${GRAPHICS_LIB_NAME} OBJECT "${sourcesB}")
#add_library(${CORE_LIB_NAME} SHARED $<TARGET_OBJECTS:${IMAGING_LIB_NAME}> $<TARGET_OBJECTS:${GRAPHICS_LIB_NAME}>)

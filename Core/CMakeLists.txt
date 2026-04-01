cmake_minimum_required(VERSION 3.25)

project(Core LANGUAGES CXX)

add_subdirectory(Common)
add_subdirectory(Mathematics)
add_subdirectory(Imaging)
add_subdirectory(Graphics)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Null.cpp "")

set(CORE_LIB_NAME "Core")
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

cmake_minimum_required(VERSION 3.25)

project(Common LANGUAGES CXX)
set(COMMON_LIB_NAME Common)

add_library(${COMMON_LIB_NAME} INTERFACE)

target_include_directories(${COMMON_LIB_NAME} 
	INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:.>
)

target_compile_features(${COMMON_LIB_NAME} 
    INTERFACE 
        cxx_std_20
)

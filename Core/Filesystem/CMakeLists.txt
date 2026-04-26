# Filesystem

cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CACHE{FILESYSTEM_LIB_VERSION})
	string(TIMESTAMP VER_YEAR "%y")
	string(TIMESTAMP VER_MONTH "%m")
	math(EXPR VER_MONTH "${VER_MONTH} + 0")
    set(FILESYSTEM_LIB_VERSION "${VER_YEAR}.${VER_MONTH}" CACHE STRING "Project version (auto-set at first configure)")
endif()

project(Filesystem
	VERSION ${FILESYSTEM_LIB_VERSION}
	LANGUAGES CXX
)

set(FILESYSTEM_LIB_NAME Filesystem)

add_library(${FILESYSTEM_LIB_NAME} STATIC)

target_sources(${FILESYSTEM_LIB_NAME}
	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
		#BASE_DIRS Source
    	FILES
			Endianness.hpp
			Filesystem.hpp
			Path.hpp
)

target_compile_features(${FILESYSTEM_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

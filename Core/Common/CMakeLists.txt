# Common

cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CACHE{COMMON_LIB_VERSION})
	string(TIMESTAMP VER_YEAR "%y")
	string(TIMESTAMP VER_MONTH "%m")
	math(EXPR VER_MONTH "${VER_MONTH} + 0")
    set(COMMON_LIB_VERSION "${VER_YEAR}.${VER_MONTH}" CACHE STRING "Project version (auto-set at first configure)")
endif()

project(Common
	VERSION ${COMMON_LIB_VERSION}
	LANGUAGES CXX
)

set(COMMON_LIB_NAME Common)

add_library(${COMMON_LIB_NAME} INTERFACE)

target_include_directories(${COMMON_LIB_NAME} 		# to remove after moving header files to Include folder
	INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set(COMMON_HEADERS
	Inlude/Common/Collections/ComponentVector.hpp
	Inlude/Common/Collections/Hash.inl
	Inlude/Common/Collections/ObservableSequentialCollection.hpp
	Inlude/Common/Collections/ObservableVector.hpp
	Inlude/Common/Collections/UnorderedVector.hpp
	Inlude/Common/Strings/NameString.hpp
	Inlude/Common/Strings/PathString.hpp
	Inlude/Common/Strings/Strings.hpp
)

target_sources(${COMMON_LIB_NAME}
	INTERFACE
    	FILE_SET HEADERS
		BASE_DIRS Include
    	FILES
			${COMMON_HEADERS}
)

#source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Include" FILES ${COMMON_HEADERS})

target_compile_features(${COMMON_LIB_NAME} 
    INTERFACE 
        cxx_std_20
)

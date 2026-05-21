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

add_subdirectory(observable_unique_ptr)
add_subdirectory(intrusive_shared_ptr)
add_subdirectory(sigslot)
add_subdirectory(json)

add_library(${COMMON_LIB_NAME} INTERFACE)
add_library(Core::${COMMON_LIB_NAME} ALIAS ${COMMON_LIB_NAME})
#set_target_properties(${COMMON_LIB_NAME} PROPERTIES EXPORT_NAME Core::${COMMON_LIB_NAME})

#target_link_libraries(${COMMON_LIB_NAME} INTERFACE oup::oup)
#target_link_libraries(${COMMON_LIB_NAME} INTERFACE isptr::isptr)
target_link_libraries(${COMMON_LIB_NAME} INTERFACE Pal::Sigslot)

set(COMMON_HEADERS
	Include/Common/Collections/Clonable.inl
	Include/Common/Collections/ComponentVector.hpp
	Include/Common/Collections/Concepts.hpp
	Include/Common/Collections/Hash.inl
	Include/Common/Collections/ObservableSequentialCollection.hpp
	Include/Common/Collections/ObservableVector.hpp
	Include/Common/Collections/UnorderedVector.hpp
	Include/Common/Strings/LocalString.hpp
	Include/Common/Strings/NameString.hpp
	Include/Common/Strings/PathString.hpp
	Include/Common/Strings/Strings.hpp
	Include/Common/Endianness.hpp
	Include/Common/StreamCaptureBuffer.hpp
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

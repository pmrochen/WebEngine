# Filesystem

cmake_minimum_required(VERSION 3.25)

include(FetchContent)

FetchContent_Declare(zlib
		GIT_REPOSITORY https://github.com/madler/zlib.git
		GIT_TAG v1.3.2
)

FetchContent_Declare(zlib-ng
		GIT_REPOSITORY https://github.com/zlib-ng/zlib-ng.git
		GIT_TAG 2.3.3
)

FetchContent_Declare(minizip-ng
		GIT_REPOSITORY https://github.com/zlib-ng/minizip-ng.git
		GIT_TAG 4.2.2
)

FetchContent_Declare(cereal
		GIT_REPOSITORY https://github.com/USCiLab/cereal.git
		GIT_TAG v1.3.2
)

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

option(FILESYSTEM_USE_ZLIB_NG "Use zlib-ng instead of zlib" ON)

if(FILESYSTEM_USE_ZLIB_NG)
	#FetchContent_MakeAvailable(zlib-ng)
	#FetchContent_MakeAvailable(minizip-ng)
	#configure_file(${zlib-ng_SOURCE_DIR}/zconf.h.in ${zlib-ng_SOURCE_DIR}/zconf.h COPYONLY)
	#configure_file(${zlib-ng_SOURCE_DIR}/zlib_name_mangling.h.empty Source/Filesystem/zlib_name_mangling.h COPYONLY)
	#configure_file(${zlib-ng_SOURCE_DIR}/zlib.h.in Source/Filesystem/zlib.h @ONLY)
else()
	#FetchContent_MakeAvailable(zlib)
endif()

add_library(${FILESYSTEM_LIB_NAME} STATIC)
add_library(Core::${FILESYSTEM_LIB_NAME} ALIAS ${FILESYSTEM_LIB_NAME})
#set_target_properties(${FILESYSTEM_LIB_NAME} PROPERTIES EXPORT_NAME Core::${FILESYSTEM_LIB_NAME})

target_link_libraries(${FILESYSTEM_LIB_NAME} INTERFACE Core::Common)

target_sources(${FILESYSTEM_LIB_NAME}
	PRIVATE
		Source/Filesystem/BufferedFile.cpp
		Source/Filesystem/File.cpp
		Source/Filesystem/FilesystemError.cpp
		Source/Filesystem/LowLevelFilesystem.cpp
		Source/Filesystem/MemoryFilesystem.cpp
		Source/Filesystem/NativeFilesystem.cpp
		Source/Filesystem/VirtualFilesystem.cpp

	PUBLIC
    	FILE_SET publicHeaders
	    TYPE HEADERS
		BASE_DIRS Source
    	FILES
			Source/Filesystem/BufferedFile.hpp
			Source/Filesystem/File.hpp
			Source/Filesystem/FileDataException.hpp
			Source/Filesystem/FileAccessFlags.hpp
			Source/Filesystem/FileOpenMode.hpp
			Source/Filesystem/Filesystem.hpp
			Source/Filesystem/FilesystemError.hpp
			Source/Filesystem/FilesystemException.hpp
			Source/Filesystem/LowLevelFilesystem.hpp
			Source/Filesystem/MemoryFilesystem.hpp
			Source/Filesystem/NativeFilesystem.hpp
			Source/Filesystem/Path.hpp
			Source/Filesystem/SeekOrigin.hpp
			Source/Filesystem/VirtualFilesystem.hpp
)

target_compile_features(${FILESYSTEM_LIB_NAME} 
    PUBLIC 
        cxx_std_20
)

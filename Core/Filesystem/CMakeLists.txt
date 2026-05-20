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

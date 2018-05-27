cmake_minimum_required(VERSION 2.8)

project(cppsupport)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(_CD ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${_CD})

set(general_sources
    "${_CD}/NumberFormat.cpp"
    "${_CD}/HashGenerator.cpp"
    "${_CD}/SHA1Generator.cpp"
    "${_CD}/sha1.cpp")
set(filesystem_sources
    "${_CD}/FileSystem/FileSystemEntry.cpp"
    "${_CD}/FileSystem/File.cpp"
    "${_CD}/FileSystem/Directory.cpp"
    "${_CD}/FileSystem/Path.cpp")

set(sources
    ${general_sources})
set(sources
    ${sources}
    ${filesystem_sources})

if(WIN32)
    add_definitions(-DWINDOWS)
endif()
if(APPLE)
    add_definitions(-DAPPLE)
endif()
if(UNIX AND NOT APPLE)
    add_definitions(-DUNIX)
endif()

add_library(cppsupport ${sources})


#if(CMAKE_COMPILER_IS_GNUCXX)
#	set(CMAKE_CXX_FLAGS "-fpermissive")
#endif()

#if(NOT DEFINED CMAKE_INSTALL_PREFIX)
#	set(CMAKE_INSTALL_PREFIX "/usr/local/StdUtils")
#endif()

#install(DIRECTORY "StdUtils"
#	DESTINATION "include")
#install(TARGETS "StdUtils"
#	ARCHIVE DESTINATION "lib")

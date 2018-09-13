cmake_minimum_required(VERSION 2.8)

if(${IWYU})
include(cmake/iwyu.cmake)
endif()

project(addp CXX)

include(cmake/version.cmake)
include(cmake/packaging.cmake)
include(cmake/testing.cmake)

message("addp version: " ${ADDP_VERSION})

# FIXME: static linking triggers multiple definitions errors when using Clang
option(STATIC_LINKING "Generate standalone, statically linked, executable" OFF)

if( STATIC_LINKING )
  set(Boost_USE_STATIC_LIBS ON)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libstdc++")
endif()

if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE -O2)

add_subdirectory(src)

cmake_minimum_required(VERSION 3.8)

project(addp CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

enable_testing()

add_subdirectory(src/addp)

option(ADDP_CLIENT_APP "Build client application" ON)
if( ADDP_CLIENT_APP )
  add_subdirectory(src/addpc)
endif()

if( NOT WIN32 )
  option(ADDP_SERVER "Build server" ON)
  add_subdirectory(src/addpd)
endif()

add_subdirectory(src/unittest)

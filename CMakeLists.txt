cmake_minimum_required(VERSION 3.25)

project(addp CXX)

enable_testing()

add_subdirectory(src/addp)

option(ADDP_CLIENT_APP "Build client application" ON)
if( ADDP_CLIENT_APP )
  add_subdirectory(src/addpc)
endif()

add_subdirectory(src/unittest)

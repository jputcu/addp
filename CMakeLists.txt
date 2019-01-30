cmake_minimum_required(VERSION 3.8)

project(addp CXX)

enable_testing()

add_subdirectory(src/addp)
add_subdirectory(src/addpc)
if( NOT WIN32 )
  add_subdirectory(src/addpd)
endif()
add_subdirectory(src/unittest)

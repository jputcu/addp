cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

add_executable(example example.cpp)

find_package(addp)
target_link_libraries(example PRIVATE addp::addp)


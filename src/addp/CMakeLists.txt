file(GLOB_RECURSE SRC *.cpp)

add_library(addp_lib ${SRC})
set_target_properties(addp_lib PROPERTIES OUTPUT_NAME addp)
target_include_directories(addp_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )

find_package(Boost COMPONENTS system program_options filesystem)
target_link_libraries(addp_lib PUBLIC Boost::system Boost::program_options Boost::filesystem)

if( WIN32 )
  target_link_libraries(addp_lib PRIVATE ws2_32)
endif()

add_library(addp::addp ALIAS addp_lib)

install(TARGETS addp_lib
    DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp")

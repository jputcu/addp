add_library(addp_lib
        action/action.cpp
        packet/field.cpp
        packet/packet.cpp
        types.cpp
        )
set_target_properties(addp_lib PROPERTIES OUTPUT_NAME addp)
target_include_directories(addp_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(addp_lib PUBLIC Boost::system)

if( WIN32 )
  target_link_libraries(addp_lib PRIVATE ws2_32)
endif()

target_compile_options(addp_lib PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)

add_library(addp::addp ALIAS addp_lib)

install(TARGETS addp_lib
    DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp")

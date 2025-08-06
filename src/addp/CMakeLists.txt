add_library(addp_lib)
add_library(addp::addp ALIAS addp_lib)

target_sources(addp_lib
        PRIVATE
        action.cpp
        field.cpp
        packet.cpp
        types.cpp
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
        FILES
        action.hpp
        addp.hpp
        field.hpp
        packet.hpp
        types.hpp
        version.hpp
)
set_target_properties(addp_lib PROPERTIES OUTPUT_NAME addp)

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(addp_lib PUBLIC Boost::system)

if( WIN32 )
  target_link_libraries(addp_lib PRIVATE ws2_32)
endif()

target_compile_options(addp_lib PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)

install(TARGETS addp_lib
    DESTINATION lib
    FILE_SET HEADERS
)

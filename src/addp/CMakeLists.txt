set(LIB addp)
file(GLOB_RECURSE SRC *.cpp)

find_package(Boost
  REQUIRED
  COMPONENTS system program_options filesystem thread)

add_library(${LIB} ${SRC})
target_link_libraries(${LIB}
  Boost::system
  Boost::program_options
  Boost::filesystem
  Boost::thread)

if( WIN32 )
  target_link_libraries(${LIB} Ws2_32)
endif()

set_target_properties(${LIB} PROPERTIES
    VERSION ${ADDP_VERSION}
    SOVERSION ${ADDP_VERSION}
    )

install(TARGETS ${LIB}
    DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.h")

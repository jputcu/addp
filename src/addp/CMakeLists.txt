set(LIB addp)
file(GLOB_RECURSE SRC *.cpp)

add_library(${LIB} ${SRC})
target_include_directories(${LIB}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )
target_link_libraries(${LIB} CONAN_PKG::boost)

if( WIN32 )
  target_link_libraries(${LIB} ws2_32)
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

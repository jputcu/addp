set(BIN addpd)
file(GLOB_RECURSE SRC *.cpp)
add_executable(${BIN} ${SRC})

target_link_libraries(${BIN} addp
    CONAN_PKG::boost
    )

install(TARGETS ${BIN} DESTINATION bin)

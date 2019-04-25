set(BIN addpd)
file(GLOB_RECURSE SRC *.cpp)
add_executable(${BIN} ${SRC})

find_package(Boost REQUIRED COMPONENTS thread)
target_link_libraries(${BIN} addp
    Boost::thread
    )

install(TARGETS ${BIN} DESTINATION bin)

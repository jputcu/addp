add_executable(addpc main.cpp client.cpp options.cpp)
target_link_libraries(addpc PRIVATE -static addp::addp)
install(TARGETS addpc DESTINATION bin)

add_executable(addpc main.cpp client.cpp options.cpp)
target_link_libraries(addpc PRIVATE -static addp::addp)
target_compile_options(addpc PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
install(TARGETS addpc DESTINATION bin)

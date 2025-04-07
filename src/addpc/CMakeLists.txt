add_executable(addpc
        main.cpp
        client.cpp
        options.cpp)

find_package(Boost REQUIRED COMPONENTS program_options filesystem)

target_link_libraries(addpc PRIVATE Boost::filesystem Boost::program_options -static addp::addp)
target_compile_options(addpc PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
install(TARGETS addpc DESTINATION bin)

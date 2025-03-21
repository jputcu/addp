find_package(Boost COMPONENTS unit_test_framework)

add_executable(addp_tests tests.cpp)
target_link_libraries(addp_tests PRIVATE addp::addp Boost::unit_test_framework)
target_compile_options(addp_tests PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
add_test(NAME addp_tests COMMAND addp_ip_tests)

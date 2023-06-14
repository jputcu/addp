find_package(Boost COMPONENTS unit_test_framework)

add_executable(addp_ip_tests ip_address_io.cpp)
target_link_libraries(addp_ip_tests PRIVATE addp::addp Boost::unit_test_framework)
add_test(NAME addp_ip_tests COMMAND addp_ip_tests)

add_executable(addp_mac_tests mac_address_io.cpp)
target_link_libraries(addp_mac_tests PRIVATE addp::addp Boost::unit_test_framework)
add_test(NAME addp_mac_tests COMMAND addp_mac_tests)


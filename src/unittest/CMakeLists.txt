set(ADDP_TEST_IP_BIN addp_ip_tests)
add_executable(${ADDP_TEST_IP_BIN}
    ip_address_io.cpp)
target_link_libraries(${ADDP_TEST_IP_BIN}
    addp
    CONAN_PKG::boost)
add_test(NAME ${ADDP_TEST_IP_BIN} COMMAND ${ADDP_TEST_IP_BIN})

set(ADDP_TEST_MAC_BIN addp_mac_tests)
add_executable(${ADDP_TEST_MAC_BIN}
    mac_address_io.cpp)
target_link_libraries(${ADDP_TEST_MAC_BIN}
    addp
    CONAN_PKG::boost)
add_test(NAME ${ADDP_TEST_MAC_BIN} COMMAND ${ADDP_TEST_MAC_BIN})

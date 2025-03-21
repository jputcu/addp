#ifndef ADDP_CONSTANTS_H
#define ADDP_CONSTANTS_H

#include <cstdint>
#include <string>

#include <addp/types.hpp>

namespace addp {

constexpr std::string MCAST_IP_ADDRESS = "224.0.5.128";
constexpr uint16_t UDP_PORT = 2362;
const mac_address MAC_ADDR_BROADCAST{"ff:ff:ff:ff:ff:ff"};
constexpr ssize_t MAX_UDP_MESSAGE_LEN = 4096;
constexpr std::string DEFAULT_PASSWORD = "dbps";
constexpr size_t DEFAULT_TIMEOUT = 2000;
constexpr size_t DEFAULT_MAX_COUNT = 0;

} // namespace addp

#endif // ADDP_CONSTANTS_H

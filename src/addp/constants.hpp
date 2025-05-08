#ifndef ADDP_CONSTANTS_H
#define ADDP_CONSTANTS_H

#include <cstdint>
#include <string>

namespace addp {

constexpr std::string MCAST_IP_ADDRESS = "224.0.5.128";
constexpr uint16_t UDP_PORT = 2362;
const std::string MAC_ADDR_ALL = "ff:ff:ff:ff:ff:ff";
constexpr ssize_t MAX_UDP_MESSAGE_LEN = 4096;
constexpr std::string DEFAULT_PASSWORD = "dbps";
constexpr size_t DEFAULT_TIMEOUT = 1000;

} // namespace addp

#endif // ADDP_CONSTANTS_H

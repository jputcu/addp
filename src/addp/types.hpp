#ifndef ADDP_TYPES_H
#define ADDP_TYPES_H

#include <array>
#include <cstdint>
#include <string>

namespace addp {

typedef std::array<uint8_t, 4> ip_address;
typedef std::array<uint8_t, 6> mac_address;
typedef std::array<uint8_t, 16> guid;

mac_address parse_mac_str(const std::string &mac_str);

ip_address parse_ip_str(const std::string &ip_str);

} // namespace addp

#endif // ADDP_TYPES_H

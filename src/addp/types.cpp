#include "types.hpp"
#include "types_io.hpp"

#include <sstream>

namespace addp {

mac_address parse_mac_str(const std::string &mac_str) {
  std::istringstream is(mac_str);
  mac_address mac_addr;
  is >> mac_addr;
  return mac_addr;
}

ip_address parse_ip_str(const std::string &ip_str) {
  std::istringstream is(ip_str);
  ip_address ip_addr;
  is >> ip_addr;
  return ip_addr;
}

} // namespace addp

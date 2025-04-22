#ifndef ADDP_TYPES_H
#define ADDP_TYPES_H

#include <array>
#include <string>
#include <string_view>
#include <iosfwd>

namespace addp {

struct mac_address : std::array<uint8_t, 6> {
  mac_address() = default;
  mac_address(std::string_view mac_str);

  // "ff:ff:ff:ff:ff:ff" -> "ffffff:ffffff"
  // Like printed on the DIGI itself
  static std::string Compact(std::string const &mac_str) {
    auto mac = mac_str;
    mac.erase(2, 1);
    mac.erase(4, 1);
    mac.erase(9, 1);
    mac.erase(11, 1);
    return mac;
  }
};

struct guid : std::array<uint8_t, 16> {};

std::ostream &operator<<(std::ostream&, const mac_address &);
std::ostream &operator<<(std::ostream&, const guid &);

} // namespace addp

#endif // ADDP_TYPES_H

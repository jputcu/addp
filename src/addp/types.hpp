#ifndef ADDP_TYPES_H
#define ADDP_TYPES_H

#include <array>
#include <string>
#include <iosfwd>

namespace addp {

struct mac_address : std::array<uint8_t, 6> {
  mac_address() = default;
  mac_address(std::string const &mac_str);
};

struct guid : std::array<uint8_t, 16> {};

std::ostream &operator<<(std::ostream &os, const mac_address &mac_addr);
std::istream &operator>>(std::istream &is, mac_address &mac_addr);
std::ostream &operator<<(std::ostream &os, const guid &guid);

} // namespace addp

#endif // ADDP_TYPES_H

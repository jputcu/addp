#ifndef ADDP_TYPES_H
#define ADDP_TYPES_H

#include <array>
#include <string>
#include <string_view>
#include <iosfwd>

namespace addp {

struct mac_address : std::array<uint8_t, 6> {
  mac_address() = default;

  // Accept:
  // * xx:xx:xx:xx:xx:xx e.g. used in Linux
  // * XX-XX-XX-XX-XX-XX e.g. used in Windows
  // * XXXXXX:XXXXXX e.g. found on the Digi label
  explicit mac_address(std::string_view mac_str);

  std::string ToString() const;

  // "XXXXXX:XXXXXX"
  std::string ToDigiMac() const;
};

struct guid : std::array<uint8_t, 16> {};

std::ostream &operator<<(std::ostream&, const mac_address &);
std::ostream &operator<<(std::ostream&, const guid &);

} // namespace addp

#endif // ADDP_TYPES_H

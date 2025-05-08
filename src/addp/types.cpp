#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/asio/ip/address_v4.hpp>

#include "types.hpp"
using namespace addp;

mac_address::mac_address(std::string_view mac_str) {
  auto ParseByte = [](std::string_view str) {
    const char two_char_str[] = {str[0], str[1], '\0'};
    return boost::numeric_cast<uint8_t>(std::stoi(two_char_str, nullptr, 16));
  };

  if (mac_str.size() == std::strlen("xx:xx:xx:xx:xx:xx") &&
          ((mac_str[2] == ':' && mac_str[5] == ':' && mac_str[8] == ':' && mac_str[11] == ':' &&
           mac_str[14] == ':') ||
      (mac_str[2] == '-' && mac_str[5] == '-' && mac_str[8] == '-' && mac_str[11] == '-' &&
       mac_str[14] == '-'))) {
    auto my_data = this->data();
    my_data[0] = ParseByte(mac_str.substr(0, 2));
    my_data[1] = ParseByte(mac_str.substr(3, 2));
    my_data[2] = ParseByte(mac_str.substr(6, 2));
    my_data[3] = ParseByte(mac_str.substr(9, 2));
    my_data[4] = ParseByte(mac_str.substr(12, 2));
    my_data[5] = ParseByte(mac_str.substr(15, 2));
  } else if (mac_str.size() == std::strlen("xxxxxx:xxxxxx") && mac_str[6] == ':') {
    auto my_data = this->data();
    my_data[0] = ParseByte(mac_str.substr(0, 2));
    my_data[1] = ParseByte(mac_str.substr(2, 2));
    my_data[2] = ParseByte(mac_str.substr(4, 2));
    my_data[3] = ParseByte(mac_str.substr(7, 2));
    my_data[4] = ParseByte(mac_str.substr(9, 2));
    my_data[5] = ParseByte(mac_str.substr(11, 2));
  } else
    throw boost::bad_lexical_cast();
}

std::string mac_address::ToString() const {
  boost::format fmt("%02x:%02x:%02x:%02x:%02x:%02x");
  for (auto b : *this)
    fmt % unsigned{b};
  return fmt.str();
}

std::string mac_address::ToDigiMac() const {
  boost::format fmt("%02X%02X%02X:%02X%02X%02X");
  for (auto b : *this)
    fmt % unsigned{b};
  return fmt.str();
}

std::ostream &addp::operator<<(std::ostream &os, const mac_address &mac_addr) {
  os << mac_addr.ToString();
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const guid &guid) {
  boost::format fmt("%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x-%02x%02x%02x%02x");
  for (auto b : guid)
    fmt % unsigned{b};
  os << fmt;
  return os;
}

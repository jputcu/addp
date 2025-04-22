#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <iomanip>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "types.hpp"
using namespace addp;

mac_address::mac_address(std::string_view mac_str) {
  std::vector<std::string> tokens;
  boost::algorithm::split(tokens, mac_str, boost::is_any_of(":"));

  if (tokens.size() != size())
    throw boost::bad_lexical_cast();

  for (size_t i = 0; i < size(); i++) {
    // parse hex values
    std::stringstream ss;
    ss << std::hex << tokens[i];

    int value;
    ss >> value;
    // convert them to one byte, checking overflow
    this->operator[](i) = boost::numeric_cast<uint8_t>(value);

    // make sure there's no trailing garbage
    if (!ss.eof())
      throw boost::bad_lexical_cast();
  }
}

std::ostream &addp::operator<<(std::ostream &os, const mac_address &mac_addr) {
  boost::format fmt("%02x:%02x:%02x:%02x:%02x:%02x");
  for (auto b : mac_addr)
    fmt % unsigned{b};
  os << fmt;
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const guid &guid) {
  boost::format fmt("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x");
  for (auto b : guid)
    fmt % unsigned{b};
  os << fmt;
  return os;
}

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <vector>

#include "types.hpp"
using namespace addp;

mac_address::mac_address(std::string_view mac_str) {
  std::vector<std::string> tokens;
  boost::algorithm::split(tokens, mac_str, boost::is_any_of(":"));
  if (tokens.size() != size())
    throw boost::bad_lexical_cast();

  std::transform(tokens.cbegin(), tokens.cend(), begin(), [](auto const &h_str) {
    size_t pos = 0;
    const auto value = std::stoi(h_str, &pos, 16);
    if (pos != h_str.size()) // make sure there's no trailing garbage
      throw boost::bad_lexical_cast();
    return boost::numeric_cast<uint8_t>(value);
  });
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

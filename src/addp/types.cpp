#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <iomanip>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "types.hpp"
using namespace addp;

mac_address addp::parse_mac_str(const std::string &mac_str) {
  std::istringstream is(mac_str);
  mac_address mac_addr;
  is >> mac_addr;
  return mac_addr;
}

ip_address addp::parse_ip_str(const std::string &ip_str) {
  std::istringstream is(ip_str);
  ip_address ip_addr;
  is >> ip_addr;
  return ip_addr;
}

std::ostream &addp::operator<<(std::ostream &os, const ip_address &ip_addr) {
  os << std::dec;
  for (size_t i = 0; i < ip_addr.size(); ++i)
    os << (i ? "." : "") << static_cast<int>(ip_addr[i]);
  return os;
}

std::istream &addp::operator>>(std::istream &is, ip_address &ip_addr) {
  std::string str;
  is >> str;

  std::vector<std::string> tokens;
  boost::algorithm::split(tokens, str, boost::is_any_of("."));

  if (tokens.size() != ip_addr.size())
    throw boost::bad_lexical_cast();

  for (size_t i = 0; i < ip_addr.size(); i++)
    ip_addr[i] = boost::numeric_cast<uint8_t>(boost::lexical_cast<int>(tokens[i]));

  return is;
}

std::ostream &addp::operator<<(std::ostream &os, const mac_address &mac_addr) {
  os << std::hex << std::setfill('0') << std::nouppercase;

  for (size_t i = 0; i < mac_addr.size(); ++i)
    os << (i ? ":" : "") << std::setw(2) << boost::numeric_cast<int>(mac_addr[i]);

  return os;
}

std::istream &addp::operator>>(std::istream &is, mac_address &mac_addr) {
  std::string str;
  is >> str;

  std::vector<std::string> tokens;
  boost::algorithm::split(tokens, str, boost::is_any_of(":"));

  if (tokens.size() != mac_addr.size())
    throw boost::bad_lexical_cast();

  for (size_t i = 0; i < mac_addr.size(); i++) {
    int value;

    // parse hex values
    std::stringstream ss;
    ss << std::hex << tokens[i];
    ss >> value;

    // convert them to one byte, checking overflow
    mac_addr[i] = boost::numeric_cast<uint8_t>(value);

    // make sure there's no trailing garbage
    if (!ss.eof())
      throw boost::bad_lexical_cast();
  }

  return is;
}

std::ostream &addp::operator<<(std::ostream &os, const guid &guid) {
  os << std::hex << std::setfill('0');

  for (size_t i = 0; i < guid.size(); ++i) {
    switch (i) {
    case 4:
    case 4 + 2:
    case 4 + 2 + 2:
    case 4 + 2 + 2 + 2:
      os << "-";
      break;
    default:
      break;
    }
    os << std::setw(2) << static_cast<int>(guid[i]);
  }

  return os;
}

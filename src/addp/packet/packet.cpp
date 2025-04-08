#include "packet.hpp"

#include <algorithm>
#include <boost/format.hpp>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>
#include <iomanip>
using namespace addp;

packet::packet(const uint8_t *begin_it, const uint8_t *end_it) {
  if ( static_cast<size_t>(std::distance(begin_it, end_it)) >= sizeof(_header) ) {
    // header
    std::memcpy(&_header, begin_it, sizeof(_header));
    begin_it += sizeof(_header);

    // payload
    _payload.assign(begin_it, end_it);
  }
}

void packet::add(const mac_address &data) {
  std::copy(data.cbegin(), data.cend(), std::back_inserter(_payload));
  _header.size = htons(static_cast<u_short>(_payload.size()));
}

void packet::add(const ip_address &data) {
  std::copy(data.cbegin(), data.cend(), std::back_inserter(_payload));
  _header.size = htons(static_cast<u_short>(_payload.size()));
}

void packet::add(const std::string &str) {
  // 1 byte length
  _payload.push_back(static_cast<uint8_t>(str.size()));

  const auto data = reinterpret_cast<const uint8_t *>(str.data());
  std::copy_n(data, str.size(), std::back_inserter(_payload));
  _header.size = htons(static_cast<u_short>(_payload.size()));
}

std::vector<uint8_t> packet::raw() const {
  std::vector<uint8_t> buffer;
  buffer.reserve(sizeof(_header) + _payload.size());
  auto headerp = reinterpret_cast<const uint8_t *>(&_header);
  std::copy_n(headerp, sizeof(_header), std::back_inserter(buffer));
  std::copy(_payload.begin(), _payload.end(), std::back_inserter(buffer));
  return buffer;
}

std::ostream &addp::operator<<(std::ostream &os, const packet_type type) {
  switch (type) {
  case packet_type::NONE:
    os << "None";
    break;
  case packet_type::DISCOVERY_REQUEST:
    os << "Discovery Request";
    break;
  case packet_type::DISCOVERY_RESPONSE:
    os << "Discovery Response";
    break;
  case packet_type::STATIC_NET_CONFIG_REQUEST:
    os << "Net Config Request";
    break;
  case packet_type::STATIC_NET_CONFIG_RESPONSE:
    os << "Net Config Response";
    break;
  case packet_type::REBOOT_REQUEST:
    os << "Reboot Request";
    break;
  case packet_type::REBOOT_RESPONSE:
    os << "Reboot Response";
    break;
  case packet_type::DHCP_NET_CONFIG_REQUEST:
    os << "DHCP Net Config Request";
    break;
  case packet_type::DHCP_NET_CONFIG_RESPONSE:
    os << "DHCP Net Config Response";
    break;
  default:
    os << str(boost::format("unknown (0x%02x)") % static_cast<int>(type));
    break;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const packet &packet) {
  os << packet.type() << "\n";

  for (const auto &f : packet.fields())
    if (f.type() != field_type::none)
      os << "  " << f;

  if ( packet.type() == packet_type::DISCOVERY_REQUEST ) {
    os << std::hex << std::setfill('0');
    auto const &payload = packet.payload();
    for (size_t i = 0; i < payload.size(); ++i)
      os << (i ? ":" : " ") << static_cast<int>(payload[i]);
  }

  return os;
}

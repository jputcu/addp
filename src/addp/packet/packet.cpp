#include "packet.hpp"

#include <algorithm>
#include <boost/format.hpp>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>
#include <iomanip>

#include <addp/packet/field.hpp>
using namespace addp;

packet::packet(std::span<const uint8_t> const &data) {
  // header
  std::memcpy(&_header, data.data(), std::min(sizeof(_header), data.size()));

  // payload
  if (data.size() > sizeof(_header)) {
    _payload.clear();
    _payload.reserve(data.size() - sizeof(_header));
    std::copy_n(data.data() + sizeof(_header), data.size() - sizeof(_header), std::back_inserter(_payload));
  }
}

void packet::add(const field::bool_flag &data) {
  _payload.push_back(data);
  _header.size = htons(static_cast<u_short>(_payload.size()));
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

void packet::parse_fields() {
  auto iter = _payload.begin();
  const auto end = _payload.end();
  while (iter != end) {
    _fields.emplace_back(iter, end);
  }
}

std::string packet::packet_type2str(const Type type) {
  switch (type) {
  case Type::NONE:
    return "None";
  case Type::DISCOVERY_REQUEST:
    return "Discovery Request";
  case Type::DISCOVERY_RESPONSE:
    return "Discovery Response";
  case Type::STATIC_NET_CONFIG_REQUEST:
    return "Net Config Request";
  case Type::STATIC_NET_CONFIG_RESPONSE:
    return "Net Config Response";
  case Type::REBOOT_REQUEST:
    return "Reboot Request";
  case Type::REBOOT_RESPONSE:
    return "Reboot Response";
  case Type::DHCP_NET_CONFIG_REQUEST:
    return "DHCP Net Config Request";
  case Type::DHCP_NET_CONFIG_RESPONSE:
    return "DHCP Net Config Response";
  default:
    return str(boost::format("unknown (0x%02x)") % static_cast<int>(type));
  }
}

std::ostream &addp::operator<<(std::ostream &os, const packet &packet) {
  os << packet.type_str() << "\n";

  for (const auto &f : packet.fields())
    if (f.type() != field::FT_NONE)
      os << "  " << f;

  if ( packet.type() == packet::Type::DISCOVERY_REQUEST ) {
    os << std::hex << std::setfill('0');
    auto const &payload = packet.payload();
    for (size_t i = 0; i < payload.size(); ++i)
      os << (i ? ":" : " ") << static_cast<int>(payload[i]);
  }

  return os;
}

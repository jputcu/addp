#include "packet.hpp"

#include <algorithm>
#include <boost/format.hpp>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace addp;

request &request::add(const bool data) {
  auto it = _packet.out_it();
  *it++ = data ? field::BF_TRUE : field::BF_FALSE;
  _packet.update_payload_size(it);
  return *this;
}

request &request::add(const mac_address &data) {
  auto it = _packet.out_it();
  it = std::copy(data.cbegin(), data.cend(), it);
  _packet.update_payload_size(it);
  return *this;
}

request &request::add(const ip_address &data) {
  auto it = _packet.out_it();
  it = std::copy(data.cbegin(), data.cend(), it);
  _packet.update_payload_size(it);
  return *this;
}

request &request::add(const std::string &str) {
  // 1 byte length
  auto it = _packet.out_it();
  *it++ = static_cast<uint8_t>(str.size());

  const auto data = reinterpret_cast<const uint8_t *>(str.data());
  it = std::copy_n(data, str.size(), it);
  _packet.update_payload_size(it);
  return *this;
}

response::response(const uint8_t *begin_it, const uint8_t *end_it) {
  if (static_cast<size_t>(std::distance(begin_it, end_it)) >= sizeof(_header)) {
    // header
    {
      packet_header h;
      std::memcpy(&h, begin_it, sizeof(_header));
      begin_it += sizeof(_header);
      assert(ntohs(h.size) == std::distance(begin_it, end_it));
      _header = h;
    }

    // payload
    _payload.assign(begin_it, end_it);

    // parse fields
    {
      auto iter = _payload.begin();
      const auto end = _payload.end();
      while (iter != end) {
        field f{iter, end};
        _fields.emplace(f.type(), f);
      }
    }
  }
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

std::ostream &addp::operator<<(std::ostream &os, const request &packet) {
  os << packet.type() << "\n";
  if (packet.type() == packet_type::DISCOVERY_REQUEST) {
    mac_address mac{};
    std::copy(packet.payload().cbegin(), packet.payload().cend(), mac.begin());
    os << mac;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const response &packet) {
  os << packet.type() << "\n";
  for (const auto &f : packet.fields())
    os << "  " << f.second;
  return os;
}

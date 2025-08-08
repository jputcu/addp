#include "packet.hpp"

#include <algorithm>
#include <boost/format.hpp>
#include <iostream>
using namespace addp;

request &request::add_bool(const bool data) {
  auto it = _packet.out_it();
  *it++ = static_cast<std::byte>(data ? field::BF_TRUE : field::BF_FALSE);
  _packet.update_payload_size(it);
  return *this;
}

request &request::add_mac(const std::string_view mac_str) {
  const mac_address mac{mac_str};
  auto it = _packet.out_it();
  it = std::transform(mac.cbegin(), mac.cend(), it, [](auto b) { return std::byte{b}; });
  _packet.update_payload_size(it);
  return *this;
}

request &request::add_ip(const std::string_view ip_str) {
  auto ip = boost::asio::ip::make_address_v4(ip_str);
  auto it = _packet.out_it();
  const auto ip_bytes = ip.to_bytes();
  it = std::transform(ip_bytes.cbegin(), ip_bytes.cend(), it, [](auto b) { return std::byte{b}; });
  _packet.update_payload_size(it);
  return *this;
}

request &request::add_string(const std::string_view str) {
  // 1 byte length
  auto it = _packet.out_it();
  *it++ = static_cast<std::byte>(str.size());

  const auto data = reinterpret_cast<const std::byte *>(str.data());
  it = std::copy_n(data, str.size(), it);
  _packet.update_payload_size(it);
  return *this;
}

response::response(const std::byte *data, const size_t len) {
  if (len >= sizeof(_header)) {
    size_t payload_len = 0;
    // header
    {
      packet_header h;
      std::memcpy(&h, data, sizeof(_header));
      data += sizeof(_header);
      payload_len = ntohs(h.size);
      if (payload_len != len - sizeof(_header))
        throw std::runtime_error("packet len doesn't match embedded length");
      _header = h;
    }

    // payload
    _payload.assign(data, data + payload_len);

    // Parse
    {
      auto iter = _payload.cbegin();
      const auto end = _payload.cend();
      while (iter != end) {
        field f{iter, end};
        if ( f.type() == field_type::mac_addr )
          m_mac_address = std::get<mac_address>(f.value());
        else
          _fields.emplace(f.type(), f);
      }
      assert(mac() != mac_address{});
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
    os << boost::format("unknown (0x%02x)") % static_cast<int>(type);
    break;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const request &packet) {
  os << packet.type() << "\n";
  if (packet.type() == packet_type::DISCOVERY_REQUEST) {
    mac_address mac{};
    std::transform(packet.payload().cbegin(), packet.payload().cend(), mac.begin(),
                   [](auto b) { return std::to_integer<uint8_t>(b); });
    os << mac;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const response &packet) {
  os << packet.type() << "\n";
  os << "  " << field_type::mac_addr << " = " << packet.mac() << "\n";
  for (const auto &f : packet.fields())
    os << "  " << f.second << "\n";
  return os;
}

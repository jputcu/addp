#ifndef ADDP_PACKET_PACKET_H
#define ADDP_PACKET_PACKET_H

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <span>

#include <boost/asio.hpp>
#include <boost/core/span.hpp>

#include <addp/constants.hpp>
#include <addp/types.hpp>
#include "field.hpp"

namespace addp {

enum class packet_type : uint16_t {
  NONE = 0x0000,
  DISCOVERY_REQUEST = 0x0001,
  DISCOVERY_RESPONSE = 0x0002,
  STATIC_NET_CONFIG_REQUEST = 0x0003,
  STATIC_NET_CONFIG_RESPONSE = 0x0004,
  REBOOT_REQUEST = 0x0005,
  REBOOT_RESPONSE = 0x0006,
  DHCP_NET_CONFIG_REQUEST = 0x0007,
  DHCP_NET_CONFIG_RESPONSE = 0x0008
};

struct packet_header {
  char magic[4]{'D', 'I', 'G', 'I'};
  uint16_t type{};
  uint16_t size{};
};

class request {
public:
  packet_type type() const { return static_cast<packet_type>(ntohs(_header.type)); }

  request &add(const bool data) {
    _payload.push_back(data ? field::BF_TRUE : field::BF_FALSE);
    _header.size = htons(static_cast<u_short>(_payload.size()));
    return *this;
  }

  request &add(const mac_address &);
  request &add(const ip_address &);
  request &add(const std::string &);

  boost::span<const uint8_t> payload() const { return _payload; }

  std::vector<uint8_t> raw() const;

  static request discovery_request(mac_address const &mac = MAC_ADDR_BROADCAST) {
    return request(packet_type::DISCOVERY_REQUEST).add(mac);
  }

  static request static_net_config_request(const mac_address &mac, const ip_address &ip,
                                           const ip_address &subnet, const ip_address &gateway,
                                           const std::string &auth = DEFAULT_PASSWORD) {
    return request(packet_type::STATIC_NET_CONFIG_REQUEST)
        .add(ip)
        .add(subnet)
        .add(gateway)
        .add(mac)
        .add(auth);
  }

  static request reboot_request(const mac_address &mac = MAC_ADDR_BROADCAST,
                                const std::string &auth = DEFAULT_PASSWORD) {
    return request(packet_type::REBOOT_REQUEST).add(mac).add(auth);
  }

  static request dhcp_net_config_request(const mac_address &mac, bool enable,
                                         const std::string &auth = DEFAULT_PASSWORD) {
    return request(packet_type::DHCP_NET_CONFIG_REQUEST).add(enable).add(mac).add(auth);
  }

private:
  explicit request(packet_type type) { _header.type = htons(static_cast<u_short>(type)); }

  packet_header _header;
  std::vector<uint8_t> _payload;
};

class response {
public:
  explicit response(const uint8_t *begin_it, const uint8_t *end_it);

  bool check() const { return htons(static_cast<u_short>(_payload.size())) == _header.size; }

  packet_type type() const { return static_cast<packet_type>(ntohs(_header.type)); }

  boost::span<const uint8_t> payload() const { return _payload; }

  void parse_fields() {
    auto iter = _payload.begin();
    const auto end = _payload.end();
    while (iter != end) {
      _fields.emplace_back(iter, end);
    }
  }

  const std::vector<field> &fields() const { return _fields; }

private:
  packet_header _header;
  std::vector<uint8_t> _payload;
  std::vector<field> _fields;
};

std::ostream &operator<<(std::ostream &, packet_type);
std::ostream &operator<<(std::ostream &, const request &);
std::ostream &operator<<(std::ostream &, const response &);

} // namespace addp

#endif // ADDP_PACKET_PACKET_H

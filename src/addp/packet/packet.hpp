#ifndef ADDP_PACKET_PACKET_H
#define ADDP_PACKET_PACKET_H

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <span>

#include <boost/asio.hpp>

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

class packet {
public:
  explicit packet(const uint8_t *begin_it, const uint8_t *end_it);

  bool check() const { return htons(static_cast<u_short>(_payload.size())) == _header.size; }

  packet_type type() const { return static_cast<packet_type>(ntohs(_header.type)); }

  packet &add(const field::bool_flag data) {
    _payload.push_back(data);
    _header.size = htons(static_cast<u_short>(_payload.size()));
    return *this;
  }

  packet &add(const mac_address &);
  packet &add(const ip_address &);
  packet &add(const std::string &);

  const std::vector<uint8_t> &payload() const { return _payload; }

  std::vector<uint8_t> raw() const;

  void parse_fields() {
    auto iter = _payload.begin();
    const auto end = _payload.end();
    while (iter != end) {
      _fields.emplace_back(iter, end);
    }
  }

  const std::vector<field> &fields() const { return _fields; }

  static packet discovery_request(mac_address const &mac = MAC_ADDR_BROADCAST) {
    return packet(packet_type::DISCOVERY_REQUEST).add(mac);
  }

  static packet discovery_response() { return packet(packet_type::DISCOVERY_RESPONSE); }

  static packet static_net_config_request(const mac_address &mac, const ip_address &ip,
                                          const ip_address &subnet, const ip_address &gateway,
                                          const std::string &auth = DEFAULT_PASSWORD) {
    return packet(packet_type::STATIC_NET_CONFIG_REQUEST)
        .add(ip)
        .add(subnet)
        .add(gateway)
        .add(mac)
        .add(auth);
  }

  static packet static_net_config_response() {
    return packet(packet_type::STATIC_NET_CONFIG_RESPONSE);
  }

  static packet reboot_request(const mac_address &mac = MAC_ADDR_BROADCAST,
                               const std::string &auth = DEFAULT_PASSWORD) {
    return packet(packet_type::REBOOT_REQUEST).add(mac).add(auth);
  }

  static packet reboot_response() { return packet(packet_type::REBOOT_RESPONSE); }

  static packet dhcp_net_config_request(const mac_address &mac, bool enable,
                                        const std::string &auth = DEFAULT_PASSWORD) {
    return packet(packet_type::DHCP_NET_CONFIG_REQUEST)
        .add(enable ? field::BF_TRUE : field::BF_FALSE)
        .add(mac)
        .add(auth);
  }

  static packet dhcp_net_config_response() { return packet(packet_type::DHCP_NET_CONFIG_RESPONSE); }

private:
  explicit packet(packet_type type) { _header.type = htons(static_cast<u_short>(type)); }

  struct header {
    char magic[4]{'D', 'I', 'G', 'I'};
    uint16_t type{};
    uint16_t size{};
  };

  header _header;
  std::vector<uint8_t> _payload;
  std::vector<field> _fields;
};

std::ostream &operator<<(std::ostream &, packet_type);
std::ostream &operator<<(std::ostream &, const packet &);

} // namespace addp

#endif // ADDP_PACKET_PACKET_H

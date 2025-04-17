#ifndef ADDP_PACKET_PACKET_H
#define ADDP_PACKET_PACKET_H

#include <cstdint>
#include <vector>
#include <map>
#include <iosfwd>

#include <boost/asio.hpp>
#include <boost/core/span.hpp>
#include <boost/asio/ip/address_v4.hpp>

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
static_assert(sizeof(packet_header) == 8);

class request {
public:
  static request discover(mac_address const &mac = MAC_ADDR_ALL) {
    return request(packet_type::DISCOVERY_REQUEST).add(mac);
  }

  static request static_net_config(const mac_address &mac,
                                           const boost::asio::ip::address_v4 &ip,
                                           const boost::asio::ip::address_v4 &subnet,
                                           const boost::asio::ip::address_v4 &gateway,
                                           const std::string &auth = DEFAULT_PASSWORD) {
    return request(packet_type::STATIC_NET_CONFIG_REQUEST)
        .add(ip)
        .add(subnet)
        .add(gateway)
        .add(mac)
        .add(auth);
  }


  static request reboot(const mac_address &mac, const std::string &auth = DEFAULT_PASSWORD) {
    return request(packet_type::REBOOT_REQUEST).add(mac).add(auth);
  }

  static request dhcp_net_config(const mac_address &mac, bool enable,
                                         const std::string &auth = DEFAULT_PASSWORD) {
    return request(packet_type::DHCP_NET_CONFIG_REQUEST).add(enable).add(mac).add(auth);
  }

  packet_type type() const { return static_cast<packet_type>(ntohs(_packet.header.type)); }

  boost::span<const uint8_t> raw() const {
    return {reinterpret_cast<const uint8_t *>(&_packet),
            sizeof(packet_header) + _packet.payload_size()};
  }

  boost::span<const uint8_t> payload() const { return {_packet.payload, _packet.payload_size()}; }

private:
  explicit request(packet_type type) { _packet.header.type = htons(static_cast<u_short>(type)); }

  request &add(bool data);
  request &add(const mac_address &);
  request &add(const boost::asio::ip::address_v4 &);
  request &add(const std::string &);

  struct packet {
    static constexpr auto MAX_PAYLOAD_SIZE = 200;
    packet_header header;
    uint8_t payload[MAX_PAYLOAD_SIZE];

    size_t payload_size() const { return ntohs(header.size); }
    void update_payload_size(uint8_t *new_out_it) {
      const auto payload_len = std::distance(payload, new_out_it);
      assert(payload_len < MAX_PAYLOAD_SIZE);
      header.size = htons(static_cast<u_short>(payload_len));
    }
    uint8_t *out_it() { return payload + payload_size(); }
  };
  static_assert(sizeof(packet) == sizeof(packet_header) + packet::MAX_PAYLOAD_SIZE);

  packet _packet;
};

class response {
public:
  explicit response(const uint8_t *data, size_t len);

  packet_type type() const { return static_cast<packet_type>(ntohs(_header.type)); }

  std::map<field_type, field> fields() const {
    std::map<field_type, field> _fields;
    auto iter = _payload.cbegin();
    const auto end = _payload.cend();
    while (iter != end) {
      field f{iter, end};
      _fields.emplace(f.type(), f);
    }
    return _fields;
  }

private:
  packet_header _header;
  std::vector<uint8_t> _payload;
};

std::ostream &operator<<(std::ostream &, packet_type);
std::ostream &operator<<(std::ostream &, const request &);
std::ostream &operator<<(std::ostream &, const response &);

} // namespace addp

#endif // ADDP_PACKET_PACKET_H

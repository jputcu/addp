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

class packet {
public:
  enum class Type : uint16_t {
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

  struct header {
    char magic[4]{'D', 'I', 'G', 'I'};
    uint16_t type {};
    uint16_t size {};
  };

  explicit packet(Type type) { _header.type = htons(static_cast<u_short>(type)); }

  explicit packet(const uint8_t *begin_it, const uint8_t *end_it);

  bool check() const { return htons(static_cast<u_short>(_payload.size())) == _header.size; }

  Type type() const { return static_cast<Type>(ntohs(_header.type)); }

  std::string type_str() const { return packet_type2str(type()); }

  void add(const field::bool_flag data) {
    _payload.push_back(data);
    _header.size = htons(static_cast<u_short>(_payload.size()));
  }

  void add(const mac_address &);
  void add(const ip_address &);
  void add(const std::string &);

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

private:
  static std::string packet_type2str(Type type);

  header _header;
  std::vector<uint8_t> _payload;
  std::vector<field> _fields;
};

std::ostream &operator<<(std::ostream &os, const packet &packet);

struct discovery_request : packet {
  explicit discovery_request(mac_address const &mac = MAC_ADDR_BROADCAST)
      : packet(Type::DISCOVERY_REQUEST) {
    add(mac);
  }
};

struct discovery_response : packet {
  discovery_response() : packet(Type::DISCOVERY_RESPONSE) {}
};

struct static_net_config_request : packet {
  static_net_config_request(const mac_address &mac, const ip_address &ip, const ip_address &subnet,
                            const ip_address &gateway, const std::string &auth = DEFAULT_PASSWORD)
      : packet(Type::STATIC_NET_CONFIG_REQUEST) {
    add(ip);
    add(subnet);
    add(gateway);
    add(mac);
    add(auth);
  }
};

struct static_net_config_response : packet {
  static_net_config_response() : packet(Type::STATIC_NET_CONFIG_RESPONSE) {}
};

struct reboot_request : packet {
  explicit reboot_request(const mac_address &mac = MAC_ADDR_BROADCAST,
                          const std::string &auth = DEFAULT_PASSWORD)
      : packet(Type::REBOOT_REQUEST) {
    add(mac);
    add(auth);
  }
};

struct reboot_response : packet {
  reboot_response() : packet(Type::REBOOT_RESPONSE) {}
};

struct dhcp_net_config_request : packet {
  dhcp_net_config_request(const mac_address &mac, bool enable,
                          const std::string &auth = DEFAULT_PASSWORD)
      : packet(Type::DHCP_NET_CONFIG_REQUEST) {
    add(enable ? field::BF_TRUE : field::BF_FALSE);
    add(mac);
    add(auth);
  }
};

struct dhcp_net_config_response : packet {
  dhcp_net_config_response() : packet(Type::DHCP_NET_CONFIG_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_PACKET_H

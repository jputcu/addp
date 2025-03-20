#ifndef ADDP_PACKET_PACKET_H
#define ADDP_PACKET_PACKET_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>
#include <iosfwd>

#include <boost/asio.hpp>

#include <addp/types.hpp>
#include <addp/packet/field.hpp>

namespace addp {

class packet {
public:
  struct header {
    char magic[4] {'D','I','G','I'};
    uint16_t type;
    uint16_t size;
  };

  enum class Type {
    NONE,
    DISCOVERY_REQUEST,
    DISCOVERY_RESPONSE,
    STATIC_NET_CONFIG_REQUEST,
    STATIC_NET_CONFIG_RESPONSE,
    REBOOT_REQUEST,
    REBOOT_RESPONSE,
    DHCP_NET_CONFIG_REQUEST,
    DHCP_NET_CONFIG_RESPONSE,
  };

  explicit packet(Type type) { _header.type = htons(static_cast<int>(type)); }

  packet(const uint8_t *data, size_t len);

  bool check() const { return _payload.size() == _header.size; }

  Type type() const { return static_cast<Type>(ntohs(_header.type)); }

  std::string type_str() const { return packet_type2str(type()); }

  template <class T> void add(const T &);

  const std::vector<uint8_t> &payload() const { return _payload; }

  std::vector<uint8_t> raw() const;

  bool parse_fields();
  const std::vector<field> &fields() const { return _fields; }

private:
  static std::string packet_type2str(Type type);

  header _header;
  std::vector<uint8_t> _payload;
  std::vector<field> _fields;
};

std::ostream &operator<<(std::ostream &os, const packet &packet);

} // namespace addp

#endif // ADDP_PACKET_PACKET_H

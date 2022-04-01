#ifndef ADDP_PACKET_PACKET_H
#define ADDP_PACKET_PACKET_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <addp/packet/field.h>

namespace addp {

class packet {
public:
  static const char *MAGIC;

  struct packet_header {
    char magic[4];
    uint16_t type;
    uint16_t size;
    packet_header() { strncpy(magic, MAGIC, 4); }
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

  explicit packet(Type t);
  packet(const uint8_t *data, size_t len);

  bool check() const;

  Type type() const;
  std::string type_str() const;

  uint16_t size() const;
  template <class T> void add(const T &data);
  template <class T, std::size_t N> void add(const std::array<T, N> &data) {
    copy(data.begin(), data.end(), back_inserter(_payload));
    _header.size = htons(static_cast<u_short>(_payload.size()));
  }
  const std::vector<uint8_t> &payload() const;
  std::vector<uint8_t> raw() const;

  bool parse_fields();
  const std::vector<field> &fields() const;

private:
  static std::string packet_type2str(Type type);

  packet_header _header;
  std::vector<uint8_t> _payload;
  std::vector<field> _fields;
};

} // namespace addp

#endif // ADDP_PACKET_PACKET_H

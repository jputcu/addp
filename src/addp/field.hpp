#ifndef ADDP_PACKET_FIELD_H
#define ADDP_PACKET_FIELD_H

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <string>
#include <boost/core/span.hpp>
#include <cstring>
#include <cassert>

namespace addp {

enum class field_type : uint8_t {
  none = 0x00,
  mac_addr,        // 6 byte MAC address
  ip_addr,         // 4 byte IP address
  netmask,         // 4 byte netmask
  name,            // string name
  domain,          // domain
  hw_type,         // hardware type
  hw_rev,          // hardware Revision
  firmware,        // string firmware
  result_msg,      // string result message
  result_flag,     // 1 byte result flag
  gateway,         // 4 byte gateway IP
  conf_err_code,   // 2 byte configuration error code
  device,          // string device
  port,            // 4 byte port
  dns,             // 4 byte DNS IP
  dhcp,            // bool DHCP enabled
  err_code,        // 1 byte error code
  serial_count,    // 1 byte serial port count
  ssl_port,        // 4 byte encrypted port
  version,         // version ID
  vendor = 0x15,   // vendor GUID
  device_id = 0x1a // Device ID: '00000000-00000000-00409DFF-FF300000'
};

// view onto a response payload. Doesn't own the data
class field {
public:
  // FT_ERR_CODE
  enum error_code : uint8_t {
    EC_SUCCESS = 0x00,
    EC_AUTH,
    EC_INVALID = 0x03,
    EC_SAVE = 0x06,
  };

  // FT_RESULT_FLAG
  enum result_flag : uint8_t {
    RF_SUCCESS = 0x00,
    RF_ERROR = 0xff,
  };

  // FT_CONF_ERR_CODE
  enum class config_error : uint16_t {
    no_error = 0x0000,
    other_subnet = 0x0001, // Exclamation mark next to digi in digi discovery
  };

  enum bool_flag : uint8_t {
    BF_FALSE = 0x00,
    BF_TRUE = 0x01,
  };

  // Consumes the data: moves iter to the next field
  field(std::vector<uint8_t>::const_iterator &iter, const std::vector<uint8_t>::const_iterator &end);

  field_type type() const { return _type; }

  template <typename T> T as() const {
    T t;
    const auto payload_bytes = payload();
    assert(payload_bytes.size() == sizeof(T));
    std::memcpy(&t, payload_bytes.cbegin(), sizeof(T));
    return t;
  }

  std::ostream &value_str(std::ostream&) const;

  void stream_unknown_as_hex(std::ostream&) const;

  boost::span<const uint8_t> payload() const { return _payload; }

private:
  field_type _type;
  boost::span<const uint8_t> _payload;
};

std::ostream &operator<<(std::ostream &, field::error_code);
std::ostream &operator<<(std::ostream &, field::result_flag);
std::ostream &operator<<(std::ostream &, field::config_error);
std::ostream &operator<<(std::ostream &, field_type);
std::ostream &operator<<(std::ostream &, const field &);

} // namespace addp

#endif // ADDP_PACKET_FIELD_H

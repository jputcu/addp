#ifndef ADDP_PACKET_FIELD_H
#define ADDP_PACKET_FIELD_H

#include <cstdint>
#include <vector>
#include <iosfwd>
#include <algorithm>
#include <string>
#include <stdexcept>

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

class field {
public:
  struct header {
    field_type type{};
    uint8_t size{};
  };

  // FT_ERR_CODE
  enum error_code {
    EC_SUCCESS = 0x00,
    EC_AUTH,
    EC_INVALID = 0x03,
    EC_SAVE = 0x06,
  };

  // FT_RESULT_FLAG
  enum result_flag {
    RF_SUCCESS = 0x00,
    RF_ERROR = 0xff,
  };

  // FT_CONF_ERR_CODE
  enum config_error {
    CE_SUCCESS = 0x00,
    CE_ERROR, // Digi in different subnet than sender
  };

  // FT_DCHP
  enum bool_flag {
    BF_FALSE = 0x00,
    BF_TRUE = 0x01,
  };

  field(std::vector<uint8_t>::iterator &iter, const std::vector<uint8_t>::iterator &end) {
    // header
    std::copy_n(iter, sizeof(_header), reinterpret_cast<uint8_t *>(&_header));
    std::advance(iter, sizeof(_header));

    // payload
    if (std::distance(iter, end) >= _header.size) {
      std::copy_n(iter, _header.size, back_inserter(_payload));
      std::advance(iter, _header.size);
    } else {
      throw std::runtime_error("not enough data for field");
    }
  }

  field_type type() const { return _header.type; }

  template <typename T> T value() const;
  std::string value_str() const;

  size_t size() const { return _payload.size(); }

  const std::vector<uint8_t> &payload() const { return _payload; }

  std::vector<uint8_t> raw() const;

private:
  static std::string error_code2str(error_code);
  static std::string result_flag2str(result_flag);
  static std::string config_error2str(config_error);

  header _header;
  std::vector<uint8_t> _payload;
};

std::ostream &operator<<(std::ostream &, field_type);
std::ostream &operator<<(std::ostream &, const field &);

} // namespace addp

#endif // ADDP_PACKET_FIELD_H

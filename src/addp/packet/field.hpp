#ifndef ADDP_PACKET_FIELD_H
#define ADDP_PACKET_FIELD_H

#include <array>
#include <cstdint>
#include <vector>

#include <boost/asio.hpp>

#include <addp/constants.hpp>

namespace addp {

class field {
public:
  struct field_header {
    uint8_t type;
    uint8_t size;
  };

  enum field_type {
    FT_NONE = 0x00,
    FT_MAC_ADDR,      // 6 byte MAC address
    FT_IP_ADDR,       // 4 byte IP address
    FT_NETMASK,       // 4 byte netmask
    FT_NAME,          // string name
    FT_DOMAIN,        // domain
    FT_HW_TYPE,       // hardware type
    FT_HW_REV,        // hardware Revision
    FT_FIRMWARE,      // string firmware
    FT_RESULT_MSG,    // string result message
    FT_RESULT_FLAG,   // 1 byte result flag
    FT_GATEWAY,       // 4 byte gateway IP
    FT_CONF_ERR_CODE, // 2 byte configuration error code
    FT_DEVICE,        // string device
    FT_PORT,          // 4 byte port
    FT_DNS,           // 4 byte DNS IP
    FT_DCHP,          // bool DHCP enabled
    FT_ERR_CODE,      // 1 byte error code
    FT_SERIAL_COUNT,  // 1 byte serial port count
    FT_SSL_PORT,      // 4 byte encrypted port
    FT_VERSION,       // version ID
    FT_VENDOR,        // vendor GUID
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

  explicit field(field_type t);
  field(std::vector<uint8_t>::iterator &iter, const std::vector<uint8_t>::iterator &end);

  bool check() const { return _payload.size() == _header.size; }

  field_type type() const { return static_cast<field::field_type>(_header.type); }

  std::string type_str() const { return field_type2str(type()); }

  template <typename T> T value() const;
  std::string value_str() const;

  size_t size() const { return _payload.size(); }

  const std::vector<uint8_t> &payload() const { return _payload; }

  template <class T, std::size_t N> void add_raw(const std::array<T, N> &data) {
    copy(data.begin(), data.end(), back_inserter(_payload));
    _header.size = static_cast<uint8_t>(htons(static_cast<u_short>(_payload.size())));
  }
  std::vector<uint8_t> raw() const;

private:
  static std::string field_type2str(field_type type);
  static std::string error_code2str(error_code code);
  static std::string result_flag2str(result_flag flag);
  static std::string config_error2str(config_error error);

  field_header _header;
  std::vector<uint8_t> _payload;
};

} // namespace addp

#endif // ADDP_PACKET_FIELD_H

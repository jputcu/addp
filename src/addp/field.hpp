#ifndef ADDP_PACKET_FIELD_H
#define ADDP_PACKET_FIELD_H

#include "types.hpp"

#include <vector>
#include <iosfwd>
#include <boost/core/span.hpp>
#include <variant>
#include <optional>
#include <boost/asio/ip/address_v4.hpp>

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
  firmware,        // string firmware, e.g. "Version 82000856_F7 04/24/2017"
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
  field(std::vector<std::byte>::const_iterator &iter, const std::vector<std::byte>::const_iterator &end);

  field_type type() const { return _type; }

  std::ostream &value_str(std::ostream&) const;

  std::variant<bool, unsigned, std::string_view, boost::asio::ip::address_v4,
               mac_address, guid, config_error, error_code, result_flag, boost::span<const std::byte>>
  value() const;

  boost::span<const std::byte> payload() const { return _payload; }

  // field_type::firmware payload "Version 82000856_F7 04/24/2017" -> "82000856_F7"
  static constexpr std::string_view ParseFirmwareVersion(std::string_view fw_version) {
    fw_version.remove_prefix(std::strlen("Version "));
    return fw_version.substr(0, fw_version.find(' '));
  }

  // Suggests the latest DIGI fw version.
  // * nullopt: Up-to-date
  // * string: update to this version
  // * exception: unknown version, don't know what to suggest
  static constexpr std::optional<std::string_view> SuggestedFirmwareUpgrade(
      std::string_view fw_version) {
    // 2Mb, older HW version
    {
      constexpr std::string_view latest_2mb_version = "82000856_F7";
      if (fw_version == latest_2mb_version)
        return std::nullopt;
      else if (fw_version == "82000856_F1" || fw_version == "82000856_F3" || fw_version ==
               "82000856_F5" || fw_version == "82000856_F6")
        return latest_2mb_version;
    }
    // 4Mb, newer HW version
    {
      constexpr std::string_view latest_4mb_version = "82004424_D";
      if (fw_version == latest_4mb_version)
        return std::nullopt;
      else if (fw_version == "82004424_B" || fw_version == "82004424_C" || fw_version ==
               "82004424_D")
        return latest_4mb_version;
    }
    throw std::runtime_error("Unknown DIGI fw version");
  }

private:
  bool as_bool() const { return as_uint8() == BF_TRUE; };
  uint8_t as_uint8() const;
  uint16_t as_uint16() const;
  uint32_t as_uint32() const;
  std::string_view as_string() const;
  config_error as_config_error() const { return config_error{as_uint16()}; }
  error_code as_error_code() const { return error_code{as_uint8()}; }
  result_flag as_result_flag() const { return result_flag{as_uint8()}; }
  boost::asio::ip::address_v4 as_ip_address() const;
  guid as_guid() const;
  mac_address as_mac_address() const;

  field_type _type;
  boost::span<const std::byte> _payload;
};

std::ostream &operator<<(std::ostream &, field::error_code);
std::ostream &operator<<(std::ostream &, field::result_flag);
std::ostream &operator<<(std::ostream &, field::config_error);
std::ostream &operator<<(std::ostream &, field_type);
std::ostream &operator<<(std::ostream &, const field &);

} // namespace addp

#endif // ADDP_PACKET_FIELD_H

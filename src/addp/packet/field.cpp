#include "field.hpp"

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <iomanip>
#include <sstream>
#include <iostream>

#include <addp/types.hpp>
using namespace addp;

template <> bool field::value() const { return _payload.front() == BF_TRUE; }

template <> uint8_t field::value() const { return _payload.front(); }

template <> uint16_t field::value() const {
  return ntohs(*reinterpret_cast<const uint16_t *>(_payload.data()));
}

template <> uint32_t field::value() const {
  return ntohl(*reinterpret_cast<const uint32_t *>(_payload.data()));
}

template <> std::string field::value() const {
  std::string s;
  std::copy(_payload.begin(), _payload.end(), back_inserter(s));
  return s;
}

template <> field::config_error field::value() const {
  return static_cast<config_error>(value<uint16_t>());
}

template <> field::error_code field::value() const {
  return static_cast<error_code>(value<uint8_t>());
}

template <> field::result_flag field::value() const {
  return static_cast<result_flag>(value<uint8_t>());
}

template <typename T> T field::value() const {
  T t {};
  if (_payload.size() == t.size())
    std::copy(_payload.begin(), _payload.end(), t.begin());
  return t;
}

std::string field::value_str() const {
  std::ostringstream os;

  switch (type()) {
  case FT_DCHP:
    os << (value<bool>() ? "true" : "false");
    break;

  case FT_HW_TYPE:
  case FT_HW_REV:
  case FT_SERIAL_COUNT:
    os << std::dec << static_cast<int>(value<uint8_t>());
    break;

  case FT_VERSION:
    os << std::dec << value<uint16_t>();
    break;

  case FT_PORT:
  case FT_SSL_PORT:
    os << std::dec << value<uint32_t>();
    break;

  case FT_NAME:
  case FT_DOMAIN:
  case FT_FIRMWARE:
  case FT_RESULT_MSG:
  case FT_DEVICE:
    os << value<std::string>();
    break;

  case FT_IP_ADDR:
  case FT_NETMASK:
  case FT_GATEWAY:
  case FT_DNS:
    os << value<ip_address>();
    break;

  case FT_MAC_ADDR:
    os << value<mac_address>();
    break;

  case FT_VENDOR:
    os << value<guid>();
    break;

  case FT_CONF_ERR_CODE:
    os << std::dec << value<config_error>();
    break;

  case FT_ERR_CODE:
    os << std::dec << value<error_code>();
    break;

  case FT_RESULT_FLAG:
    os << std::dec << value<result_flag>();
    break;

  case FT_DEVICE_ID:
  default: {
    for (const uint8_t b : payload())
      os << " " << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned>(b);
    break;
  }
  }
  return os.str();
}

std::vector<uint8_t> field::raw() const {
  std::vector<uint8_t> buffer;
  auto headerp = reinterpret_cast<const uint8_t *>(&_header);
  std::copy_n(headerp, sizeof(_header), back_inserter(buffer));
  std::copy(_payload.begin(), _payload.end(), back_inserter(buffer));
  return buffer;
}

std::string field::field_type2str(const field_type type) {
  switch (type) {
  case FT_NONE:
    return "None";
  case FT_MAC_ADDR:
    return "MAC address";
  case FT_IP_ADDR:
    return "IP address";
  case FT_NETMASK:
    return "Netmask";
  case FT_NAME:
    return "Name";
  case FT_DOMAIN:
    return "Domain";
  case FT_HW_TYPE:
    return "Hardware type";
  case FT_HW_REV:
    return "Hardware Revision";
  case FT_FIRMWARE:
    return "Firmware";
  case FT_RESULT_MSG:
    return "Result message";
  case FT_RESULT_FLAG:
    return "Result flag";
  case FT_GATEWAY:
    return "Gateway IP";
  case FT_CONF_ERR_CODE:
    return "Configuration error code";
  case FT_DEVICE:
    return "Device";
  case FT_PORT:
    return "Port";
  case FT_DNS:
    return "DNS IP";
  case FT_DCHP:
    return "DHCP enabled";
  case FT_ERR_CODE:
    return "Error code";
  case FT_SERIAL_COUNT:
    return "Serial port count";
  case FT_SSL_PORT:
    return "Encrypted port";
  case FT_VERSION:
    return "Version ID";
  case FT_VENDOR:
    return "Vendor GUID";
  case FT_DEVICE_ID:
    return "Device-ID";
  default:
    return str(boost::format("Unknown (0x%02x)") % type);
  }
}

std::string field::error_code2str(const error_code code) {
  switch (code) {
  case EC_SUCCESS:
    return "Success";
  case EC_AUTH:
    return "Auth";
  case EC_INVALID:
    return "Invalid";
  case EC_SAVE:
    return "Save";
  default:
    return str(boost::format("Unknown (0x%02x)") % code);
  }
}

std::string field::result_flag2str(const result_flag flag) {
  switch (flag) {
  case RF_SUCCESS:
    return "Success";
  case RF_ERROR:
    return "Error";
  default:
    return str(boost::format("Unknown (0x%02x)") % flag);
  }
}

std::string field::config_error2str(const config_error error) {
  switch (error) {
  case CE_SUCCESS:
    return "Success";
  case CE_ERROR:
    return "Error";
  default:
    return str(boost::format("Unknown (0x%02x)") % error);
  }
}

std::ostream &addp::operator<<(std::ostream &os, const field &field) {
  if (field.type() != field::FT_NONE)
    os << field.type_str() << " = " << field.value_str() << "\n";
  return os;
}

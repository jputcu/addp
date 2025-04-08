#include "field.hpp"

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "types.hpp"
using namespace addp;

template <> bool field::value() const { return payload().front() == BF_TRUE; }

template <> uint8_t field::value() const { return payload().front(); }

template <> uint16_t field::value() const {
  return ntohs(*reinterpret_cast<const uint16_t *>(payload().data()));
}

template <> uint32_t field::value() const {
  return ntohl(*reinterpret_cast<const uint32_t *>(payload().data()));
}

template <> std::string field::value() const {
  std::string s;
  std::copy(payload().begin(), payload().end(), back_inserter(s));
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
  const auto payload_bytes = payload();
  if (payload_bytes.size() == t.size())
    std::copy(payload_bytes.begin(), payload_bytes.end(), t.begin());
  return t;
}

std::string field::value_str() const {
  std::ostringstream os;

  switch (type()) {
  case field_type::dhcp:
    os << (value<bool>() ? "true" : "false");
    break;

  case field_type::hw_type:
  case field_type::hw_rev:
  case field_type::serial_count:
    os << std::dec << static_cast<int>(value<uint8_t>());
    break;

  case field_type::version:
    os << std::dec << value<uint16_t>();
    break;

  case field_type::port:
  case field_type::ssl_port:
    os << std::dec << value<uint32_t>();
    break;

  case field_type::name:
  case field_type::domain:
  case field_type::firmware:
  case field_type::result_msg:
  case field_type::device:
    os << value<std::string>();
    break;

  case field_type::ip_addr:
  case field_type::netmask:
  case field_type::gateway:
  case field_type::dns:
    os << value<ip_address>();
    break;

  case field_type::mac_addr:
    os << value<mac_address>();
    break;

  case field_type::vendor:
    os << value<guid>();
    break;

  case field_type::conf_err_code:
    os << std::dec << value<config_error>();
    break;

  case field_type::err_code:
    os << std::dec << value<error_code>();
    break;

  case field_type::result_flag:
    os << std::dec << value<result_flag>();
    break;

  case field_type::device_id:
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

std::ostream &addp::operator<<(std::ostream &os, const field_type type) {
  switch (type) {
  case field_type::none:
    os << "None";
    break;
  case field_type::mac_addr:
    os << "MAC address";
    break;
  case field_type::ip_addr:
    os << "IP address";
    break;
  case field_type::netmask:
    os << "Netmask";
    break;
  case field_type::name:
    os << "Name";
    break;
  case field_type::domain:
    os << "Domain";
    break;
  case field_type::hw_type:
    os << "Hardware type";
    break;
  case field_type::hw_rev:
    os << "Hardware Revision";
    break;
  case field_type::firmware:
    os << "Firmware";
    break;
  case field_type::result_msg:
    os << "Result message";
    break;
  case field_type::result_flag:
    os << "Result flag";
    break;
  case field_type::gateway:
    os << "Gateway IP";
    break;
  case field_type::conf_err_code:
    os << "Configuration error code";
    break;
  case field_type::device:
    os << "Device";
    break;
  case field_type::port:
    os << "Port";
    break;
  case field_type::dns:
    os << "DNS IP";
    break;
  case field_type::dhcp:
    os << "DHCP enabled";
    break;
  case field_type::err_code:
    os << "Error code";
    break;
  case field_type::serial_count:
    os << "Serial port count";
    break;
  case field_type::ssl_port:
    os << "Encrypted port";
    break;
  case field_type::version:
    os << "Version ID";
    break;
  case field_type::vendor:
    os << "Vendor GUID";
    break;
  case field_type::device_id:
    os << "Device-ID";
    break;
  default:
    os << str(boost::format("Unknown (0x%02x)") % static_cast<unsigned>(type));
    break;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const field &field) {
  if (field.type() != field_type::none)
    os << field.type() << " = " << field.value_str() << "\n";
  return os;
}

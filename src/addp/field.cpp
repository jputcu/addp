#include "field.hpp"

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <iomanip>
#include <iostream>

#include "types.hpp"
using namespace addp;

field::field(std::vector<uint8_t>::const_iterator &iter, const std::vector<uint8_t>::const_iterator &end) {
  struct header {
    field_type type{};
    uint8_t size{};
  };
  static_assert(sizeof(header) == 2);

  const auto remaining_len [[maybe_unused]] = static_cast<size_t>(std::distance(iter, end));
  if ( remaining_len < sizeof(header) )
    throw std::runtime_error("field too small");

  _type = field_type{*iter++};
  const auto payload_len = size_t{*iter++};
  const auto total_field_len [[maybe_unused]] = payload_len + sizeof(header);
  if ( total_field_len > remaining_len )
    throw std::runtime_error("field too large");

  _payload = {&iter[0], payload_len};
  std::advance(iter, payload_len);
}

bool field::as_bool() const { return as_uint8() == BF_TRUE; }

uint8_t field::as_uint8() const { return payload().front(); }

uint16_t field::as_uint16() const { return ntohs(*reinterpret_cast<const uint16_t *>(payload().data())); }

uint32_t field::as_uint32() const {
  return ntohl(*reinterpret_cast<const uint32_t *>(payload().data()));
}

std::string_view field::as_string() const {
  return {reinterpret_cast<const char *>(payload().cbegin()),
          reinterpret_cast<const char *>(payload().cend())};
}

field::config_error field::as_config_error() const { return config_error{as_uint16()}; }

field::error_code field::as_error_code() const {
  return error_code{as_uint8()};
}

field::result_flag field::as_result_flag() const {
  return result_flag{as_uint8()};
}

boost::asio::ip::address_v4 field::as_ip_address() const {
  boost::asio::ip::address_v4::bytes_type ip_bytes;
  const auto payload_bytes = payload();
  std::copy(payload_bytes.cbegin(), payload_bytes.cend(), ip_bytes.begin());
  return boost::asio::ip::address_v4{ip_bytes};
}

guid field::as_guid() const {
  guid t;
  const auto payload_bytes = payload();
  assert(payload_bytes.size() == sizeof(guid));
  std::memcpy(&t, payload_bytes.cbegin(), sizeof(guid));
  return t;
}

mac_address field::as_mac_address() const {
  mac_address t;
  const auto payload_bytes = payload();
  assert(payload_bytes.size() == sizeof(mac_address));
  std::memcpy(&t, payload_bytes.cbegin(), sizeof(mac_address));
  return t;
}

std::ostream &field::value_str(std::ostream &os) const {
  switch (type()) {
  case field_type::dhcp:
    os << (as_bool() ? "true" : "false");
    break;

  case field_type::hw_type:
  case field_type::hw_rev:
  case field_type::serial_count:
    os << std::dec << static_cast<int>(as_uint8());
    break;

  case field_type::version:
    os << std::dec << as_uint16();
    break;

  case field_type::port:
  case field_type::ssl_port:
    os << std::dec << as_uint32();
    break;

  case field_type::name:
  case field_type::domain:
  case field_type::firmware:
  case field_type::result_msg:
  case field_type::device:
    os << std::quoted(as_string());
    break;

  case field_type::ip_addr:
  case field_type::netmask:
  case field_type::gateway:
  case field_type::dns:
    os << as_ip_address();
    break;

  case field_type::mac_addr:
    os << as_mac_address();
    break;

  case field_type::device_id:
  case field_type::vendor:
    os << as_guid();
    break;

  case field_type::conf_err_code:
    os << std::dec << as_config_error();
    break;

  case field_type::err_code:
    os << std::dec << as_error_code();
    break;

  case field_type::result_flag:
    os << std::dec << as_result_flag();
    break;

  default:
    stream_unknown_as_hex(os);
    break;
  }
  return os;
}

void field::stream_unknown_as_hex(std::ostream& os) const {
  for (const auto b : payload())
    os << boost::format(" %02x") % unsigned{b};
}

std::ostream &addp::operator<<(std::ostream &os, field::error_code ec) {
  switch (ec) {
  case field::EC_SUCCESS:
    os << "Success";
    break;
  case field::EC_AUTH:
    os << "Auth";
    break;
  case field::EC_INVALID:
    os << "Invalid";
    break;
  case field::EC_SAVE:
    os << "Save";
    break;
  default:
    os << boost::format("Unknown (0x%02x)") % ec;
    break;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, field::result_flag fl) {
  switch (fl) {
  case field::RF_SUCCESS:
    os << "Success";
    break;
  case field::RF_ERROR:
    os << "Error";
    break;
  default:
    os << boost::format("Unknown (0x%02x)") % fl;
    break;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, field::config_error error) {
  switch (error) {
  case field::config_error::no_error:
    os << "Success";
    break;
  case field::config_error::other_subnet:
    os << "Other subnet";
    break;
  default:
    os << boost::format("Unknown (0x%02x)") % error;
    break;
  }
  return os;
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
    os << boost::format("Unknown (0x%02x)") % static_cast<unsigned>(type);
    break;
  }
  return os;
}

std::ostream &addp::operator<<(std::ostream &os, const field &field) {
  os << field.type() << " = ";
  field.value_str(os);
  return os;
}

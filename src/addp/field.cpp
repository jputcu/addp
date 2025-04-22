#include "field.hpp"

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/asio/ip/address_v4.hpp>
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
  assert(remaining_len >= sizeof(header));

  _type = field_type{*iter++};
  const auto payload_len = size_t{*iter++};
  const auto total_field_len [[maybe_unused]] = payload_len + sizeof(header);
  assert(total_field_len <= remaining_len);

  _payload = {&iter[0], payload_len};
  std::advance(iter, payload_len);
}

template <> bool field::as() const { return as<uint8_t>() == BF_TRUE; }

template <> uint16_t field::as() const {
  return ntohs(*reinterpret_cast<const uint16_t *>(payload().data()));
}

template <> uint32_t field::as() const {
  return ntohl(*reinterpret_cast<const uint32_t *>(payload().data()));
}

template <> std::string field::as() const { return {payload().cbegin(), payload().cend()}; }

template <> field::config_error field::as() const {
  return config_error{as<uint16_t>()};
}

template <> field::error_code field::as() const {
  return error_code{as<uint8_t>()};
}

template <> field::result_flag field::as() const {
  return result_flag{as<uint8_t>()};
}

template <> boost::asio::ip::address_v4 field::as() const {
  boost::asio::ip::address_v4::bytes_type ip_bytes;
  const auto payload_bytes = payload();
  std::copy(payload_bytes.cbegin(), payload_bytes.cend(), ip_bytes.begin());
  return boost::asio::ip::address_v4{ip_bytes};
}

std::ostream &field::value_str(std::ostream &os) const {
  switch (type()) {
  case field_type::dhcp:
    os << (as<bool>() ? "true" : "false");
    break;

  case field_type::hw_type:
  case field_type::hw_rev:
  case field_type::serial_count:
    os << std::dec << static_cast<int>(as<uint8_t>());
    break;

  case field_type::version:
    os << std::dec << as<uint16_t>();
    break;

  case field_type::port:
  case field_type::ssl_port:
    os << std::dec << as<uint32_t>();
    break;

  case field_type::name:
  case field_type::domain:
  case field_type::firmware:
  case field_type::result_msg:
  case field_type::device:
    os << std::quoted(as<std::string>());
    break;

  case field_type::ip_addr:
  case field_type::netmask:
  case field_type::gateway:
  case field_type::dns:
    os << as<boost::asio::ip::address_v4>();
    break;

  case field_type::mac_addr:
    os << as<mac_address>();
    break;

  case field_type::device_id:
  case field_type::vendor:
    os << as<guid>();
    break;

  case field_type::conf_err_code:
    os << std::dec << as<config_error>();
    break;

  case field_type::err_code:
    os << std::dec << as<error_code>();
    break;

  case field_type::result_flag:
    os << std::dec << as<result_flag>();
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
  field.value_str(os) << "\n";
  return os;
}

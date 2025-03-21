#include <iostream>
#include <string>

#define BOOST_TEST_MODULE addp_tests

#include <boost/numeric/conversion/converter_policies.hpp>
#include <boost/test/included/unit_test.hpp>

#include <addp/types.hpp>

BOOST_AUTO_TEST_CASE(ip_address_format) {
  addp::ip_address ip = {{192, 168, 1, 1}};
  std::ostringstream os;
  os << ip;
  BOOST_CHECK_EQUAL(os.str(), "192.168.1.1");
}

BOOST_AUTO_TEST_CASE(ip_address_parse) {
  addp::ip_address ip;
  std::istringstream is("192.168.1.1");
  is >> ip;
  BOOST_CHECK_EQUAL(ip[0], 192);
  BOOST_CHECK_EQUAL(ip[1], 168);
  BOOST_CHECK_EQUAL(ip[2], 1);
  BOOST_CHECK_EQUAL(ip[3], 1);
}

BOOST_AUTO_TEST_CASE(ip_address_parse_single_values) {
  addp::ip_address ip;
  std::istringstream is("1.2.3.4");
  is >> ip;
  std::ostringstream os;
  os << ip;
  BOOST_CHECK_EQUAL(os.str(), "1.2.3.4");
}

BOOST_AUTO_TEST_CASE(ip_address_parse_max_ip) {
  std::istringstream is("255.255.255.255");
  addp::ip_address ip;
  is >> ip;
  BOOST_CHECK_EQUAL(ip[0], 255);
  BOOST_CHECK_EQUAL(ip[1], 255);
  BOOST_CHECK_EQUAL(ip[2], 255);
  BOOST_CHECK_EQUAL(ip[3], 255);
}

BOOST_AUTO_TEST_CASE(ip_address_dont_parse_octal) {
  std::istringstream is("01.07.08.09");
  addp::ip_address ip;
  is >> ip;
  std::ostringstream os;
  os << ip;
  BOOST_CHECK_EQUAL(os.str(), "1.7.8.9");
}

BOOST_AUTO_TEST_CASE(ip_address_throw_on_value_overflow) {
  std::istringstream is("1.2.3.256");
  addp::ip_address ip;
  BOOST_CHECK_THROW(is >> ip, boost::numeric::positive_overflow);
}

BOOST_AUTO_TEST_CASE(ip_address_throw_on_value_underflow) {
  std::istringstream is("1.2.3.-4");
  addp::ip_address ip;
  BOOST_CHECK_THROW(is >> ip, boost::numeric::negative_overflow);
}

BOOST_AUTO_TEST_CASE(mac_address_format) {
  addp::mac_address mac {"01:20:03:4d:fe:ef"};
  std::ostringstream os;
  os << mac;
  BOOST_CHECK_EQUAL(os.str(), "01:20:03:4d:fe:ef");
}

BOOST_AUTO_TEST_CASE(mac_address_parse) {
  addp::mac_address mac;
  std::istringstream is("01:20:03:4d:fe:ef");
  is >> mac;
  BOOST_CHECK_EQUAL(mac[0], 0x01);
  BOOST_CHECK_EQUAL(mac[1], 0x20);
  BOOST_CHECK_EQUAL(mac[2], 0x03);
  BOOST_CHECK_EQUAL(mac[3], 0x4d);
  BOOST_CHECK_EQUAL(mac[4], 0xfe);
  BOOST_CHECK_EQUAL(mac[5], 0xef);
}

BOOST_AUTO_TEST_CASE(mac_address_parse_single_values) {
  std::istringstream is("1:2:3:7:8:9");
  addp::mac_address mac;
  is >> mac;
  std::ostringstream os;
  os << mac;
  BOOST_CHECK_EQUAL(os.str(), "01:02:03:07:08:09");
}

BOOST_AUTO_TEST_CASE(mac_address_dont_parse_octal) {
  std::istringstream is("01:02:03:07:08:09");
  addp::mac_address mac;
  is >> mac;
  std::ostringstream os;
  os << mac;
  BOOST_CHECK_EQUAL(os.str(), "01:02:03:07:08:09");
}

BOOST_AUTO_TEST_CASE(mac_address_parse_hex_to_lower) {
  std::istringstream is("01:23:45:67:89:AB");
  addp::mac_address mac;
  is >> mac;
  std::ostringstream os;
  os << mac;
  BOOST_CHECK_EQUAL(os.str(), "01:23:45:67:89:ab");
}

BOOST_AUTO_TEST_CASE(mac_address_throw_on_value_overflow) {
  std::istringstream is("01:23:45:67:89:123");
  addp::mac_address mac;
  BOOST_CHECK_THROW(is >> mac, boost::numeric::positive_overflow);
}

BOOST_AUTO_TEST_CASE(mac_address_throw_on_value_underflow) {
  std::istringstream is("01:23:45:67:89:-1");
  addp::mac_address mac;
  BOOST_CHECK_THROW(is >> mac, boost::numeric::negative_overflow);
}

BOOST_AUTO_TEST_CASE(mac_address_parse_from_str) {
  const auto mac = addp::mac_address{"00:11:22:33:44:55"};
  BOOST_CHECK_EQUAL(mac[0], 0x00);
  BOOST_CHECK_EQUAL(mac[1], 0x11);
  BOOST_CHECK_EQUAL(mac[2], 0x22);
  BOOST_CHECK_EQUAL(mac[3], 0x33);
  BOOST_CHECK_EQUAL(mac[4], 0x44);
  BOOST_CHECK_EQUAL(mac[5], 0x55);
}

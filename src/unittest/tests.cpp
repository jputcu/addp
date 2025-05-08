#include <iostream>
#include <string>

#define BOOST_TEST_MODULE addp_tests

#include <boost/numeric/conversion/converter_policies.hpp>
#include <boost/test/included/unit_test.hpp>

#include <addp/types.hpp>

BOOST_AUTO_TEST_CASE(mac_address_stream) {
  addp::mac_address mac {"01:20:03:4d:fe:ef"};
  std::ostringstream os;
  os << mac;
  BOOST_CHECK_EQUAL(os.str(), "01:20:03:4d:fe:ef");
}

BOOST_AUTO_TEST_CASE(mac_address_parse) {
  addp::mac_address mac("01:20:03:4d:fe:ef");
  BOOST_CHECK_EQUAL(mac[0], 0x01);
  BOOST_CHECK_EQUAL(mac[1], 0x20);
  BOOST_CHECK_EQUAL(mac[2], 0x03);
  BOOST_CHECK_EQUAL(mac[3], 0x4d);
  BOOST_CHECK_EQUAL(mac[4], 0xfe);
  BOOST_CHECK_EQUAL(mac[5], 0xef);
}

BOOST_AUTO_TEST_CASE(mac_address_parse_windows_notation) {
  addp::mac_address mac("01-20-03-4D-FE-EF");
  BOOST_CHECK_EQUAL(mac.ToString(), "01:20:03:4d:fe:ef");
}

BOOST_AUTO_TEST_CASE(mac_address_parse_digi_notation) {
  addp::mac_address mac("012003:4dfeef");
  BOOST_CHECK_EQUAL(mac.ToString(), "01:20:03:4d:fe:ef");
  BOOST_CHECK_EQUAL(mac.ToDigiMac(), "012003:4DFEEF");
}

BOOST_AUTO_TEST_CASE(mac_address_dont_parse_octal) {
  addp::mac_address mac("01:02:03:07:08:09");
  std::ostringstream os;
  os << mac;
  BOOST_CHECK_EQUAL(os.str(), "01:02:03:07:08:09");
}

BOOST_AUTO_TEST_CASE(mac_address_parse_hex_to_lower) {
  addp::mac_address mac("01:23:45:67:89:AB");
  std::ostringstream os;
  os << mac;
  BOOST_CHECK_EQUAL(os.str(), "01:23:45:67:89:ab");
}

BOOST_AUTO_TEST_CASE(mac_address_throw_on_value_underflow) {
  BOOST_CHECK_THROW(addp::mac_address("01:23:45:67:89:-1"), boost::numeric::negative_overflow);
}

BOOST_AUTO_TEST_CASE(guid_to_string) {
  std::ostringstream os;
  addp::guid guid{0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04,
                  0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04};
  os << guid;
  BOOST_CHECK_EQUAL(os.str(), "01020304-01020304-01020304-01020304");
}

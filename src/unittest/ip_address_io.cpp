#include <limits>
#include <sstream>
#include <string>

#define BOOST_TEST_MODULE addp_tests

#include <boost/numeric/conversion/converter_policies.hpp>
#include <boost/test/included/unit_test.hpp>

#include <addp/types_io.hpp>

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

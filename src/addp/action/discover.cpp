#include "discover.hpp"

#include <iostream>

#include <addp/packet/discovery.hpp>
using namespace addp;

discover::discover(mac_address const &mac) : action(discovery_request(mac)) { }

void discover::set_mac_address(mac_address const &mac) {
  set_request(discovery_request(mac));
}

void discover::print_brief(const boost::asio::ip::udp::endpoint &sender,
                           const packet &response) const {
  std::string mac_addr;
  std::string name;
  std::string device;
  std::string firmware;

  for (const auto &f : response.fields()) {
    switch (f.type()) {
    case field::FT_MAC_ADDR:
      mac_addr = f.value_str();
      break;
    case field::FT_NAME:
      name = f.value_str();
      break;
    case field::FT_DEVICE:
      device = f.value_str();
      break;
    case field::FT_FIRMWARE:
      firmware = f.value_str();
      break;
    default:
      break;
    }
  }

  std::cout << sender.address() << '\t' << name << '\t' << mac_addr << '\t' << device << '\t'
            << firmware << "\n";
}

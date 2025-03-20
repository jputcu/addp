#include "discover.hpp"

#include <iostream>

#include <addp/packet/discovery.hpp>
using namespace addp;

discover::discover(const mac_address &mac_address) : action(discovery_request(mac_address)) {
  if (mac_address != MAC_ADDR_BROADCAST)
    set_max_count(1);
}

void discover::set_mac_address(const std::string &mac) {
  const auto mac_addr = parse_mac_str(mac);
  set_request(discovery_request(mac_addr));

  if (mac_addr != MAC_ADDR_BROADCAST)
    set_max_count(1);
}

void discover::print_brief(const boost::asio::ip::udp::endpoint &sender,
                           const packet &response) const {
  std::string mac_addr;
  std::string name;
  std::string device;
  std::string firmware;

  for (const field &f : response.fields()) {
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

#include "reboot.hpp"

#include <iostream>

#include <addp/packet/packet.hpp>
using namespace addp;

reboot::reboot(mac_address const &mac, const std::string &password)
    : action(reboot_request(mac, password)), _mac_address(mac),
      _password(password) {
}

void reboot::set_mac_address(mac_address const &mac) {
  set_request(reboot_request(mac, _password));
}

void reboot::set_password(const std::string &password) {
  _password = password;
  set_request(reboot_request(_mac_address, _password));
}

void reboot::print_brief(const boost::asio::ip::udp::endpoint & /*sender*/,
                         const packet &response) const {
  for (const field &f : response.fields())
    if (f.type() == field::FT_RESULT_MSG) {
      std::cout << f.value<std::string>() << "\n";
      break;
    }
}

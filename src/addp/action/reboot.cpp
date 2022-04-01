#include "reboot.h"

#include <iostream>

#include <addp/packet/packet_io.h>
#include <addp/packet/reboot.h>
#include <addp/types.h>
#include <addp/types_io.h>

namespace addp {

reboot::reboot(const mac_address &mac_address, const std::string &password)
    : action(reboot_request(mac_address, password)), _mac_address(mac_address),
      _password(password) {
  set_max_count(1);
}

void reboot::set_mac_address(const std::string &mac) {
  set_request(reboot_request(addp::parse_mac_str(mac), _password));
}

void reboot::set_password(const std::string &password) {
  _password = password;
  set_request(reboot_request(_mac_address, _password));
}

void reboot::print_brief(const boost::asio::ip::udp::endpoint & /*sender*/,
                         const packet &response) const {
  for (const field &f : response.fields())
    if (f.type() == field::FT_RESULT_MSG) {
      std::cout << f.value<std::string>() << std::endl;
      break;
    }
}

} // namespace addp

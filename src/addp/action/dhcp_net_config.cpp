#include "dhcp_net_config.hpp"

#include <iostream>

#include <addp/packet/dhcp_net_config.hpp>
#include <addp/packet/packet_io.hpp>
#include <addp/types.hpp>
#include <addp/types_io.hpp>

namespace addp {

dhcp_net_config::dhcp_net_config(const mac_address &mac_address, bool enable,
                                 const std::string &password)
    : action(dhcp_net_config_request(mac_address, enable, password)), _mac_address(mac_address),
      _enable(enable), _password(password) {
  set_max_count(1);
}

void dhcp_net_config::set_mac_address(const std::string &mac) {
  set_request(dhcp_net_config_request(addp::parse_mac_str(mac), _enable, _password));
}

void dhcp_net_config::set_enable(bool enable) {
  _enable = enable;
  set_request(dhcp_net_config_request(_mac_address, _enable, _password));
}

void dhcp_net_config::set_password(const std::string &password) {
  _password = password;
  set_request(dhcp_net_config_request(_mac_address, _enable, _password));
}

void dhcp_net_config::print_brief(const boost::asio::ip::udp::endpoint & /*sender*/,
                                  const packet &response) const {
  for (const field &f : response.fields())
    if (f.type() == field::FT_RESULT_MSG) {
      std::cout << f.value_str() << std::endl;
      break;
    }
}

} // namespace addp

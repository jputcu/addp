#include "static_net_config.hpp"

#include <iostream>

#include <addp/packet/packet_io.hpp>
#include <addp/packet/static_net_config.hpp>
#include <addp/types.hpp>
#include <addp/types_io.hpp>

namespace addp {

static_net_config::static_net_config(const mac_address &mac_address, const ip_address &ip,
                                     const ip_address &subnet, const ip_address &gateway,
                                     const std::string &password)
    : action(static_net_config_request(mac_address, ip, subnet, gateway, password)),
      _mac_address(mac_address), _ip(ip), _subnet(subnet), _gateway(gateway), _password(password) {
  set_max_count(1);
}

void static_net_config::set_mac_address(const std::string &mac) {
  set_request(
      static_net_config_request(addp::parse_mac_str(mac), _ip, _subnet, _gateway, _password));
}

void static_net_config::set_ip(const std::string &ip) {
  set_request(static_net_config_request(_mac_address, addp::parse_ip_str(ip), _subnet, _gateway,
                                        _password));
}

void static_net_config::set_subnet(const std::string &subnet) {
  set_request(static_net_config_request(_mac_address, _ip, addp::parse_ip_str(subnet), _gateway,
                                        _password));
}

void static_net_config::set_gateway(const std::string &gateway) {
  set_request(static_net_config_request(_mac_address, _ip, _subnet, addp::parse_ip_str(gateway),
                                        _password));
}

void static_net_config::set_password(const std::string &password) {
  _password = password;
  set_request(static_net_config_request(_mac_address, _ip, _subnet, _gateway, _password));
}

void static_net_config::print_brief(const boost::asio::ip::udp::endpoint & /*sender*/,
                                    const packet &response) const {
  for (const field &f : response.fields())
    if (f.type() == field::FT_RESULT_MSG) {
      std::cout << f.value<std::string>() << std::endl;
      break;
    }
}

} // namespace addp

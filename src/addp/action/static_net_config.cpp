#include "static_net_config.hpp"

#include <iostream>

#include <addp/packet/packet.hpp>
using namespace addp;

static_net_config::static_net_config(mac_address const &mac, const ip_address &ip,
                                     const ip_address &subnet, const ip_address &gateway,
                                     const std::string &password)
    : action(static_net_config_request(mac, ip, subnet, gateway, password)),
      _mac_address(mac), _ip(ip), _subnet(subnet), _gateway(gateway), _password(password) {
}

void static_net_config::set_mac_address(mac_address const &mac) {
  set_request(static_net_config_request(mac, _ip, _subnet, _gateway, _password));
}

void static_net_config::set_ip(const std::string &ip) {
  set_request(static_net_config_request(_mac_address, parse_ip_str(ip), _subnet, _gateway,
                                        _password));
}

void static_net_config::set_subnet(const std::string &subnet) {
  set_request(static_net_config_request(_mac_address, _ip, parse_ip_str(subnet), _gateway,
                                        _password));
}

void static_net_config::set_gateway(const std::string &gateway) {
  set_request(static_net_config_request(_mac_address, _ip, _subnet, parse_ip_str(gateway),
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
      std::cout << f.value<std::string>() << "\n";
      break;
    }
}

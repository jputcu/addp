#ifndef ADDP_ACTION_STATIC_NET_CONFIG_H
#define ADDP_ACTION_STATIC_NET_CONFIG_H

#include "action.hpp"

namespace addp {

class static_net_config : public action {
public:
  static_net_config(mac_address const &mac, const ip_address &ip, const ip_address &subnet,
                    const ip_address &gateway, const std::string &password = DEFAULT_PASSWORD)
      : action(static_net_config_request(mac, ip, subnet, gateway, password)), _mac_address(mac),
        _ip(ip), _subnet(subnet), _gateway(gateway), _password(password) {}

  void set_mac_address(mac_address const &mac) {
    set_request(static_net_config_request(mac, _ip, _subnet, _gateway, _password));
  }

  void set_ip(const std::string &ip) {
    set_request(
        static_net_config_request(_mac_address, parse_ip_str(ip), _subnet, _gateway, _password));
  }

  void set_subnet(const std::string &subnet) {
    set_request(
        static_net_config_request(_mac_address, _ip, parse_ip_str(subnet), _gateway, _password));
  }

  void set_gateway(const std::string &gateway) {
    set_request(
        static_net_config_request(_mac_address, _ip, _subnet, parse_ip_str(gateway), _password));
  }

  void set_password(const std::string &password) {
    _password = password;
    set_request(static_net_config_request(_mac_address, _ip, _subnet, _gateway, _password));
  }

protected:
  void print_brief(const boost::asio::ip::udp::endpoint &sender,
                   const packet &response) const override;

private:
  mac_address _mac_address;
  ip_address _ip;
  ip_address _subnet;
  ip_address _gateway;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_STATIC_NET_CONFIG_H

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

private:
  mac_address _mac_address;
  ip_address _ip;
  ip_address _subnet;
  ip_address _gateway;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_STATIC_NET_CONFIG_H

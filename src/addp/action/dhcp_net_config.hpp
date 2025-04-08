#ifndef ADDP_ACTION_DHCP_NET_CONFIG_H
#define ADDP_ACTION_DHCP_NET_CONFIG_H

#include "action.hpp"

namespace addp {

class dhcp_net_config : public action {
public:
  dhcp_net_config(const mac_address &mac_address, bool enable,
                  const std::string &password = DEFAULT_PASSWORD)
      : action(packet::dhcp_net_config_request(mac_address, enable, password)), _mac_address(mac_address),
        _enable(enable), _password(password) {}

private:
  mac_address _mac_address;
  bool _enable;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_DHCP_NET_CONFIG_H

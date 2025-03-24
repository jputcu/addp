#ifndef ADDP_ACTION_DHCP_NET_CONFIG_H
#define ADDP_ACTION_DHCP_NET_CONFIG_H

#include "action.hpp"

namespace addp {

class dhcp_net_config : public action {
public:
  dhcp_net_config(const mac_address &mac_address, bool enable,
                  const std::string &password = DEFAULT_PASSWORD)
      : action(dhcp_net_config_request(mac_address, enable, password)), _mac_address(mac_address),
        _enable(enable), _password(password) {}

  void set_mac_address(mac_address const &mac) {
    set_request(dhcp_net_config_request(mac, _enable, _password));
  }

  void set_enable(bool enable) {
    _enable = enable;
    set_request(dhcp_net_config_request(_mac_address, _enable, _password));
  }

  void set_password(const std::string &password) {
    _password = password;
    set_request(dhcp_net_config_request(_mac_address, _enable, _password));
  }

private:
  mac_address _mac_address;
  bool _enable;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_DHCP_NET_CONFIG_H

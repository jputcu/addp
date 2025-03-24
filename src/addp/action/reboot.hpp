#ifndef ADDP_ACTION_REBOOT_H
#define ADDP_ACTION_REBOOT_H

#include "action.hpp"

namespace addp {

class reboot : public action {
public:
  explicit reboot(mac_address const &mac, const std::string &password = DEFAULT_PASSWORD)
      : action(reboot_request(mac, password)), _mac_address(mac), _password(password) {}

  void set_mac_address(mac_address const &mac) { set_request(reboot_request(mac, _password)); }

  void set_password(const std::string &password) {
    _password = password;
    set_request(reboot_request(_mac_address, _password));
  }

private:
  mac_address _mac_address;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_REBOOT_H

#ifndef ADDP_ACTION_REBOOT_H
#define ADDP_ACTION_REBOOT_H

#include "action.hpp"

namespace addp {

class reboot : public action {
public:
  explicit reboot(mac_address const &mac, const std::string &password = DEFAULT_PASSWORD)
      : action(reboot_request(mac, password)), _mac_address(mac), _password(password) {}

private:
  mac_address _mac_address;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_REBOOT_H

#ifndef ADDP_ACTION_REBOOT_H
#define ADDP_ACTION_REBOOT_H

#include "action.hpp"

namespace addp {

class reboot : public action {
public:
  explicit reboot(mac_address const &mac, const std::string &password = DEFAULT_PASSWORD);

  void set_mac_address(mac_address const &);
  void set_password(const std::string &password);

protected:
  void print_brief(const boost::asio::ip::udp::endpoint &sender,
                   const packet &response) const override;

private:
  mac_address _mac_address;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_REBOOT_H

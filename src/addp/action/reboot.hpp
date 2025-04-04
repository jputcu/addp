#ifndef ADDP_ACTION_REBOOT_H
#define ADDP_ACTION_REBOOT_H

#include <addp/action/action.hpp>
#include <addp/constants.hpp>
#include <addp/types.hpp>

namespace addp {

class reboot : public action {
public:
  explicit reboot(const mac_address &mac_address, const std::string &password = DEFAULT_PASSWORD);

  void set_mac_address(const std::string &mac);
  void set_password(const std::string &password);

protected:
  void print_brief(const boost::asio::ip::udp::endpoint &sender, const packet &response) const override;

private:
  mac_address _mac_address;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_REBOOT_H

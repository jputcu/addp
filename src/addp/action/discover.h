#ifndef ADDP_ACTION_DISCOVER_H
#define ADDP_ACTION_DISCOVER_H

#include <addp/action/action.h>
#include <addp/constants.h>
#include <addp/types.h>

namespace addp {

class discover : public action {
public:
  explicit discover(const mac_address &mac_address = MAC_ADDR_BROADCAST);

  void set_mac_address(const std::string &mac);

protected:
  void print_brief(const boost::asio::ip::udp::endpoint &sender, const packet &) const override;
};

} // namespace addp

#endif // ADDP_ACTION_DISCOVER_H

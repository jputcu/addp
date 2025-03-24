#ifndef ADDP_ACTION_DISCOVER_H
#define ADDP_ACTION_DISCOVER_H

#include "action.hpp"

namespace addp {

class discover : public action {
public:
  explicit discover(mac_address const &mac = MAC_ADDR_BROADCAST) : action(discovery_request(mac)) {}

  void set_mac_address(mac_address const &mac) { set_request(discovery_request(mac)); }

protected:
  void print_brief(const boost::asio::ip::udp::endpoint &sender, const packet &) const override;
};

} // namespace addp

#endif // ADDP_ACTION_DISCOVER_H

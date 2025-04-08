#ifndef ADDP_ACTION_DISCOVER_H
#define ADDP_ACTION_DISCOVER_H

#include "action.hpp"

namespace addp {

class discover : public action {
public:
  explicit discover(mac_address const &mac = MAC_ADDR_BROADCAST) : action(packet::discovery_request(mac)) {}
};

} // namespace addp

#endif // ADDP_ACTION_DISCOVER_H

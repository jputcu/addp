#ifndef ADDP_PACKET_DISCOVERY_H
#define ADDP_PACKET_DISCOVERY_H

#include <addp/packet/packet.hpp>

namespace addp {

struct discovery_request : packet {
  explicit discovery_request(const mac_address mac = MAC_ADDR_BROADCAST)
      : packet(Type::DISCOVERY_REQUEST) {
    add(mac);
  }
};

struct discovery_response : packet {
  discovery_response() : packet(Type::DISCOVERY_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_DISCOVERY_H

#ifndef ADDP_PACKET_DISCOVERY_H
#define ADDP_PACKET_DISCOVERY_H

#include <addp/packet/packet.h>
#include <addp/types.h>

namespace addp {

class discovery_request : public packet {
public:
  discovery_request(const mac_address mac = MAC_ADDR_BROADCAST)
      : packet(packet::PT_DISCOVERY_REQUEST) {
    add(mac);
  }
};

class discovery_response : public packet {
public:
  discovery_response() : packet(packet::PT_DISCOVERY_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_DISCOVERY_H

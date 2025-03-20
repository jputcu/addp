#ifndef ADDP_PACKET_REBOOT_H
#define ADDP_PACKET_REBOOT_H

#include <addp/packet/packet.hpp>

namespace addp {

struct reboot_request : packet {
  explicit reboot_request(const mac_address &mac = MAC_ADDR_BROADCAST,
                 const std::string &auth = DEFAULT_PASSWORD)
      : packet(Type::REBOOT_REQUEST) {
    add(mac);
    add(auth);
  }
};

struct reboot_response : packet {
  reboot_response() : packet(Type::REBOOT_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_REBOOT_H

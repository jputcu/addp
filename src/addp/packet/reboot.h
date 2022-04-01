#ifndef ADDP_PACKET_REBOOT_H
#define ADDP_PACKET_REBOOT_H

#include <addp/packet/packet.h>
#include <addp/types.h>

namespace addp {

class reboot_request : public packet {
public:
  reboot_request(const mac_address &mac = MAC_ADDR_BROADCAST,
                 const std::string &auth = DEFAULT_PASSWORD)
      : packet(packet::PT_REBOOT_REQUEST) {
    add(mac);
    add(auth);
  }
};

class reboot_response : public packet {
public:
  reboot_response() : packet(packet::PT_REBOOT_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_REBOOT_H

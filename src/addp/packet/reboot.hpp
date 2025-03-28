#ifndef ADDP_PACKET_REBOOT_H
#define ADDP_PACKET_REBOOT_H

#include <addp/packet/packet.hpp>
#include <addp/types.hpp>

namespace addp {

class reboot_request : public packet {
public:
  explicit reboot_request(const mac_address &mac = MAC_ADDR_BROADCAST,
                 const std::string &auth = DEFAULT_PASSWORD)
      : packet(packet::Type::REBOOT_REQUEST) {
    add(mac);
    add(auth);
  }
};

class reboot_response : public packet {
public:
  reboot_response() : packet(packet::Type::REBOOT_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_REBOOT_H

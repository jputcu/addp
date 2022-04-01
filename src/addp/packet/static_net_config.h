#ifndef ADDP_PACKET_PACKETS_H
#define ADDP_PACKET_PACKETS_H

#include <addp/packet/packet.h>
#include <addp/types.h>

namespace addp {

class static_net_config_request : public packet {
public:
  static_net_config_request(const mac_address &mac, const ip_address &ip, const ip_address &subnet,
                            const ip_address &gateway, const std::string &auth = DEFAULT_PASSWORD)
      : packet(packet::Type::STATIC_NET_CONFIG_REQUEST) {
    add(ip);
    add(subnet);
    add(gateway);
    add(mac);
    add(auth);
  }
};

class static_net_config_response : public packet {
public:
  static_net_config_response() : packet(packet::Type::STATIC_NET_CONFIG_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_PACKETS_H

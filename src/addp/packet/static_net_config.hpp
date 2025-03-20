#ifndef ADDP_PACKET_PACKETS_H
#define ADDP_PACKET_PACKETS_H

#include <addp/packet/packet.hpp>

namespace addp {

struct static_net_config_request : packet {
  static_net_config_request(const mac_address &mac, const ip_address &ip, const ip_address &subnet,
                            const ip_address &gateway, const std::string &auth = DEFAULT_PASSWORD)
      : packet(Type::STATIC_NET_CONFIG_REQUEST) {
    add(ip);
    add(subnet);
    add(gateway);
    add(mac);
    add(auth);
  }
};

struct static_net_config_response : packet {
  static_net_config_response() : packet(Type::STATIC_NET_CONFIG_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_PACKETS_H

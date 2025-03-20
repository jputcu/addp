#ifndef ADDP_PACKET_DHCP_NET_CONFIG_H
#define ADDP_PACKET_DHCP_NET_CONFIG_H

#include <addp/packet/packet.hpp>

namespace addp {

struct dhcp_net_config_request : packet {
  dhcp_net_config_request(const mac_address &mac, bool enable,
                          const std::string &auth = DEFAULT_PASSWORD)
      : packet(Type::DHCP_NET_CONFIG_REQUEST) {
    add(enable ? field::BF_TRUE : field::BF_FALSE);
    add(mac);
    add(auth);
  }
};

struct dhcp_net_config_response : packet {
  dhcp_net_config_response() : packet(Type::DHCP_NET_CONFIG_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_DHCP_NET_CONFIG_H

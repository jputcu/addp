#ifndef ADDP_PACKET_DHCP_NET_CONFIG_H
#define ADDP_PACKET_DHCP_NET_CONFIG_H

#include <addp/packet/packet.h>
#include <addp/types.h>

namespace addp {

class dhcp_net_config_request : public packet {
public:
  dhcp_net_config_request(const mac_address &mac, bool enable,
                          const std::string &auth = DEFAULT_PASSWORD)
      : packet(packet::Type::DHCP_NET_CONFIG_REQUEST) {
    add(enable ? field::BF_TRUE : field::BF_FALSE);
    add(mac);
    add(auth);
  }
};

class dhcp_net_config_response : public packet {
public:
  dhcp_net_config_response() : packet(packet::Type::DHCP_NET_CONFIG_RESPONSE) {}
};

} // namespace addp

#endif // ADDP_PACKET_DHCP_NET_CONFIG_H

#ifndef ADDP_PACKET_IO_H
#define ADDP_PACKET_IO_H

#include <ostream>

#include <addp/packet/packet.hpp>

namespace addp {

std::ostream &operator<<(std::ostream &os, const packet &packet);

} // namespace addp

#endif // ADDP_PACKET_IO_H

#include "packet_io.h"

#include <iomanip>

#include <addp/packet/field_io.h>

namespace addp {

std::ostream &operator<<(std::ostream &os, const packet &packet) {
  os << packet.type_str() << std::endl;

  for (const field &f : packet.fields())
    if (f.type() != field::FT_NONE)
      os << "  " << f;

  switch (packet.type()) {
  case packet::Type::DISCOVERY_REQUEST: {
    os << std::hex << std::setfill('0');

    std::vector<uint8_t> payload = packet.payload();
    for (size_t i = 0; i < payload.size(); ++i)
      os << (i ? ":" : " ") << int(payload[i]);
    break;
  }
  default:
    break;
  }

  return os;
}

} // namespace addp

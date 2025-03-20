#include "packet_io.hpp"

#include <iomanip>
#include <iostream>

#include <addp/packet/field_io.hpp>
using namespace addp;

std::ostream &addp::operator<<(std::ostream &os, const packet &packet) {
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

#ifndef ADDP_PACKET_FIELD_IO_H
#define ADDP_PACKET_FIELD_IO_H

#include <iosfwd>
#include <addp/packet/field.hpp>

namespace addp {

std::ostream &operator<<(std::ostream &os, const field &field);

} // namespace addp

#endif // ADDP_PACKET_FIELD_IO_H

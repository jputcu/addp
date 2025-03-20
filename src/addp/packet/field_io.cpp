#include "field_io.hpp"
using namespace addp;

std::ostream &addp::operator<<(std::ostream &os, const field &field) {
  if (field.type() == field::FT_NONE)
    return os;

  return os << field.type_str() << " = " << field.value_str() << std::endl;
}

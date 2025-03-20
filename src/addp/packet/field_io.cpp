#include "field_io.hpp"

namespace addp {

std::ostream &operator<<(std::ostream &os, const field &field) {
  if (field.type() == field::FT_NONE)
    return os;

  return os << field.type_str() << " = " << field.value_str() << std::endl;
}

} // namespace addp

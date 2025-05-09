#ifndef ADDP_OPTIONS_H
#define ADDP_OPTIONS_H

#include <boost/program_options.hpp>
#include <cstdint>
#include <string>

#include "generic_options.hpp"

namespace addp {

class options : public generic_options {
public:
  options() {}
  options(int argc, char *argv[]);

  std::string multicast() const;
  uint16_t port() const;

protected:
  boost::program_options::options_description all_options() const override;
};

} // namespace addp

#endif // ADDP_OPTIONS_H

#ifndef ADDPD_OPTIONS_H
#define ADDPD_OPTIONS_H

#include <addp/addp.h>
#include <boost/program_options.hpp>
#include <string>

namespace addpd {

class options : public addp::options {
public:
  options(int argc, char *argv[]);

  size_t threads() const;

protected:
  virtual boost::program_options::options_description all_options() const;
};

} // namespace addpd

#endif // ADDPD_OPTIONS_H

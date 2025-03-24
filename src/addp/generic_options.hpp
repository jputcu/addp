#ifndef ADDP_GENERIC_OPTIONS_H
#define ADDP_GENERIC_OPTIONS_H

#include <boost/program_options.hpp>
#include <string>

namespace addp {

class generic_options {
public:
  generic_options() = default;
  generic_options(int argc, char *argv[]) { parse(argc, argv); }

  void usage() const;

  bool version() const { return _vm.contains("version"); }
  bool verbose() const { return _vm.contains("verbose"); }

protected:
  void parse(int argc, char *argv[]);

  virtual boost::program_options::options_description all_options() const;
  virtual boost::program_options::positional_options_description positional_options() const {
    return {};
  }

  virtual boost::program_options::options_description visible_options() const {
    return all_options();
  }

  boost::program_options::variables_map _vm;
  std::string _usage = "Usage: %s [options...]\n";

private:
  std::string _progname;
};

} // namespace addp

#endif // ADDP_GENERIC_OPTIONS_H

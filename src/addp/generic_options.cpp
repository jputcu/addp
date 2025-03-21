#include "generic_options.hpp"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>

using namespace addp;

void generic_options::parse(int argc, char *argv[]) {
  _progname = boost::filesystem::path(argv[0]).filename().string();

  boost::program_options::command_line_parser parser(argc, argv);
  boost::program_options::store(
      parser.options(all_options()).positional(positional_options()).run(), _vm);
  boost::program_options::notify(_vm);

  if (_vm.count("help")) {
    usage();
    std::exit(1);
  }
}

void generic_options::usage() const {
  std::cout << str(boost::format(_usage) % _progname) << "\n" << visible_options();
}

boost::program_options::options_description generic_options::all_options() const {
  boost::program_options::options_description opts("Generic options");
  // clang-format off
  opts.add_options()
    ("help,h", "produce help message")
    ("version,V", boost::program_options::value<bool>()->default_value(false)->zero_tokens(),
      "program version")
    ("verbose,v",
      boost::program_options::value<size_t>()->default_value(0)->implicit_value(1)->zero_tokens(),
      "verbose")
    ("logfile,l",
      boost::program_options::value<std::string>()->default_value("/dev/stdout"), "logfile");
  // clang-format on
  return opts;
}

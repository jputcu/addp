#ifndef ADDPC_OPTIONS_H
#define ADDPC_OPTIONS_H

#include <addp/addp.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iostream>

struct options {
  options(int argc, char *argv[]);

  std::string listen() const { return _vm["listen"].as<std::string>(); }

  size_t timeout() const { return _vm["timeout"].as<size_t>(); }

  std::string action() const { return _vm["action"].as<std::string>(); }

  std::string mac() const { return _vm["mac"].as<std::string>(); }

  std::vector<std::string> args() const { return _vm["args"].as<std::vector<std::string>>(); }

  std::string multicast() const { return _vm["multicast"].as<std::string>(); }

  uint16_t port() const { return _vm["port"].as<uint16_t>(); }
  bool version() const { return _vm.contains("version"); }
  std::string password() const;
  std::string ip() const { return args()[0]; }
  std::string subnet() const { return args()[1]; }
  std::string gateway() const { return args()[2]; }
  bool dhcp() const;

  void usage() const {
    std::cout << "Usage: addpc [options...] <action> [args...]\n"
                 "\n"
                 " actions:\n"
                 "  discover [device]\n"
                 "  reboot <device> [passwd]\n"
                 "  config <device> <ip> <netmask> <gateway> [passwd]\n"
                 "  dhcp <device> <on|off> [passwd]\n\n"
              << all_options();
  }

  boost::program_options::options_description all_options() const;
  boost::program_options::positional_options_description positional_options() const {
    boost::program_options::positional_options_description pos_args;
    return pos_args.add("action", 1).add("mac", 1).add("args", -1);
  }

  boost::program_options::variables_map _vm;
  size_t _password_index;
};

#endif // ADDPC_OPTIONS_H

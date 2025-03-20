#ifndef ADDPC_OPTIONS_H
#define ADDPC_OPTIONS_H

#include <addp/addp.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace addpc {

class options : public addp::options {
public:
  options(int argc, char *argv[]) {
    _usage = {"Usage: %s [options...] <action> [args...]\n"
              "\n"
              " actions:\n"
              "  discover [device]\n"
              "  reboot <device> [passwd]\n"
              "  config <device> <ip> <netmask> <gateway> [passwd]\n"
              "  dhcp <device> <on|off> [passwd]\n"};

    opt_parse(argc, argv);
  }

  std::string listen() const { return _vm["listen"].as<std::string>(); }

  size_t timeout() const { return _vm["timeout"].as<size_t>(); }

  size_t max_count() const { return _vm["max_count"].as<size_t>(); }

  std::string action() const { return _vm["action"].as<std::string>(); }

  std::string mac() const { return _vm["mac"].as<std::string>(); }

  std::vector<std::string> args() const { return _vm["args"].as<std::vector<std::string>>(); }

  // action options
  std::string password() const;
  std::string ip() const { return args()[0]; }
  std::string subnet() const { return args()[1]; }
  std::string gateway() const { return args()[2]; }

  bool dhcp() const;

protected:
  void opt_parse(int argc, char *argv[]);

  boost::program_options::options_description all_options() const override;
  boost::program_options::positional_options_description positional_options() const override;
  boost::program_options::options_description visible_options() const override;

private:
  boost::program_options::options_description addpc_options() const;
  boost::program_options::options_description addpc_hidden_options() const;

  size_t _password_index;
};

} // namespace addpc

#endif // ADDPC_OPTIONS_H

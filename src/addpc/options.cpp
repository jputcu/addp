#include "options.hpp"

#include <addp/types.hpp>
#include <iostream>

using namespace addpc;

void options::opt_parse(int argc, char *argv[]) {
  addp::options::parse(argc, argv);

  if (_vm.count("action") == 0) {
    std::cerr << "No action given\n\n";
    usage();
    std::exit(1);
  }

  /*
   *  discover [device]
   *  reboot <device> [passwd]
   *  config <device> <ip> <netmask> <gateway> [passwd]
   *  dhcp <device> <on|off> [passwd]
   */

  size_t min = 0;
  size_t max = 0;

  const std::string &action = this->action();

  if (action == "discover") {
    min = 0;
    max = 0;
  } else {
    // must have a specific device address for further actions
    if (addp::parse_mac_str(mac()) == addp::MAC_ADDR_BROADCAST) {
      std::cerr << "Please select a device's mac address\n\n";
      usage();
      std::exit(1);
    }

    if (action == "reboot") {
      min = 0;
      max = 1;
    } else if (action == "config") {
      min = 3;
      max = 4;
    } else if (action == "dhcp") {
      min = 1;
      max = 2;
    } else {
      std::cerr << "Unknown action \"" << action << "\"\n\n";
      usage();
      std::exit(1);
    }
  }

  if (size_t count = args().size(); count < min || count > max) {
    usage();
    std::exit(1);
  }

  // optional password is always the last argument
  _password_index = max - 1;
}

boost::program_options::options_description options::addpc_options() const {
  boost::program_options::options_description addpc_opts("ADDP client options");
  addpc_opts.add_options()("listen,L",
                           boost::program_options::value<std::string>()->default_value("0.0.0.0"),
                           "ip address to listen")(
      "timeout,t", boost::program_options::value<size_t>()->default_value(addp::DEFAULT_TIMEOUT),
      "response timeout (in ms)")(
      "max_count,c",
      boost::program_options::value<size_t>()->default_value(addp::DEFAULT_MAX_COUNT),
      "stop after receiving n responses");
  return addpc_opts;
}

boost::program_options::options_description options::addpc_hidden_options() const {
  boost::program_options::options_description hidden_opts;
  hidden_opts.add_options()("action", boost::program_options::value<std::string>(),
                            "action (discover/static/reboot/dhcp)")(
      "mac", boost::program_options::value<std::string>()->default_value("ff:ff:ff:ff:ff:ff"),
      "mac address of target device")("args",
                                      boost::program_options::value<std::vector<std::string>>()
                                          ->default_value(std::vector<std::string>(), "")
                                          ->multitoken(),
                                      "action arguments");
  return hidden_opts;
}

boost::program_options::options_description options::visible_options() const {
  boost::program_options::options_description opts = addp::options::all_options();
  opts.add(addpc_options());
  return opts;
}

boost::program_options::options_description options::all_options() const {
  boost::program_options::options_description opts = addp::options::all_options();
  opts.add(addpc_options());
  opts.add(addpc_hidden_options());
  return opts;
}

boost::program_options::positional_options_description options::positional_options() const {
  boost::program_options::positional_options_description positional =
      addp::options::positional_options();
  positional.add("action", 1).add("mac", 1).add("args", -1);
  return positional;
}

std::string options::password() const {
  if (args().size() <= _password_index)
    return addp::DEFAULT_PASSWORD;
  else
    return args()[_password_index];
}

bool options::dhcp() const {
  const std::string value = args()[0];
  if (value == "on")
    return true;
  else if (value == "off")
    return false;
  else {
    std::cerr << "illegal value for dhcp: \"" << value << "\"\n\n";
    usage();
    std::exit(1);
  }
}

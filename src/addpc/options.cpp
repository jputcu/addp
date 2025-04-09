#include "options.hpp"

#include <addp/types.hpp>
#include <iostream>
#include <boost/format.hpp>

using namespace addpc;

void options::usage() const {
  std::cout << _usage << "\n" << all_options();
}

void options::opt_parse(int argc, char *argv[]) {
  boost::program_options::command_line_parser parser(argc, argv);
  boost::program_options::store(
      parser.options(all_options()).positional(positional_options()).run(), _vm);
  boost::program_options::notify(_vm);

  if (_vm.count("help")) {
    usage();
    std::exit(1);
  }

  if (!_vm.contains("action")) {
    std::cerr << "No action given\n\n";
    usage();
    std::exit(1);
  }

  size_t min = 0;
  size_t max = 0;

  const std::string &action_string = this->action();
  if (action_string == "discover") {
    min = 0;
    max = 0;
  } else {
    // must have a specific device address for further actions
    if (addp::mac_address{mac()} == addp::MAC_ADDR_BROADCAST) {
      std::cerr << "Please select a device's mac address\n\n";
      usage();
      std::exit(1);
    }

    if (action_string == "reboot") {
      min = 0;
      max = 1;
    } else if (action_string == "config") {
      min = 3;
      max = 4;
    } else if (action_string == "dhcp") {
      min = 1;
      max = 2;
    } else {
      std::cerr << "Unknown action \"" << action_string << "\"\n\n";
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

boost::program_options::options_description options::all_options() const {
  boost::program_options::options_description addp_opts("ADDP options");
  addp_opts.add_options()(
        "multicast,m",
        boost::program_options::value<std::string>()->default_value(addp::MCAST_IP_ADDRESS),
        "multicast address for discovery")(
        "port,p", boost::program_options::value<uint16_t>()->default_value(addp::UDP_PORT), "udp port");

  // clang-format off
  boost::program_options::options_description addpc_opts("ADDP client options");
  addpc_opts.add_options()
    ("listen,L", boost::program_options::value<std::string>()->default_value("0.0.0.0"),
      "ip address to listen")
    ("timeout,t", boost::program_options::value<size_t>()->default_value(addp::DEFAULT_TIMEOUT),
      "response timeout (in ms)");
  // clang-format on

  boost::program_options::options_description hidden_opts;
  hidden_opts.add_options()
    ("action", boost::program_options::value<std::string>(),
      "action (discover/static/reboot/dhcp)")
    ("mac", boost::program_options::value<std::string>()->default_value("ff:ff:ff:ff:ff:ff"),
      "mac address of target device")
    ("args", boost::program_options::value<std::vector<std::string>>()
       ->default_value(std::vector<std::string>(), "")
       ->multitoken(),
       "action arguments");
  // clang-format on

  boost::program_options::options_description opts("Generic options");
  // clang-format off
  opts.add_options()
    ("help,h", "produce help message")
    ("version,V", "program version");
  // clang-format on
  opts.add(addp_opts).add(addpc_opts).add(hidden_opts);
  return opts;
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

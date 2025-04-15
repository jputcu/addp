#include "options.hpp"
#include <addp/addp.hpp>

options::options(int argc, char *argv[]) {
  boost::program_options::positional_options_description pos_args;
  pos_args.add("action", 1).add("mac", 1).add("args", -1);

  boost::program_options::command_line_parser parser(argc, argv);
  boost::program_options::store(
      parser.options(all_options()).positional(pos_args).run(), _vm);
  boost::program_options::notify(_vm);

  if (_vm.contains("help")) {
    usage();
    std::exit(1);
  }
  else if (_vm.contains("version")) {
    std::cout << addp::VERSION << "\n";
    std::exit(1);
  }

  if (!_vm.contains("action")) {
    std::cerr << "No action given\n\n";
    usage();
    std::exit(1);
  }

  size_t min = 0;
  size_t max = 0;

  if (const std::string &action_string = this->action(); action_string == "discover") {
    min = 0;
    max = 0;
  } else {
    // must have a specific device address for further actions
    if (mac() == addp::MAC_ADDR_ALL) {
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

  if (auto count = args().size(); count < min || count > max) {
    usage();
    std::exit(1);
  }

  // optional password is always the last argument
  _password_index = max - 1;
}

boost::program_options::options_description options::all_options() const {
  // clang-format off
  boost::program_options::options_description opts;
  opts.add_options()
    ("help,h", "produce help message")
    ("version,V", "program version")
    ("multicast,m",
      boost::program_options::value<std::string>()->default_value(addp::MCAST_IP_ADDRESS),
      "multicast address for discovery")
    ("port,p", boost::program_options::value<uint16_t>()->default_value(addp::UDP_PORT), "udp port")
    ("listen,L", boost::program_options::value<std::string>()->default_value("0.0.0.0"),
      "ip address to listen")
    ("timeout,t", boost::program_options::value<size_t>()->default_value(addp::DEFAULT_TIMEOUT),
      "response timeout (in ms)")
    ("action", boost::program_options::value<std::string>(),
      "action (discover/static/reboot/dhcp)")
    ("mac", boost::program_options::value<std::string>()->default_value(addp::MAC_ADDR_ALL),
      "mac address of target device")
    ("args", boost::program_options::value<std::vector<std::string>>()
       ->default_value(std::vector<std::string>(), "")
       ->multitoken(),
       "action arguments");
  // clang-format on
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

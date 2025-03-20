#ifndef ADDP_OPTIONS_H
#define ADDP_OPTIONS_H

#include <boost/program_options.hpp>
#include <cstdint>
#include <string>

#include "constants.hpp"
#include "generic_options.hpp"

namespace addp {

class options : public generic_options {
public:
  options() = default;
  options(int argc, char *argv[]) { parse(argc, argv); }

  std::string multicast() const { return _vm["multicast"].as<std::string>(); }

  uint16_t port() const { return _vm["port"].as<uint16_t>(); }

protected:
  boost::program_options::options_description all_options() const override {
    boost::program_options::options_description addp_opts("ADDP options");
    addp_opts.add_options()(
        "multicast,m",
        boost::program_options::value<std::string>()->default_value(addp::MCAST_IP_ADDRESS),
        "multicast address for discovery")(
        "port,p", boost::program_options::value<uint16_t>()->default_value(UDP_PORT), "udp port");

    auto opts = generic_options::all_options();
    opts.add(addp_opts);
    return opts;
  }
};

} // namespace addp

#endif // ADDP_OPTIONS_H

#include "options.hpp"

#include <addp/addp.hpp>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  options opts{argc, argv};

  addp::action a;
  a.set_timeout(opts.timeout());
  a.set_dest_address(opts.multicast(), opts.port());
  a.set_listen_address(opts.listen(), opts.port());

  std::map<addp::mac_address, addp::response> responses;
  if (opts.action() == "discover")
    responses = a.discover(opts.mac());
  else if (opts.action() == "config")
    responses =
        a.static_net_config(opts.mac(), boost::asio::ip::make_address_v4(opts.ip()),
                            boost::asio::ip::make_address_v4(opts.subnet()),
                            boost::asio::ip::make_address_v4(opts.gateway()), opts.password());
  else if (opts.action() == "dhcp")
    responses = a.dhcp_net_config(opts.mac(), opts.dhcp(), opts.password());
  else if (opts.action() == "reboot") {
    // The reboot not always gets a response.
    // Using digi tool it mostly does. It first sends a digi discovery request for that digi
    // before sending the reboot
    a.discover(opts.mac());
    // Check digi is available in the response
    responses = a.reboot(opts.mac(), opts.password());
  } else
    return EXIT_FAILURE;

  // print collection:
  {
    std::cout << "Responses #" << responses.size() << ":\n";
    for (auto const &[mac, response] : responses)
      std::cout << mac << " " << response << "\n";
  }

  return responses.size() ? EXIT_SUCCESS : EXIT_FAILURE;
}

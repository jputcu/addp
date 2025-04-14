#include "options.hpp"

#include <addp/addp.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  options opts{argc, argv};

  if (opts.version()) {
    std::cout << addp::VERSION << "\n";
    return EXIT_SUCCESS;
  }

  addp::action a;
  a.set_dest_address(opts.multicast(), opts.port());
  a.set_timeout(opts.timeout());
  a.set_listen_address(opts.listen(), opts.port());

  auto response_handler = [](const boost::asio::ip::udp::endpoint &sender,
                             const addp::response &packet) {
    std::cout << sender << " " << packet << "\n";
  };

  if (opts.action() == "discover")
    return a.run(addp::request::discover(opts.mac()), response_handler);
  else if (opts.action() == "config")
    return a.run(addp::request::static_net_config(
                     opts.mac(), boost::asio::ip::make_address_v4(opts.ip()),
                     boost::asio::ip::make_address_v4(opts.subnet()),
                     boost::asio::ip::make_address_v4(opts.gateway()), opts.password()),
                 response_handler);
  else if (opts.action() == "dhcp")
    return a.run(addp::request::dhcp_net_config(opts.mac(), opts.dhcp(), opts.password()),
                 response_handler);
  else if (opts.action() == "reboot") {
    // The reboot not always gets a response.
    // Using digi tool it mostly does. It first sends a digi discovery request for that digi
    // before sending the reboot
    a.run(addp::request::discover(opts.mac()), response_handler);
    // Check digi is available in the response
    return a.run(addp::request::reboot(opts.mac(), opts.password()), response_handler);
  } else
    return EXIT_FAILURE;
}

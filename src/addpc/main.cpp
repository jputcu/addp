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

  std::vector<std::pair<std::string, addp::response>> responses;
  auto collector = [&responses](const boost::asio::ip::udp::endpoint &sender,
                                        const addp::response &packet) {
    responses.emplace_back(sender.address().to_string(), packet);
  };

  bool action_result = false;
  if (opts.action() == "discover")
    action_result = a.run(addp::request::discover(opts.mac()), collector);
  else if (opts.action() == "config")
    action_result = a.run(addp::request::static_net_config(
                              opts.mac(), boost::asio::ip::make_address_v4(opts.ip()),
                              boost::asio::ip::make_address_v4(opts.subnet()),
                              boost::asio::ip::make_address_v4(opts.gateway()), opts.password()),
                          collector);
  else if (opts.action() == "dhcp")
    action_result = a.run(addp::request::dhcp_net_config(opts.mac(), opts.dhcp(), opts.password()),
                          collector);
  else if (opts.action() == "reboot") {
    // The reboot not always gets a response.
    // Using digi tool it mostly does. It first sends a digi discovery request for that digi
    // before sending the reboot
    a.run(addp::request::discover(opts.mac()), collector);
    // Check digi is available in the response
    action_result = a.run(addp::request::reboot(opts.mac(), opts.password()), collector);
  } else
    return EXIT_FAILURE;

  // print collection:
  {
    std::cout << "Responses #" << responses.size() << ":\n";
    for (auto const &[endp, response] : responses)
      std::cout << endp << " " << response << "\n";
  }

  return action_result ? EXIT_SUCCESS : EXIT_FAILURE;
}

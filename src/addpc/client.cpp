#include "client.hpp"

#include <addp/addp.hpp>
#include <addp/types.hpp>
#include <iostream>

#include "options.hpp"

using namespace addpc;

client::client(const options &options) : _options(options) {}

bool client::run() {
  if (_options.version()) {
    std::cout << addp::VERSION << std::endl;
    return true;
  }

  if (_options.action() == "discover")
    return discover();
  if (_options.action() == "config")
    return static_net_config();
  if (_options.action() == "dhcp")
    return dhcp_net_config();
  if (_options.action() == "reboot")
    return reboot();

  // not reached
  return false;
}

bool client::run_action(addp::action &action) {
  action.set_verbose(_options.verbose());
  action.set_dest_address(_options.multicast(), _options.port());
  action.set_timeout(_options.timeout());

  return action.run();
}

bool client::discover() {
  addp::discover action;
  action.set_listen_address(_options.listen(), _options.port());
  action.set_max_count(_options.max_count());
  action.set_mac_address(_options.mac());

  return run_action(action);
}

bool client::static_net_config() {
  addp::static_net_config action(
      addp::parse_mac_str(_options.mac()), addp::parse_ip_str(_options.ip()),
      addp::parse_ip_str(_options.subnet()), addp::parse_ip_str(_options.gateway()));
  action.set_password(_options.password());

  return run_action(action);
}

bool client::dhcp_net_config() {
  addp::dhcp_net_config action(addp::parse_mac_str(_options.mac()), _options.dhcp());
  action.set_password(_options.password());

  return run_action(action);
}

bool client::reboot() {
  addp::reboot action(addp::parse_mac_str(_options.mac()));
  action.set_password(_options.password());

  return run_action(action);
}

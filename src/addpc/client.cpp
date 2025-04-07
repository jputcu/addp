#include "client.hpp"

#include <addp/addp.hpp>
#include <addp/types.hpp>
#include <iostream>

#include "options.hpp"

using namespace addpc;

bool client::run() {
  if (_options.version()) {
    std::cout << addp::VERSION << "\n";
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

bool client::run_action(addp::action &&action) {
  action.set_verbose(_options.verbose());
  action.set_dest_address(_options.multicast(), _options.port());
  action.set_timeout(_options.timeout());
  return action.run();
}

bool client::discover() {
  struct DiscoverHandle : addp::discover {
    using discover::discover;

    void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                     const addp::packet &packet) override {
      std::cout << sender << " " << packet << "\n";
    }
  };

  DiscoverHandle action{_options.mac()};
  action.set_listen_address(_options.listen(), _options.port());
  return run_action(std::move(action));
}

bool client::static_net_config() {
  struct StaticNetHandle : addp::static_net_config {
    using static_net_config::static_net_config;

    void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                     const addp::packet &packet) override {
      for (auto const &f : packet.fields())
        if (f.type() == addp::field::FT_RESULT_MSG) {
          std::cout << f.value<std::string>() << "\n";
          break;
        }
    }
  };

  return run_action(StaticNetHandle{_options.mac(), _options.ip(), _options.subnet(),
                         _options.gateway(), _options.password()});
}

bool client::dhcp_net_config() {
  struct DhcpNetHandle : addp::dhcp_net_config {
    using dhcp_net_config::dhcp_net_config;

    void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                     const addp::packet &packet) override {
      for (auto const &f : packet.fields())
        if (f.type() == addp::field::FT_RESULT_MSG) {
          std::cout << f.value_str() << "\n";
          break;
        }
    }
  };

  return run_action(DhcpNetHandle{_options.mac(), _options.dhcp(), _options.password()});
}

bool client::reboot() {
  struct RebootHandle : addp::reboot {
    using reboot::reboot;

    void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                     const addp::packet &packet) override {
      for (auto const &f : packet.fields())
        if (f.type() == addp::field::FT_RESULT_MSG) {
          std::cout << f.value<std::string>() << "\n";
          break;
        }
    }
  };

  return run_action(RebootHandle{_options.mac(), _options.password()});
}

#include "options.hpp"

#include <addp/addp.hpp>
#include <iostream>

struct client {
  int run_action(addp::action &&action) {
    action.set_dest_address(_options.multicast(), _options.port());
    action.set_timeout(_options.timeout());
    return action.run() ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  int discover() {
    struct DiscoverHandle : addp::discover {
      using discover::discover;

      void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                       const addp::response &packet) override {
        std::cout << sender << " " << packet << "\n";
      }
    };

    DiscoverHandle action{_options.mac()};
    action.set_listen_address(_options.listen(), _options.port());
    return run_action(std::move(action));
  }

  int static_net_config() {
    struct StaticNetHandle : addp::static_net_config {
      using static_net_config::static_net_config;

      void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                       const addp::response &packet) override {
        for (auto const &f : packet.fields())
          if (f.first == addp::field_type::result_msg) {
            std::cout << f.second.value<std::string>() << "\n";
            break;
          }
      }
    };

    return run_action(StaticNetHandle{_options.mac(), _options.ip(), _options.subnet(),
                                      _options.gateway(), _options.password()});
  }

  int dhcp_net_config() {
    struct DhcpNetHandle : addp::dhcp_net_config {
      using dhcp_net_config::dhcp_net_config;

      void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                       const addp::response &packet) override {
        for (auto const &f : packet.fields())
          if (f.first == addp::field_type::result_msg) {
            std::cout << f.second.value_str() << "\n";
            break;
          }
      }
    };

    return run_action(DhcpNetHandle{_options.mac(), _options.dhcp(), _options.password()});
  }

  int reboot() {
    struct RebootHandle : addp::reboot {
      using reboot::reboot;

      void on_response(const boost::asio::ip::udp::endpoint &sender [[maybe_unused]],
                       const addp::response &packet) override {
        for (auto const &f : packet.fields())
          if (f.first == addp::field_type::result_msg) {
            std::cout << f.second.value<std::string>() << "\n";
            break;
          }
      }
    };

    return run_action(RebootHandle{_options.mac(), _options.password()});
  }

  options _options;
};

int main(int argc, char *argv[]) {
  client c{{argc, argv}};

  if (c._options.version()) {
    std::cout << addp::VERSION << "\n";
    return EXIT_SUCCESS;
  }

  if (c._options.action() == "discover")
    return c.discover();
  else if (c._options.action() == "config")
    return c.static_net_config();
  else if (c._options.action() == "dhcp")
    return c.dhcp_net_config();
  else if (c._options.action() == "reboot")
    return c.reboot();
  else
    return EXIT_FAILURE;
}

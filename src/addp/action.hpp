#ifndef ADDP_ACTION_ACTION_H
#define ADDP_ACTION_ACTION_H

#include <array>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <list>

#include <addp/packet.hpp>
#include <addp/constants.hpp>

namespace addp {

class action {
public:
  using callback_t = std::function<void(boost::asio::ip::udp::endpoint sender, packet const &)>;

  explicit action(packet &&request);

  void set_listen_address(const std::string &listen_ip, uint16_t port = UDP_PORT) {
    _listen_address =
        boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(listen_ip), port);
  }

  void set_dest_address(const std::string &dest_ip, uint16_t port = UDP_PORT) {
    _dest_address = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(dest_ip), port);
  }

  void set_timeout(const size_t timeout_ms) { _timeout_ms = timeout_ms; }

  void set_verbose(bool verbose) { _verbose = verbose; }

  bool run();

  void stop() {
    _socket.cancel();
    _io_context.stop();
  }

protected:
  virtual void on_response(const boost::asio::ip::udp::endpoint &sender, const packet &) = 0;

private:
  void check_timeout();
  void handle_send_to(const boost::system::error_code &error, size_t bytes_sent);
  void handle_receive_from(const boost::system::error_code &error, size_t bytes_recvd);

  boost::asio::ip::udp::endpoint _listen_address;
  boost::asio::ip::udp::endpoint _dest_address;
  boost::asio::ip::udp::endpoint _sender_address;
  boost::asio::io_context _io_context;
  boost::asio::ip::udp::socket _socket;
  boost::asio::deadline_timer _deadline;
  std::array<uint8_t, MAX_UDP_MESSAGE_LEN> _data;

  packet _request;
  size_t _timeout_ms{DEFAULT_TIMEOUT};
  bool _verbose{};
};

class discover : public action {
public:
  explicit discover(mac_address const &mac = MAC_ADDR_BROADCAST)
      : action(packet::discovery_request(mac)) {}
};

class reboot : public action {
public:
  explicit reboot(mac_address const &mac, const std::string &password = DEFAULT_PASSWORD)
      : action(packet::reboot_request(mac, password)), _mac_address(mac), _password(password) {}

private:
  mac_address _mac_address;
  std::string _password;
};

class static_net_config : public action {
public:
  static_net_config(mac_address const &mac, const ip_address &ip, const ip_address &subnet,
                    const ip_address &gateway, const std::string &password = DEFAULT_PASSWORD)
      : action(packet::static_net_config_request(mac, ip, subnet, gateway, password)),
        _mac_address(mac), _ip(ip), _subnet(subnet), _gateway(gateway), _password(password) {}

private:
  mac_address _mac_address;
  ip_address _ip;
  ip_address _subnet;
  ip_address _gateway;
  std::string _password;
};

class dhcp_net_config : public action {
public:
  dhcp_net_config(const mac_address &mac_address, bool enable,
                  const std::string &password = DEFAULT_PASSWORD)
      : action(packet::dhcp_net_config_request(mac_address, enable, password)),
        _mac_address(mac_address), _enable(enable), _password(password) {}

private:
  mac_address _mac_address;
  bool _enable;
  std::string _password;
};

} // namespace addp

#endif // ADDP_ACTION_ACTION_H

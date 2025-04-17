#ifndef ADDP_ACTION_ACTION_H
#define ADDP_ACTION_ACTION_H

#include <array>
#include <string_view>
#include <vector>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

#include <addp/packet.hpp>
#include <addp/constants.hpp>

namespace addp {

class action {
public:
  using callback_t = std::function<void(boost::asio::ip::udp::endpoint sender, response const &)>;

  explicit action();

  void set_listen_address(std::string_view listen_ip, uint16_t port = UDP_PORT) {
    _listen_address =
        boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(listen_ip), port);
  }

  void set_dest_address(std::string_view dest_ip, uint16_t port = UDP_PORT) {
    _dest_address = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(dest_ip), port);
  }

  void set_timeout(const size_t timeout_ms) { _timeout_ms = timeout_ms; }

  template<class ...Args>
  auto discover(Args&& ...args) {
    return run(request::discover(std::forward<Args>(args)...));
  }

  template<class ...Args>
  auto static_net_config(Args&& ...args) {
    return run(request::static_net_config(std::forward<Args>(args)...));
  }

  template<class ...Args>
  auto dhcp_net_config(Args&& ...args) {
    return run(request::dhcp_net_config(std::forward<Args>(args)...));
  }

  template<class ...Args>
  auto reboot(Args&& ...args) {
    return run(request::reboot(std::forward<Args>(args)...));
  }

  void stop() {
    _socket.cancel();
    _io_context.stop();
  }

private:
  void check_timeout();
  void handle_send_to(const boost::system::error_code &error, size_t bytes_sent);
  void handle_receive_from(const boost::system::error_code &error, size_t bytes_recvd);
  std::vector<std::pair<std::string, response>> run(request const &);

  boost::asio::ip::udp::endpoint _listen_address;
  boost::asio::ip::udp::endpoint _dest_address;
  boost::asio::ip::udp::endpoint _sender_address;
  boost::asio::io_context _io_context;
  boost::asio::ip::udp::socket _socket;
  boost::asio::deadline_timer _deadline;
  std::array<uint8_t, MAX_UDP_MESSAGE_LEN> _data;

  size_t _timeout_ms{DEFAULT_TIMEOUT};
  std::vector<std::pair<std::string, response>> m_responses;
};

} // namespace addp

#endif // ADDP_ACTION_ACTION_H

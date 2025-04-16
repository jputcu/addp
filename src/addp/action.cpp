#include "action.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <iostream>

using namespace addp;

action::action()
    : _listen_address(boost::asio::ip::udp::v4(), UDP_PORT),
      _dest_address(boost::asio::ip::make_address(MCAST_IP_ADDRESS), UDP_PORT),
      _socket(_io_context), _deadline(_io_context) {
  // disable timer by setting expiry time to infinity
  _deadline.expires_at(boost::posix_time::pos_infin);
  check_timeout();
  _socket.open(boost::asio::ip::udp::v4());
  _socket.bind(_listen_address);
}

bool action::run(request const &req, callback_t const &cb) {
  _io_context.reset();
  m_cb = cb;
  m_response_cnt = 0;

  std::cout << "sending to: " << _dest_address << " packet: " << req << "\n\n";

  // send request to multicast address
  _socket.async_send_to(boost::asio::buffer(req.raw()), _dest_address,
                        [this](boost::system::error_code const &ec, std::size_t bytes_sent) {
                          handle_send_to(ec, bytes_sent);
                        });

  // set timeout from now
  if (_timeout_ms)
    _deadline.expires_from_now(boost::posix_time::milliseconds(_timeout_ms));

  try {
    _io_context.run();
  } catch (const boost::system::system_error &error) {
    std::cerr << str(boost::format("Error: %s (%d)") % error.code().message() %
                     error.code().value())
              << "\n";
    return false;
  }

  return true;
}

void action::check_timeout() {
  if (_deadline.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
    std::cout << "timeout reached (" << std::dec << _timeout_ms << "ms), " << m_response_cnt << " responses\n";

    stop();
    _deadline.expires_at(boost::posix_time::pos_infin);
  }
  _deadline.async_wait([&](const boost::system::error_code &) { check_timeout(); });
}

void action::handle_send_to(const boost::system::error_code &error, const size_t bytes_sent) {
  if (error)
    std::cerr << "error: " << error.value() << "(" << error.message() << ")"
              << " sent: " << bytes_sent << "\n";

  _socket.async_receive_from(boost::asio::buffer(_data), _sender_address,
                             [this](boost::system::error_code const &ec, std::size_t bytes_recvd) {
                               handle_receive_from(ec, bytes_recvd);
                             });
}

void action::handle_receive_from(const boost::system::error_code &error, const size_t bytes_recvd) {
  if (!error && bytes_recvd > 0) {
    m_cb(_sender_address, response{_data.data(), _data.data() + bytes_recvd});
    ++m_response_cnt;
  }

  // timeout reached?
  if (error == boost::asio::error::operation_aborted) {
    return;
  }

  // other error?
  if (error)
    std::cerr << "error: " << error.value() << "(" << error.message() << ")"
              << " received: " << bytes_recvd << "\n";

  // continue receiving
  _socket.async_receive_from(boost::asio::buffer(_data, _data.size()), _sender_address,
                             [this](boost::system::error_code const &ec, std::size_t n_bytes_recvd) {
                               handle_receive_from(ec, n_bytes_recvd);
                             });
}

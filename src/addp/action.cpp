#include "action.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <iostream>

using namespace addp;

action::action() {
  // disable timer by setting expiry time to infinity
  _deadline.expires_at(boost::posix_time::pos_infin);
  check_timeout();
}

std::map<mac_address, response> action::run(request const &req) {
  m_responses.clear();
  std::cout << "sending to: " << _dest_address << " packet: " << req << "\n\n";

  _io_context.reset();
  _socket.open(boost::asio::ip::udp::v4());
  _socket.bind(_listen_address);
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
    std::cerr << boost::format("Error: %s (%d)\n") % error.code().message() % error.code().value();
    m_responses.clear();
  }

  _socket.close();
  return m_responses;
}

void action::check_timeout() {
  if (_deadline.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
    std::cout << "timeout reached (" << std::dec << _timeout_ms << "ms)\n";

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
    response resp{_data.data(), bytes_recvd};
    // For discovery, assume field_type::ip_addr matches sender IP
    auto mac = resp.fields().at(field_type::mac_addr).as_mac_address();
    m_responses.emplace(mac, std::move(resp));
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
  _socket.async_receive_from(boost::asio::buffer(_data), _sender_address,
                             [this](boost::system::error_code const &ec, std::size_t n_bytes_recvd) {
                               handle_receive_from(ec, n_bytes_recvd);
                             });
}

#include "action.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <iostream>

#include <addp/packet/packet.h>
#include <addp/packet/packet_io.h>
#include <addp/types_io.h>

namespace addp {

action::action(const packet &request)
    : _listen_address(boost::asio::ip::udp::v4(), UDP_PORT),
      _dest_address(boost::asio::ip::make_address(MCAST_IP_ADDRESS), UDP_PORT), _sender_address(),
      _io_context(), _socket(_io_context), _deadline(_io_context), _request(request),
      _callback(boost::bind(&action::print_brief, this, _1, _2)), _count(0),
      _max_count(DEFAULT_MAX_COUNT), _timeout_ms(DEFAULT_TIMEOUT), _verbose(0) {
  // disable timer by setting expiry time to infinity
  _deadline.expires_at(boost::posix_time::pos_infin);
  check_timeout();
}

void action::set_listen_address(const std::string &listen_ip, uint16_t port) {
  _listen_address = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(listen_ip), port);
}

void action::set_dest_address(const std::string &dest_ip, uint16_t port) {
  _dest_address = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(dest_ip), port);
}

void action::set_verbose(const size_t verbose) {
  _verbose = verbose;

  if (verbose)
    set_callback(boost::bind(&action::print_verbose, this, _1, _2));
  else
    set_callback(boost::bind(&action::print_brief, this, _1, _2));
}

bool action::run() {
  _socket.open(boost::asio::ip::udp::v4());
  _socket.bind(_listen_address);

  if (_verbose)
    std::cout << "sending to: " << _dest_address << " packet: " << _request << std::endl
              << std::endl;

  // send request to multicast address
  _socket.async_send_to(boost::asio::buffer(_request.raw()), _dest_address,
                        [this](boost::system::error_code ec, std::size_t bytes_sent) {
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
              << std::endl;
    return false;
  }

  return true;
}

void action::stop() {
  _socket.cancel();
  _io_context.stop();
}

void action::check_timeout() {
  if (_deadline.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
    if (_verbose >= 2)
      std::cout << "timeout reached (" << std::dec << _timeout_ms << "ms)" << std::endl;

    stop();
    _deadline.expires_at(boost::posix_time::pos_infin);
  }
  _deadline.async_wait(boost::bind(&action::check_timeout, this));
}

void action::handle_send_to(const boost::system::error_code &error, size_t bytes_sent) {
  if (error)
    std::cerr << "error: " << error.value() << "(" << error.message() << ")"
              << " sent: " << bytes_sent << std::endl;

  _socket.async_receive_from(boost::asio::buffer(_data, MAX_UDP_MESSAGE_LEN), _sender_address,
                             [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                               handle_receive_from(ec, bytes_recvd);
                             });
}

void action::handle_receive_from(const boost::system::error_code &error, size_t bytes_recvd) {
  if (!error && bytes_recvd > 0) {
    packet response(_data.data(), bytes_recvd);

    if (!response.parse_fields())
      std::cerr << "failed to parse fields!" << std::endl;

    _callback(_sender_address, response);

    ++_count;
  }

  // max count reached?
  if (_max_count && _count == _max_count) {
    if (_verbose >= 2)
      std::cout << "max_count reached (" << std::dec << _max_count << ")" << std::endl;

    stop();
    return;
  }

  // timeout reached?
  if (error == boost::asio::error::operation_aborted) {
    return;
  }

  // other error?
  if (error)
    std::cerr << "error: " << error.value() << "(" << error.message() << ")"
              << " received: " << bytes_recvd << std::endl;

  // continue receiving
  _socket.async_receive_from(boost::asio::buffer(_data, MAX_UDP_MESSAGE_LEN), _sender_address,
                             [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                               handle_receive_from(ec, bytes_recvd);
                             });
}

void action::print_brief(const boost::asio::ip::udp::endpoint &sender,
                         const packet & /*pckt*/) const {
  std::cout << sender.address() << std::endl;
}

void action::print_verbose(const boost::asio::ip::udp::endpoint &sender, const packet &pckt) const {
  std::cout << sender << " " << pckt << std::endl;
}

} // namespace addp

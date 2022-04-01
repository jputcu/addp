#ifndef ADDPD_SERVER_H
#define ADDPD_SERVER_H

#include <addp/addp.h>
#include <array>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/system/system_error.hpp>
#include <cstdint>
#include <string>

class server {
public:
  server(uint16_t port = addp::UDP_PORT, const std::string &mcast_ip = addp::MCAST_IP_ADDRESS);

  void run();
  void stop();

private:
  void handle_receive_from(const boost::system::error_code &error, size_t bytes_recvd);

  boost::asio::io_context _io_context;
  boost::asio::ip::udp::endpoint _listen_address;
  boost::asio::ip::udp::socket _socket;
  size_t _thread_count;
  boost::asio::ip::udp::endpoint _sender_address;
  std::array<uint8_t, 4096> _data;
};

#endif // ADDPD_SERVER_H

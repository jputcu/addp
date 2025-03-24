#include "reboot.hpp"
#include <iostream>
using namespace addp;

void reboot::print_brief(const boost::asio::ip::udp::endpoint & /*sender*/,
                         const packet &response) const {
  for (const field &f : response.fields())
    if (f.type() == field::FT_RESULT_MSG) {
      std::cout << f.value<std::string>() << "\n";
      break;
    }
}
